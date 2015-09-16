#include "stdafx.h"
#include "Exception.h"
#include "ThreadLocal.h"
#include "DBHelper.h"

SQLHENV DBHelper::sqlHenv;
SQL_CONN* DBHelper::sqlConnPool;
int DBHelper::dbWorkerThreadCount = 0;

DBHelper::DBHelper()
{
	id = skylark::TLS::threadId;
	currentSqlHstmt = sqlConnPool[id].sqlHstmt;
	currentResultCol = 1;
	currentBindParam = 1;
	skylark::CRASH_ASSERT(currentSqlHstmt != nullptr);

	sqlConnPool[id].usingNow = true;
}

DBHelper::~DBHelper()
{
	SQLFreeStmt(currentSqlHstmt, SQL_UNBIND);
	SQLFreeStmt(currentSqlHstmt, SQL_RESET_PARAMS);
	SQLFreeStmt(currentSqlHstmt, SQL_CLOSE);

	sqlConnPool[id].usingNow = false;
}

bool DBHelper::initialize(const wchar_t* connInfoStr, int workerThreadCount)
{
	sqlConnPool = new SQL_CONN[workerThreadCount];
	dbWorkerThreadCount = workerThreadCount;

	if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &sqlHenv))
	{
		printf_s("DbHelper Initialize SQLAllocHandle failed\n");
		return false;
	}

	if (SQL_SUCCESS != SQLSetEnvAttr(sqlHenv, SQL_ATTR_ODBC_VERSION, (SQLPOINTER)SQL_OV_ODBC3, SQL_IS_INTEGER))
	{
		printf_s("DbHelper Initialize SQLSetEnvAttr failed\n");
		return false;
	}

	/// 스레드별로 SQL connection을 풀링하는 방식
	for (int i = 0; i < dbWorkerThreadCount; ++i)
	{
		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_DBC, sqlHenv, &sqlConnPool[i].sqlHdbc))
		{
			printf_s("DbHelper Initialize SQLAllocHandle failed\n");
			return false;
		}

		SQLSMALLINT resultLen = 0;
		SQLRETURN ret = SQLDriverConnect(sqlConnPool[i].sqlHdbc, NULL, (SQLWCHAR*)connInfoStr, (SQLSMALLINT)wcslen(connInfoStr), NULL, 0, &resultLen, SQL_DRIVER_NOPROMPT);
		if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
		{
			SQLWCHAR sqlState[1024] = { 0, };
			SQLINTEGER nativeError = 0;
			SQLWCHAR msgText[1024] = { 0, };
			SQLSMALLINT textLen = 0;

			SQLGetDiagRec(SQL_HANDLE_DBC, sqlConnPool[i].sqlHdbc, 1, sqlState, &nativeError, msgText, 1024, &textLen);

			wprintf_s(L"DbHelper Initialize SQLDriverConnect failed: %s \n", msgText);

			return false;
		}

		if (SQL_SUCCESS != SQLAllocHandle(SQL_HANDLE_STMT, sqlConnPool[i].sqlHdbc, &sqlConnPool[i].sqlHstmt))
		{
			printf_s("DbHelper Initialize SQLAllocHandle SQL_HANDLE_STMT failed\n");
			return false;
		}

	}

	return true;
}


void DBHelper::finalize()
{
	for (int i = 0; i < dbWorkerThreadCount; ++i)
	{
		SQL_CONN* currConn = &sqlConnPool[i];
		if (currConn->sqlHstmt)
			SQLFreeHandle(SQL_HANDLE_STMT, currConn->sqlHstmt);

		if (currConn->sqlHdbc)
			SQLFreeHandle(SQL_HANDLE_DBC, currConn->sqlHdbc);
	}

	delete[] sqlConnPool;


}

bool DBHelper::execute(const wchar_t* sqlstmt)
{

	SQLRETURN ret = SQLExecDirect(currentSqlHstmt, (SQLWCHAR*)sqlstmt, SQL_NTSL);
	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		printSqlStmtError();
		return false;
	}

	return true;
}

bool DBHelper::fetchRow()
{
	SQLRETURN ret = SQLFetch(currentSqlHstmt);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		if (SQL_NO_DATA != ret)
		{
			printSqlStmtError();
		}

		return false;
	}

	return true;
}



bool DBHelper::bindParamInt(int* param)
{
	SQLRETURN ret = SQLBindParameter(currentSqlHstmt, currentBindParam++, SQL_PARAM_INPUT,
		SQL_C_LONG, SQL_INTEGER, 10, 0, param, 0, NULL);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		printSqlStmtError();
		return false;
	}

	return true;
}

bool DBHelper::bindParamFloat(float* param)
{
	SQLRETURN ret = SQLBindParameter(currentSqlHstmt, currentBindParam++, SQL_PARAM_INPUT,
		SQL_C_FLOAT, SQL_REAL, 15, 0, param, 0, NULL);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		printSqlStmtError();
		return false;
	}

	return true;
}

bool DBHelper::bindParamBool(bool* param)
{
	SQLRETURN ret = SQLBindParameter(currentSqlHstmt, currentBindParam++, SQL_PARAM_INPUT,
		SQL_C_TINYINT, SQL_TINYINT, 3, 0, param, 0, NULL);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		printSqlStmtError();
		return false;
	}

	return true;
}

bool DBHelper::bindParamText(const wchar_t* text)
{
	SQLRETURN ret = SQLBindParameter(currentSqlHstmt, currentBindParam++, SQL_PARAM_INPUT,
		SQL_C_WCHAR, SQL_WVARCHAR, wcslen(text), 0, (SQLPOINTER)text, 0, NULL);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		printSqlStmtError();
		return false;
	}

	return true;
}


void DBHelper::bindResultColumnInt(int* r)
{
	SQLLEN len = 0;
	SQLRETURN ret = SQLBindCol(currentSqlHstmt, currentResultCol++, SQL_C_LONG, r, 4, &len);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		printSqlStmtError();
	}
}
void DBHelper::bindResultColumnFloat(float* r)
{
	SQLLEN len = 0;
	SQLRETURN ret = SQLBindCol(currentSqlHstmt, currentResultCol++, SQL_C_FLOAT, r, 4, &len);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		printSqlStmtError();
	}
}

void DBHelper::bindResultColumnBool(bool* r)
{
	SQLLEN len = 0;
	SQLRETURN ret = SQLBindCol(currentSqlHstmt, currentResultCol++, SQL_C_TINYINT, r, 1, &len);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		printSqlStmtError();
	}
}
void DBHelper::bindResultColumnText(wchar_t* text, size_t count)
{
	SQLLEN len = 0;
	SQLRETURN ret = SQLBindCol(currentSqlHstmt, currentResultCol++, SQL_C_WCHAR, text, count * 2, &len);

	if (SQL_SUCCESS != ret && SQL_SUCCESS_WITH_INFO != ret)
	{
		printSqlStmtError();
	}
}


void DBHelper::printSqlStmtError()
{
	SQLWCHAR sqlState[1024] = { 0, };
	SQLINTEGER nativeError = 0;
	SQLWCHAR msgText[1024] = { 0, };
	SQLSMALLINT textLen = 0;

	SQLGetDiagRec(SQL_HANDLE_STMT, currentSqlHstmt, 1, sqlState, &nativeError, msgText, 1024, &textLen);

	wprintf_s(L"DbHelper SQL Statement Error: %ls \n", msgText);
}