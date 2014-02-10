// DataManager.h: interface for the CDataManager class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_DATAMANAGER_H__B04CF5A0_40E1_4439_9C64_51D081E038D1__INCLUDED_)
#define AFX_DATAMANAGER_H__B04CF5A0_40E1_4439_9C64_51D081E038D1__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "input.h"
#include "history.h"
#include "company.h"
#include "universe.h"

#include "BaseMath.h" 

////////////////////////////////////////////////////////////////////////////////////
struct S_DATA_NAME_INFO
{
	char name[16];
	char dbname[32];
	int  col;
};

////////////////////////////////////////////////////////////////////////////////////
class _declspec(dllexport) CData
{
public:
	CData();
	CData(int nSize);
	~CData();

	void Clear();
	void SetSize(int nSize);
	int GetSize()						{ return m_nSize;			}

	double operator [](int index)		{ return m_pValues[index];	}
	double operator [](double date);
	void operator =(const CData& src);

	int	GetIndexFromDate(double date);	

public:
	char		m_szToken[16];

	double*		m_pDates;
	double*		m_pValues;
	int			m_nSize;
};
typedef CArray<CData*,CData*>	CDataCollection;

////////////////////////////////////////////////////////////////////////////////////
class _declspec(dllexport) CDataManager  
{
public:
	CDataManager(CUniverse* pUniverse);
	~CDataManager();

public:
	int		GetDataCollection(const char* szTokens, const char* szBase, CDataCollection& dataCollection, COleDateTime from, COleDateTime to);
	
	///////////////////////////////////////////////////////////////////////////////////////////
	int		GetDataForToken(const char* szToken, CData& data, COleDateTime from, COleDateTime to);

	///////////////////////////////////////////////////////////////////////////////////////////
	// different cases (ticker, industry, sector)
	int		GetDataForTicker(const char* szTicker, const char* item, CData& data, COleDateTime from, COleDateTime to);
	
	int		GetDataForIndustryAveMethod1(const char* szTicker, const char* szItem, CData& data, COleDateTime from, COleDateTime to);
	int		GetDataForIndustryAveMethod2(const char* szTicker, const char* szItem, CData& data, COleDateTime from, COleDateTime to);
	int		GetDataForIndustryAveMethod3(const char* szTicker, const char* szItem, CData& data, COleDateTime from, COleDateTime to);
	
	int		GetDataForSectorAveMethod1(const char* szTicker, const char* szItem, CData& data, COleDateTime from, COleDateTime to);
	int		GetDataForSectorAveMethod2(const char* szTicker, const char* szItem, CData& data, COleDateTime from, COleDateTime to);
	int		GetDataForSectorAveMethod3(const char* szTicker, const char* szItem, CData& data, COleDateTime from, COleDateTime to);

	int		GetDataForIndex(const char* szIndex, const char* szItem, CData& data, COleDateTime from, COleDateTime to);
	int		GetDataForT10(const char* szItem, CData& data, COleDateTime from, COleDateTime to);

	int		GetDataForDecileStatic(int nQuant, const char* szItem, CData& data, COleDateTime from, COleDateTime to, int nMethod=1);
	int		GetDataForDecileDynamic(int nQuant, const char* szItem, CData& data, COleDateTime from, COleDateTime to, int nMethod=1);

	int		GetDataForQuantileStatic(int nQuant, const char* szItem, CData& data, COleDateTime from, COleDateTime to, int nMethod=1);
	int		GetDataForQuantileDynamic(int nQuant, const char* szItem, CData& data, COleDateTime from, COleDateTime to, int nMethod=1);

	int		CalcDataSimpleAve(CCompany* pBaseCompany, CArray<CCompany*,CCompany*>& arCompanies, const char* szItem, CData& data, COleDateTime from, COleDateTime to);
	int		CalcDataPriceWeightedAve(CCompany* pBaseCompany, CArray<CCompany*,CCompany*>& arCompanies, const char* szItem, CData& data, COleDateTime from, COleDateTime to);
	int		CalcDataMCapWeightedAve(CCompany* pBaseCompany, CArray<CCompany*,CCompany*>& arCompanies, const char* szItem, CData& data, COleDateTime from, COleDateTime to);

	void	AlignDataToBase(CData& data, CData& base); 

public:
	static const char* GetColName(int i);
	static int GetColIndex(int i);
	static int GetColIndexFromName(const char* name);
	static CString GetColDBNameFromName(const char* name);

	static void CopyDataCollection(const CDataCollection& src, CDataCollection& target);
	static void	FreeDataCollection(CDataCollection& dataCollection);

protected:
	CUniverse*		m_pUniverse;

};

#endif // !defined(AFX_DATAMANAGER_H__B04CF5A0_40E1_4439_9C64_51D081E038D1__INCLUDED_)
