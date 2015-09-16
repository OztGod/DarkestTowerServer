#pragma once
#include <sqlext.h>
#include <sqltypes.h>

struct SQL_CONN
{
	SQLHDBC		sqlHdbc = nullptr;
	SQLHSTMT	sqlHstmt = nullptr;
	bool		usingNow = false;
};

class DBHelper
{
public:
	DBHelper();
	~DBHelper();

	static bool initialize(const wchar_t* connInfoStr, int workerThreadCount);
	static void finalize();

	bool execute(const wchar_t* sqlstmt);
	bool fetchRow();


	/// 아래는 DbHelper 객체를 선언하고 사용해야 함

	bool bindParamInt(int* param);
	bool bindParamFloat(float* param);
	bool bindParamBool(bool* param);
	bool bindParamText(const wchar_t* text);

	/// FetchRow시 데이터가 있다면 반드시 차례대로 호출해야함
	void bindResultColumnInt(int* r);
	void bindResultColumnFloat(float* r);
	void bindResultColumnBool(bool* r);
	void bindResultColumnText(wchar_t* text, size_t count);

	//TODO: log에 남기도록 바꿀 것
	void printSqlStmtError();

private:
	int id;
	SQLHSTMT	currentSqlHstmt = nullptr;

	SQLSMALLINT	currentResultCol;
	SQLSMALLINT	currentBindParam;

	static SQLHENV sqlHenv;
	static SQL_CONN* sqlConnPool; ///< 워커스레드수만큼
	static int dbWorkerThreadCount;

};