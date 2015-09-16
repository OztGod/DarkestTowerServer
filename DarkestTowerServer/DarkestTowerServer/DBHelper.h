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


	/// �Ʒ��� DbHelper ��ü�� �����ϰ� ����ؾ� ��

	bool bindParamInt(int* param);
	bool bindParamFloat(float* param);
	bool bindParamBool(bool* param);
	bool bindParamText(const wchar_t* text);

	/// FetchRow�� �����Ͱ� �ִٸ� �ݵ�� ���ʴ�� ȣ���ؾ���
	void bindResultColumnInt(int* r);
	void bindResultColumnFloat(float* r);
	void bindResultColumnBool(bool* r);
	void bindResultColumnText(wchar_t* text, size_t count);

	//TODO: log�� ���⵵�� �ٲ� ��
	void printSqlStmtError();

private:
	int id;
	SQLHSTMT	currentSqlHstmt = nullptr;

	SQLSMALLINT	currentResultCol;
	SQLSMALLINT	currentBindParam;

	static SQLHENV sqlHenv;
	static SQL_CONN* sqlConnPool; ///< ��Ŀ���������ŭ
	static int dbWorkerThreadCount;

};