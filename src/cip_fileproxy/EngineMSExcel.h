// EngineMSExcel.h: interface for the CEngineMSExcel class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ENGINEMSEXCEL_H__4C0F1AFC_56C6_4BED_BA7D_A1854A307B4C__INCLUDED_)
#define AFX_ENGINEMSEXCEL_H__4C0F1AFC_56C6_4BED_BA7D_A1854A307B4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "input.h"

class _Application;
class _Worksheet;
class _Workbook;
class Range;

//////////////////////////////////////////////////////////////////////////////////
class _declspec(dllexport) CEngineMSExcel
{
public:
	CEngineMSExcel();
	~CEngineMSExcel();

	BOOL	InitBaseline();
	COleVariant GetBaselineData(const char* szTicker, const char* szItem, const char* szTime);
	void	ReleaseBaseline();

	int		LoadEpsRocRoeFromBloomberg(const char* szExcelFileName, CStringArray& arTickers, SUniverseDataEpsRocRoe& UniverseData);
	BOOL	LoadEpsRocRoeBloomberg(_Worksheet* pSheet, CStringArray& arTickers, SUniverseDataEpsRocRoe& UniverseData);

	int		LoadFundamentalsFromBaseLine(const char* szExcelFileName, CStringArray& arTickers, SUniverseDataFundamentals& UniverseData);
	int		LoadPricesFromBaseLine(const char* szExcelFileName, CStringArray& arTickers, SUniverseDataPricesAndReturns& UniverseData);
	int		LoadEpsRocRoeFromBaseLine(const char* szExcelFileName, CStringArray& arTickers, SUniverseDataEpsRocRoe& UniverseData);

	int		LoadFundamentalsFromFactSet(const char* szExcelFileName, const COleDateTime &date, CStringArray& arTickers, SUniverseDataFundamentals& UniverseData);
	int		LoadEpsRocRoeFromFactSet(const char* szExcelFileName, const COleDateTime &date, CStringArray& arTickers, SUniverseDataEpsRocRoe& UniverseData);

public:
	BOOL	LoadFundamentals(const char* sFile, SUniverseDataFundamentals& UniverseData);
	BOOL	LoadEpsRocAndRoe(const char* sFile, SUniverseDataEpsRocRoe& UniverseData);
	BOOL	LoadPricesAndReturns(const char* sFilePrices, const char* sFileReturns, SUniverseDataPricesAndReturns& UniverseData);
	BOOL	LoadWeightsInIndices(const char* sFile, SUniverseWeightsInIndices& UniverseData);
	BOOL	LoadShares(const char* sFile, SUniverseDataShares& UniverseData);

	BOOL	LoadHistory(const char* sFile, SHistory& history);

	BOOL	PutTickersIntoFundamentalsSheet(_Worksheet* pSheet, CStringArray& arTickers);
	BOOL	PutTickersIntoPricesSheet(_Worksheet* pSheet, CStringArray& arTickers);

	BOOL	ActivateBaseLineAddIn();

	static COleDateTime GetFromStr(const CString &sDate);
	static COleDateTime GetLastFriday(const COleDateTime &date);

	COleDateTime	GetLastWeekT10ValuesFromBaseLine(const COleDateTime &date, double values[5]);

////////////////////////////////////////////////
	BOOL	LoadFundamentalsFromFactSet(_Worksheet* pSheet, const COleDateTime &date, CStringArray& arTickers, SUniverseDataFundamentals& data);
	BOOL	LoadEpsRocRoeFactSet(_Worksheet* pSheet, const COleDateTime &date, CStringArray& arTickers, SUniverseDataEpsRocRoe& data);

// protected functions for internal usage
protected:
	BOOL	LoadFundamentals(_Worksheet* pSheet, SUniverseDataFundamentals& data);
	BOOL	LoadFundamentals(_Worksheet* pSheet, CStringArray& arTickers, SUniverseDataFundamentals& data);

	BOOL	LoadEps(_Worksheet* pSheet, SUniverseDataEpsRocRoe& data);
	BOOL	LoadRocRoeIntoExistingData(_Worksheet* pSheet, SUniverseDataEpsRocRoe& data);
	
	BOOL	LoadEpsRocRoeCleanSweet(_Worksheet* pSheet, CStringArray& arTickers, SUniverseDataEpsRocRoe& data);
	
	BOOL	LoadPrices(_Worksheet* pSheet, SUniverseDataPricesAndReturns& data);
	BOOL	LoadPrices(_Worksheet* pSheet, CStringArray& arTickers, SUniverseDataPricesAndReturns& data);
	BOOL	LoadReturnsIntoExistingData(_Worksheet* pSheet, SUniverseDataPricesAndReturns& data);

	BOOL	LoadWeightsInIndices(_Worksheet* pSheet, SUniverseWeightsInIndices& data);
	BOOL	LoadShares(_Worksheet* pSheet, SUniverseDataShares& data);

	BOOL	LoadHistory(_Worksheet* pSheet, SHistory& history);

	static COleDateTime ConvertUSToOleDate(const char* szUSDate);

public:

	_Application*	m_pExcelApp;
	_Workbook*		m_pActiveWBook;
	Range*			m_pProxy;

	BOOL			m_bAutoQuit;

};

#endif // !defined(AFX_ENGINEMSEXCEL_H__4C0F1AFC_56C6_4BED_BA7D_A1854A307B4C__INCLUDED_)
