// SQLUser.cpp: implementation of the CSQLUser class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cip_dbproxy.h"
#include "SQLUser.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CCip_dbproxyApp	theApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSQLUser::CSQLUser()
{

}

CSQLUser::~CSQLUser()
{

}

int CSQLUser::AddUser(const char* szLogin, const char* szPassw, int nLevel, const char* szName, const char* szDesc)
{
	char szSql[2048];
	int nResult;

	sprintf(szSql, "INSERT INTO users (id, Login, Passw, Level, Name, Description) VALUES (DEFAULT, '%s', '%s', %u, '%s', '%s')", 
		szLogin, szPassw, nLevel, szName, szDesc);
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}

	int nID = (int)mysql_insert_id(theApp.m_sql);
	return nID;
}

int CSQLUser::GetUserIdAndNames(CArray<int,int>& arIds, CStringArray& arNames)
{
	char szSql[2048];
	int nResult;
	MYSQL_RES* result;
	MYSQL_ROW row;
	char szName[64];

	sprintf(szSql, "SELECT id, Name FROM users ORDER BY Name");
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}
	result = mysql_store_result(theApp.m_sql);
	while(row = mysql_fetch_row(result))
	{
		int nID = atoi(row[0]);
		strcpy(szName, row[1]);

		arIds.Add(nID);
		arNames.Add(szName);
	}
	mysql_free_result(result); 
	return arIds.GetSize();
}

int CSQLUser::Login(const char* szLogin, const char* szPswd)
{
	char szSql[2048];
	int nResult;
	MYSQL_RES* result;
	MYSQL_ROW row;
	char szPassw[128];

	sprintf(szSql, "SELECT id, Passw FROM users WHERE Login = '%s'", szLogin);
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return -1;
	}

	result = mysql_store_result(theApp.m_sql);
	int nRows = (int)mysql_num_rows(result);

	// No such user
	if(nRows != 1)
	{
		mysql_free_result(result);
		return -1;
	}

	row = mysql_fetch_row(result);
	int nId = atoi(row[0]);
	strcpy(szPassw, row[1]);

	// Wrong password
	mysql_free_result(result);
	if(strcmp(szPassw, szPswd) != 0)
		return -2;

	return nId;	
}

int CSQLUser::GetLevel(int nUserId)
{
	char szSql[2048];
	int nResult;
	MYSQL_RES* result;
	MYSQL_ROW row;

	sprintf(szSql, "SELECT Level FROM users WHERE id = '%u'", nUserId);
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return -1;
	}

	result = mysql_store_result(theApp.m_sql);
	int nRows = (int)mysql_num_rows(result);

	// No such user
	if(nRows != 1)
	{
		mysql_free_result(result);
		return -1;
	}

	row = mysql_fetch_row(result);
	int nLevel = atoi(row[0]);

	return nLevel;
}

int CSQLUser::ChangePassword(const char* szName, const char* szPwdOld, const char* szPwd)
{
	int nId = Login(szName, szPwdOld);
	if(nId < 0) // error while loggin in (nId contains error code)
		return nId;

	char szSql[2048];
	int nResult;

	sprintf(szSql, "UPDATE users SET Passw = '%s' WHERE id = '%u'", 
		szPwd, nId);
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return -1;
	}

	return 1;
}