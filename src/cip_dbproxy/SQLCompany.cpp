// SQLCompany.cpp: implementation of the CSQLCompany class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cip_dbproxy.h"
#include "SQLCompany.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CCip_dbproxyApp	theApp;

COleDateTime CSQLCompany::m_Date = COleDateTime::GetCurrentTime();

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSQLCompany::CSQLCompany()
{

}

CSQLCompany::~CSQLCompany()
{

}

CString CSQLCompany::GetLastError()
{
	return theApp.m_sError;
}

COleDateTime CSQLCompany::GetDate()				
{	
	return m_Date;		
}

void CSQLCompany::SetDate(COleDateTime date)
{
	m_Date = date;
}

COleDateTime CSQLCompany::ConvertSqlToOleDate(const char* szSqlDate)
{
	COleDateTime date;
	date.SetStatus(COleDateTime::null);

	if(strlen(szSqlDate) != 10)	// invalid string
		return date;

	if(szSqlDate[4] != '-' || szSqlDate[7] != '-')
		return date;

	char szYear[5]; 
	strncpy(szYear, szSqlDate, 4); szYear[4] = '\0';
	int nYear = atoi(szYear);

	char szMonth[3];
	strncpy(szMonth, &szSqlDate[5], 2); szMonth[2] = '\0';
	int nMonth = atoi(szMonth);

	char szDay[3];
	strncpy(szDay, &szSqlDate[8], 2); szDay[2] = '\0';
	int nDay = atoi(szDay);

	date = COleDateTime(nYear, nMonth, nDay, 0,0,0);
	return date;
}

COleDateTime CSQLCompany::ConvertUSToOleDate(const char* szUSDate)
{
	COleDateTime date;
	date.SetStatus(COleDateTime::null);

	if(strlen(szUSDate) != 10)	// invalid string
		return date;

	if(szUSDate[2] != '/' || szUSDate[5] != '/')
		return date;

	char szMonth[3];
	strncpy(szMonth, szUSDate, 2); szMonth[2] = '\0';
	int nMonth = atoi(szMonth);

	char szDay[3];
	strncpy(szDay, &szUSDate[3], 2); szDay[2] = '\0';
	int nDay = atoi(szDay);

	char szYear[5]; 
	strncpy(szYear, &szUSDate[6], 4); szYear[4] = '\0';
	int nYear = atoi(szYear);

	date = COleDateTime(nYear, nMonth, nDay, 0,0,0);
	return date;
}

int CSQLCompany::AddCompany(const char* szTicker, const char* szName, const char* szIndustry, const char* szSector, 
							 double fEPSMultiply, double fEPSAdd)
{
	if(!theApp.m_bConnected)
		return false;

	// sql query string and result
	char szSql[2048];
	int nResult;
	int nRows;
	MYSQL_RES* result;
	MYSQL_ROW row;

	// get industry id (or add if doesn't exist)
	unsigned int nIndustryID;
	sprintf(szSql, "SELECT id FROM industries WHERE Name='%s'", szIndustry);
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);

	// get the ID
	if(nRows == 1)
	{
		row = mysql_fetch_row(result);
		nIndustryID = atoi(row[0]);
	}
	else
	{
		sprintf(szSql, "INSERT INTO industries (id, Name, Description) VALUES (DEFAULT, '%s', '%s')", szIndustry, "");
		nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
		nIndustryID = (unsigned int)mysql_insert_id(theApp.m_sql);
	}
	mysql_free_result(result);

	// get sector id (or add if doesn't exist)
	unsigned int nSectorID;
	sprintf(szSql, "SELECT id FROM sectors WHERE Name='%s'", szSector);
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);

	// get the ID
	if(nRows == 1)
	{
		row = mysql_fetch_row(result);
		nSectorID = atoi(row[0]);
	}
	else
	{
		sprintf(szSql, "INSERT INTO sectors (id, Name, Description) VALUES (DEFAULT, '%s', '%s')", szSector, "");
		nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
		nSectorID = (unsigned int)mysql_insert_id(theApp.m_sql);
	}
	mysql_free_result(result);

	// Add the company
	sprintf(szSql, "INSERT INTO companies (id, Ticker, Name, IndustryID, SectorID, EPSMultiply, EPSAdd) VALUES (DEFAULT, '%s', '%s', %u, %u, %f, %f)",
			szTicker, szName, nIndustryID, nSectorID, fEPSMultiply, fEPSAdd);
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

int CSQLCompany::AddCompany(const char* szTicker, const char* szName, 
 							int nIndustryID, int nSectorID, const char* szCIQTicker, int nType, int nLastYear,
							BOOL bLoadBS, BOOL bLoadEPS, BOOL bLoadROCROE, BOOL bUseDebtAdjust, double* weights, COleDateTime date)
{
	char szSql[2048];
	int nResult;

	CString sWeights;
	for(int i=0; i<NUM_WEIGHTS; i++)
	{
		CString sItem;
		sItem.Format(", %s = '%f'", WeightsIndices[i], weights[i]);
		sWeights += sItem;
	}

	CString sDate = date.Format("%Y-%m-%d");
	sprintf(szSql, "INSERT INTO companies SET \
									id = DEFAULT, \
									Ticker = '%s', \
									Name = '%s', \
									IndustryID = '%u', \
									SectorID = '%u', \
									CIQTicker = '%s', \
									Type = '%u', \
									LastReport = '%u', \
									LoadBS = '%u', \
									LoadEPS = '%u', \
									LoadROCROE = '%u', \
									UseDebtAdjust = '%u', \
									DateBirth = '%s' \
									%s",

			szTicker, szName, nIndustryID, nSectorID, szCIQTicker, nType, nLastYear, bLoadBS, bLoadEPS, bLoadEPS, bUseDebtAdjust, sDate, sWeights);

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

BOOL CSQLCompany::UpdateCompanyBasicInfo(const char* szTicker, const char* szNewTicker, const char* szName, 
		int nIndustryID, int nSectorID, const char* szCIQTicker, int nType, int nLastYear, 
		BOOL bLoadBS, BOOL bLoadEPS, BOOL bLoadROCROE, BOOL bUseDebtAdjust, double* weights)
{
	char szSql[2048];
	int nResult;

	CString sWeights;
	for(int i=0; i<NUM_WEIGHTS; i++)
	{
		CString sLine;
		if(weights[i] > 0)
			sLine.Format(", `%s`=%f ", WeightsIndices[i], weights[i]);
		else 
			sLine.Format(", `%s`=null ", WeightsIndices[i]);

		sWeights += sLine;
	}

	sprintf(szSql, "UPDATE companies SET \
									Ticker = '%s', \
									Name = '%s', \
									IndustryID = '%u', \
									SectorID = '%u', \
									CIQTicker = '%s', \
									Type = '%u', \
									LastReport = '%u', \
									LoadBS = '%u', \
									LoadEPS = '%u', \
									LoadROCROE = '%u', \
									UseDebtAdjust = '%u' \
									%s \
					WHERE Ticker='%s'",

			szNewTicker, szName, nIndustryID, nSectorID, szCIQTicker, nType, nLastYear, bLoadBS, bLoadEPS, bLoadROCROE, bUseDebtAdjust, sWeights, szTicker);

	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		//theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}

	return true;
}

BOOL CSQLCompany::DeleteCompany(const char* szTicker, COleDateTime date)
{
	char szSql[2048];
	int nResult;
	
	//sprintf(szSql, "DELETE FROM companies WHERE Ticker='%s'", szTicker);
	sprintf(szSql, "UPDATE companies SET DateDeath='%s' WHERE Ticker='%s'", date.Format("%Y-%m-%d"), szTicker);
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		//theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return false;
	}
	
	return true;	
}

int CSQLCompany::AddIndustry(const char* szName, const char* szDescription)
{
	char szSql[2048];
	int nResult;

	sprintf(szSql, "INSERT INTO industries (id, Name, Description) VALUES (DEFAULT, '%s', '%s')", szName, szDescription);
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

int CSQLCompany::AddSector(const char* szName, const char* szDescription)
{
	char szSql[2048];
	int nResult;

	sprintf(szSql, "INSERT INTO sectors (id, Name, Description) VALUES (DEFAULT, '%s', '%s')", szName, szDescription);
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

int CSQLCompany::GetIndustryIdsAndNames(CArray<int,int> &arIds, CStringArray &arNames)
{
	char szSql[2048];
	int nResult;
	MYSQL_RES* result;
	MYSQL_ROW row;

	sprintf(szSql, "SELECT id, Name FROM industries ORDER BY Name");
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
		arIds.Add(atoi(row[0]));
		arNames.Add(row[1]);
	}

	mysql_free_result(result);

	int nCount = arNames.GetSize();
	return nCount;
}

int CSQLCompany::GetSectorIdsAndNames(CArray<int,int> &arIds, CStringArray &arNames)
{
	char szSql[2048];
	int nResult;
	MYSQL_RES* result;
	MYSQL_ROW row;

	sprintf(szSql, "SELECT id, Name FROM sectors ORDER BY Name");
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
		arIds.Add(atoi(row[0]));
		arNames.Add(row[1]);
	}

	mysql_free_result(result);

	int nCount = arNames.GetSize();
	return nCount;
}

BOOL CSQLCompany::SetIndustryName(int nID, const char* szName)
{
	char szSql[2048];
	int nResult;
	
	sprintf(szSql, "UPDATE industries SET Name='%s' WHERE id='%u'", szName, nID);
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return false;
	}
	
	return true;
}

BOOL CSQLCompany::SetSectorName(int nID, const char* szName)
{
	char szSql[2048];
	int nResult;
	
	sprintf(szSql, "UPDATE sectors SET Name='%s' WHERE id='%u'", szName, nID);
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return false;
	}
	
	return true;
}

BOOL CSQLCompany::DeleteIndustry(int nID)
{
	char szSql[2048];
	int nResult;
	
	sprintf(szSql, "DELETE FROM industries WHERE id='%u'", nID);
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return false;
	}
	
	return true;
}

BOOL CSQLCompany::DeleteSector(int nID)
{
	char szSql[2048];
	int nResult;
	
	sprintf(szSql, "DELETE FROM sectors WHERE id='%u'", nID);
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return false;
	}
	
	return true;
}

BOOL CSQLCompany::GetExtraInfoByID(const int nID, unsigned int &nType, BOOL& bUpdateBS, BOOL& bUpdateEPS, BOOL& bUpdateROCROE, BOOL& bUseDebtAdjust, unsigned int& nReportYear)
{
	// sql query string and result
	char szSql[2048];
	int nResult;
	int nRows;
	MYSQL_RES* result;
	MYSQL_ROW row;

	sprintf(szSql, "SELECT Type, LoadBS, LoadEPS, LoadROCROE, UseDebtAdjust, LastReport FROM companies WHERE id='%u'", nID);
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		//theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);

	// get the ID
	if(nRows == 1)
	{
		row = mysql_fetch_row(result);
		nType = (unsigned int)atoi(row[0]);
		bUpdateBS = (BOOL)atoi(row[1]);
		bUpdateEPS = (BOOL)atoi(row[2]);
		bUpdateROCROE = (BOOL)atoi(row[3]);
		bUseDebtAdjust = (BOOL)atoi(row[4]);
		nReportYear = (int)atoi(row[5]);
	}

	mysql_free_result(result);
	return true;
}

int CSQLCompany::GetCompanyIDAndTickers(CArray<int,int> &arIds, CStringArray &arTickers, COleDateTime date)
{
	// sql query string and result
	char szSql[2048];
	int nResult;
	MYSQL_RES* result;
	MYSQL_ROW row;

	CString sTicker;

	CString sDate = date.Format("%Y-%m-%d"); 
	sprintf(szSql, "SELECT id, Ticker FROM companies WHERE DateBirth<='%s' AND DateDeath>'%s' AND (Univ_C800='1' OR Univ_ADR='1') AND Ticker >= '' ORDER BY Ticker", sDate, sDate);
	//sprintf(szSql, "SELECT id, Ticker FROM companies WHERE DateBirth<='%s' AND DateDeath>'%s' ORDER BY Ticker", sDate, sDate);
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
		CString sTicker = row[1];
		arIds.Add(nID);
		arTickers.Add(sTicker);

	}

	mysql_free_result(result);
	return arIds.GetSize();
}

CString CSQLCompany::GetCompanyTickerByID(const int nID)
{
	// sql query string and result
	char szSql[2048];
	int nResult;
	int nRows;
	MYSQL_RES* result;
	MYSQL_ROW row;

	CString sTicker;

	sprintf(szSql, "SELECT Ticker FROM companies WHERE id='%u'", nID);
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return sTicker;
	}
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);

	// get the Ticker
	if(nRows == 1)
	{
		row = mysql_fetch_row(result);
		sTicker = row[0];
	}

	mysql_free_result(result);
	return sTicker;
}

int CSQLCompany::GetCompanyIDByTicker(const char* szTicker)
{
	// sql query string and result
	char szSql[2048];
	int nResult;
	int nRows;
	MYSQL_RES* result;
	MYSQL_ROW row;

	int nID = 0;
	CString sDate = m_Date.Format("%Y-%m-%d");
	sprintf(szSql, "SELECT id FROM companies WHERE Ticker='%s' AND DateBirth<='%s' AND DateDeath>'%s'", szTicker, sDate, sDate);
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);

	// get the ID
	if(nRows == 1)
	{
		row = mysql_fetch_row(result);
		nID = atoi(row[0]);
	}

	mysql_free_result(result);
	return nID;
}

int CSQLCompany::GetCompanyIDByName(const char* szName)
{
	// sql query string and result
	char szSql[2048];
	int nResult;
	int nRows;
	MYSQL_RES* result;
	MYSQL_ROW row;

	int nID = 0;
	sprintf(szSql, "SELECT id FROM companies WHERE Name='%s'", szName);
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);

	// get the ID
	if(nRows == 1)
	{
		row = mysql_fetch_row(result);
		nID = atoi(row[0]);
	}

	mysql_free_result(result);
	return nID;
}

int CSQLCompany::GetIndustryIDByName(const char* szIndustry, BOOL bAddIfNotExist)
{
	char szSql[2048];
	int nResult;
	int nRows;
	MYSQL_RES* result;
	MYSQL_ROW row;

	unsigned int nIndustryID = 0;
	sprintf(szSql, "SELECT id FROM industries WHERE Name='%s'", szIndustry);
	
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);

	// get the ID
	if(nRows == 1)
	{
		row = mysql_fetch_row(result);
		nIndustryID = atoi(row[0]);
	}
	else if(bAddIfNotExist)
	{
		sprintf(szSql, "INSERT INTO industries (id, Name, Description) VALUES (DEFAULT, '%s', '%s')", szIndustry, "");
		nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
		nIndustryID = (unsigned int)mysql_insert_id(theApp.m_sql);
	}

	mysql_free_result(result);
	return nIndustryID;
}

int CSQLCompany::GetSectorIDByName(const char* szSector, BOOL bAddIfNotExist)
{
	// sql query string and result
	char szSql[2048];
	int nResult;
	int nRows;
	MYSQL_RES* result;
	MYSQL_ROW row;

	unsigned int nSectorID = 0;
	sprintf(szSql, "SELECT id FROM sectors WHERE Name='%s'", szSector);

	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);

	// get the ID
	if(nRows == 1)
	{
		row = mysql_fetch_row(result);
		nSectorID = atoi(row[0]);
	}
	else if(bAddIfNotExist)
	{
		sprintf(szSql, "INSERT INTO sectors (id, Name, Description) VALUES (DEFAULT, '%s', '%s')", szSector, "");
		nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
		nSectorID = (unsigned int)mysql_insert_id(theApp.m_sql);
	}
	mysql_free_result(result);
	return nSectorID;
}

BOOL CSQLCompany::UpdateUniverseWeightsInIndices(SUniverseWeightsInIndices& UniverseData)
{
	int i;
	for(i=0; i<UniverseData.GetSize(); i++)
	{
		if(!UpdateCompanyWeightsInIndices(UniverseData[i]))
		{
			// handle error
		}
	}

	return true;
}

BOOL CSQLCompany::UpdateCompanyWeightsInIndices(SCompanyWeightsInIndices& data)
{
	int nID = GetCompanyIDByTicker(data.sTicker);
	if(!nID)
	{
		if(data.data[3] > 0)  // In SP500 but not in universe
		{
			CString sMessage;
			sMessage.Format("Attention: The company '%s' is in S&P500 index, but not in the current Universe.\n\nTo add the company into the Universe open Add/Edit Companies Tab and input the appropriate values.", data.sTicker);
			AfxMessageBox(sMessage, MB_ICONINFORMATION);
		}
		return false;
	}

	CString sSql;
	int nResult;

	CString sSet; 
	BOOL bFirst=true;
	for(int i=0; i<NUM_WEIGHTS; i++)
	{
		CString sLine;
		if(data.data[i] > 0)
			sLine.Format("%s`%s`=%f ", bFirst ? "" : ", ", WeightsIndices[i], data.data[i]);
		else 
			sLine.Format("%s`%s`=null ", bFirst ? "" : ", ", WeightsIndices[i]);

		sSet += sLine;
		bFirst = false;
	}

	sSql.Format("UPDATE `companies` SET %s WHERE `id`=%u", sSet, nID);
	nResult = mysql_real_query(theApp.m_sql, sSql, sSql.GetLength());
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		//theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}

	return true;	
}

int CSQLCompany::UpdateUniverseFundamentals(SUniverseDataFundamentals& UniverseData, BOOL bForce)
{
	int i;
	int nUpdated = 0;
	theApp.m_sError.Empty();

	for(i=0; i<UniverseData.GetSize(); i++)
	{
		if(!UpdateCompanyFundamentals(UniverseData[i], bForce))
		{
			//theApp.m_sError += UniverseData[i].sTicker;
			//theApp.m_sError += "\n";
		}
		else
			++nUpdated;
	}

	return nUpdated;
}

BOOL CSQLCompany::UpdateCompanyFundamentals(SCompanyDataFundamentals& data, BOOL bForce)
{
	char szSql[2048];
	int nResult;

	if(data.dtReport.GetStatus() != COleDateTime::valid)
		return true;
	
	// clear strings for MySQL
	CString sName = PrepStrForSQL(data.sName);  
	CString sIndustry = PrepStrForSQL(data.sIndustry);
	CString sSector = PrepStrForSQL(data.sSector);

	// Get the company by the ticker or add new one
	BOOL bExisted = true;
	int nID = GetCompanyIDByTicker(data.sTicker);
	if(!nID)
	{
		nID = AddCompany(data.sTicker, sName, sIndustry, sSector, -1, -1);
		if(!nID)
		{
			// handle error
		}
		bExisted = false;
	}

	unsigned int nType, nLastYear; BOOL bUpdateBS, bUpdateEPS, bUpdateROCROE, bUseDebtAdjust;
	if(!GetExtraInfoByID(nID, nType, bUpdateBS, bUpdateEPS, bUpdateROCROE, bUseDebtAdjust, nLastYear))
	{
		// handle error
		return false;
	}

	if(!bForce && (!bUpdateBS && !bUpdateEPS))
		return true;

	// correct the extra info (the data that don't come from files)
 	if((int)data.type == -1)			data.type = nType;
	if((int)data.bLoadBS == -1)			data.bLoadBS = bUpdateBS;
	if((int)data.bLoadEPS == -1)		data.bLoadEPS = bUpdateEPS;
	if((int)data.bLoadROCROE == -1)		data.bLoadROCROE = bUpdateROCROE;
	if((int)data.bUseDebtAdjust == -1)	data.bUseDebtAdjust = bUseDebtAdjust;

	int nIndustryID = GetIndustryIDByName(sIndustry, true);
	int nSectorID = GetSectorIDByName(sSector, true);

	// update company info
	sprintf(szSql,	"UPDATE `companies` SET Name='%s', IndustryID='%u', SectorID='%u', " 
					"LoadBS=%u, LoadEPS=%u, LoadROCROE=%u, Type=%u, LastReport=%u, UseDebtAdjust=%u, IsInSP500=%u, RecentRollOver=%s WHERE id='%u'",
			sName, nIndustryID, nSectorID, data.bLoadBS, data.bLoadEPS, data.bLoadROCROE, data.type, 
			data.nLastReport, data.bUseDebtAdjust, data.bInSP, 
			(data.nLastReport == nLastYear) ? "RecentRollOver" : "1",
			nID);

	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		//theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return false;
	}

	static char szInsertTemplate[] = "INSERT INTO `balancesheets` SET CompanyID = '%u', DateReport = '%s', %s, %s ON DUPLICATE KEY UPDATE DateReport = '%s', %s, %s ";
	
	CString sBalanceSheet = "";
	CString sEps = "";
	if(bUpdateBS || bForce)
	{
		CString sSplitDate;
		if(data.dtSplit.GetStatus() == COleDateTime::valid)
			sSplitDate = data.dtSplit.Format("%Y-%m-%d");
		else
			sSplitDate = "0000-00-00";

		sBalanceSheet.Format("Shares = %f, NetPlant = %f, DeprAndAmort = %f, Assets = %f, Intangibles = %f, Liabilities = %f, \
							ShortTermDebt = %f, LongTermDebt = %f, CommonEquity = %f, Cash = %f, InterestExpense = %f, PreTaxIncome = %f, \
							Dividend = %f, BookValue = %f, SplitRatio = '%s', SplitDate = '%s'",
											
				data.fShares, 
				data.fFixed,
				data.fDeprAmort,
				data.fAssets,
				data.fIntang,
				data.fLiabilities,
				data.fShortDebt,
				data.fLongDebt,
				data.fCommonEquity,
				data.fCash,
				data.fInterest,
				data.fPreTaxIncome,
				data.fDiv,
				data.fBookVal,
				data.sSplit,
				sSplitDate);
	}
	else
		sBalanceSheet = "Shares = Shares, NetPlant = NetPlant, DeprAndAmort = DeprAndAmort, Assets = Assets, Intangibles = Intangibles, Liabilities = Liabilities, " \
						"ShortTermDebt = ShortTermDebt, LongTermDebt = LongTermDebt, CommonEquity = CommonEquity, Cash = Cash, InterestExpense = InterestExpense, " \
						"PreTaxIncome = PreTaxIncome, Dividend = Dividend, BookValue = BookValue, SplitRatio = SplitRatio, SplitDate = SplitDate";

	
	if(bUpdateEPS || bForce)
		sEps.Format("EpsLastYear = %f, EpsLast12Months = %f, EpsThisYearForecast = %f, EpsNextYearForecast = %f",
			data.fEpsLastYear,
			data.fEpsLast12Months,
			data.fEpsThisYForecast,
			data.fEpsNextYForecast);
	else
		sEps = "EpsLastYear = EpsLastYear, EpsLast12Months = EpsLast12Months, EpsThisYearForecast = EpsThisYearForecast, EpsNextYearForecast = EpsNextYearForecast";

	CString sDateReport = data.dtReport.Format("%Y-%m-%d");
	sprintf(szSql, szInsertTemplate, nID, 
		sDateReport, sBalanceSheet, sEps, 
		sDateReport, sBalanceSheet, sEps);

	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		//theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return false;
	}

	return true;
}

/*BOOL CSQLCompany::UpdateUniverseFundamentals(SUniverseDataFundamentals& UniverseData)
{
	int i;
	for(i=0; i<UniverseData.GetSize(); i++)
	{
		if(!UpdateCompanyFundamentals(UniverseData[i]))
		{
			// handle error
		}
	}

	return true;
}

BOOL CSQLCompany::UpdateCompanyFundamentals(SCompanyDataFundamentals& data)
{
	char szSql[2048];
	int nResult;

	// Get the company by the ticker or add new one
	int nID = GetCompanyIDByTicker(data.sTicker);
	if(!nID)
	{
		// handle error
		return false;
	}

	BOOL bUpdateBS, bUpdateEPS;
	if(!GetUpdateBSAndEPS(nID, bUpdateBS, bUpdateEPS))
	{
		// handle error
		return false;
	}

	if(!bUpdateBS)
		return true;

	// update company info
	if(data.type != -1)
	{
		sprintf(szSql, "UPDATE `companies` SET Name='%s', LoadBS=%u, LoadEPS=%u, Type=%u, LastReport=%u WHERE id='%u'",
				data.sName, data.bLoadBS, data.bLoadEPS, data.type, data.nLastReport, nID);
	}
	else
	{
		sprintf(szSql, "UPDATE `companies` SET Name='%s', LoadBS=%u, LoadEPS=%u, LastReport=%u WHERE id='%u'",
				data.sName, data.bLoadBS, data.bLoadEPS, data.nLastReport, nID);
	}
	
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return false;
	}

	// update/add to Balance Sheet
	static char szInsertTemplate[] =	"UPDATE `balancesheets` SET \
											\
											Shares = %f, \
											NetPlant = %f, \
											DeprAndAmort = %f, \
											Assets = %f, \
											Intangibles = %f, \
											Liabilities = %f, \
											ShortTermDebt = %f, \
											LongTermDebt = %f, \
											CommonEquity = %f, \
											Cash = %f, \
											InterestExpense = %f, \
											PreTaxIncome = %f, \
											EpsLastYear = %f, \
											EpsLast12Months = %f, \
											EpsThisYearForecast = %f, \
											EpsNextYearForecast = %f, \
											Dividend = %f, \
											BookValue = %f, \
											SplitRatio = '%s', \
											SplitDate = '%s' \
										WHERE \
											CompanyID = '%u'";

	CString sSplitDate;
	if(data.dtSplit.GetStatus() == COleDateTime::valid)
		sSplitDate = data.dtSplit.Format("%Y-%m-%d");
	else
		sSplitDate = "0000-00-00";

	sprintf(szSql, szInsertTemplate, 
		
		data.fShares,
		data.fFixed,
		data.fDeprAmort,
		data.fAssets,
		data.fIntang,
		data.fLiabilities,
		data.fShortDebt,
		data.fLongDebt,
		data.fCommonEquity,
		data.fCash,
		data.fInterest,
		data.fPreTaxIncome,
		data.fEpsLastYear,
		data.fEpsLast12Months,
		data.fEpsThisYForecast,
		data.fEpsNextYForecast,
		data.fDiv,
		data.fBookVal,
		data.sSplit,
		sSplitDate,
		
		nID);
	
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
 		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return false;
	}

	return true;
}*/

int	CSQLCompany::UpdateUniverseShares(SUniverseDataShares& UniverseData, BOOL bForce, BOOL bOnlyForRollOvers)
{
	int i;
	int nUpdated = 0;
	theApp.m_sError.Empty();

	for(i=0; i<UniverseData.GetSize(); i++)
	{
		if(!UpdateCompanyShares(UniverseData[i], bForce, bOnlyForRollOvers))
		{
			//theApp.m_sError += UniverseData[i].sTicker;
			//theApp.m_sError += "\n";
		}
		else
			++nUpdated;
	}

	return nUpdated;
}

BOOL CSQLCompany::UpdateCompanyShares(SCompanyDataShares& data, BOOL bForce, BOOL bOnlyForRollOvers)
{
	char szSql[2048];
	int nResult, i;

	int nID = GetCompanyIDByTicker(data.sTicker);
	if(!nID)
	{
		// handle error
		return false;
	}

	unsigned int nType, nLastYear; BOOL bUpdateBS, bUpdateEPS, bUpdateROCROE, bUseDebtAdjust;
	if(!GetExtraInfoByID(nID, nType, bUpdateBS, bUpdateEPS, bUpdateROCROE, bUseDebtAdjust, nLastYear))
	{
		// handle error
		return false;
	}
	
	//if(!bForce && !bUpdateROCROE)    ALWAYS UPDATE - There is skipping for shares (soon to be converted to FactSet)
	//	return true;

	unsigned int nYear0 = data.nYear0;
	int nStart = 0;
	if(bOnlyForRollOvers)
		nStart = (nYear0+10 != nLastYear) ? 0 : NUM_YEARS_FOR_EPS;

	// add shares data
	for(i=nStart; i<NUM_YEARS_FOR_EPS; i++)
	{
		int nYear = nYear0+i;

		CString sShares; if(data.arShares[i] < _INF_) sShares.Format("'%f'", data.arShares[i]); else sShares = "Shares";
		sprintf(szSql, "INSERT INTO `epsrocroe` SET \
								CompanyID=%u, \
								Year = %u, \
								Shares = %f \
						ON DUPLICATE KEY UPDATE \
								Shares = %s",
				
				nID,  nYear, data.arShares[i],sShares);
		
		nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
		if(nResult != 0)
		{
			const char* err = mysql_error(theApp.m_sql);
			//theApp.m_sError.Format(_T("MySql Error: %s"), err);
			// handle error
		}
	}

	return true;
}

int CSQLCompany::UpdateUniverseEpsRocRoe(SUniverseDataEpsRocRoe& UniverseData, BOOL bForce, BOOL bOnlyForRollOvers)
{
	int i;
	int nUpdated = 0;
	theApp.m_sError.Empty();

	for(i=0; i<UniverseData.GetSize(); i++)
	{
		if(!UpdateCompanyEpsRocRoe(UniverseData[i], bForce, bOnlyForRollOvers))
		{
			//theApp.m_sError += UniverseData[i].sTicker;
			//theApp.m_sError += "\n";
		}
		else
			++nUpdated;
	}

	return nUpdated;
}

BOOL CSQLCompany::UpdateCompanyEpsRocRoe(SCompanyDataEpsRocRoe& data, BOOL bForce, BOOL bOnlyForRollOvers)
{
	char szSql[2048];
	int nResult, i;

	int nID = GetCompanyIDByTicker(data.sTicker);
	if(!nID)
	{
		// handle error
		return false;
	}

	unsigned int nType, nLastYear; BOOL bUpdateBS, bUpdateEPS, bUpdateROCROE, bUseDebtAdjust;
	if(!GetExtraInfoByID(nID, nType, bUpdateBS, bUpdateEPS, bUpdateROCROE, bUseDebtAdjust, nLastYear))
	{
		// handle error
		return false;
	}
	
	if(!bForce && !bUpdateROCROE)
		return true;

	unsigned int nYear0 = data.nYear0;
	int nStart = 0;
	if(bOnlyForRollOvers)
		nStart = (nYear0+10 != nLastYear) ? 0 : NUM_YEARS_FOR_EPS;
	
	// update add and multiply adjustments in companies table
	if(data.fAdjustMult < _INF_ && data.fAdjustAdd < _INF_)
	{
		sprintf(szSql, "UPDATE `companies` SET EPSMultiply=%f, EPSAdd=%f WHERE id=%u", data.fAdjustMult, data.fAdjustAdd, nID);
		nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
		if(nResult != 0)
		{
			const char* err = mysql_error(theApp.m_sql);
			//theApp.m_sError.Format(_T("MySql Error: %s"), err);
			// handle error
		}
	}

	// add eps roc and roe values for the first 11 columns
	for(i=nStart; i<NUM_YEARS_FOR_EPS; i++)
	{
		int nYear = nYear0+i;

		CString sEps; if(data.arEps[i] < _INF_) sEps.Format("'%f'", data.arEps[i]); else sEps = "EPS";
		CString sRoc; if(data.arRoc[i] < _INF_) sRoc.Format("'%f'", data.arRoc[i]); else sRoc = "ROC";
		CString sRoe; if(data.arRoe[i] < _INF_) sRoe.Format("'%f'", data.arRoe[i]); else sRoe = "ROE";
		CString sShares; if(data.arShares[i] < _INF_) sShares.Format("'%f'", data.arShares[i]); else sShares = "Shares";

		sprintf(szSql, "INSERT INTO `epsrocroe` SET \
								CompanyID=%u, \
								Year = %u, \
								EPS = %f, \
								ROC = %f, \
								ROE = %f, \
								Shares = %f \
						ON DUPLICATE KEY UPDATE \
								EPS = %s, \
								ROC = %s, \
								ROE = %s, \
								Shares = %s ",
				
				nID,  nYear, data.arEps[i], data.arRoc[i], data.arRoe[i], data.arShares[i],
							 sEps, sRoc, sRoe, sShares);
		
		nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
		if(nResult != 0)
		{
			const char* err = mysql_error(theApp.m_sql);
			//theApp.m_sError.Format(_T("MySql Error: %s"), err);
			// handle error
		}
		
	}

	// add the remaining 2 (NUM_YEARS_FOR_EPS .. NUM_YEARS_FOR_ROC
	for(i=NUM_YEARS_FOR_EPS; i<NUM_YEARS_FOR_ROC; i++)
	{
		int nYear = nYear0+i;

		CString sRoc; if(data.arRoc[i] < _INF_) sRoc.Format("'%f'", data.arRoc[i]); else sRoc = "ROC";
		CString sRoe; if(data.arRoe[i] < _INF_) sRoe.Format("'%f'", data.arRoe[i]); else sRoe = "ROE";
		CString sShares; if(data.arShares[i] < _INF_) sShares.Format("'%f'", data.arShares[i]); else sShares = "Shares";

		sprintf(szSql, "INSERT INTO `epsrocroe` SET \
								CompanyID=%u, \
								Year = %u, \
								EPS = %f, \
								ROC = %f, \
								ROE = %f, \
								Shares = %f \
						ON DUPLICATE KEY UPDATE \
								EPS = EPS, \
								ROC = %s, \
								ROE = %s, \
								Shares = %s ",
				
				nID,  nYear, (double)_INF_, data.arRoc[i], data.arRoe[i], data.arShares[i],
							 sRoc, sRoe, sShares);
		
		nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
		if(nResult != 0)
		{
			const char* err = mysql_error(theApp.m_sql);
			//theApp.m_sError.Format(_T("MySql Error: %s"), err);
			// handle error
		}
	}
	
	return true;
}

int CSQLCompany::UpdateUniverseRocRoe(SUniverseDataEpsRocRoe& UniverseData, BOOL bForce, BOOL bOnlyForRollOvers)
{
	int i;
	int nUpdated = 0;
	theApp.m_sError.Empty();

	for(i=0; i<UniverseData.GetSize(); i++)
	{
		if(!UpdateCompanyRocRoe(UniverseData[i], bForce, bOnlyForRollOvers))
		{
			//theApp.m_sError += UniverseData[i].sTicker;
			//theApp.m_sError += "\n";
		}
		else
			++nUpdated;
	}

	return nUpdated;
}

BOOL CSQLCompany::UpdateCompanyRocRoe(SCompanyDataEpsRocRoe& data, BOOL bForce, BOOL bOnlyForRollOvers)
{
	char szSql[2048];
	int nResult, i;

	int nID = GetCompanyIDByTicker(data.sTicker);
	if(!nID)
	{
		// handle error
		return false;
	}

	unsigned int nType, nLastYear; BOOL bUpdateBS, bUpdateEPS, bUpdateROCROE, bUseDebtAdjust;
	if(!GetExtraInfoByID(nID, nType, bUpdateBS, bUpdateEPS, bUpdateROCROE, bUseDebtAdjust, nLastYear))
	{
		// handle error
		return false;
	}
	
	if(!bForce && !bUpdateROCROE)
		return true;

	unsigned int nYear0 = data.nYear0;
	int nStart = 0;
	if(bOnlyForRollOvers)
		nStart = (nYear0+10 != nLastYear) ? 0 : NUM_YEARS_FOR_EPS;
	
	// update add and multiply adjustments in companies table
	if(data.fAdjustMult < _INF_ && data.fAdjustAdd < _INF_)
	{
		sprintf(szSql, "UPDATE `companies` SET EPSMultiply=%f, EPSAdd=%f WHERE id=%u", data.fAdjustMult, data.fAdjustAdd, nID);
		nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
		if(nResult != 0)
		{
			const char* err = mysql_error(theApp.m_sql);
			//theApp.m_sError.Format(_T("MySql Error: %s"), err);
			// handle error
		}
	}

	// add eps roc and roe values for the first 11 columns
	for(i=nStart; i<NUM_YEARS_FOR_EPS; i++)
	{
		int nYear = nYear0+i;

		CString sRoc; if(data.arRoc[i] < _INF_) sRoc.Format("'%f'", data.arRoc[i]); else sRoc = "ROC";
		CString sRoe; if(data.arRoe[i] < _INF_) sRoe.Format("'%f'", data.arRoe[i]); else sRoe = "ROE";

		sprintf(szSql, "INSERT INTO `epsrocroe` SET \
								CompanyID=%u, \
								Year = %u, \
								ROC = %f, \
								ROE = %f \
						ON DUPLICATE KEY UPDATE \
								ROC = %s, \
								ROE = %s",
				
				nID,  nYear, data.arRoc[i], data.arRoe[i], 
							 sRoc, sRoe);
		
		nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
		if(nResult != 0)
		{
			const char* err = mysql_error(theApp.m_sql);
			//theApp.m_sError.Format(_T("MySql Error: %s"), err);
			// handle error
		}
	}

	// add the remaining 2 (NUM_YEARS_FOR_EPS .. NUM_YEARS_FOR_ROC
	for(i=NUM_YEARS_FOR_EPS; i<NUM_YEARS_FOR_ROC; i++)
	{
		int nYear = nYear0+i;

		CString sRoc; if(data.arRoc[i] < _INF_) sRoc.Format("'%f'", data.arRoc[i]); else sRoc = "ROC";
		CString sRoe; if(data.arRoe[i] < _INF_) sRoe.Format("'%f'", data.arRoe[i]); else sRoe = "ROE";

		sprintf(szSql, "INSERT INTO `epsrocroe` SET \
								CompanyID=%u, \
								Year = %u, \
								ROC = %f, \
								ROE = %f \
						ON DUPLICATE KEY UPDATE \
								ROC = %s, \
								ROE = %s",
				
				nID,  nYear, data.arRoc[i], data.arRoe[i], 
							 sRoc, sRoe);
		
		nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
		if(nResult != 0)
		{
			const char* err = mysql_error(theApp.m_sql);
			//theApp.m_sError.Format(_T("MySql Error: %s"), err);
			// handle error
		}
	}
	
	return true;
}

int CSQLCompany::UpdateUniversePricesAndReturns(SUniverseDataPricesAndReturns& UniverseData, BOOL bForce)
{
	int i;
	int nUpdated = 0;
	theApp.m_sError.Empty();

	for(i=0; i<UniverseData.GetSize(); i++)
	{
		if(!UpdateCompanyPricesAndReturns(UniverseData[i], bForce))
		{
			//theApp.m_sError += UniverseData[i].sTicker;
			//theApp.m_sError += "\n";
		}
		else
			++nUpdated;
	}

	return nUpdated;
}

BOOL CSQLCompany::UpdateCompanyPricesAndReturns(SCompanyDataPricesAndReturns& data, BOOL bForce)
{
	char szSql[2048];
	int nResult;

	int nID = GetCompanyIDByTicker(data.sTicker);
	if(!nID)
	{
		// handle error
		//CString sErr; sErr.Format("Can't get Comp. ID by ticker: '%s'", data.sTicker);
		//AfxMessageBox(sErr);
		//AfxMessageBox("Unable to get name from id");
		return false;
	}

	if(!bForce)
	{
		unsigned int nType, nLastYear; BOOL bUpdateBS, bUpdateEPS, bUpdateROCROE, bUseDebtAdjust;
		if(!GetExtraInfoByID(nID, nType, bUpdateBS, bUpdateEPS, bUpdateROCROE, bUseDebtAdjust, nLastYear))
		{
			// handle error
			//CString sErr; sErr.Format("Can't get extra info: '%s'", data.sTicker);
			//AfxMessageBox(sErr);
			AfxMessageBox("Unable to get extra info");
			return false;
		}

		if(!bUpdateBS) 
		{
			AfxMessageBox("Don't update BS");
			return true;
		}
	}

	// report date
	CString sDate = data.date.Format("%Y-%m-%d");
	
	// sql query
	sprintf(szSql, "INSERT INTO `pricereturns` SET \
								CompanyID=%u, \
								ReportDate = '%s', \
								Price = %f, \
								Ret = %f \
						ON DUPLICATE KEY UPDATE \
								Price = %f, \
								Ret = %f",
				
			nID,  sDate, data.fPrice, data.fReturn, data.fPrice, data.fReturn);

	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		//AfxMessageBox(err);
		//theApp.m_sError.Format(_T("MySql Error: %s"), err);
		// handle error
	}

	return true;
}

BOOL CSQLCompany::GetUniverseRawData(int nUniverse, int nThisYearForEPS, COleDateTime date, SUniverseRawData& UniverseData)
{
	// sql query string and result
	char szSql[2048];
	int nResult;
	MYSQL_RES* result;
	MYSQL_ROW row;

	// sql query to get all companies in the universe
	CString sDate = date.Format("%Y-%m-%d");
	CString sUniverse;
	switch(nUniverse)
	{
	case 1:  sUniverse = " AND (Univ_C800=1)  ";							break;
	case 2:	 sUniverse = " AND (Univ_ADR=1)  ";								break;
	case 3:  sUniverse = " AND (Univ_R3000=1) ";							break;
	case 4:  sUniverse = " AND (Univ_R1000=1) ";							break;
	case 5:  sUniverse = " AND (Univ_R3000=1 AND Univ_R1000=0) ";			break;
	case 6:	 sUniverse = "";												break;
	}

	sprintf(szSql, "SELECT Ticker FROM companies WHERE DateBirth<='%s' AND DateDeath>'%s' %s ORDER BY Ticker", sDate, sDate, sUniverse);

	// execute sql query
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
		SCompanyRawData data;
		if(!GetCompanyRawData(nThisYearForEPS, row[0], date, data))
		{
			//mysql_free_result(result);
			continue;//return false;
		}

		UniverseData.Add(data);
	}

	mysql_free_result(result);
	return true;
}

BOOL CSQLCompany::GetCompanyRawData(int nThisYearForEPS, const char* szTicker, COleDateTime date, SCompanyRawData& data)
{
	// sql query string and result
	char szSql[2048];
	int nResult;
	int nRows, i;
	MYSQL_RES* result;
	MYSQL_ROW row;

	int nCompanyID;

	//////////////////////////////////////////////////////////////////////////////////////////////
	// Get the Company, Industry and Sector (From Tables: companies, industries, sectors)
	//////////////////////////////////////////////////////////////////////////////////////////////
	CString sWeights;
	for(i=0; i<NUM_WEIGHTS; i++) {
		sWeights += " , companies.";
		sWeights += WeightsIndices[i];
	}

	sprintf(szSql, "SELECT	companies.id, \
							companies.Name,	\
							industries.Name, \
							sectors.Name, \
							companies.EPSMultiply, \
							companies.EPSAdd, \
							companies.LoadBS, \
							companies.LoadEPS, \
							companies.LoadROCROE, \
							companies.Type, \
							companies.LastReport, \
							companies.UseDebtAdjust, \
							companies.CIQTicker, \
							companies.IsInSP500, \
							companies.RecentRollOver \
							%s \
					FROM ( (companies INNER JOIN industries ON (companies.IndustryID=industries.id)) INNER JOIN sectors ON (companies.SectorID=sectors.id) ) \
							\
					WHERE companies.Ticker = '%s'", sWeights, szTicker);

	// execute sql query
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	} 
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);
	if(nRows == 0)
	{
		// handle error - probably wrong ticker
		return false;
	}
	row = mysql_fetch_row(result);

	nCompanyID = atoi(row[0]);
	strcpy(data.Fundamentals.sTicker, szTicker);
	strcpy(data.Fundamentals.sName, row[1]);
	strcpy(data.Fundamentals.sIndustry, row[2]); 
	strcpy(data.Fundamentals.sSector, row[3]);
	data.EpsRocRoe.fAdjustMult = atof(row[4]);
	data.EpsRocRoe.fAdjustAdd = atof(row[5]);
	data.Fundamentals.bLoadBS = (BOOL)atoi(row[6]);
	data.Fundamentals.bLoadEPS = (BOOL)atoi(row[7]);
	data.Fundamentals.bLoadROCROE = (BOOL)atoi(row[8]);
	data.Fundamentals.type = (unsigned int)atoi(row[9]);
	data.Fundamentals.nLastReport = (unsigned int)atoi(row[10]);
	data.Fundamentals.bUseDebtAdjust = (BOOL)atoi(row[11]);
	strcpy(data.Fundamentals.sCIQTicker, row[12]);
	data.Fundamentals.bInSP = (BOOL)atoi(row[13]);
	data.Fundamentals.bRollOver = (BOOL)atoi(row[14]);

	for(i=0; i<NUM_WEIGHTS; i++)
	{
		if(row[15+i])
			data.WeightsInIndices.data[i] = atof(row[15+i]);
		else
			data.WeightsInIndices.data[i] = 0;
	}
	
	mysql_free_result(result);

	//////////////////////////////////////////////////////////////////////////////////////////////
	// Get Fundamentals (From Table: balancesheets)
	//////////////////////////////////////////////////////////////////////////////////////////////
	CString sDate = date.Format("%Y-%m-%d");
	sprintf(szSql,	"SELECT * FROM balancesheets \
					 WHERE CompanyID=%u AND DateReport <= '%s' \
					 ORDER BY DateReport DESC LIMIT 1", 
			nCompanyID, sDate);

	// execute sql query
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);
	if(nRows == 1)
	{
		row = mysql_fetch_row(result);

		data.Fundamentals.dtReport = ConvertSqlToOleDate(row[1]);
		data.Fundamentals.fShares = atof(row[2]);
		data.Fundamentals.fFixed = atof(row[3]);
		data.Fundamentals.fDeprAmort = atof(row[4]);
		data.Fundamentals.fAssets = atof(row[5]);
		data.Fundamentals.fIntang = atof(row[6]);
		data.Fundamentals.fLiabilities = atof(row[7]);
		data.Fundamentals.fShortDebt = atof(row[8]);
		data.Fundamentals.fLongDebt = atof(row[9]);
		data.Fundamentals.fCommonEquity = atof(row[10]);
		data.Fundamentals.fCash = atof(row[11]);
		data.Fundamentals.fInterest = atof(row[12]);
		data.Fundamentals.fPreTaxIncome = atof(row[13]);
		data.Fundamentals.fEpsLastYear = atof(row[14]);
		data.Fundamentals.fEpsLast12Months = atof(row[15]);
		data.Fundamentals.fEpsThisYForecast = atof(row[16]);
		data.Fundamentals.fEpsNextYForecast = atof(row[17]);
		data.Fundamentals.fDiv = atof(row[18]);
		data.Fundamentals.fBookVal = atof(row[19]);
		strcpy(data.Fundamentals.sSplit, row[20]);
		data.Fundamentals.dtSplit = ConvertSqlToOleDate(row[21]);
	}
	mysql_free_result(result);

	//////////////////////////////////////////////////////////////////////////////////////////////
	// Get EPS, ROC, and ROE (From Table: epsrocroe)
	//////////////////////////////////////////////////////////////////////////////////////////////
	int nYearThis;
	if(nThisYearForEPS == -1)
		nYearThis = data.Fundamentals.nLastReport+1;
	else
		nYearThis = nThisYearForEPS;
	
	int nYear0 = nYearThis - NUM_YEARS_FOR_EPS;
	int nYear1 = nYear0 + NUM_YEARS_FOR_EPS - 1;
	int nYear2 = nYear0 + NUM_YEARS_FOR_ROC - 1;

	// fill arrays with _INF_ values
	for(i=0; i<NUM_YEARS_FOR_EPS; i++)
	{
		data.EpsRocRoe.arEps[i] = (double)_INF_;
		data.Shares.arShares[i] = (double)_INF_;
	}
	for(i=0; i<NUM_YEARS_FOR_ROC; i++)
	{
		data.EpsRocRoe.arRoc[i] = (double)_INF_;
		data.EpsRocRoe.arRoe[i] = (double)_INF_;
	}

	// sql query line
	sprintf(szSql,	"SELECT Year, EPS, ROC, ROE, Shares FROM epsrocroe \
					 WHERE CompanyID=%u AND Year >= '%u' AND Year <= '%u' \
					 ORDER BY Year ASC", 
			nCompanyID, nYear0, nYear2);

	// execute sql query
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);
	while(row = mysql_fetch_row(result))
	{
		int nYear = atoi(row[0]);
		int index = nYear - nYear0;
		
		if(index >= 0 && index < NUM_YEARS_FOR_EPS)
		{
			data.EpsRocRoe.arEps[index] = atof(row[1]);
			data.Shares.arShares[index] = row[4] ? atof(row[4]) : (double)_INF_;
		}
		if(index >= 0 && index < NUM_YEARS_FOR_ROC)
		{
			data.EpsRocRoe.arRoc[index] = atof(row[2]);
			data.EpsRocRoe.arRoe[index] = atof(row[3]);
		}
	}
	data.EpsRocRoe.nYear0 = nYear0;
	mysql_free_result(result);

	//////////////////////////////////////////////////////////////////////////////////////////////
	// Get Prices and Returns (From Table: pricereturns)
	//////////////////////////////////////////////////////////////////////////////////////////////
	sDate = date.Format("%Y-%m-%d");
	sprintf(szSql,	"SELECT * FROM pricereturns \
					 WHERE CompanyID=%u AND ReportDate <= '%s' \
					 ORDER BY ReportDate DESC LIMIT 1", 
			nCompanyID, sDate);

	// execute sql query
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);
	if(nRows == 1)
	{
		row = mysql_fetch_row(result);

		data.PricesAndReturns.date = ConvertSqlToOleDate(row[1]);
		data.PricesAndReturns.fPrice = atof(row[2]);
		data.PricesAndReturns.fReturn = atof(row[3]);
	}
	mysql_free_result(result);
	return true;
}

BOOL CSQLCompany::UpdateCompany(SCompanyRawData& data, BOOL bForceBS, BOOL bForceEPS)
{
	if(!UpdateCompanyFundamentals(data.Fundamentals, bForceBS))
		return false;

	if(!UpdateCompanyEpsRocRoe(data.EpsRocRoe, bForceEPS))
		return false;

	if(!UpdateCompanyPricesAndReturns(data.PricesAndReturns, bForceBS))
		return false;

	return true;

}

BOOL CSQLCompany::UpdateHistory(SHistory& history, int nColumns)
{
	int nID = GetCompanyIDByTicker(history.sTicker);
	if(nID == -1)
	{
		// handle error
		return false;
	}

	for(int i=0; i<history.arItems.GetSize(); i++)
		if(!UpdateHistoryItem(nID, history.arItems[i], nColumns))
		{
			// handle error
			continue;
		}

	return true;
}

BOOL CSQLCompany::UpdateHistoryItem(const char* szTicker, SHistoryItem& item, int nColumns)
{
	int nID = GetCompanyIDByTicker(szTicker);
	if(nID == -1)
		return false;

	return UpdateHistoryItem(nID, item, nColumns);
}

BOOL CSQLCompany::UpdateHistoryItem(int nCompanyID, SHistoryItem& item, int nColumns)
{
	CString sSql;
	int nResult;

	CString sSet; 
	BOOL bFirst=true;
	for(int i=0; i<nColumns; i++)
	{
		CString sLine;
		if(item.data[i] < (double)_INF_)
			sLine.Format("%s`%s`=%f ", bFirst ? "" : ", ", HistCols[i], item.data[i]);
		else 
			sLine.Format("%s`%s`=null ", bFirst ? "" : ", ", HistCols[i]);

		sSet += sLine;
		bFirst = false;
	}

	CString sDate = item.date.Format("%Y-%m-%d");
	sSql.Format("INSERT INTO `history` SET CompanyID=%u, ItemDate='%s', %s ON DUPLICATE KEY UPDATE %s", nCompanyID, sDate, sSet, sSet);

	nResult = mysql_real_query(theApp.m_sql, sSql, sSql.GetLength());
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}

	return true;
}

BOOL CSQLCompany::DeleteHistoryItem(const char* szTicker, COleDateTime date)
{
	int nID = GetCompanyIDByTicker(szTicker);
	if(nID == -1)
		return false;

	return DeleteHistoryItem(nID, date);
}

BOOL CSQLCompany::DeleteHistoryItem(int nCompanyID, COleDateTime date)
{
	CString sDate = date.Format("%Y-%m-%d");

	CString sSql;
	sSql.Format("DELETE FROM history WHERE CompanyID='%u' AND ItemDate='%s'", nCompanyID, sDate);
	
	int nResult = mysql_real_query(theApp.m_sql, sSql, sSql.GetLength());
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}

	return true;
}

BOOL CSQLCompany::DeleteAllHistoryItems(COleDateTime date)
{
	CString sDate = date.Format("%Y-%m-%d");

	CString sSql;
	sSql.Format("DELETE FROM history WHERE ItemDate='%s'", sDate);
	
	int nResult = mysql_real_query(theApp.m_sql, sSql, sSql.GetLength());
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}

	return true;
}

BOOL CSQLCompany::GetHistory(const char* szTicker, SHistory& history, int nNumRecords)
{
	int nResult;
	MYSQL_RES* result;
	MYSQL_ROW row;

	strcpy(history.sTicker, szTicker);

	CString sSql, s;
	sSql.Format("SELECT * FROM `history` INNER JOIN `companies` ON history.CompanyID=companies.id WHERE companies.Ticker='%s' ", szTicker);
	sSql += " ORDER BY `ItemDate` DESC ";

	if(nNumRecords != -1)
	{
		s.Format(" LIMIT 0, %u", nNumRecords);
		sSql += s;
	}

	// execute sql query
	nResult = mysql_real_query(theApp.m_sql, sSql, sSql.GetLength());
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}
	result = mysql_store_result(theApp.m_sql);
	while(row = mysql_fetch_row(result))
	{
		SHistoryItem item;
		item.date = ConvertSqlToOleDate(row[1]);

		for(int i=0; i<NUM_COLUMNS_IN_HISTORY_DB; i++)
		{
			if(row[i+2])
				item.data[i] = atof(row[i+2]);
			else
				item.data[i] = _INF_;
		}

		history.arItems.Add(item);
	}

	mysql_free_result(result);

	return true;
}

BOOL CSQLCompany::GetHistory(const char* szTicker, SHistory& history, COleDateTime from, COleDateTime to)
{
	int nResult;
	MYSQL_RES* result;
	MYSQL_ROW row;

	strcpy(history.sTicker, szTicker);

	CString sSql, s;
	sSql.Format("SELECT * FROM `history` INNER JOIN `companies` ON history.CompanyID=companies.id WHERE companies.Ticker='%s' ", szTicker);
	
	if(from.GetStatus() == COleDateTime::valid)
	{
		s.Format(" AND history.ItemDate >= '%s' ", from.Format("%Y-%m-%d"));
		sSql += s;
	}

	if(to.GetStatus() == COleDateTime::valid)
	{
		s.Format(" AND history.ItemDate <= '%s' ", to.Format("%Y-%m-%d"));
		sSql += s;
	}

	sSql += " ORDER BY `ItemDate` DESC ";

	// execute sql query
	nResult = mysql_real_query(theApp.m_sql, sSql, sSql.GetLength());
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}
	result = mysql_store_result(theApp.m_sql);
	while(row = mysql_fetch_row(result))
	{
		SHistoryItem item;
		item.date = ConvertSqlToOleDate(row[1]);

		for(int i=0; i<NUM_COLUMNS_IN_HISTORY_DB; i++)
		{
			if(row[i+2])
				item.data[i] = atof(row[i+2]);
			else
				item.data[i] = _INF_;
		}

		history.arItems.Add(item);
	}

	mysql_free_result(result);

	return true;
}

BOOL CSQLCompany::FillHistoryItem(const char* szTicker, int nItem, SHistory& history, int nRows)
{
	int nResult;
	MYSQL_RES* result;
	MYSQL_ROW row;

	strcpy(history.sTicker, szTicker);

	CString sSql, s;
	sSql.Format("SELECT `%s` FROM `history` INNER JOIN `companies` ON history.CompanyID=companies.id WHERE companies.Ticker='%s' ", HistCols[nItem], szTicker);
	sSql += " ORDER BY `ItemDate` DESC ";
	if(nRows != -1)
	{
		CString sRows;
		sRows.Format(" LIMIT 0, %u", nRows);
		sSql += sRows;
	}

	// execute sql query
	nResult = mysql_real_query(theApp.m_sql, sSql, sSql.GetLength());
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}
	result = mysql_store_result(theApp.m_sql);
	int index = 0;
	while(row = mysql_fetch_row(result))
	{
		if(row[0])
			history.arItems[index].data[nItem] = atof(row[0]);
		else
			history.arItems[index].data[nItem] = _INF_;
		++index;
	}

	mysql_free_result(result);
	return true;
}

COleDateTime CSQLCompany::GetMinDateInHistories()
{
	CString sSql;
	int nResult, nRows;
	MYSQL_RES* result;
	MYSQL_ROW row;

	sSql = "SELECT ItemDate FROM history ORDER BY ItemDate LIMIT 0, 1";
	
	COleDateTime date;
	date.SetStatus(COleDateTime::invalid);

	// execute sql query
	nResult = mysql_real_query(theApp.m_sql, sSql, sSql.GetLength());
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return date;
	}
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);
	if(nRows != 1)
		return date;

	row = mysql_fetch_row(result);
	date = ConvertSqlToOleDate(row[0]);

	mysql_free_result(result);
	return date;
}

int CSQLCompany::GetHistoryDates(COleDateTime from, COleDateTime to, CArray<COleDateTime,COleDateTime>& dates)
{
	CString sSql;
	int nResult;
	int nRows;
	MYSQL_RES* result;
	MYSQL_ROW row;

	CString sFrom = (from.GetStatus() == COleDateTime::valid) ? from.Format("%Y-%m-%d") : "1901-01-01";
	CString sTo = (to.GetStatus() == COleDateTime::valid) ? to.Format("%Y-%m-%d") : "2101-01-01";

	sSql.Format("SELECT DISTINCT ItemDate FROM history WHERE ItemDate>='%s' AND ItemDate<='%s' ORDER BY ItemDate ASC ", sFrom, sTo);
	
	nResult = mysql_real_query(theApp.m_sql, sSql, sSql.GetLength());
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);
	
	while(row = mysql_fetch_row(result))
	{
		COleDateTime date = ConvertSqlToOleDate(row[0]);
		dates.Add(date);
	}

	mysql_free_result(result);
	return nRows;
}

int CSQLCompany::GetValuesFromHistoryByCompanyIds(COleDateTime date, CArray<double,double> &ids, const char* szItem, CArray<double,double> &data)
{
	CString sSql;
	int nResult;
	int nRows;
	MYSQL_RES* result;
	MYSQL_ROW row;

	CString sIN = " (";
	for(int i=0; i<ids.GetSize(); i++)
	{
		CString s; s.Format("'%u'", (unsigned int)(ids[i]));
		sIN += s;
		if(i<ids.GetSize()-1)
			sIN += ", ";
	}
	sIN += ") ";

	CString sDate = date.Format("%Y-%m-%d");
	sSql.Format("SELECT `%s` FROM history WHERE `ItemDate`='%s' AND `CompanyID` IN %s", szItem, sDate, sIN);
	nResult = mysql_real_query(theApp.m_sql, sSql, sSql.GetLength());
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);

	while(row = mysql_fetch_row(result))
	{
		double f = row[0] ? atof(row[0]) : _INF_;
		data.Add(f);
	}

	mysql_free_result(result);
	return nRows;
}

int CSQLCompany::GetRankedValuesFromHistory(COleDateTime date, int nPercentFrom, int nPercentTo, const char* szRankBy, const char* szItem, CArray<double,double> &data)
{
	CString sSql;
	int nResult;
	int nRows;
	MYSQL_RES* result;
	MYSQL_ROW row;

	CString sDate = date.Format("%Y-%m-%d");
	sSql.Format("SELECT `%s`, `%s` FROM history WHERE ItemDate='%s' ORDER BY `%s` ASC", szItem, szRankBy, sDate, szRankBy);
	
	nResult = mysql_real_query(theApp.m_sql, sSql, sSql.GetLength());
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);

	int nStart = nPercentFrom*nRows/100;
	int nEnd = nPercentTo*nRows/100;
	int nNum = nEnd-nStart;

	mysql_data_seek(result, nStart);
	for(int i=0; i<nNum; i++)
	{
		row = mysql_fetch_row(result);
		double f = row[0] ? atof(row[0]) : _INF_;
		data.Add(f);
	}

	mysql_free_result(result);
	return nNum;
}

BOOL CSQLCompany::GetHistoryDates(const char* szTicker, SHistory& history)
{
	int nResult;
	MYSQL_RES* result;
	MYSQL_ROW row;

	strcpy(history.sTicker, szTicker);

	CString sSql, s;
	sSql.Format("SELECT `ItemDate` FROM `history` INNER JOIN `companies` ON history.CompanyID=companies.id WHERE companies.Ticker='%s'  ORDER BY `ItemDate` DESC", szTicker);

	// execute sql query
	nResult = mysql_real_query(theApp.m_sql, sSql, sSql.GetLength());
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}
	result = mysql_store_result(theApp.m_sql);
	while(row = mysql_fetch_row(result))
	{
		SHistoryItem item;
		item.date = ConvertSqlToOleDate(row[0]);
		history.arItems.Add(item);
	}

	mysql_free_result(result);
	return true;
}

BOOL CSQLCompany::DoesInterestRateRecordExist(const COleDateTime& dateFriday)
{
	char szSql[2048];
	int nResult;
	int nRows;
	MYSQL_RES* result;

	sprintf(szSql, "SELECT DateFriday FROM t10 WHERE DateFriday = '%s'", dateFriday.Format("%Y-%m-%d"));
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return false;
	}
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);

	BOOL bExist = (nRows != 0);

	mysql_free_result(result);
	return bExist;
}

double CSQLCompany::GetInterestRate(const COleDateTime& date)
{
	// sql query string and result
	char szSql[2048];
	int nResult;
	int nRows;
	MYSQL_RES* result;
	MYSQL_ROW row;

	sprintf(szSql, "SELECT WeeklyAve, 3MAve, FridayClose FROM t10 WHERE DateFriday<='%s' ORDER BY DateFriday DESC LIMIT 0, 1", date.Format("%Y-%m-%d"));
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return _INF_;
	}
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);

	if(nRows != 1)
	{
		mysql_free_result(result);
		return _INF_;
	}

	row = mysql_fetch_row(result);
	double fWeeklyAve = atof(row[0]);
	double f3MAve = atof(row[1]);
	double fFridayClose = atof(row[2]);

	double fDiscount = fWeeklyAve;
	if(f3MAve < _INF_ && f3MAve > fDiscount)
		fDiscount = f3MAve;
	if(fFridayClose < _INF_ && fFridayClose > fDiscount)
		fDiscount = fFridayClose;

	mysql_free_result(result);
	return fDiscount;
}

BOOL CSQLCompany::UpdateInterestRate(const COleDateTime& date, double fWeeklyAve, double f3MAve, double fFridayClose)
{
	char szSql[2048];
	int nResult;
	CString sDate = date.Format("%Y-%m-%d");

	sprintf(szSql,  "INSERT INTO t10 SET "
					"DateFriday='%s', WeeklyAve=%f, 3MAve=%f, FridayClose=%f  "
					"ON DUPLICATE KEY UPDATE "
					"WeeklyAve=%f, 3MAve=%f, FridayClose=%f",
					
					sDate, fWeeklyAve, f3MAve, fFridayClose, fWeeklyAve, f3MAve, fFridayClose);
	
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return false;
	}
	return true;
}

int CSQLCompany::GetInterestRateHistory(const COleDateTime& from, const COleDateTime& to, SInterestRateHistory& data)
{
	CString sSql;
	int nResult;
	int nRows;
	MYSQL_RES* result;
	MYSQL_ROW row;

	CString sFrom = (from.GetStatus() == COleDateTime::valid) ? from.Format("%Y-%m-%d") : "1901-01-01";
	CString sTo = (to.GetStatus() == COleDateTime::valid) ? to.Format("%Y-%m-%d") : "2101-01-01";

	sSql.Format("SELECT * FROM t10 WHERE DateFriday>='%s' AND DateFriday<='%s' ORDER BY DateFriday DESC ", sFrom, sTo);
	
	nResult = mysql_real_query(theApp.m_sql, sSql, sSql.GetLength());
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return false;
	}
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);
	
	while(row = mysql_fetch_row(result))
	{
		SInterestRate item;
		item.date = ConvertSqlToOleDate(row[0]);
		item.fWeeklyAve = atof(row[1]);
		item.f3MAve = row[2] ? atof(row[2]) : _INF_;
		item.fFridayClose = row[3] ? atof(row[3]) : _INF_;
		
		data.Add(item);
	}

	mysql_free_result(result);
	return nRows;
}

int CSQLCompany::GetInterestRateHistory(const COleDateTime& date, int nPeriodsBack, SInterestRateHistory& data)
{
	CString sSql, sLimit;
	int nResult;
	int nRows;
	MYSQL_RES* result;
	MYSQL_ROW row;

	sSql.Format("SELECT * FROM t10 WHERE DateFriday<='%s' ORDER BY DateFriday DESC ", date.Format("%Y-%m-%d"));
	if(nPeriodsBack != -1)
	{
		sLimit.Format("LIMIT 0, %u", nPeriodsBack);
		sSql += sLimit;
	}

	nResult = mysql_real_query(theApp.m_sql, sSql, sSql.GetLength());
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return false;
	}
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);
	
	while(row = mysql_fetch_row(result))
	{
		SInterestRate item;
		item.date = COleDateTime(atof(row[0]));
		item.fWeeklyAve = atof(row[1]);
		item.f3MAve = row[2] ? atof(row[2]) : _INF_;
		item.fFridayClose = row[3] ? atof(row[3]) : _INF_;
		
		data.Add(item);
	}

	mysql_free_result(result);
	return nRows;
}

COleDateTime CSQLCompany::GetLastAutoSplitDate()
{
	CString sSql;
	int nResult;
	int nRows;
	MYSQL_RES* result;
	MYSQL_ROW row;

	COleDateTime date;
	date.SetStatus(COleDateTime::invalid);

	sSql = "SELECT LastAutoSplitDate FROM misc";
	nResult = mysql_real_query(theApp.m_sql, sSql, sSql.GetLength());
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return date;
	}
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);
	if(nRows == 0)
	{
		mysql_free_result(result);
		return date;
	}

	row = mysql_fetch_row(result);
	date = ConvertSqlToOleDate(row[0]);

	mysql_free_result(result);
	return date;
}

BOOL CSQLCompany::SetLastAutoSplitDate(const COleDateTime &date)
{
	CString sSql;
	sSql.Format("UPDATE misc SET LastAutoSplitDate='%s'", date.Format("%Y-%m-%d"));
	
	int nResult = mysql_real_query(theApp.m_sql, sSql, sSql.GetLength());
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return false;
	}
	return true;
}

int CSQLCompany::GetCompanyLastReportYear(const int nID)
{
	CString sSql;
	int nResult;
	int nRows;
	MYSQL_RES* result;
	MYSQL_ROW row;

	sSql.Format("SELECT LastReport FROM companies WHERE id='%u'", nID);
	nResult = mysql_real_query(theApp.m_sql, sSql, sSql.GetLength());
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);
	if(nRows == 0)
	{
		mysql_free_result(result);
		return 0;
	}

	row = mysql_fetch_row(result);
	int nYear = atoi(row[0]);

	mysql_free_result(result);
	return nYear;
}

BOOL CSQLCompany::SetCompanyRollOver(const char* szTicker, int nRollOver)
{
	char szSql[2048];
	int nResult;

	sprintf(szSql, "UPDATE companies SET RecentRollOver=%u WHERE Ticker='%s'", nRollOver, szTicker);
	nResult = mysql_real_query(theApp.m_sql, szSql, strlen(szSql));
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}

	return true;
}

const CString CSQLCompany::GetDatabase()
{
	return theApp.m_sDatabase;
}

CString CSQLCompany::PrepStrForSQL(const char* string)
{
	char *szClear = new char[strlen(string)*2+1];
	mysql_real_escape_string(theApp.m_sql, szClear, string, strlen(string));

	CString s = szClear;
	delete [] szClear;
	return s;

/*	CString s;

	const char* sz = string;
	while(*sz != '\0')
	{
		if(*sz == '\'')
		{
			if(sz == string || (sz > string && *(sz-1) != '\\') )
				s += '\\';
		}
		s += *sz;
		++sz;
	}

	return s;*/
}