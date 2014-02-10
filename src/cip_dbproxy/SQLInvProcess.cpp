// SQLInvProcess.cpp: implementation of the CSQLInvProcess class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cip_dbproxy.h"
#include "SQLInvProcess.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CCip_dbproxyApp	theApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSQLInvProcess::CSQLInvProcess()
{

}

CSQLInvProcess::~CSQLInvProcess()
{

}

int CSQLInvProcess::Add(int nCompanyId, int nStageId)
{
	char szSql[2048];
	int nResult;

	sprintf(szSql, "INSERT INTO invprocess (id, CompanyId, StageId) VALUES (DEFAULT, %u, %u)", 
		nCompanyId, nStageId);
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

int CSQLInvProcess::Delete(int nCompanyId, int nStageId)
{
	char szSql[2048];
	int nResult;

	sprintf(szSql, "Delete from invprocess WHERE CompanyId = %u AND StageId = %u ", 
		nCompanyId, nStageId);
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}
	return 1;
}

int CSQLInvProcess::Get(int nStageId, CArray<int,int>& arCompIds, CStringArray& arTickers)
{
	char szSql[2048];
	int nResult;
	MYSQL_RES* result;
	MYSQL_ROW row;
	char szTicker[8];

	sprintf(szSql, "SELECT invprocess.CompanyId as id, companies.Ticker as Ticker FROM invprocess inner join companies on invprocess.CompanyId=companies.id where invprocess.StageId=%u",
		nStageId);
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
		strcpy(szTicker, row[1]);

		arCompIds.Add(nID);
		arTickers.Add(szTicker);
	}
	mysql_free_result(result); 
	return 1;
}
