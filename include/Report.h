// Report.h: interface for the CReport class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_REPORT_H__F04D7C4D_8029_4CAA_89C5_2B3D3070CF69__INCLUDED_)
#define AFX_REPORT_H__F04D7C4D_8029_4CAA_89C5_2B3D3070CF69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Universe.h"
#include "Company.h"

///////////////////////////////////////////////////////////////////////////////
struct SReportItem
{
	CString					sTicker;
	CString					sName;
	CArray<double,double>	arData;
};

///////////////////////////////////////////////////////////////////////////////
struct SReportGroup
{
	SReportGroup()
	{
		pAve = pCapAve = pPriceAve = NULL;
	}

	~SReportGroup()
	{
		for(int i=0; i<arItems.GetSize(); i++)
			delete arItems[i];
		delete pAve;
		delete pCapAve;
		delete pPriceAve;
	}
	
	CString sGroupName;

	CArray<SReportItem*,SReportItem*>	arItems;
	
	SReportItem*	pAve;
	SReportItem*	pCapAve;
	SReportItem*	pPriceAve;
};

///////////////////////////////////////////////////////////////////////////////
struct SReport
{
	SReport()
	{ 
		bSortByIndustry = bDontPrintIndStocks = false; 
	}
	
	~SReport()
	{
		for(int i=0; i<arGroups.GetSize(); i++)
			delete arGroups[i];
	}

	CString sName;
	CString sSortAndDate;
	CString sOptCaption;
	
	BOOL bSortByIndustry;
	BOOL bDontPrintIndStocks;

	CStringArray						arColNames;
	CArray<SReportGroup*,SReportGroup*>	arGroups; 
};

struct SReportFilterItem
{
	SReportFilterItem()
	{
	}
	
	SReportFilterItem(char* name, char* nameshort, BOOL inc, double min, double max, int style)
	{
		strcpy(szName, name);
		strcpy(szNameShort, nameshort);
		bInclude = inc;
		fMin = min;
		fMax = max;
		nStyle = style;
	}

	char szName[128];
	char szNameShort[64];
	BOOL bInclude;
	double fMin;
	double fMax;

	int	nStyle;  // 0 - %.2f (double), 1 - %.0f (int), 2 - %.2f%% (percent)
};
typedef CArray<SReportFilterItem,SReportFilterItem> SReportFilter;

///////////////////////////////////////////////////////////////////////////////
class _declspec(dllexport) CReport  
{
public:
	CReport();
	~CReport();

	static void ConstructDefaultFilter(SReportFilter& filter);

	static BOOL SortByColumn(SReportGroup& group, int nColumn, BOOL bAscending);
	static BOOL SortAsDouble(SReportGroup& group, int nColumn, int nSize, BOOL bAscending, int low, int high, BOOL &bContniue);

	static double GetCompanyItem(CCompany* pCompany, int nIndex);
	static void CalcAves(SReportGroup& group, CArray<CCompany*,CCompany*>& arCompanies, double fCapTotal, double fPriceTotal);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void GetGeneral( CUniverse* pUniverse, int nGrouping, BOOL bNotIndividualNames, int nSort, const char* szTitle, 
									double fMinCap, double fMaxCap, CArray<int,int>& arFilters, SReportFilter& filter, SReport& rep);
	static void GetGeneralGroup(CArray<CCompany*,CCompany*>& arCompanies, BOOL bNotIndividualNames, int nSort,  
									double fMinCap, double fMaxCap, CArray<int,int>& arFilters, SReportFilter& filter, SReportGroup* &pGroup);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void GetEBITDA_Leverage(	CUniverse* pUniverse, int nGrouping, BOOL bNotIndividualNames, int nSort, const char* szTitle, 
									double fMinCap, double fMaxCap, CArray<int,int>& arFilters, SReport& rep);	
	static void GetEBITDA_LeverageGroup(CArray<CCompany*,CCompany*>& arCompanies, BOOL bNotIndividualNames, int nSort,  
									double fMinCap, double fMaxCap, CArray<int,int>& arFilters, SReportGroup* &pGroup);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void GetEarningsMomentum(CUniverse* pUniverse, int nGrouping, BOOL bNotIndividualNames, int nSort, const char* szTitle, 
									double fMinCap, double fMaxCap, CArray<int,int>& arFilters, SReport& rep);
	static void GetEarningsMomentumGroup(CArray<CCompany*,CCompany*>& arCompanies, BOOL bNotIndividualNames, int nSort,  
									double fMinCap, double fMaxCap, CArray<int,int>& arFilters, SReportGroup* &pGroup);

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	static void GetValueMomentum1(CUniverse* pUniverse, int nGrouping, BOOL bNotIndividualNames, int nSort, const char* szTitle, 
									double fMinCap, double fMaxCap, CArray<int,int>& arFilters, SReport& rep);
	static void GetValueMomentumGroup1(CArray<CCompany*,CCompany*>& arCompanies, BOOL bNotIndividualNames, int nSort,  
									double fMinCap, double fMaxCap, CArray<int,int>& arFilters, SReportGroup* &pGroup);

};

#endif // !defined(AFX_REPORT_H__F04D7C4D_8029_4CAA_89C5_2B3D3070CF69__INCLUDED_)
