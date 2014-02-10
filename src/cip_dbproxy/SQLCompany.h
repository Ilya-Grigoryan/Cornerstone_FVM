// SQLCompany.h: interface for the CSQLCompany class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_SQLCOMPANY_H__C4F2DCD3_CF94_4EF6_9457_C75A5CE98432__INCLUDED_)
#define AFX_SQLCOMPANY_H__C4F2DCD3_CF94_4EF6_9457_C75A5CE98432__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "input.h"
#include "basemath.h"

class _declspec(dllexport) CSQLCompany  
{
public:
	CSQLCompany();
	~CSQLCompany();

public:
	static CString GetLastError();
	static COleDateTime GetDate();
	static void SetDate(COleDateTime date);		

	static int AddCompany(const char* szTicker, const char* szName, const char* szIndustry, const char* szSector, double fEPSMultiply, double fEPSAdd);
	static int AddCompany(const char* szTicker, const char* szName, 
		int nIndustryID, int nSectorID, const char* szCIQTicker, int nType, int nLastYear, 
		BOOL bLoadBS, BOOL bLoadEPS, BOOL bLoadROCROE, BOOL bUseDebtAdjust, double* weights, COleDateTime date);

	static BOOL UpdateCompanyBasicInfo(const char* szTicker, const char* szNewTicker, const char* szName, 
		int nIndustryID, int nSectorID, const char* szCIQTicker, int nType, int nLastYear, 
		BOOL bLoadBS, BOOL bLoadEPS, BOOL bLoadROCROE, BOOL bUseDebtAdjust, double* weights);

	static BOOL DeleteCompany(const char* szTicker, COleDateTime date);
	
	static int AddIndustry(const char* szName, const char* szDescription);
	static int AddSector(const char* szName, const char* szDescription);

	static int GetIndustryIdsAndNames(CArray<int,int> &arIds, CStringArray &arNames);
	static int GetSectorIdsAndNames(CArray<int,int> &arIds, CStringArray &arNames);

	static BOOL SetIndustryName(int nID, const char* szName);
	static BOOL SetSectorName(int nID, const char* szName);

	static BOOL DeleteIndustry(int nID);
	static BOOL DeleteSector(int nID);

	static int GetCompanyIDByTicker(const char* szTicker);
	static int GetCompanyIDByName(const char* szName);
	static CString GetCompanyTickerByID(const int nID);
	static int GetCompanyIDAndTickers(CArray<int,int> &arIds, CStringArray &arTickers, COleDateTime date);
	static BOOL SetCompanyRollOver(const char* szTicker, int nRollOver);

	static int GetIndustryIDByName(const char* szName, BOOL bAddIfNotExist=false);
	static int GetSectorIDByName(const char* szName, BOOL bAddIfNotExist=false);

	static BOOL GetExtraInfoByID(const int nID, unsigned int &nType, BOOL& bUpdateBS, BOOL& bUpdateEPS, BOOL& bUpdateROCROE, BOOL& bUseDebtAdjust, unsigned int &nReportYear);
	static int GetCompanyLastReportYear(const int nID);

	static BOOL UpdateCompany(SCompanyRawData& data, BOOL bForceBS, BOOL bForceEPS);

	static int	UpdateUniverseFundamentals(SUniverseDataFundamentals& UniverseData, BOOL bForce);
	static BOOL UpdateCompanyFundamentals(SCompanyDataFundamentals& data, BOOL bForce);

	static int	UpdateUniverseEpsRocRoe(SUniverseDataEpsRocRoe& UniverseData, BOOL bForce, BOOL bOnlyForRollOvers=false);
	static BOOL UpdateCompanyEpsRocRoe(SCompanyDataEpsRocRoe& data, BOOL bForce, BOOL bOnlyForRollOvers=false);

	static int	UpdateUniverseRocRoe(SUniverseDataEpsRocRoe& UniverseData, BOOL bForce, BOOL bOnlyForRollOvers=false);
	static BOOL UpdateCompanyRocRoe(SCompanyDataEpsRocRoe& data, BOOL bForce, BOOL bOnlyForRollOvers=false);

	static int	UpdateUniversePricesAndReturns(SUniverseDataPricesAndReturns& UniverseData, BOOL bForce);
	static BOOL UpdateCompanyPricesAndReturns(SCompanyDataPricesAndReturns& data, BOOL bForce);

	static BOOL	UpdateUniverseWeightsInIndices(SUniverseWeightsInIndices& UniverseData);
	static BOOL	UpdateCompanyWeightsInIndices(SCompanyWeightsInIndices& data);

	static int	UpdateUniverseShares(SUniverseDataShares& UniverseData, BOOL bForce, BOOL bOnlyForRollOvers=false);
	static BOOL UpdateCompanyShares(SCompanyDataShares& data, BOOL bForce, BOOL bOnlyForRollOvers=false);

	static BOOL GetUniverseRawData(int nUniverse, int nThisYearForEPS, COleDateTime date, SUniverseRawData& data);
	static BOOL GetCompanyRawData(int nThisYearForEPS, const char* szTicker, COleDateTime date, SCompanyRawData& data);

	static BOOL UpdateHistory(SHistory& history, int nColumns);
	static BOOL UpdateHistoryItem(const char* szTicker, SHistoryItem& item, int nColumns);
	static BOOL UpdateHistoryItem(int nCompanyID, SHistoryItem& item, int nColumns);
	
	static BOOL DeleteHistoryItem(const char* szTicker, COleDateTime date);
	static BOOL DeleteHistoryItem(int nCompanyID, COleDateTime date);
	static BOOL DeleteAllHistoryItems(COleDateTime date);

	static BOOL GetHistory(const char* szTicker, SHistory& history, COleDateTime from, COleDateTime to); 
	static BOOL GetHistory(const char* szTicker, SHistory& history, int nNumRecords=-1); 
	static BOOL FillHistoryItem(const char* szTicker, int nItem, SHistory& history, int nRows=-1); 
	static BOOL GetHistoryDates(const char* szTicker, SHistory& history); 
	
	static COleDateTime GetMinDateInHistories();
	static int GetHistoryDates(COleDateTime from, COleDateTime to, CArray<COleDateTime,COleDateTime>& dates);
	static int GetRankedValuesFromHistory(COleDateTime date, int nPercentFrom, int nPercentTo, const char* szRankBy, const char* szItem, CArray<double,double> &data);
	static int GetValuesFromHistoryByCompanyIds(COleDateTime date, CArray<double,double> &ids, const char* szItem, CArray<double,double> &data);

	static BOOL DoesInterestRateRecordExist(const COleDateTime& dateFriday);
	static int GetInterestRateHistory(const COleDateTime& date, int nPeriodsBack, SInterestRateHistory& data);
	static int GetInterestRateHistory(const COleDateTime& from, const COleDateTime& to, SInterestRateHistory& data);
	static double GetInterestRate(const COleDateTime& date);
	static BOOL UpdateInterestRate(const COleDateTime& date, double fWeeklyAve, double f3MAve, double fFridayClose);

	static COleDateTime GetLastAutoSplitDate();
	static BOOL SetLastAutoSplitDate(const COleDateTime &date);

	static const CString GetDatabase();

public:
	static COleDateTime ConvertSqlToOleDate(const char* szSqlDate);
	static COleDateTime ConvertUSToOleDate(const char* szUSDate);
	static CString PrepStrForSQL(const char* sz);

	static COleDateTime		m_Date;
};

#endif // !defined(AFX_SQLCOMPANY_H__C4F2DCD3_CF94_4EF6_9457_C75A5CE98432__INCLUDED_)
