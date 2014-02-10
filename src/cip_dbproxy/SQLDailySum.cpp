// SQLDailySum.cpp: implementation of the CSQLDailySum class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cip_dbproxy.h"
#include "SQLDailySum.h"

#include "SQLCompany.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

extern CCip_dbproxyApp	theApp;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CSQLDailySum::CSQLDailySum()
{

}

CSQLDailySum::~CSQLDailySum()
{

}

int CSQLDailySum::UpdateDailySum(	int nCompanyID, int nAnalystID, COleDateTime date, 
								double fTargetWeight, const char* szBias,
								int nCoordValue, int nCoordFund, 
								int nConfidenceValue, int nConfidenceFund,
								const char* szNotesOnValue, const char* szNotesOnFund,
								int nScoreMgmtVision, int nScoreMgmtExec,
								const char* szNotesOnMgmt, 
								BOOL bConfirmed, double fTargetWeight2, const char* szBias2,
								const char* szOneLiner)
{
	CString sSql;
	int nResult;

	CString sDate = date.Format("%Y-%m-%d");
	char szTemplate[] = "INSERT INTO dailysummaries SET "
							"id=DEFAULT, "
							"CompanyID = '%u', "
							"AnalystID = '%u', "
							"ItemDate = '%s', "
							"TargetWeight = '%f', "
							"Bias = '%s', "
							"CoordValue = '%u', "
							"CoordFundamentals = '%u', "
							"ConfidenceValue = '%u', "
							"ConfidenceFundamentals = '%u', "
							"NotesOnValuation ='%s', "
							"NotesOnFundamentals ='%s', "
							"ScoreMgmtVision = '%u', "
							"ScoreMgmtExecution = '%u', "
							"NotesOnMgmt ='%s', "
							"Confirmed = '%u', "
							"TargetWeight2 = '%f', "
							"Bias2 = '%s', "
							"OneLiner = '%s' "
						"ON DUPLICATE KEY UPDATE "
							"TargetWeight = '%f', "
							"Bias = '%s', "
							"CoordValue = '%u', "
							"CoordFundamentals = '%u', "
							"ConfidenceValue = '%u', "
							"ConfidenceFundamentals = '%u', "
							"NotesOnValuation ='%s', "
							"NotesOnFundamentals ='%s', "
							"ScoreMgmtVision = '%u', "
							"ScoreMgmtExecution = '%u', "
							"NotesOnMgmt ='%s', "
							"Confirmed = '%u', "
							"TargetWeight2 = '%f', "
							"Bias2 = '%s', "
							"OneLiner = '%s' ";
	
	sSql.Format(szTemplate, nCompanyID, nAnalystID, sDate,	
				   fTargetWeight, szBias, nCoordValue, nCoordFund, nConfidenceValue, nConfidenceFund,
				   szNotesOnValue, szNotesOnFund, nScoreMgmtVision, nScoreMgmtExec, szNotesOnMgmt,
				   bConfirmed, fTargetWeight2, szBias2, szOneLiner,
				   
				   fTargetWeight, szBias, nCoordValue, nCoordFund, nConfidenceValue, nConfidenceFund,
				   szNotesOnValue, szNotesOnFund, nScoreMgmtVision, nScoreMgmtExec, szNotesOnMgmt,
				   bConfirmed, fTargetWeight2, szBias2, szOneLiner);

	nResult = mysql_real_query(theApp.m_sql, sSql, sSql.GetLength());
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}

	int nID = (unsigned int)mysql_insert_id(theApp.m_sql);
	return 1;
}

int CSQLDailySum::GetDailySumsForTickerAndDate( int nCompanyID, COleDateTime date, SDailySumArray& arReturn)
{
	int nResult, nRows;
	MYSQL_RES* result;
	MYSQL_ROW row;

	CString sDate = date.Format("%Y-%m-%d");

	CString sSql;
	sSql.Format("SELECT dailysummaries.*, users.Initials FROM dailysummaries INNER JOIN USERS ON (users.id=dailysummaries.AnalystID) WHERE \
				CompanyID='%u' AND ItemDate='%s' AND (users.Level='1' OR users.Level='3')", 

				nCompanyID, sDate);
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
		SDailySum *dailySum = new SDailySum;
		dailySum->nID = (int)atoi(row[0]);
		dailySum->nCompanyID = (int)atoi(row[1]);
		dailySum->nAnalystID = (int)atoi(row[2]);
		dailySum->date = CSQLCompany::ConvertSqlToOleDate(row[3]);
		dailySum->fWeight = (double)atof(row[4]);
		dailySum->sBias = row[5];
		dailySum->nCoordValue = (int)atoi(row[6]);
		dailySum->nCoordFund = (int)atoi(row[7]);
		dailySum->nConfidenceValue = (int)atoi(row[8]);
		dailySum->nConfidenceFund = (int)atoi(row[9]);
		dailySum->sNotesOnValue = row[10];
		dailySum->sNotesOnFund = row[11];
		dailySum->nScoreMgmtVision = (int)atoi(row[12]);
		dailySum->nScoreMgmtExec = (int)atoi(row[13]);
		dailySum->sNotesOnMgmt = row[14];
		dailySum->sAnalystInitials = row[19];

		arReturn.Add(dailySum);
	}
	
	mysql_free_result(result);
	return arReturn.GetSize();
}

BOOL CSQLDailySum::GetDailySum( int nCompanyID, int nAnalystID, COleDateTime date, SDailySum& dailySum)
{
	int nResult, nRows;
	MYSQL_RES* result;
	MYSQL_ROW row;

	CString sDate = date.Format("%Y-%m-%d");

	CString sSql;
	sSql.Format("SELECT * FROM dailysummaries WHERE CompanyID='%u' AND AnalystID='%u' AND ItemDate='%s'", 
				nCompanyID, nAnalystID, sDate);
	nResult = mysql_real_query(theApp.m_sql, sSql, sSql.GetLength());
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return false;
	}
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);

	if(nRows != 1) // we expect exactly 1 row (this is with a unique key in DB)
	{
		mysql_free_result(result);
		return false;
	}

	row = mysql_fetch_row(result);
	dailySum.nID = (int)atoi(row[0]);
	dailySum.nCompanyID = (int)atoi(row[1]);
	dailySum.nAnalystID = (int)atoi(row[2]);
	dailySum.date = CSQLCompany::ConvertSqlToOleDate(row[3]);
	dailySum.fWeight = (double)atof(row[4]);
	dailySum.sBias = row[5];
	dailySum.nCoordValue = (int)atoi(row[6]);
	dailySum.nCoordFund = (int)atoi(row[7]);
	dailySum.nConfidenceValue = (int)atoi(row[8]);
	dailySum.nConfidenceFund = (int)atoi(row[9]);
	dailySum.sNotesOnValue = row[10];
	dailySum.sNotesOnFund = row[11];
	dailySum.nScoreMgmtVision = (int)atoi(row[12]);
	dailySum.nScoreMgmtExec = (int)atoi(row[13]);
	dailySum.sNotesOnMgmt = row[14];
	dailySum.bConfirmed = (int)atoi(row[15]);
	if(row[16])
		dailySum.fWeight2 = (double)atof(row[16]);
	if(row[17])
		dailySum.sBias2 = row[17];
	dailySum.sOneLiner = row[18];


	mysql_free_result(result);
	return true;
}

int CSQLDailySum::IsUnlocked(int nCompanyId, COleDateTime& date)
{
	int nResult, nRows;
	MYSQL_RES* result;
	MYSQL_ROW row;

	CString sDate = date.Format("%Y-%m-%d");

	CString sSql;
	sSql.Format("SELECT id FROM dailysumlocks WHERE CompanyID='%u' AND ItemDate='%s'", 
				nCompanyId, sDate);
	nResult = mysql_real_query(theApp.m_sql, sSql, sSql.GetLength());
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);

	if(nRows != 1) // we expect exactly 1 row (this is with a unique key in DB)
	{
		mysql_free_result(result);
		return 0;
	}

	row = mysql_fetch_row(result);
	int nId = (int)atoi(row[0]);

	return nId;
}

int CSQLDailySum::Unlock(int nCompanyId, COleDateTime& date)
{
	CString sSql;
	int nResult;

	CString sDate = date.Format("%Y-%m-%d");
	char szTemplate[] = "INSERT INTO dailysumlocks SET "
							"id=DEFAULT, "
							"CompanyID = '%u', "
							"ItemDate = '%s' ";
	
	sSql.Format(szTemplate, nCompanyId, sDate);
	nResult = mysql_real_query(theApp.m_sql, sSql, sSql.GetLength());

	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}

	int nId = (unsigned int)mysql_insert_id(theApp.m_sql);
	return nId;
}

int CSQLDailySum::Lock(int nCompanyId, COleDateTime& date)
{
	CString sSql;
	int nResult;

	CString sDate = date.Format("%Y-%m-%d");
	char szTemplate[] = "DELETE FROM dailysumlocks WHERE "
							"CompanyID = '%u', "
							"ItemDate = '%s' ";
	
	sSql.Format(szTemplate, nCompanyId, sDate);
	nResult = mysql_real_query(theApp.m_sql, sSql, sSql.GetLength());

	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}

	return 1;
}

int CSQLDailySum::Update2MinuteDrill(	int nCompanyID, COleDateTime date,
									int nNormalized, const char* szNormalizedNotes,
									int nRazor, const char* szRazorNotes,
									int nUndervalued, const char* szUndervaluedNotes,
									int nPredictor, const char* szPredictorNotes,
									int nMeanEPS, const char* szMeanEPSNotes,
									int n3Year, const char* sz3YearNotes,
									int nGrowth, const char* szGrowthNotes,
									int nEstimates, const char* szEstimatesNotes,
									int nReturns, const char* szReturnsNotes,
									int nAcct, const char* szAcctNotes,
									const char* szThesis)
{
	CString sSql;
	int nResult;

	CString sDate = date.Format("%Y-%m-%d");
	char szTemplate[] = "INSERT INTO 2minutedrill SET "
							"id=DEFAULT, "
							"CompanyID = '%u', "
							"ItemDate = '%s', "
							"Normalized = '%u', "
							"NormalizedNotes = '%s', "
							"Razor = '%u', "
							"RazorNotes = '%s', "
							"Undervalued = '%u', "
							"UndervaluedNotes = '%s', "
							"Predictor = '%u', "
							"PredictorNotes = '%s', "
							"MeanEPS = '%u', "
							"MeanEPSNotes = '%s', "
							"3Year = '%u', "
							"3YearNotes = '%s', "
							"Growth = '%u', "
							"GrowthNotes = '%s', "
							"Estimates = '%u', "
							"EstimatesNotes = '%s', "
							"Returns = '%u', "
							"ReturnsNotes = '%s', "
							"Accounting = '%u', "
							"AccountingNotes = '%s', "
							"Thesis = '%s' "
						"ON DUPLICATE KEY UPDATE "
							"Normalized = '%u', "
							"NormalizedNotes = '%s', "
							"Razor = '%u', "
							"RazorNotes = '%s', "
							"Undervalued = '%u', "
							"UndervaluedNotes = '%s', "
							"Predictor = '%u', "
							"PredictorNotes = '%s', "
							"MeanEPS = '%u', "
							"MeanEPSNotes = '%s', "
							"3Year = '%u', "
							"3YearNotes = '%s', "
							"Growth = '%u', "
							"GrowthNotes = '%s', "
							"Estimates = '%u', "
							"EstimatesNotes = '%s', "
							"Returns = '%u', "
							"ReturnsNotes = '%s', "
							"Accounting = '%u', "
							"AccountingNotes = '%s', "
							"Thesis = '%s' ";
	
	sSql.Format(szTemplate, nCompanyID, sDate, // 21 items including CompanyId and Date, so total number is 21+ 21-2 = 40
							
							nNormalized, szNormalizedNotes,
							nRazor, szRazorNotes,
							nUndervalued, szUndervaluedNotes,
							nPredictor, szPredictorNotes,
							nMeanEPS, szMeanEPSNotes,
							n3Year, sz3YearNotes,
							nGrowth, szGrowthNotes,
							nEstimates, szEstimatesNotes,
							nReturns, szReturnsNotes, 
							nAcct, szAcctNotes,
							szThesis,
							
							nNormalized, szNormalizedNotes,
							nRazor, szRazorNotes,
							nUndervalued, szUndervaluedNotes,
							nPredictor, szPredictorNotes,
							nMeanEPS, szMeanEPSNotes,
							n3Year, sz3YearNotes,
							nGrowth, szGrowthNotes,
							nEstimates, szEstimatesNotes,
							nReturns, szReturnsNotes, 
							nAcct, szAcctNotes,
							szThesis);

	nResult = mysql_real_query(theApp.m_sql, sSql, sSql.GetLength());
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return 0;
	}

	int nID = (unsigned int)mysql_insert_id(theApp.m_sql);
	return 1;	
}

BOOL CSQLDailySum::Get2MinuteDrill( int nCompanyID, S2MinuteDrill& ret)
{
	int nResult, nRows;
	MYSQL_RES* result;
	MYSQL_ROW row;

	CString sSql;
	sSql.Format("SELECT * FROM 2minutedrill WHERE CompanyID='%u'", 
				nCompanyID);
	nResult = mysql_real_query(theApp.m_sql, sSql, sSql.GetLength());
	if(nResult != 0)
	{
		const char* err = mysql_error(theApp.m_sql);
		theApp.m_sError.Format(_T("MySql Error: %s"), err);
		return false;
	}
	result = mysql_store_result(theApp.m_sql);
	nRows = (int)mysql_num_rows(result);

	if(nRows != 1) // we expect exactly 1 row (this is with a unique key in DB)
	{
		mysql_free_result(result);
		return false;
	}

	row = mysql_fetch_row(result);
	ret.nID = (int)atoi(row[0]);
	ret.nCompanyID = (int)atoi(row[1]);
	ret.date = CSQLCompany::ConvertSqlToOleDate(row[2]);
	ret.nNormalized = (int)atoi(row[3]);
	ret.sNormalizedNotes = row[4];
	ret.nRazor = (int)atoi(row[5]);
	ret.sRazorNotes = row[6];;
	ret.nUndervalued = (int)atoi(row[7]);
	ret.sUndervaluedNotes = row[8];;
	ret.nPredictor = (int)atoi(row[9]);
	ret.sPredictorNotes = row[10];;
	ret.nMeanEPS = (int)atoi(row[11]);
	ret.sMeanEPSNotes = row[12];;
	ret.n3Year = (int)atoi(row[13]);			
	ret.s3YearNotes = row[14];;
	ret.nGrowth = (int)atoi(row[15]);		
	ret.sGrowthNotes = row[16];;
	ret.nEstimates = (int)atoi(row[17]);
	ret.sEstimatesNotes = row[18];;
	ret.nReturns = (int)atoi(row[19]);
	ret.sReturnsNotes = row[20];
	ret.nAcct = (int)atoi(row[21]);
	ret.sAcctNotes = row[22];
	ret.sThesis = row[23];

	mysql_free_result(result);
	return true;
}