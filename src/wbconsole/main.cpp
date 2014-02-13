#include <stdio.h>
#include <stdlib.h>
#include <afx.h>
#include <iostream>
using namespace std;

#include "stdafx.h" 

#include "json.h"
#include "cgi/cgi.h"

typedef unsigned int           SOCKET;
#include <mysql.h>

#define NUM_YEARS_FOR_EPS			11
#define NUM_YEARS_FOR_ROC			13
#define _INF_	((double)(0xffffffff-1))

/////////////////////////////
MYSQL*		connecttodb(const char* server, const char* db, char* err);
CString		GetCompanyNameByID(MYSQL*	m_sql, int nId);
int			GetCompanyAutoFill(MYSQL* m_sql, const char* sz, CArray<int,int>& ids, CStringArray& tickers, CStringArray& names);
int			GetPriceValueHistory(MYSQL* m_sql, const int nId, 
						 CStringArray& dates, CStringArray& prices, CStringArray& nepv, CStringArray& repv);
int			GetReturns(MYSQL* m_sql, const int nId, CArray<int,int>& years, 
						 CArray<double,double>& eps, CArray<double,double>& roe, CArray<double,double>& roc);


////////////////////////////
int main(void)
{
	// Always set the return type to JSON
	printf("Content-Type: application/json\n\n");

	// Initialize CGI parser object
	rude::CGI cgi;

	// get cmd var
	const char* szCmd = cgi.value("cmd");

	//char szServer[] = "10.20.26.115";
	char szServer[] = "localhost";
	char szErr[256];
	MYSQL*	m_sql = ::connecttodb(szServer, "cip_v2"/*"cip_universe"*/, szErr);
	if(!m_sql)
	{
		CJson js; 
		js.Begin();
		js.AddString("Status","Failed");
		js.AddString("Ret", szErr);
		js.End();
		return 0;
	}

	// auto fill company list by Ticker or Name
	if(strcmp(szCmd, "autofill")==0) 
	{
		const char* sz = cgi.value("txt");

		CArray<int,int> ids;
		CStringArray tickers;
		CStringArray names;
		int nCount = GetCompanyAutoFill(m_sql, sz, ids, tickers, names);
		
		CJson js; 
		js.Begin();
		js.AddString("Status","OK");
		
		CJson jsRet;
		jsRet.BeginArray();
		for(int i=0; i<nCount; i++)
		{
			CJson js1; js1.Begin();
			js1.AddInt("id",ids[i]);
			js1.AddString("ticker",tickers[i]);
			js1.AddString("name",names[i]);
			js1.End();
			
			jsRet.Add(js1.GetStr());
		}
		jsRet.EndArray();

		js.Add("Ret", jsRet.GetStr());
		js.End();

		printf(js.GetStr());
		return 0;
	}

	// load company 
	if(strcmp(szCmd, "loadcomp")==0) 
	{
		const char* sz = cgi.value("id");
		int nId = atoi(sz);

		CString name = GetCompanyNameByID(m_sql, nId);
		
		CJson js; 
		js.Begin();
		js.AddString("Status","OK");
		js.AddString("Ret", name);
		js.End();

		printf(js.GetStr());
		return 0;
	}

	// get price, nepv, repv history
	if(strcmp(szCmd, "gethist")==0)
	{
		const char* sz = cgi.value("id");
		int nId = atoi(sz);

		CStringArray dates; 
		CStringArray prices;
		CStringArray nepv;
		CStringArray repv;
		int nCount = GetPriceValueHistory(m_sql, nId, dates, prices, nepv, repv);

		CJson js;
		js.BeginArray();
		
		CJson js1;
		js1.BeginArray();
		js1.Add("\"Date\""); js1.Add("\"Price\""); js1.Add("\"NEPV\""); js1.Add("\"REPV\"");
		js1.EndArray();

		js.Add(js1.GetStr());
		for(int i=0; i<nCount; i++)
		{
			char szDate[16]; sprintf(szDate, "\"%s\"", dates[i]);
			CJson js2;
			js2.BeginArray();
			js2.Add(szDate); js2.Add(prices[i]); js2.Add(nepv[i]); js2.Add(repv[i]);
			js2.EndArray();

			js.Add(js2.GetStr());
		}
		js.EndArray();

		CJson jsAll;
		jsAll.Begin();
		jsAll.AddString("Status","OK");
		jsAll.Add("Ret", js.GetStr());
		jsAll.End();

		printf(jsAll.GetStr());
		return 0;
	}

	// get price, nepv, repv history
	if(strcmp(szCmd, "getepsroeroc")==0)
	{
		const char* sz = cgi.value("id");
		int nId = atoi(sz);

		CArray<int,int> years;
		CArray<double,double> eps; 
		CArray<double,double> roe; 
		CArray<double,double> roc;
		int nCount = GetReturns(m_sql, nId, years, eps, roe, roc);

		CJson jsAll;
		jsAll.Begin();
		
		// eps table
		CJson js, js1;
		js.BeginArray();
		js1.BeginArray();
		js1.Add("\"Year\""); js1.Add("\"EPS\"");
		js1.EndArray();

		js.Add(js1.GetStr());
		for(int i=0; i<nCount; i++)
		{
			CJson js2;
			js2.BeginArray();
			if(i==nCount-2)
				js2.Add("\"LTM\"");
			else
				js2.AddIntQ(NULL, years[i]); 
			
			js2.AddDouble(NULL, eps[i]);
			js2.EndArray();

			js.Add(js2.GetStr());
		}
		js.EndArray();
		jsAll.Add("EPS", js.GetStr());

		// roe, roc
		js.Clear();
		js1.Clear();

		js.BeginArray();
		js1.BeginArray();
		js1.Add("\"Year\""); js1.Add("\"ROE\""); js1.Add("\"ROC\"");
		js1.EndArray();

		js.Add(js1.GetStr());
		for(int i=0; i<nCount; i++)
		{
			CJson js2;
			js2.BeginArray();
			js2.AddIntQ(NULL, years[i]); js2.AddDouble(NULL, roe[i]); js2.AddDouble(NULL, roc[i]);
			js2.EndArray();

			js.Add(js2.GetStr());
		}
		js.EndArray();
		jsAll.Add("ROEROC", js.GetStr());
		
		jsAll.AddString("Status","OK");
		jsAll.End();

		printf(jsAll.GetStr());
		return 0;
	}

	printf("{ \"Status\": \"OK\",   \"Ret\": \"%s\"}", szCmd);
	return 0;
}

MYSQL* connecttodb(const char* server, const char* db, char* err)
{
	MYSQL*	sql;
	if( !(sql = mysql_init((MYSQL*)0)) )
	{
		strcpy(err, "Unable to init mysql engine.");
		return 0;
	}

	if(!mysql_real_connect( sql, server, "root"/*"cip_client"*/, "root"/*"cipclientresearch"*/, "cip_v2"/*NULL*/, MYSQL_PORT, NULL, 0 ))
	{
		strcpy(err, "Unable to connect to MySQL server.");
		return 0;
	}

	/*sql->reconnect= 1;
	if(mysql_select_db( sql, db ) != 0)
	{
		strcpy(err, "Unable to connect to database.");
		return 0;
	}*/
	
	return sql;
}

int sql2usdate(const char* szSqlDate, char* szUSDate)
{
	if(strlen(szSqlDate) != 10)	// invalid string
		return 0;

	if(szSqlDate[4] != '-' || szSqlDate[7] != '-')
		return 0;

	char szYear[5]; 
	strncpy(szYear, szSqlDate, 4); szYear[4] = '\0';

	char szMonth[3];
	strncpy(szMonth, &szSqlDate[5], 2); szMonth[2] = '\0';

	char szDay[3];
	strncpy(szDay, &szSqlDate[8], 2); szDay[2] = '\0';

	sprintf(szUSDate, "%s/%s/%s", szMonth,szDay, szYear);
	return 1;
}

int GetReturns(MYSQL* m_sql, const int nId, CArray<int,int>& years, 
			CArray<double,double>& eps, CArray<double,double>& roe, CArray<double,double>& roc)
{
	// sql query string and result
	char szSql[2048];
	int nResult;
	int nRows;
	MYSQL_RES* result;
	MYSQL_ROW row;

	// get last reported year
	sprintf(szSql, "select LastReport from companies where id=%u", nId);
	nResult = mysql_real_query(m_sql, szSql, strlen(szSql));
	if(nResult != 0)
		return 0;

	result = mysql_store_result(m_sql);
	nRows = (int)mysql_num_rows(result);

	// get the ID
	if(nRows != 1)
	{
		mysql_free_result(result);
		return 0;
	}
	row = mysql_fetch_row(result);
	int nCurrFY = atoi(row[0]);
	mysql_free_result(result);

	++ nCurrFY;
	int nYStart = nCurrFY - NUM_YEARS_FOR_EPS;
	int nYEnd = nYStart + NUM_YEARS_FOR_ROC - 1;

	sprintf(szSql,	"SELECT Year, EPS, ROC, ROE FROM epsrocroe \
					 WHERE CompanyID=%u AND Year >= '%u' AND Year <= '%u' \
					 ORDER BY Year ASC", 
			nId, nYStart, nYEnd);

	nResult = mysql_real_query(m_sql, szSql, strlen(szSql));
	if(nResult != 0)
		return 0;
	
	result = mysql_store_result(m_sql);
	nRows = (int)mysql_num_rows(result);
	while(row = mysql_fetch_row(result))
	{
		years.Add(atoi(row[0]));
		eps.Add(atof(row[1]));
		roc.Add(atof(row[2]));
		roe.Add(atof(row[3]));
	}
	mysql_free_result(result);

	// LTM EPS from balancesheets
	sprintf(szSql,	"SELECT EpsLast12Months, EpsNextYearForecast FROM balancesheets \
					 WHERE CompanyID=%u \
					 ORDER BY DateReport DESC LIMIT 1", nId);

	nResult = mysql_real_query(m_sql, szSql, strlen(szSql));
	if(nResult != 0)
		return 0;
	
	result = mysql_store_result(m_sql);
	nRows = (int)mysql_num_rows(result);
	if(nRows != 1)
	{
		mysql_free_result(result);
		return 0;
	}
	row = mysql_fetch_row(result);
	eps[eps.GetSize()-2] = atof(row[0]);
	eps[eps.GetSize()-1] = atof(row[1]);
	mysql_free_result(result);

	return eps.GetSize();
}

int GetPriceValueHistory(MYSQL* m_sql, const int nId, 
						 CStringArray& dates, CStringArray& prices, CStringArray& nepv, CStringArray& repv)
{
	// sql query string and result
	char szSql[2048];
	int nResult;
	int nRows;
	MYSQL_RES* result;
	MYSQL_ROW row;

	sprintf(szSql, "SELECT ItemDate, Price, NEPV, REPV FROM history WHERE CompanyId=%u ORDER BY ItemDate", nId);
	nResult = mysql_real_query(m_sql, szSql, strlen(szSql));
	if(nResult != 0)
		return 0;

	result = mysql_store_result(m_sql);
	nRows = (int)mysql_num_rows(result);

	char szDate[16];
	for(int i=0; i<nRows; i++)
	{
		row = mysql_fetch_row(result);

		sql2usdate(row[0], szDate);
		dates.Add(szDate);
		prices.Add(row[1]);
		nepv.Add(row[2]);
		repv.Add(row[3]);
	}

	mysql_free_result(result);
	return nRows;
}

CString GetCompanyNameByID(MYSQL* m_sql, int nId)
{
	// sql query string and result
	char szSql[2048];
	int nResult;
	int nRows;
	MYSQL_RES* result;
	MYSQL_ROW row;

	CString sName = "N/A";
//	CString sDate = m_Date.Format("%Y-%m-%d");
	//sprintf(szSql, "SELECT id FROM companies WHERE Ticker='%s' AND DateBirth<='%s' AND DateDeath>'%s'", szTicker, sDate, sDate);
	sprintf(szSql, "SELECT name FROM companies WHERE id='%u'", nId);
	nResult = mysql_real_query(m_sql, szSql, strlen(szSql));
	if(nResult != 0)
		return sName;

	result = mysql_store_result(m_sql);
	nRows = (int)mysql_num_rows(result);

	// get the ID
	if(nRows == 1)
	{
		row = mysql_fetch_row(result);
		sName = row[0];
	}

	mysql_free_result(result);
	return sName;
}

int GetCompanyAutoFill(MYSQL* m_sql, const char* sz, CArray<int,int>& ids, CStringArray& tickers, CStringArray& names)
{
	// sql query string and result
	char szSql[2048];
	int nResult;
	int nRows;
	MYSQL_RES* result;
	MYSQL_ROW row;

//	CString sDate = m_Date.Format("%Y-%m-%d");
	//sprintf(szSql, "SELECT id FROM companies WHERE Ticker='%s' AND DateBirth<='%s' AND DateDeath>'%s'", szTicker, sDate, sDate);
	sprintf(szSql, "SELECT id, Ticker, Name FROM companies WHERE (Ticker LIKE '%s%%' OR Name LIKE '%s%%')", sz, sz);
	nResult = mysql_real_query(m_sql, szSql, strlen(szSql));
	if(nResult != 0)
		return 0;

	result = mysql_store_result(m_sql);
	nRows = (int)mysql_num_rows(result);

	// get the ID
	for(int i=0; i<nRows; i++)
	{
		row = mysql_fetch_row(result);
		ids.Add(atoi(row[0]));
		tickers.Add(row[1]);
		names.Add(row[2]);
	}

	mysql_free_result(result);
	return nRows;
}

/*int parseQueryString(const char* szQuery, CListEx<CString>& list);
int main(void) 
{

	rude::CGI cgi;
	const char* sCmd = cgi.value("cmd2");

	char* data = getenv("QUERY_STRING"); //"cmd=mult&n1=2&n2=3_=122333";
	char* cookie = getenv("HTTP_COOKIE");
	double d1,d2;

	CListEx<CString> _GET;

	// set the response type as json
	printf("Content-Type: application/json\n\n");

	if(data == NULL) 
	{
		printf("{ \"Status\": \"Failed\",   \"Ret\": \"Error passing QUERY_STRING\"}");
		return 0;
	}

	parseQueryString(data, _GET);
	char* cmd = (char*)_GET["cmd"];
	
	if(strcmp(cmd, "mult")==0) 
	{
		char* sd1 = (char*)_GET["n1"];
		char* sd2 = (char*)_GET["n2"];

		if(sd1==NULL || sscanf(sd1,"%lf", &d1)!=1 || sd2==NULL || sscanf(sd2,"%lf", &d2)!=1)
		{
			printf("{ \"Status\": \"Failed\",  \"Ret\": \"Data must be numeric\", \"D1\": \"%s\"}", sd2);
			return 0;
		} 
		double d3 = d1*d2;
		printf("{ \"Status\": \"OK\",  \"Ret\": %f }", d3);
		return 0;
	}

	printf("{ "
		"\"Status\": \"OK\", "
		"\"Cookies\": \"%s\", "
		"\"GET\": \"%s\", "
		"\"IntVal\": 1234, "
		"\"Num1\": %f, "
		"\"Num2\": %f }", 
	
	cookie!=NULL ? cookie : "", cmd, d1, d2);	
	return 0;
}

int parseQueryString(const char* szQuery, CListEx<CString>& list)
{
	char szKey[32];
	char* ch = (char*)szQuery;
	char* ch0 = ch;
	while(*ch != '\0')
	{
		if(*ch == '=')
		{
			size_t n = (size_t)(ch-ch0);
			strncpy(szKey, ch0, n);
			szKey[n] = '\0';
			ch++; ch0=ch;
		}
		else if(*ch == '&')
		{
			size_t n = (size_t)(ch-ch0);
			char* szVal = (char*)malloc(n+1);
			strncpy(szVal, ch0, n);
			szVal[n] = '\0';
			ch++; ch0=ch;
			
			if(strlen(szKey) > 0)
				list.Add(szKey, szVal);
			else 
				free(szVal);
			szKey[0]='\0';
		}
		++ch;
	}

	size_t n = (size_t)(ch-ch0);
	char* szVal = (char*)malloc(n+1);
	strncpy(szVal, ch0, n);
	szVal[n] = '\0';
	
	if(strlen(szKey) > 0)
		list.Add(szKey, szVal);
	else 
		free(szVal);

	return 1;
}

int clearGetList(CListEx<CString>& list)
{
	int nCount = list.GetSize();
	for(int i=0; i<nCount; i++)
	{
		CListExItem<CString>* pItem = list.GetItem(i);
		free(pItem->m_pData);
	}

	list.RemoveAll();
	return 1;
}

int _unicode2utf8(const unsigned char* szInput, unsigned char* szOut) 
{
	unsigned char cMask1 = 0x8;

	unsigned char* ch = (unsigned char*)szInput;
	unsigned char ch2;
	while(*ch != '\0')
	{
		// the leading bit 0xxx - one byte char
		if((*ch & 0x1F) == 0)
			ch2 = *ch;
		else

		++ch;
	}

	return 1;
}

*/
