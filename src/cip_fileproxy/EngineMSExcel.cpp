// EngineMSExcel.cpp: implementation of the CEngineMSExcel class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "cip_fileproxy.h"
#include "EngineMSExcel.h"
#include "excel.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW 
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CEngineMSExcel::CEngineMSExcel()
{
	m_pExcelApp = NULL;
	m_pActiveWBook = NULL;
	m_pProxy = NULL;
	m_bAutoQuit = true;
}

CEngineMSExcel::~CEngineMSExcel()
{
	if(m_pProxy)
		delete m_pProxy;

	if(m_pActiveWBook)
		delete m_pActiveWBook;

	if(m_bAutoQuit && m_pExcelApp)
	{
		m_pExcelApp->Quit();
		delete m_pExcelApp;
		m_pExcelApp = NULL;
	}
}

BOOL CEngineMSExcel::InitBaseline()
{
	if(!m_pExcelApp)
	{
		m_pExcelApp = new _Application;
		BOOL bResult = m_pExcelApp->CreateDispatch("Excel.Application");
		if(!bResult)
		{
			::MessageBox(NULL, "Unable to run MS Excel. Please make sure that MS Office is properly installed.", "Cornerstone Fair Value Model", MB_ICONSTOP);
			delete m_pExcelApp;
			m_pExcelApp = NULL;
			return false;
		}

		if(!ActivateBaseLineAddIn())
			return false;
	}

	if(!m_pProxy || !m_pProxy->m_lpDispatch)
	{
		// get Workbooks interface 
		Workbooks wbooks(m_pExcelApp->GetWorkbooks());
		LPDISPATCH pD = wbooks.Add(COleVariant((long)1));
		m_pActiveWBook = new _Workbook(pD); 
		Worksheets sheets(m_pActiveWBook->GetWorksheets());

		// get the sheet
		pD = sheets.GetItem(COleVariant((long)1));
		_Worksheet sheet(pD);

		m_pProxy = new Range(sheet.GetRange(COleVariant("A1"), COleVariant("A1")));
	}
	return true;
}

COleVariant CEngineMSExcel::GetBaselineData(const char* szTicker, const char* szItem, const char* szTime)
{
	COleVariant val;

	if(!m_pProxy || !m_pProxy->m_lpDispatch)
		return val;

	CString sFormula;
	sFormula.Format("=Baseline(\"%s\", \"%s\", \"%s\")", szTicker, szItem, szTime);

	m_pProxy->SetFormula(COleVariant(sFormula));	
	return m_pProxy->GetValue2();

}

void CEngineMSExcel::ReleaseBaseline()
{
	if(m_pProxy)
	{
		delete m_pProxy;
		m_pProxy = NULL;
	}

	if(m_pActiveWBook)
	{
		COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
		m_pActiveWBook->Close(COleVariant((long)FALSE), covOptional, covOptional);

		delete m_pActiveWBook;
		m_pActiveWBook = NULL;
	}

	if(m_pExcelApp)
	{
		m_pExcelApp->Quit();
		delete m_pExcelApp;
		m_pExcelApp = NULL;
	}
}

COleDateTime CEngineMSExcel::GetLastWeekT10ValuesFromBaseLine(const COleDateTime &date, double values[5])
{
	COleDateTime friday;
	friday.SetStatus(COleDateTime::invalid);

	if(!m_pExcelApp)
	{
		m_pExcelApp = new _Application;
		BOOL bResult = m_pExcelApp->CreateDispatch("Excel.Application");
		if(!bResult)
		{
			::MessageBox(NULL, "Unable to run MS Excel. Please make sure that MS Office is properly installed.", "Cornerstone Fair Value Model", MB_ICONSTOP);
			delete m_pExcelApp;
			m_pExcelApp = NULL;
			return friday;
		}
	}

	//m_pExcelApp->SetVisible(true);

	// Activate Baseline
	if(!ActivateBaseLineAddIn())
		return friday;

	// get Workbooks interface
	Workbooks wbooks(m_pExcelApp->GetWorkbooks());

	// open the Fundamentals file
	LPDISPATCH pD = wbooks.Add(COleVariant((long)1));
	_Workbook wbook(pD); 
	Worksheets sheets(wbook.GetWorksheets());
	_Worksheet sheet1(sheets.GetItem(COleVariant((long)1)));

	Range rgDate, rgVal;
	rgDate.AttachDispatch(sheet1.GetRange(COleVariant("A1"), COleVariant("A1")));
	rgVal.AttachDispatch(sheet1.GetRange(COleVariant("B1"), COleVariant("B1")));
	rgVal.SetFormula(COleVariant("=Baseline(\"t10\",\"Daily Closing Price\",A1)"));

	friday = GetLastFriday(date);	

	for(int i=0; i<5; i++)
	{
		COleDateTimeSpan sp(i, 0,0,0);
		COleDateTime dt = friday - sp;
		
		CString sDate = dt.Format("%m/%d/%Y");
		rgDate.SetValue2(COleVariant(sDate));
		COleVariant v = rgVal.GetValue2();
		if(v.vt == VT_R8)
			values[4-i] = v.dblVal;
		else
			values[4-i] = _INF_;
	}

	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	wbook.Close(COleVariant((long)FALSE), covOptional, covOptional);

	return friday;
}

int	CEngineMSExcel::LoadFundamentalsFromBaseLine(const char* szExcelFileName, CStringArray& arTickers, SUniverseDataFundamentals& UniverseData)
{
	if(!m_pExcelApp)
	{
		m_pExcelApp = new _Application;
		BOOL bResult = m_pExcelApp->CreateDispatch("Excel.Application");
		if(!bResult)
		{
			::MessageBox(NULL, "Unable to run MS Excel. Please make sure that MS Office is properly installed.", "Cornerstone Fair Value Model", MB_ICONSTOP);
			delete m_pExcelApp;
			m_pExcelApp = NULL;
			return -1;
		}
	}

	// Activate Baseline
	if(!ActivateBaseLineAddIn())
		return -2;

	// get Workbooks interface
	Workbooks wbooks(m_pExcelApp->GetWorkbooks());

	// open the Fundamentals file
	LPDISPATCH pD = wbooks.Open(szExcelFileName);
	if(pD == NULL)
	{
		CString sErr; sErr.Format("Unable to open Excel file: '%s'.", szExcelFileName);
		::MessageBox(NULL, sErr, "Cornerstone Fair Value Model", MB_ICONSTOP);
		m_pExcelApp->Quit();
		delete m_pExcelApp;
		m_pExcelApp = NULL;
		return false;
	}

	// Workbook and Worksheets
	_Workbook wbook(pD); 
	Worksheets sheets(wbook.GetWorksheets());

	// Load from Data from worksheet
	_Worksheet sheet1(sheets.GetItem(COleVariant((long)1)));
	BOOL bRet = LoadFundamentals(&sheet1, arTickers, UniverseData);

	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	wbook.Close(COleVariant((long)TRUE), covOptional, covOptional);

	return bRet;
}

int	CEngineMSExcel::LoadFundamentalsFromFactSet(const char* szExcelFileName, const COleDateTime &date, CStringArray& arTickers, SUniverseDataFundamentals& UniverseData)
{
	if(!m_pExcelApp)
	{
		m_pExcelApp = new _Application;
		BOOL bResult = m_pExcelApp->CreateDispatch("Excel.Application");
		if(!bResult)
		{
			::MessageBox(NULL, "Unable to run MS Excel. Please make sure that MS Office is properly installed.", "Cornerstone Fair Value Model", MB_ICONSTOP);
			delete m_pExcelApp;
			m_pExcelApp = NULL;
			return -1;
		}
	}

	// get Workbooks interface
	Workbooks wbooks(m_pExcelApp->GetWorkbooks());

	// open the Fundamentals file
	LPDISPATCH pD = wbooks.Open(szExcelFileName);
	if(pD == NULL)
	{
		CString sErr; sErr.Format("Unable to open Excel file: '%s'.", szExcelFileName);
		::MessageBox(NULL, sErr, "Cornerstone Fair Value Model", MB_ICONSTOP);
		m_pExcelApp->Quit();
		delete m_pExcelApp;
		m_pExcelApp = NULL;
		return false;
	}

	// Workbook and Worksheets
	_Workbook wbook(pD); 
	Worksheets sheets(wbook.GetWorksheets());

	// Load from Data from worksheet
	_Worksheet sheet1(sheets.GetItem(COleVariant("FactSet")));
	BOOL bRet = LoadFundamentalsFromFactSet(&sheet1, date, arTickers, UniverseData);

	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	wbook.Close(COleVariant((long)FALSE), covOptional, covOptional);

	return bRet;
}

int CEngineMSExcel::LoadPricesFromBaseLine(const char* szExcelFileName, CStringArray& arTickers, SUniverseDataPricesAndReturns& UniverseData)
{
	if(!m_pExcelApp)
	{
		m_pExcelApp = new _Application;
		BOOL bResult = m_pExcelApp->CreateDispatch("Excel.Application");
		if(!bResult)
		{
			::MessageBox(NULL, "Unable to run MS Excel. Please make sure that MS Office is properly installed.", "Cornerstone Fair Value Model", MB_ICONSTOP);
			delete m_pExcelApp;
			m_pExcelApp = NULL;
			return -1;
		}
	}

	// Activate Baseline
	if(!ActivateBaseLineAddIn())
		return -2;
	
	// get Workbooks interface
	Workbooks wbooks(m_pExcelApp->GetWorkbooks());

	// open the Fundamentals file
	LPDISPATCH pD = wbooks.Open(szExcelFileName);
	if(pD == NULL)
	{
		CString sErr; sErr.Format("Unable to open Excel file: '%s'.", szExcelFileName);
		::MessageBox(NULL, sErr, "Cornerstone Fair Value Model", MB_ICONSTOP);
		m_pExcelApp->Quit();
		delete m_pExcelApp;
		m_pExcelApp = NULL;
		return false;
	}

	// Workbook and Worksheets
	_Workbook wbook(pD); 
	Worksheets sheets(wbook.GetWorksheets());

	// Load Data from worksheet
	_Worksheet sheet1(sheets.GetItem(COleVariant((long)1)));

	BOOL bRet = LoadPrices(&sheet1, arTickers, UniverseData);

	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	wbook.Close(COleVariant((long)TRUE), covOptional, covOptional);

	return bRet;
}

int CEngineMSExcel::LoadEpsRocRoeFromBloomberg(const char* szExcelFileName, CStringArray& arTickers, SUniverseDataEpsRocRoe& UniverseData)
{
	if(!m_pExcelApp)
	{
		m_pExcelApp = new _Application;
		BOOL bResult = m_pExcelApp->CreateDispatch("Excel.Application");
		if(!bResult)
		{
			::MessageBox(NULL, "Unable to run MS Excel. Please make sure that MS Office is properly installed.", "Cornerstone Fair Value Model", MB_ICONSTOP);
			delete m_pExcelApp;
			m_pExcelApp = NULL;
			return -1;
		}
	}
	m_pExcelApp->SetVisible(true);

	// get Workbooks interface
	Workbooks wbooks(m_pExcelApp->GetWorkbooks());

	// open the Fundamentals file
	LPDISPATCH pD = wbooks.Open(szExcelFileName);
	if(pD == NULL)
	{
		CString sErr; sErr.Format("Unable to open Excel file: '%s'.", szExcelFileName);
		::MessageBox(NULL, sErr, "Cornerstone Fair Value Model", MB_ICONSTOP);
		m_pExcelApp->Quit();
		delete m_pExcelApp;
		m_pExcelApp = NULL;
		return false;
	}

	// Workbook and Worksheets
	_Workbook wbook(pD); 
	Worksheets sheets(wbook.GetWorksheets());

	// Load from Data from worksheet
	_Worksheet sheet1(sheets.GetItem(COleVariant((long)1)));
	BOOL bRet = LoadEpsRocRoeBloomberg(&sheet1, arTickers, UniverseData);

	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	wbook.Close(COleVariant((long)FALSE), covOptional, covOptional);

	return bRet;
}

BOOL CEngineMSExcel::LoadEpsRocRoeBloomberg(_Worksheet* pSheet, CStringArray& arTickers, SUniverseDataEpsRocRoe& UniverseData)
{
	Range rgYear, rgTicker, rgEps, rgRoe, rgRoe1, rgRoc, rgRoc1, rgShares;
	COleVariant v;

	rgYear.AttachDispatch(pSheet->GetRange(COleVariant("E1"), COleVariant("E1")));
	rgTicker.AttachDispatch(pSheet->GetRange(COleVariant("A4"), COleVariant("A4")));
	rgEps.AttachDispatch(pSheet->GetRange(COleVariant("B6"), COleVariant("N6")));
	rgRoe.AttachDispatch(pSheet->GetRange(COleVariant("B14"), COleVariant("N14")));
	rgRoe1.AttachDispatch(pSheet->GetRange(COleVariant("B15"), COleVariant("N15")));
	rgRoc.AttachDispatch(pSheet->GetRange(COleVariant("B20"), COleVariant("N20")));
	rgRoc1.AttachDispatch(pSheet->GetRange(COleVariant("B21"), COleVariant("N21")));
	rgShares.AttachDispatch(pSheet->GetRange(COleVariant("B23"), COleVariant("N23")));

	for(int i=0; i<arTickers.GetSize(); i++)
	{
		SCompanyDataEpsRocRoe data;

		CString sTicker = arTickers[i];
		rgTicker.SetValue2(COleVariant((LPCTSTR)sTicker));

		v = rgYear.GetValue2();
		if(v.vt != VT_R8)
			continue;
		
		strcpy(data.sTicker, sTicker);
		data.nYear0 = (int)v.dblVal - 10;
		data.fAdjustAdd = _INF_;
		data.fAdjustMult = _INF_;

		COleSafeArray saEps = rgEps.GetValue2();
		COleSafeArray saRoe = rgRoe.GetValue2();
		COleSafeArray saRoc = rgRoc.GetValue2();
		COleSafeArray saRoe1 = rgRoe1.GetValue2();
		COleSafeArray saRoc1 = rgRoc1.GetValue2();
		COleSafeArray saShares = rgShares.GetValue2();

		double fSumRoe=0, fSumRoe1=0;
		int nRoe=0, nRoe1=0;

		double fSumRoc=0, fSumRoc1=0;
		int nRoc=0, nRoc1=0;

		double roe[NUM_YEARS_FOR_ROC], roc[NUM_YEARS_FOR_ROC], roe1[NUM_YEARS_FOR_ROC], roc1[NUM_YEARS_FOR_ROC];

		long index[2]; 
		index[0] = 1;
		for(int i=0; i<NUM_YEARS_FOR_ROC; i++)
		{
			index[1] = i+1;

			if(i<NUM_YEARS_FOR_EPS)
			{
				saEps.GetElement(index , &v);
				data.arEps[i] = (v.vt == VT_R8) ? v.dblVal : _INF_;
			}
				
			saRoe.GetElement(index , &v);
			roe[i] = (v.vt == VT_R8) ? v.dblVal*100 : _INF_;

			saRoc.GetElement(index , &v);
			roc[i] = (v.vt == VT_R8) ? v.dblVal*100 : _INF_;

			saRoe1.GetElement(index , &v);
			roe1[i] = (v.vt == VT_R8) ? v.dblVal*100 : _INF_;

			saRoc1.GetElement(index , &v);
			roc1[i] = (v.vt == VT_R8) ? v.dblVal*100 : _INF_;

			saShares.GetElement(index , &v);
			data.arShares[i] = (v.vt == VT_R8) ? v.dblVal : _INF_;

			if(i >= NUM_YEARS_FOR_ROC-7)
			{
				if(roe[i] < _INF_)	{	fSumRoe+=roe[i]; nRoe++;	}
				if(roe1[i] < _INF_) {	fSumRoe1+=roe1[i]; nRoe1++;	}
				if(roc[i] < _INF_)	{	fSumRoc+=roc[i]; nRoc++;	}
				if(roc1[i] < _INF_) {	fSumRoc1+=roc1[i]; nRoc1++;	}
			}
		}

		BOOL bRoc = true;
		if(nRoc > 0 && nRoc1 > 0)
		{
			double fAveRoc = fSumRoc/(double)nRoc;
			double fAveRoc1 = fSumRoc1/(double)nRoc1;
			if(fAveRoc1 < fAveRoc)
				bRoc = false;
		}
		if(bRoc)
		{
			memcpy(data.arRoc, roc, sizeof(double)*NUM_YEARS_FOR_ROC);
			memcpy(data.arRoe, roe, sizeof(double)*NUM_YEARS_FOR_ROC);
		}
		else
		{
			memcpy(data.arRoc, roc1, sizeof(double)*NUM_YEARS_FOR_ROC);
			memcpy(data.arRoe, roe1, sizeof(double)*NUM_YEARS_FOR_ROC);
		}

		UniverseData.Add(data);
	}


	return true;
}

int CEngineMSExcel::LoadEpsRocRoeFromBaseLine(const char* szExcelFileName, CStringArray& arTickers, SUniverseDataEpsRocRoe& UniverseData)
{
	if(!m_pExcelApp)
	{
		m_pExcelApp = new _Application;
		BOOL bResult = m_pExcelApp->CreateDispatch("Excel.Application");
		if(!bResult)
		{
			::MessageBox(NULL, "Unable to run MS Excel. Please make sure that MS Office is properly installed.", "Cornerstone Fair Value Model", MB_ICONSTOP);
			delete m_pExcelApp;
			m_pExcelApp = NULL;
			return -1;
		}
	}

	// Activate Baseline
	if(!ActivateBaseLineAddIn())
		return -2;
	
	// get Workbooks interface
	Workbooks wbooks(m_pExcelApp->GetWorkbooks());

	// open the Fundamentals file
	LPDISPATCH pD = wbooks.Open(szExcelFileName);
	if(pD == NULL)
	{
		CString sErr; sErr.Format("Unable to open Excel file: '%s'.", szExcelFileName);
		::MessageBox(NULL, sErr, "Cornerstone Fair Value Model", MB_ICONSTOP);
		m_pExcelApp->Quit();
		delete m_pExcelApp;
		m_pExcelApp = NULL;
		return false;
	}

	// Workbook and Worksheets
	_Workbook wbook(pD); 
	Worksheets sheets(wbook.GetWorksheets());

	// Load from Data from worksheet
	_Worksheet sheet1(sheets.GetItem(COleVariant((long)1)));
	BOOL bRet = LoadEpsRocRoeCleanSweet(&sheet1, arTickers, UniverseData);

	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	wbook.Close(COleVariant((long)FALSE), covOptional, covOptional);

	return bRet;
}

int CEngineMSExcel::LoadEpsRocRoeFromFactSet(const char* szExcelFileName, const COleDateTime &date, CStringArray& arTickers, SUniverseDataEpsRocRoe& UniverseData)
{
	if(!m_pExcelApp)
	{
		m_pExcelApp = new _Application;
		BOOL bResult = m_pExcelApp->CreateDispatch("Excel.Application");
		if(!bResult)
		{
			::MessageBox(NULL, "Unable to run MS Excel. Please make sure that MS Office is properly installed.", "Cornerstone Fair Value Model", MB_ICONSTOP);
			delete m_pExcelApp;
			m_pExcelApp = NULL;
			return -1;
		}
	}
	
	// get Workbooks interface
	Workbooks wbooks(m_pExcelApp->GetWorkbooks());

	// open the Fundamentals file
	LPDISPATCH pD = wbooks.Open(szExcelFileName);
	if(pD == NULL)
	{
		CString sErr; sErr.Format("Unable to open Excel file: '%s'.", szExcelFileName);
		::MessageBox(NULL, sErr, "Cornerstone Fair Value Model", MB_ICONSTOP);
		m_pExcelApp->Quit();
		delete m_pExcelApp;
		m_pExcelApp = NULL;
		return false;
	}

	// Workbook and Worksheets
	_Workbook wbook(pD); 
	Worksheets sheets(wbook.GetWorksheets());

	// Load from Data from worksheet
	_Worksheet sheet1(sheets.GetItem(COleVariant("Template")));
	BOOL bRet = LoadEpsRocRoeFactSet(&sheet1, date, arTickers, UniverseData);

	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	wbook.Close(COleVariant((long)FALSE), covOptional, covOptional);

	return bRet;
}

BOOL CEngineMSExcel::ActivateBaseLineAddIn()
{
	CString sAddInName = "Baseline Financial Services Add-In";
	AddIns ains(m_pExcelApp->GetAddIns());
	AddIn addin;

	try
	{
		LPDISPATCH p = ains.GetItem(COleVariant(sAddInName));
		if(p)
			addin.AttachDispatch(p);
	}
	catch(...)
	{
		return false;
	}

	if(!addin.m_lpDispatch)
		return false;

	addin.SetInstalled(false);
	addin.SetInstalled(true);

	return true;
}

BOOL CEngineMSExcel::PutTickersIntoFundamentalsSheet(_Worksheet* pSheet, CStringArray& arTickers)
{
	Range r; 
	COleVariant v;
	COleSafeArray sa;

	CString sLastCell; sLastCell.Format("A%u", arTickers.GetSize()+4+1);
	

	DWORD length[2]; length[0] = length[1] = arTickers.GetSize()+1;
	sa.Create(VT_BSTR, 2, length);
	long index[2];
	index[1] = 1; 

	int nRow;
	for(nRow = 0; nRow < arTickers.GetSize(); nRow++)
	{
		CString s; s.Format("A%u", nRow+5);
		r.AttachDispatch(pSheet->GetRange( COleVariant(s),  COleVariant(s) ));
		r.SetValue2(COleVariant(arTickers[nRow]));
	}
	
	CString s; s.Format("A%u", arTickers.GetSize()+5);
	r.AttachDispatch(pSheet->GetRange( COleVariant(s),  COleVariant(s) ));
	r.SetValue2(COleVariant(""));
 
	return true;
}

BOOL CEngineMSExcel::PutTickersIntoPricesSheet(_Worksheet* pSheet, CStringArray& arTickers)
{
	Range r; 
	int nRow;
	for(nRow = 0; nRow < arTickers.GetSize(); nRow++)
	{
		CString s; s.Format("A%u", nRow+5);
		r.AttachDispatch(pSheet->GetRange( COleVariant(s),  COleVariant(s) ));
		r.SetValue2(COleVariant(arTickers[nRow]));
	}
	
	CString s; s.Format("A%u", arTickers.GetSize()+5);
	r.AttachDispatch(pSheet->GetRange( COleVariant(s),  COleVariant(s) ));
	r.SetValue2(COleVariant(""));

	return true;
}

BOOL CEngineMSExcel::LoadEpsRocAndRoe(const char* sFileEPS, SUniverseDataEpsRocRoe& dataUniverse)
{
	if(!m_pExcelApp)
	{
		m_pExcelApp = new _Application;
		BOOL bResult = m_pExcelApp->CreateDispatch("Excel.Application");
		if(!bResult)
		{
			::MessageBox(NULL, "Unable to run MS Excel. Please make sure that MS Office is properly installed.", "Cornerstone Fair Value Model", MB_ICONSTOP);
			delete m_pExcelApp;
			m_pExcelApp = NULL;
			return false;
		}
	}

	// get Workbooks interface
	Workbooks wbooks(m_pExcelApp->GetWorkbooks());

	// open the EPS file
	LPDISPATCH pD = wbooks.Open(sFileEPS);
	if(pD == NULL)
	{
		CString sErr; sErr.Format("Unable to open Excel file: '%s'.", sFileEPS);
		::MessageBox(NULL, sErr, "Cornerstone Fair Value Model", MB_ICONSTOP);
		m_pExcelApp->Quit();
		delete m_pExcelApp;
		m_pExcelApp = NULL;
		return false;
	}

	// Workbook and Worksheets
	_Workbook wbook(pD); 
	Worksheets sheets(wbook.GetWorksheets());

	// Load from EPS sheet
	pD = sheets.GetItem(COleVariant("EPS"));
	if(pD == NULL)
	{
		CString sErr; sErr.Format("Unable to open Excel file: '%s'.", sFileEPS);
		::MessageBox(NULL, sErr, "Cornerstone Fair Value Model", MB_ICONSTOP);
		m_pExcelApp->Quit();
		delete m_pExcelApp;
		m_pExcelApp = NULL;
		return false;
	}

	_Worksheet sheet1(pD);
	if(!LoadEps(&sheet1, dataUniverse))
		return false;
	
	// Load from ROC and ROE sheet
	pD = sheets.GetItem(COleVariant("Returns"));
	if(pD == NULL)
	{
		CString sErr; sErr.Format("Unable to open Excel file: '%s'.", sFileEPS);
		::MessageBox(NULL, sErr, "Cornerstone Fair Value Model", MB_ICONSTOP);
		m_pExcelApp->Quit();
		delete m_pExcelApp;
		m_pExcelApp = NULL;
		return false;
	}
	_Worksheet sheet2(pD);
	if(!LoadRocRoeIntoExistingData(&sheet2, dataUniverse))
		return false;

	return true;
}

BOOL CEngineMSExcel::LoadPrices(_Worksheet* pSheet, CStringArray& arTickers, SUniverseDataPricesAndReturns& UniverseData)
{
	Range rgDate, rgTicker, rgPrice;
	COleVariant v;

	rgDate.AttachDispatch(pSheet->GetRange(COleVariant("B1"), COleVariant("B1")));
	rgTicker.AttachDispatch(pSheet->GetRange(COleVariant("A2"), COleVariant("A2")));
	rgPrice.AttachDispatch(pSheet->GetRange(COleVariant("B2"), COleVariant("B2")));
	
/*	// Get Date
	v = rgDate.GetValue2();
	if(v.vt != VT_R8)
		return false;
	COleDateTime date = v.date;*/

	COleDateTime today(COleDateTime::GetCurrentTime());
	COleDateTime date = GetLastFriday(today);		
	

	CString sDate = date.Format("%m/%d/%Y");
	rgDate.SetValue2(COleVariant(sDate));

	for(int i=0; i<arTickers.GetSize(); i++)
	{
		CString sTicker = arTickers[i];
		rgTicker.SetValue2(COleVariant((LPCTSTR)sTicker));

		v = rgPrice.GetValue2();
		if(v.vt != VT_R8) {
			//AfxMessageBox(sDate);
			continue;
		}

		SCompanyDataPricesAndReturns data;
		strcpy(data.sTicker, sTicker);
		data.date = date;
		data.fReturn = 0;

		data.fPrice = v.dblVal;
		UniverseData.Add(data);
	}

	return true;
}

BOOL CEngineMSExcel::LoadEpsRocRoeCleanSweet(_Worksheet* pSheet, CStringArray& arTickers, SUniverseDataEpsRocRoe& UniverseData)
{
	Range rgYear, rgTicker, rgEps, rgRoe, rgRoe1, rgRoc, rgRoc1, rgShares;
	COleVariant v;

	rgYear.AttachDispatch(pSheet->GetRange(COleVariant("E1"), COleVariant("E1")));
	rgTicker.AttachDispatch(pSheet->GetRange(COleVariant("A4"), COleVariant("A4")));
	rgEps.AttachDispatch(pSheet->GetRange(COleVariant("B6"), COleVariant("N6")));
	rgRoe.AttachDispatch(pSheet->GetRange(COleVariant("B14"), COleVariant("N14")));
	rgRoe1.AttachDispatch(pSheet->GetRange(COleVariant("B15"), COleVariant("N15")));
	rgRoc.AttachDispatch(pSheet->GetRange(COleVariant("B20"), COleVariant("N20")));
	rgRoc1.AttachDispatch(pSheet->GetRange(COleVariant("B21"), COleVariant("N21")));
	rgShares.AttachDispatch(pSheet->GetRange(COleVariant("B23"), COleVariant("N23")));

	for(int i=0; i<arTickers.GetSize(); i++)
	{
		SCompanyDataEpsRocRoe data;

		CString sTicker = arTickers[i];
		rgTicker.SetValue2(COleVariant((LPCTSTR)sTicker));

		v = rgYear.GetValue2();
		if(v.vt != VT_R8)
			continue;
		
		strcpy(data.sTicker, sTicker);
		data.nYear0 = (int)v.dblVal - 10;
		data.fAdjustAdd = _INF_;
		data.fAdjustMult = _INF_;

		COleSafeArray saEps = rgEps.GetValue2();
		COleSafeArray saRoe = rgRoe.GetValue2();
		COleSafeArray saRoc = rgRoc.GetValue2();
		COleSafeArray saRoe1 = rgRoe1.GetValue2();
		COleSafeArray saRoc1 = rgRoc1.GetValue2();
		COleSafeArray saShares = rgShares.GetValue2();

		double fSumRoe=0, fSumRoe1=0;
		int nRoe=0, nRoe1=0;

		double fSumRoc=0, fSumRoc1=0;
		int nRoc=0, nRoc1=0;

		double roe[NUM_YEARS_FOR_ROC], roc[NUM_YEARS_FOR_ROC], roe1[NUM_YEARS_FOR_ROC], roc1[NUM_YEARS_FOR_ROC];

		long index[2]; 
		index[0] = 1;
		for(int i=0; i<NUM_YEARS_FOR_ROC; i++)
		{
			index[1] = i+1;

			if(i<NUM_YEARS_FOR_EPS)
			{
				saEps.GetElement(index , &v);
				data.arEps[i] = (v.vt == VT_R8) ? v.dblVal : _INF_;
			}
				
			saRoe.GetElement(index , &v);
			roe[i] = (v.vt == VT_R8) ? v.dblVal*100 : _INF_;

			saRoc.GetElement(index , &v);
			roc[i] = (v.vt == VT_R8) ? v.dblVal*100 : _INF_;

			saRoe1.GetElement(index , &v);
			roe1[i] = (v.vt == VT_R8) ? v.dblVal*100 : _INF_;

			saRoc1.GetElement(index , &v);
			roc1[i] = (v.vt == VT_R8) ? v.dblVal*100 : _INF_;

			saShares.GetElement(index , &v);
			data.arShares[i] = (v.vt == VT_R8) ? v.dblVal : _INF_;

			if(i >= NUM_YEARS_FOR_ROC-7)
			{
				if(roe[i] < _INF_)	{	fSumRoe+=roe[i]; nRoe++;	}
				if(roe1[i] < _INF_) {	fSumRoe1+=roe1[i]; nRoe1++;	}
				if(roc[i] < _INF_)	{	fSumRoc+=roc[i]; nRoc++;	}
				if(roc1[i] < _INF_) {	fSumRoc1+=roc1[i]; nRoc1++;	}
			}
		}

		BOOL bRoc = true;
		if(nRoc > 0 && nRoc1 > 0)
		{
			double fAveRoc = fSumRoc/(double)nRoc;
			double fAveRoc1 = fSumRoc1/(double)nRoc1;
			if(fAveRoc1 < fAveRoc)
				bRoc = false;
		}
		if(bRoc)
		{
			memcpy(data.arRoc, roc, sizeof(double)*NUM_YEARS_FOR_ROC);
			memcpy(data.arRoe, roe, sizeof(double)*NUM_YEARS_FOR_ROC);
		}
		else
		{
			memcpy(data.arRoc, roc1, sizeof(double)*NUM_YEARS_FOR_ROC);
			memcpy(data.arRoe, roe1, sizeof(double)*NUM_YEARS_FOR_ROC);
		}

		UniverseData.Add(data);
	}


	return true;
}

BOOL CEngineMSExcel::LoadEpsRocRoeFactSet(_Worksheet* pSheet, const COleDateTime &date, CStringArray& arTickers, SUniverseDataEpsRocRoe& UniverseData)
{
	Range rgYear, rgTicker, rgEps, rgRoe, rgRoe1, rgRoc, rgRoc1, rgShares, rgDate;
	COleVariant v;

	rgYear.AttachDispatch(pSheet->GetRange(COleVariant("E1"), COleVariant("E1")));
	rgDate.AttachDispatch(pSheet->GetRange(COleVariant("G1"), COleVariant("G1")));
	rgTicker.AttachDispatch(pSheet->GetRange(COleVariant("A4"), COleVariant("A4")));
	rgEps.AttachDispatch(pSheet->GetRange(COleVariant("B6"), COleVariant("N6")));
	rgRoe.AttachDispatch(pSheet->GetRange(COleVariant("B14"), COleVariant("N14")));
	rgRoe1.AttachDispatch(pSheet->GetRange(COleVariant("B15"), COleVariant("N15")));
	rgRoc.AttachDispatch(pSheet->GetRange(COleVariant("B20"), COleVariant("N20")));
	rgRoc1.AttachDispatch(pSheet->GetRange(COleVariant("B21"), COleVariant("N21")));
	rgShares.AttachDispatch(pSheet->GetRange(COleVariant("B23"), COleVariant("N23")));

	for(int i=0; i<arTickers.GetSize(); i++)
	{
		SCompanyDataEpsRocRoe data;

		CString sTicker = arTickers[i];
		rgTicker.SetValue2(COleVariant((LPCTSTR)sTicker));

		CString sDate = date.Format("%m/%d/%Y");
		rgDate.SetValue2(COleVariant((LPCTSTR)sDate));

		m_pExcelApp->ExecuteExcel4Macro("FDSFORCERECALC(False)");

		v = rgYear.GetValue2();
		if(v.vt != VT_R8)
			continue;
		
		strcpy(data.sTicker, sTicker);
		data.nYear0 = (int)v.dblVal - 10;
		data.fAdjustAdd = _INF_;
		data.fAdjustMult = _INF_;

		COleSafeArray saEps = rgEps.GetValue2();
		COleSafeArray saRoe = rgRoe.GetValue2();
		COleSafeArray saRoc = rgRoc.GetValue2();
		COleSafeArray saRoe1 = rgRoe1.GetValue2();
		COleSafeArray saRoc1 = rgRoc1.GetValue2();
		COleSafeArray saShares = rgShares.GetValue2();

		double fSumRoe=0, fSumRoe1=0;
		int nRoe=0, nRoe1=0;

		double fSumRoc=0, fSumRoc1=0;
		int nRoc=0, nRoc1=0;

		double roe[NUM_YEARS_FOR_ROC], roc[NUM_YEARS_FOR_ROC], roe1[NUM_YEARS_FOR_ROC], roc1[NUM_YEARS_FOR_ROC];

		long index[2]; 
		index[0] = 1;
		for(int i=0; i<NUM_YEARS_FOR_ROC; i++)
		{
			index[1] = i+1;

			if(i<NUM_YEARS_FOR_EPS)
			{
				saEps.GetElement(index , &v);
				data.arEps[i] = (v.vt == VT_R8) ? v.dblVal : _INF_;
			}
				
			saRoe.GetElement(index , &v);
			roe[i] = (v.vt == VT_R8) ? v.dblVal*100 : _INF_;

			saRoc.GetElement(index , &v);
			roc[i] = (v.vt == VT_R8) ? v.dblVal*100 : _INF_;

			saRoe1.GetElement(index , &v);
			roe1[i] = (v.vt == VT_R8) ? v.dblVal*100 : _INF_;

			saRoc1.GetElement(index , &v);
			roc1[i] = (v.vt == VT_R8) ? v.dblVal*100 : _INF_;

			saShares.GetElement(index , &v);
			data.arShares[i] = (v.vt == VT_R8) ? v.dblVal : _INF_;

			if(i >= NUM_YEARS_FOR_ROC-7)
			{
				if(roe[i] < _INF_)	{	fSumRoe+=roe[i]; nRoe++;	}
				if(roe1[i] < _INF_) {	fSumRoe1+=roe1[i]; nRoe1++;	}
				if(roc[i] < _INF_)	{	fSumRoc+=roc[i]; nRoc++;	}
				if(roc1[i] < _INF_) {	fSumRoc1+=roc1[i]; nRoc1++;	}
			}
		}

		BOOL bRoc = true;
		if(nRoc > 0 && nRoc1 > 0)
		{
			double fAveRoc = fSumRoc/(double)nRoc;
			double fAveRoc1 = fSumRoc1/(double)nRoc1;
			if(fAveRoc1 < fAveRoc)
				bRoc = false;
		}
		if(bRoc)
		{
			memcpy(data.arRoc, roc, sizeof(double)*NUM_YEARS_FOR_ROC);
			memcpy(data.arRoe, roe, sizeof(double)*NUM_YEARS_FOR_ROC);
		}
		else
		{
			memcpy(data.arRoc, roc1, sizeof(double)*NUM_YEARS_FOR_ROC);
			memcpy(data.arRoe, roe1, sizeof(double)*NUM_YEARS_FOR_ROC);
		}

		UniverseData.Add(data);
	}


	return true;
}

BOOL CEngineMSExcel::LoadEps(_Worksheet* pSheet, SUniverseDataEpsRocRoe& UniverseData)
{
	char chLastCol = (char)(((int)'A')+NUM_YEARS_FOR_EPS+2); 
	CString sLastCell; 
	sLastCell.Format("%c%u", chLastCol, MAX_NUM_COMPANIES_IN_UNIVERSE+2);
	
	Range rAll; 
	COleVariant v;
	COleSafeArray sa;
	
	rAll.AttachDispatch(pSheet->GetRange( COleVariant("A1"),  COleVariant(sLastCell) ));
	sa = rAll.GetValue2(); 
	long index[2];	

	// get Year 0 - cell B1
	int nYear0;
	index[0] = 1; index[1] = 2;
	sa.GetElement(index , &v);
	if(v.vt == VT_ERROR)
		return false;

	nYear0 = (int)v.dblVal;

	int nRow;
	for(nRow = 3; nRow <= MAX_NUM_COMPANIES_IN_UNIVERSE+2; nRow++)
	{
		// Data line for each company
		SCompanyDataEpsRocRoe data;
		data.nYear0 = nYear0;

		char cCol = 'A';
		
		// get ticker
		index[0] = nRow; index[1] = 1;
		sa.GetElement(index , &v);
		if(v.vt != VT_BSTR)
			return true;  // may be less than 800 companies
		CString sTicker = v.bstrVal;
		strcpy(data.sTicker, sTicker);

		// get eps history
		for(int nCol=0; nCol < NUM_YEARS_FOR_EPS; nCol++)
		{
			index[1] = nCol+2;
			sa.GetElement(index , &v);
			if(v.vt == VT_R8)
				data.arEps[nCol] = v.dblVal;
			else
				data.arEps[nCol] = _INF_;
		}

		// get Adjustments
		index[1] = NUM_YEARS_FOR_EPS+2;
		sa.GetElement(index , &v);
		if(v.vt == VT_R8)
			data.fAdjustMult = v.dblVal;
		else
			data.fAdjustMult = 1.0;
		
		index[1] = NUM_YEARS_FOR_EPS+3;
		sa.GetElement(index , &v);
		if(v.vt == VT_R8)
			data.fAdjustAdd = v.dblVal;
		else
			data.fAdjustAdd = 0.0;

		// Add to uni verse data
		UniverseData.Add(data);
	}

	return true;
}

BOOL CEngineMSExcel::LoadRocRoeIntoExistingData(_Worksheet* pSheet, SUniverseDataEpsRocRoe& UniverseData)
{
	char chLastCol = (char)(((int)'A')+NUM_YEARS_FOR_ROC+2); 
	CString sLastCell; 
	sLastCell.Format("%c%u", chLastCol, 2*MAX_NUM_COMPANIES_IN_UNIVERSE+1);
	
	Range rAll; 
	COleVariant v;
	COleSafeArray sa;

	rAll.AttachDispatch(pSheet->GetRange( COleVariant("A1"),  COleVariant(sLastCell) ));
	sa = rAll.GetValue2(); 
	long index[2];

	int nStock;
	for(nStock = 1; nStock <= UniverseData.GetSize(); nStock++)
	{
		int nRowRoe = nStock*2;
		int nRowRoc = nStock*2 + 1;
		SCompanyDataEpsRocRoe *data = &(UniverseData[nStock-1]);

		for(int nCol=0; nCol < NUM_YEARS_FOR_ROC; nCol++)
		{
			index[1] = nCol+3;

			// ROE
			index[0] = nRowRoe;
			sa.GetElement(index , &v);
			if(v.vt == VT_R8)
				data->arRoe[nCol] = v.dblVal*100.0;
			else
				data->arRoe[nCol] = _INF_;
			
			// ROC
			index[0] = nRowRoc;
			sa.GetElement(index , &v);
			if(v.vt == VT_R8)
				data->arRoc[nCol] = v.dblVal*100.0;
			else
				data->arRoc[nCol] = _INF_;
		}
	}

	return true;
}

BOOL CEngineMSExcel::LoadFundamentals(_Worksheet* pSheet, SUniverseDataFundamentals& UniverseData)
{
	Range rAll; 
	COleVariant v;
	COleSafeArray sa;

	CString sLastCell; sLastCell.Format("AB%u", MAX_NUM_COMPANIES_IN_UNIVERSE+4);
	rAll.AttachDispatch(pSheet->GetRange( COleVariant("A1"),  COleVariant(sLastCell) ));
	sa = rAll.GetValue2(); 
	long index[2];

	int nRow;
	for(nRow = 5; nRow <= MAX_NUM_COMPANIES_IN_UNIVERSE+4; nRow++)
	{
		// Data line for each company
		SCompanyDataFundamentals data;
		data.bLoadBS = data.bLoadEPS = data.bLoadROCROE = -1;
		data.type = -1;
		data.bUseDebtAdjust = -1;

		index[0] = nRow;
		
		// get ticker
		index[1] = 1; 
		sa.GetElement(index , &v);
		if(v.vt != VT_BSTR)		
			return true;				// end of data (last row)
		CString sTicker = v.bstrVal;
		strcpy(data.sTicker, sTicker);

		// get name
		index[1] = 2; 
		sa.GetElement(index , &v);
		if(v.vt != VT_BSTR)
			continue;
		CString sName = v.bstrVal;
		//sName.Replace("\'", "\\\'");
		strcpy(data.sName, sName);

		// get report date
		index[1] = 3; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			continue;
		data.dtReport = v.date;

		// get shares out
		index[1] = 4; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fShares = 0.0;
		else
			data.fShares = v.dblVal / 1000000.0;

		// get fixed assets (Net Plant)
		index[1] = 5; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fFixed = 0.0;
		else
			data.fFixed = v.dblVal;

		// get Depr & Amort
		index[1] = 6; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fDeprAmort = 0.0;
		else
			data.fDeprAmort = v.dblVal;

		// get Total Assets
		index[1] = 7; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fAssets = 0.0;
		else
			data.fAssets = v.dblVal;

		// get Intangibles
		index[1] = 8; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fIntang = 0.0;
		else
			data.fIntang = v.dblVal;

		// get Liabilities
		index[1] = 9; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fLiabilities = 0.0;
		else
			data.fLiabilities = v.dblVal;

		// get Short Term Debt
		index[1] = 10; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fShortDebt = 0.0;
		else
			data.fShortDebt = v.dblVal;

		// get Long Term Debt
		index[1] = 11; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fLongDebt = 0.0;
		else
			data.fLongDebt = v.dblVal;

		// get Common Equity
		index[1] = 12; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fCommonEquity = 0.0;
		else
			data.fCommonEquity = v.dblVal;

		// get Cash
		index[1] = 13; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fCash = 0.0;
		else
			data.fCash = v.dblVal;

		// get Interest
		index[1] = 14; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fInterest = 0.0;
		else
			data.fInterest = v.dblVal;

		// get PreTax Income
		index[1] = 15; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fPreTaxIncome = 0.0;
		else
			data.fPreTaxIncome = v.dblVal;

		// get EPS Last Year (FY_Act)
		index[1] = 16; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fEpsLastYear = 0.0;
		else
			data.fEpsLastYear = v.dblVal;

		// get EPS Last 12 months (LTM)
		index[1] = 17; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fEpsLast12Months = 0.0;
		else
			data.fEpsLast12Months = v.dblVal;

		// get EPS This Year Forecast (FY1)
		index[1] = 18; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fEpsThisYForecast = 0.0;
		else
			data.fEpsThisYForecast = v.dblVal;

		// get EPS Next Year Forecast (FY1)
		index[1] = 19; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fEpsNextYForecast = 0.0;
		else
			data.fEpsNextYForecast = v.dblVal;

		// get Dividend
		index[1] = 20; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fDiv = 0.0;
		else
			data.fDiv = v.dblVal;

		// get Book Value Per Share
		index[1] = 21; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fBookVal = 0.0;
		else
			data.fBookVal = v.dblVal;

		// get Split Ratio
		index[1] = 22; 
		sa.GetElement(index , &v);
		if(v.vt != VT_BSTR)
			strcpy(data.sSplit, "-");
		else
		{
			CString sSplit = v.bstrVal;
			strcpy(data.sSplit, sSplit);
		}

		// get Split Date
		index[1] = 23; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.dtSplit.SetStatus(COleDateTime::null);
		else
			data.dtSplit = v.date;
		
		// get industry
		index[1] = 24; 
		sa.GetElement(index , &v);
		if(v.vt != VT_BSTR)
			continue;
		CString sIndustry = v.bstrVal;
		//sIndustry.Replace("\'", "\\\'");
		strcpy(data.sIndustry, sIndustry);

		// get sector
		index[1] = 25; 
		sa.GetElement(index , &v);
		if(v.vt != VT_BSTR)
			continue;
		CString sSector = v.bstrVal;
		//sSector.Replace("\'", "\\\'");
		strcpy(data.sSector, sSector);

		// get Last Reported Date
		index[1] = 26; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.nLastReport = 0;
		else
			data.nLastReport = (unsigned int)v.dblVal;

		index[1] = 27; 
		sa.GetElement(index , &v);
		if(v.vt == VT_BSTR)		
		{
			CString sCIQTicker = v.bstrVal;
			strcpy(data.sCIQTicker, sCIQTicker);
		}

		index[1] = 28; 
		sa.GetElement(index , &v);
		if(v.vt == VT_BSTR)		
		{
			CString sSP = v.bstrVal;
			if(sSP == "YES")
				data.bInSP = true;
			else
				data.bInSP = false;
		}
		else
			data.bInSP = false;

		// Add to universe data
		UniverseData.Add(data);
	}

	return true;
}


BOOL CEngineMSExcel::LoadFundamentals(const char* sFile, SUniverseDataFundamentals& UniverseData)
{
	if(!m_pExcelApp)
	{
		m_pExcelApp = new _Application;
		BOOL bResult = m_pExcelApp->CreateDispatch("Excel.Application");
		if(!bResult)
		{
			::MessageBox(NULL, "Unable to run MS Excel. Please make sure that MS Office is properly installed.", "Cornerstone Fair Value Model", MB_ICONSTOP);
			delete m_pExcelApp;
			m_pExcelApp = NULL;
			return false;
		}
	}

	// get Workbooks interface
	Workbooks wbooks(m_pExcelApp->GetWorkbooks());

	// open the Fundamentals file
	LPDISPATCH pD = wbooks.Open(sFile);
	if(pD == NULL)
	{
		CString sErr; sErr.Format("Unable to open Excel file: '%s'.", sFile);
		::MessageBox(NULL, sErr, "Cornerstone Fair Value Model", MB_ICONSTOP);
		m_pExcelApp->Quit();
		delete m_pExcelApp;
		m_pExcelApp = NULL;
		return false;
	}

	// Workbook and Worksheets
	_Workbook wbook(pD); 
	Worksheets sheets(wbook.GetWorksheets());

	// Load from Data from worksheet
	_Worksheet sheet1(sheets.GetItem(COleVariant((long)1)));
	BOOL bRet = LoadFundamentals(&sheet1, UniverseData);

	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	wbook.Close(COleVariant((long)FALSE), covOptional, covOptional);

	return bRet;
}

BOOL CEngineMSExcel::LoadFundamentals(_Worksheet* pSheet, CStringArray& arTickers, SUniverseDataFundamentals& UniverseData)
{
	Range rAll, rTicker; 
	COleVariant v;
	COleSafeArray sa;

	rTicker.AttachDispatch(pSheet->GetRange( COleVariant("A5"),  COleVariant("A5") ));
	rAll.AttachDispatch(pSheet->GetRange( COleVariant("A5"),  COleVariant("AB5") ));

	long index[2];
	index[0] = 1;

	for(int i=0; i<arTickers.GetSize(); i++)
	{
		// Data line for each company
		SCompanyDataFundamentals data;
		data.bLoadBS = data.bLoadEPS = data.bLoadROCROE = -1;
		data.type = -1;
		data.bUseDebtAdjust = -1;

		// set ticker
		CString sTicker = arTickers[i];
		rTicker.SetValue2(COleVariant((LPCTSTR)sTicker));
		strcpy(data.sTicker, sTicker);
		
		sa = rAll.GetValue2(); 

		// get name
		index[1] = 2; 
		sa.GetElement(index , &v);
		if(v.vt != VT_BSTR)
			continue;
		CString sName = v.bstrVal;
		//sName.Replace("\'", "\\\'");
		strcpy(data.sName, sName);

		// get report date
		index[1] = 3; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			continue;
		data.dtReport = v.date;

		// get shares out
		index[1] = 4; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fShares = 0.0;
		else
			data.fShares = v.dblVal / 1000000.0;

		// get fixed assets (Net Plant)
		index[1] = 5; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fFixed = 0.0;
		else
			data.fFixed = v.dblVal;

		// get Depr & Amort
		index[1] = 6; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fDeprAmort = 0.0;
		else
			data.fDeprAmort = v.dblVal;

		// get Total Assets
		index[1] = 7; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fAssets = 0.0;
		else
			data.fAssets = v.dblVal;

		// get Intangibles
		index[1] = 8; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fIntang = 0.0;
		else
			data.fIntang = v.dblVal;

		// get Liabilities
		index[1] = 9; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fLiabilities = 0.0;
		else
			data.fLiabilities = v.dblVal;

		// get Short Term Debt
		index[1] = 10; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fShortDebt = 0.0;
		else
			data.fShortDebt = v.dblVal;

		// get Long Term Debt
		index[1] = 11; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fLongDebt = 0.0;
		else
			data.fLongDebt = v.dblVal;

		// get Common Equity
		index[1] = 12; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fCommonEquity = 0.0;
		else
			data.fCommonEquity = v.dblVal;

		// get Cash
		index[1] = 13; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fCash = 0.0;
		else
			data.fCash = v.dblVal;

		// get Interest
		index[1] = 14; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fInterest = 0.0;
		else
			data.fInterest = v.dblVal;

		// get PreTax Income
		index[1] = 15; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fPreTaxIncome = 0.0;
		else
			data.fPreTaxIncome = v.dblVal;

		// get EPS Last Year (FY_Act)
		index[1] = 16; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fEpsLastYear = 0.0;
		else
			data.fEpsLastYear = v.dblVal;

		// get EPS Last 12 months (LTM)
		index[1] = 17; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fEpsLast12Months = 0.0;
		else
			data.fEpsLast12Months = v.dblVal;

		// get EPS This Year Forecast (FY1)
		index[1] = 18; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fEpsThisYForecast = 0.0;
		else
			data.fEpsThisYForecast = v.dblVal;

		// get EPS Next Year Forecast (FY1)
		index[1] = 19; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fEpsNextYForecast = 0.0;
		else
			data.fEpsNextYForecast = v.dblVal;

		// get Dividend
		index[1] = 20; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fDiv = 0.0;
		else
			data.fDiv = v.dblVal;

		// get Book Value Per Share
		index[1] = 21; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fBookVal = 0.0;
		else
			data.fBookVal = v.dblVal;

		// get Split Ratio
		index[1] = 22; 
		sa.GetElement(index , &v);
		if(v.vt != VT_BSTR)
			strcpy(data.sSplit, "-");
		else
		{
			CString sSplit = v.bstrVal;
			strcpy(data.sSplit, sSplit);
		}

		// get Split Date
		index[1] = 23; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.dtSplit.SetStatus(COleDateTime::null);
		else
			data.dtSplit = v.date;
		
		// get industry
		index[1] = 24; 
		sa.GetElement(index , &v);
		if(v.vt != VT_BSTR)
			continue;
		CString sIndustry = v.bstrVal;
		//sIndustry.Replace("\'", "\\\'");
		strcpy(data.sIndustry, sIndustry);

		// get sector
		index[1] = 25; 
		sa.GetElement(index , &v);
		if(v.vt != VT_BSTR)
			continue;
		CString sSector = v.bstrVal;
		//sSector.Replace("\'", "\\\'");
		strcpy(data.sSector, sSector);

		// get Last Reported Date
		index[1] = 26; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.nLastReport = 0;
		else
			data.nLastReport = (unsigned int)v.dblVal;

		index[1] = 27; 
		sa.GetElement(index , &v);
		if(v.vt == VT_BSTR)		
		{
			CString sCIQTicker = v.bstrVal;
			strcpy(data.sCIQTicker, sCIQTicker);
		}

		index[1] = 28; 
		sa.GetElement(index , &v);
		if(v.vt == VT_BSTR)		
		{
			CString sSP = v.bstrVal;
			if(sSP == "YES")
				data.bInSP = true;
			else
				data.bInSP = false;
		}
		else
			data.bInSP = false;

		// Add to universe data
		UniverseData.Add(data);
	}

	return true;
}

COleDateTime CEngineMSExcel::ConvertUSToOleDate(const char* szUSDate)
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


BOOL CEngineMSExcel::LoadFundamentalsFromFactSet(_Worksheet* pSheet, const COleDateTime &date, CStringArray& arTickers, SUniverseDataFundamentals& UniverseData)
{
	Range rAll, rTicker, rDate; 
	COleVariant v;
	COleSafeArray sa;

	rDate.AttachDispatch(pSheet->GetRange( COleVariant("B1"),  COleVariant("B1") ));
	rTicker.AttachDispatch(pSheet->GetRange( COleVariant("A5"),  COleVariant("A5") ));
	rAll.AttachDispatch(pSheet->GetRange( COleVariant("A5"),  COleVariant("AB5") ));

	long index[2];
	index[0] = 1;

	for(int i=0; i<arTickers.GetSize(); i++)
	{
		// Data line for each company
		SCompanyDataFundamentals data;
		data.bLoadBS = data.bLoadEPS = data.bLoadROCROE = -1;
		data.type = -1;
		data.bUseDebtAdjust = -1;

		// set ticker
		CString sTicker = arTickers[i];
		rTicker.SetValue2(COleVariant((LPCTSTR)sTicker));
		strcpy(data.sTicker, sTicker);

		CString sDate = date.Format("%m/%d/%Y");
		rDate.SetValue2(COleVariant((LPCTSTR)sDate));

		m_pExcelApp->ExecuteExcel4Macro("FDSFORCERECALC(False)");
		
		sa = rAll.GetValue2(); 

		// get name
		index[1] = 2; 
		sa.GetElement(index , &v);
		if(v.vt != VT_BSTR)
			continue;
		CString sName = v.bstrVal;
		//sName.Replace("\'", "\\\'");
		strcpy(data.sName, sName);

		// get report date
		index[1] = 3; 
		sa.GetElement(index , &v);
		if(v.vt != VT_BSTR)
			continue;
		data.dtReport = ConvertUSToOleDate(CString(v.bstrVal));

		// get shares out
		index[1] = 4; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fShares = 0.0;
		else
			data.fShares = v.dblVal;

		// get fixed assets (Net Plant)
		index[1] = 5; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fFixed = 0.0;
		else
			data.fFixed = v.dblVal;

		// get Depr & Amort
		index[1] = 6; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fDeprAmort = 0.0;
		else
			data.fDeprAmort = v.dblVal;

		// get Total Assets
		index[1] = 7; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fAssets = 0.0;
		else
			data.fAssets = v.dblVal;

		// get Intangibles
		index[1] = 8; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fIntang = 0.0;
		else
			data.fIntang = v.dblVal;

		// get Liabilities
		index[1] = 9; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fLiabilities = 0.0;
		else
			data.fLiabilities = v.dblVal;

		// get Short Term Debt
		index[1] = 10; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fShortDebt = 0.0;
		else
			data.fShortDebt = v.dblVal;

		// get Long Term Debt
		index[1] = 11; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fLongDebt = 0.0;
		else
			data.fLongDebt = v.dblVal;

		// get Common Equity
		index[1] = 12; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fCommonEquity = 0.0;
		else
			data.fCommonEquity = v.dblVal;

		// get Cash
		index[1] = 13; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fCash = 0.0;
		else
			data.fCash = v.dblVal;

		// get Interest
		index[1] = 14; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fInterest = 0.0;
		else
			data.fInterest = v.dblVal;

		// get PreTax Income
		index[1] = 15; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fPreTaxIncome = 0.0;
		else
			data.fPreTaxIncome = v.dblVal;

		// get EPS Last Year (FY_Act)
		index[1] = 16; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fEpsLastYear = 0.0;
		else
			data.fEpsLastYear = v.dblVal;

		// get EPS Last 12 months (LTM)
		index[1] = 17; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fEpsLast12Months = 0.0;
		else
			data.fEpsLast12Months = v.dblVal;

		// get EPS This Year Forecast (FY1)
		index[1] = 18; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fEpsThisYForecast = 0.0;
		else
			data.fEpsThisYForecast = v.dblVal;

		// get EPS Next Year Forecast (FY1)
		index[1] = 19; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fEpsNextYForecast = 0.0;
		else
			data.fEpsNextYForecast = v.dblVal;

		// get Dividend
		index[1] = 20; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fDiv = 0.0;
		else
			data.fDiv = v.dblVal;

		// get Book Value Per Share
		index[1] = 21; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fBookVal = 0.0;
		else
			data.fBookVal = v.dblVal;

		// get Split Ratio
		index[1] = 22; 
		sa.GetElement(index , &v);
		if(v.vt != VT_BSTR)
			strcpy(data.sSplit, "-");
		else
		{
			CString sSplit = v.bstrVal;
			strcpy(data.sSplit, sSplit);
		}

		// get Split Date
		index[1] = 23; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.dtSplit.SetStatus(COleDateTime::null);
		else
			data.dtSplit = v.date;
		
		// get industry
		index[1] = 24; 
		sa.GetElement(index , &v);
		if(v.vt != VT_BSTR)
			continue;
		CString sIndustry = v.bstrVal;
		//sIndustry.Replace("\'", "\\\'");
		strcpy(data.sIndustry, sIndustry);

		// get sector
		index[1] = 25; 
		sa.GetElement(index , &v);
		if(v.vt != VT_BSTR)
			continue;
		CString sSector = v.bstrVal;
		//sSector.Replace("\'", "\\\'");
		strcpy(data.sSector, sSector);

		// get Last Reported Date
		index[1] = 26; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.nLastReport = 0;
		else
			data.nLastReport = (unsigned int)v.dblVal;

		index[1] = 27; 
		sa.GetElement(index , &v);
		if(v.vt == VT_BSTR)		
		{
			CString sCIQTicker = v.bstrVal;
			strcpy(data.sCIQTicker, sCIQTicker);
		}

		index[1] = 28; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fPrice = _INF_;
		else
			data.fPrice = v.dblVal;

		// Add to universe data
		UniverseData.Add(data);
	}

	return true;
}

/*BOOL CEngineMSExcel::LoadFundamentals(_Worksheet* pSheet, SUniverseDataFundamentals& UniverseData)
{
	Range rAll; 
	COleVariant v;
	COleSafeArray sa;

	CString sLastCell; sLastCell.Format("AB%u", MAX_NUM_COMPANIES_IN_UNIVERSE+4);
	rAll.AttachDispatch(pSheet->GetRange( COleVariant("A1"),  COleVariant(sLastCell) ));
	sa = rAll.GetValue2(); 
	long index[2];

	int nRow;
	for(nRow = 5; nRow <= MAX_NUM_COMPANIES_IN_UNIVERSE+4; nRow++)
	{
		// Data line for each company
		SCompanyDataFundamentals data;
		data.bLoadBS = data.bLoadEPS = data.bLoadROCROE = -1;
		data.type = -1;
		data.bUseDebtAdjust = -1;

		index[0] = nRow;
		
		// get ticker
		index[1] = 1; 
		sa.GetElement(index , &v);
		if(v.vt != VT_BSTR)		
			return true;				// end of data (last row)
		CString sTicker = v.bstrVal;
		strcpy(data.sTicker, sTicker);

		// get name
		index[1] = 2; 
		sa.GetElement(index , &v);
		if(v.vt != VT_BSTR)
			continue;
		CString sName = v.bstrVal;
		//sName.Replace("\'", "\\\'");
		strcpy(data.sName, sName);

		// get report date
		index[1] = 3; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			continue;
		data.dtReport = v.date;

		// get shares out
		index[1] = 4; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fShares = 0.0;
		else
			data.fShares = v.dblVal / 1000000.0;

		// get fixed assets (Net Plant)
		index[1] = 5; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fFixed = 0.0;
		else
			data.fFixed = v.dblVal;

		// get Depr & Amort
		index[1] = 6; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fDeprAmort = 0.0;
		else
			data.fDeprAmort = v.dblVal;

		// get Total Assets
		index[1] = 7; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fAssets = 0.0;
		else
			data.fAssets = v.dblVal;

		// get Intangibles
		index[1] = 8; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fIntang = 0.0;
		else
			data.fIntang = v.dblVal;

		// get Liabilities
		index[1] = 9; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fLiabilities = 0.0;
		else
			data.fLiabilities = v.dblVal;

		// get Short Term Debt
		index[1] = 10; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fShortDebt = 0.0;
		else
			data.fShortDebt = v.dblVal;

		// get Long Term Debt
		index[1] = 11; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fLongDebt = 0.0;
		else
			data.fLongDebt = v.dblVal;

		// get Common Equity
		index[1] = 12; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fCommonEquity = 0.0;
		else
			data.fCommonEquity = v.dblVal;

		// get Cash
		index[1] = 13; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fCash = 0.0;
		else
			data.fCash = v.dblVal;

		// get Interest
		index[1] = 14; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fInterest = 0.0;
		else
			data.fInterest = v.dblVal;

		// get PreTax Income
		index[1] = 15; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fPreTaxIncome = 0.0;
		else
			data.fPreTaxIncome = v.dblVal;

		// get EPS Last Year (FY_Act)
		index[1] = 16; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fEpsLastYear = 0.0;
		else
			data.fEpsLastYear = v.dblVal;

		// get EPS Last 12 months (LTM)
		index[1] = 17; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fEpsLast12Months = 0.0;
		else
			data.fEpsLast12Months = v.dblVal;

		// get EPS This Year Forecast (FY1)
		index[1] = 18; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fEpsThisYForecast = 0.0;
		else
			data.fEpsThisYForecast = v.dblVal;

		// get EPS Next Year Forecast (FY1)
		index[1] = 19; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fEpsNextYForecast = 0.0;
		else
			data.fEpsNextYForecast = v.dblVal;

		// get Dividend
		index[1] = 20; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fDiv = 0.0;
		else
			data.fDiv = v.dblVal;

		// get Book Value Per Share
		index[1] = 21; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fBookVal = 0.0;
		else
			data.fBookVal = v.dblVal;

		// get Split Ratio
		index[1] = 22; 
		sa.GetElement(index , &v);
		if(v.vt != VT_BSTR)
			strcpy(data.sSplit, "-");
		else
		{
			CString sSplit = v.bstrVal;
			strcpy(data.sSplit, sSplit);
		}

		// get Split Date
		index[1] = 23; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.dtSplit.SetStatus(COleDateTime::null);
		else
			data.dtSplit = v.date;
		
		// get industry
		index[1] = 24; 
		sa.GetElement(index , &v);
		if(v.vt != VT_BSTR)
			continue;
		CString sIndustry = v.bstrVal;
		//sIndustry.Replace("\'", "\\\'");
		strcpy(data.sIndustry, sIndustry);

		// get sector
		index[1] = 25; 
		sa.GetElement(index , &v);
		if(v.vt != VT_BSTR)
			continue;
		CString sSector = v.bstrVal;
		//sSector.Replace("\'", "\\\'");
		strcpy(data.sSector, sSector);

		// get Last Reported Date
		index[1] = 26; 
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.nLastReport = 0;
		else
			data.nLastReport = (unsigned int)v.dblVal;

		index[1] = 27; 
		sa.GetElement(index , &v);
		if(v.vt == VT_BSTR)		
		{
			CString sCIQTicker = v.bstrVal;
			strcpy(data.sCIQTicker, sCIQTicker);
		}

		index[1] = 28; 
		sa.GetElement(index , &v);
		if(v.vt == VT_BSTR)		
		{
			CString sSP = v.bstrVal;
			if(sSP == "YES")
				data.bInSP = true;
			else
				data.bInSP = false;
		}
		else
			data.bInSP = false;

		// Add to universe data
		UniverseData.Add(data);
	}

	return true;
}*/

/*BOOL CEngineMSExcel::LoadFundamentals(_Worksheet* pSheet, SUniverseDataFundamentals& UniverseData)
{
	Range r; 
	COleVariant v;
	CString sCell;

	int nRow;
	for(nRow = 5; nRow <= MAX_NUM_COMPANIES_IN_UNIVERSE+4; nRow++)
	{
		// Data line for each company
		SCompanyDataFundamentals data;
		
		// get ticker
		sCell.Format("A%u", nRow);
		r.AttachDispatch(pSheet->GetRange( COleVariant(sCell),  COleVariant(sCell) ));
		v = r.GetValue2();
		if(v.vt != VT_BSTR)
			continue;
		CString sTicker = v.bstrVal;
		strcpy(data.sTicker, sTicker);

		// get name
		sCell.Format("B%u", nRow);
		r.AttachDispatch(pSheet->GetRange( COleVariant(sCell),  COleVariant(sCell) ));
		v = r.GetValue2();
		if(v.vt != VT_BSTR)
			continue;
		CString sName = v.bstrVal;
		sName.Replace("\'", "\\\'");
		strcpy(data.sName, sName);

		// get report date
		sCell.Format("C%u", nRow);
		r.AttachDispatch(pSheet->GetRange( COleVariant(sCell),  COleVariant(sCell) ));
		v = r.GetValue2();
		if(v.vt != VT_R8)
			continue;
		data.dtReport = v.date;

		// get shares out
		sCell.Format("D%u", nRow);
		r.AttachDispatch(pSheet->GetRange( COleVariant(sCell),  COleVariant(sCell) ));
		v = r.GetValue2();
		if(v.vt != VT_R8)
			data.fShares = 0.0;
		else
			data.fShares = v.dblVal / 1000000.0;

		// get fixed assets (Net Plant)
		sCell.Format("E%u", nRow);
		r.AttachDispatch(pSheet->GetRange( COleVariant(sCell),  COleVariant(sCell) ));
		v = r.GetValue2();
		if(v.vt != VT_R8)
			data.fFixed = 0.0;
		else
			data.fFixed = v.dblVal;

		// get Depr & Amort
		sCell.Format("F%u", nRow);
		r.AttachDispatch(pSheet->GetRange( COleVariant(sCell),  COleVariant(sCell) ));
		v = r.GetValue2();
		if(v.vt != VT_R8)
			data.fDeprAmort = 0.0;
		else
			data.fDeprAmort = v.dblVal;

		// get Total Assets
		sCell.Format("G%u", nRow);
		r.AttachDispatch(pSheet->GetRange( COleVariant(sCell),  COleVariant(sCell) ));
		v = r.GetValue2();
		if(v.vt != VT_R8)
			data.fAssets = 0.0;
		else
			data.fAssets = v.dblVal;

		// get Intangibles
		sCell.Format("H%u", nRow);
		r.AttachDispatch(pSheet->GetRange( COleVariant(sCell),  COleVariant(sCell) ));
		v = r.GetValue2();
		if(v.vt != VT_R8)
			data.fIntang = 0.0;
		else
			data.fIntang = v.dblVal;

		// get Liabilities
		sCell.Format("I%u", nRow);
		r.AttachDispatch(pSheet->GetRange( COleVariant(sCell),  COleVariant(sCell) ));
		v = r.GetValue2();
		if(v.vt != VT_R8)
			data.fLiabilities = 0.0;
		else
			data.fLiabilities = v.dblVal;

		// get Short Term Debt
		sCell.Format("J%u", nRow);
		r.AttachDispatch(pSheet->GetRange( COleVariant(sCell),  COleVariant(sCell) ));
		v = r.GetValue2();
		if(v.vt != VT_R8)
			data.fShortDebt = 0.0;
		else
			data.fShortDebt = v.dblVal;

		// get Long Term Debt
		sCell.Format("K%u", nRow);
		r.AttachDispatch(pSheet->GetRange( COleVariant(sCell),  COleVariant(sCell) ));
		v = r.GetValue2();
		if(v.vt != VT_R8)
			data.fLongDebt = 0.0;
		else
			data.fLongDebt = v.dblVal;

		// get Common Equity
		sCell.Format("L%u", nRow);
		r.AttachDispatch(pSheet->GetRange( COleVariant(sCell),  COleVariant(sCell) ));
		v = r.GetValue2();
		if(v.vt != VT_R8)
			data.fCommonEquity = 0.0;
		else
			data.fCommonEquity = v.dblVal;

		// get Cash
		sCell.Format("M%u", nRow);
		r.AttachDispatch(pSheet->GetRange( COleVariant(sCell),  COleVariant(sCell) ));
		v = r.GetValue2();
		if(v.vt != VT_R8)
			data.fCash = 0.0;
		else
			data.fCash = v.dblVal;

		// get Interest
		sCell.Format("N%u", nRow);
		r.AttachDispatch(pSheet->GetRange( COleVariant(sCell),  COleVariant(sCell) ));
		v = r.GetValue2();
		if(v.vt != VT_R8)
			data.fInterest = 0.0;
		else
			data.fInterest = v.dblVal;

		// get PreTax Income
		sCell.Format("O%u", nRow);
		r.AttachDispatch(pSheet->GetRange( COleVariant(sCell),  COleVariant(sCell) ));
		v = r.GetValue2();
		if(v.vt != VT_R8)
			data.fPreTaxIncome = 0.0;
		else
			data.fPreTaxIncome = v.dblVal;

		// get EPS Last Year (FY_Act)
		sCell.Format("P%u", nRow);
		r.AttachDispatch(pSheet->GetRange( COleVariant(sCell),  COleVariant(sCell) ));
		v = r.GetValue2();
		if(v.vt != VT_R8)
			data.fEpsLastYear = 0.0;
		else
			data.fEpsLastYear = v.dblVal;

		// get EPS Last 12 months (LTM)
		sCell.Format("Q%u", nRow);
		r.AttachDispatch(pSheet->GetRange( COleVariant(sCell),  COleVariant(sCell) ));
		v = r.GetValue2();
		if(v.vt != VT_R8)
			data.fEpsLast12Months = 0.0;
		else
			data.fEpsLast12Months = v.dblVal;

		// get EPS This Year Forecast (FY1)
		sCell.Format("R%u", nRow);
		r.AttachDispatch(pSheet->GetRange( COleVariant(sCell),  COleVariant(sCell) ));
		v = r.GetValue2();
		if(v.vt != VT_R8)
			data.fEpsThisYForecast = 0.0;
		else
			data.fEpsThisYForecast = v.dblVal;

		// get EPS Next Year Forecast (FY1)
		sCell.Format("S%u", nRow);
		r.AttachDispatch(pSheet->GetRange( COleVariant(sCell),  COleVariant(sCell) ));
		v = r.GetValue2();
		if(v.vt != VT_R8)
			data.fEpsNextYForecast = 0.0;
		else
			data.fEpsNextYForecast = v.dblVal;

		// get Dividend
		sCell.Format("T%u", nRow);
		r.AttachDispatch(pSheet->GetRange( COleVariant(sCell),  COleVariant(sCell) ));
		v = r.GetValue2();
		if(v.vt != VT_R8)
			data.fDiv = 0.0;
		else
			data.fDiv = v.dblVal;

		// get Book Value Per Share
		sCell.Format("U%u", nRow);
		r.AttachDispatch(pSheet->GetRange( COleVariant(sCell),  COleVariant(sCell) ));
		v = r.GetValue2();
		if(v.vt != VT_R8)
			data.fBookVal = 0.0;
		else
			data.fBookVal = v.dblVal;

		// get Split Ratio
		sCell.Format("V%u", nRow);
		r.AttachDispatch(pSheet->GetRange( COleVariant(sCell),  COleVariant(sCell) ));
		v = r.GetValue2();
		if(v.vt != VT_BSTR)
			strcpy(data.sSplit, "-");
		else
		{
			CString sSplit = v.bstrVal;
			strcpy(data.sSplit, sSplit);
		}

		// get Split Date
		sCell.Format("W%u", nRow);
		r.AttachDispatch(pSheet->GetRange( COleVariant(sCell),  COleVariant(sCell) ));
		v = r.GetValue2();
		if(v.vt != VT_R8)
			data.dtSplit.SetStatus(COleDateTime::null);
		else
			data.dtSplit = v.date;
		
		// get industry
		sCell.Format("X%u", nRow);
		r.AttachDispatch(pSheet->GetRange( COleVariant(sCell),  COleVariant(sCell) ));
		v = r.GetValue2();
		if(v.vt != VT_BSTR)
			continue;
		CString sIndustry = v.bstrVal;
		sIndustry.Replace("\'", "\\\'");
		strcpy(data.sIndustry, sIndustry);

		// get sector
		sCell.Format("Y%u", nRow);
		r.AttachDispatch(pSheet->GetRange( COleVariant(sCell),  COleVariant(sCell) ));
		v = r.GetValue2();
		if(v.vt != VT_BSTR)
			continue;
		CString sSector = v.bstrVal;
		sSector.Replace("\'", "\\\'");
		strcpy(data.sSector, sSector);

		// Add to universe data
		UniverseData.Add(data);
	}

	return true;
}*/

BOOL CEngineMSExcel::LoadShares(const char* sFile, SUniverseDataShares& UniverseData)
{
	if(!m_pExcelApp)
	{
		m_pExcelApp = new _Application;
		BOOL bResult = m_pExcelApp->CreateDispatch("Excel.Application");
		if(!bResult)
		{
			::MessageBox(NULL, "Unable to run MS Excel. Please make sure that MS Office is properly installed.", "Cornerstone Fair Value Model", MB_ICONSTOP);
			delete m_pExcelApp;
			m_pExcelApp = NULL;
			return false;
		}
	}

	// get Workbooks interface
	Workbooks wbooks(m_pExcelApp->GetWorkbooks());

	////////////////////////////////////////////////////////////////////////////////
	// open the Prices file
	LPDISPATCH pD = wbooks.Open(sFile);
	if(pD == NULL)
	{
		CString sErr; sErr.Format("Unable to open Excel file: '%s'.", sFile);
		::MessageBox(NULL, sErr, "Cornerstone Fair Value Model", MB_ICONSTOP);
		m_pExcelApp->Quit();
		delete m_pExcelApp;
		m_pExcelApp = NULL;
		return false;
	}

	// Workbook and Worksheets
	_Workbook wbook(pD); 
	Worksheets sheets(wbook.GetWorksheets());

	// Load from Data from worksheet
	_Worksheet sheet1(sheets.GetItem(COleVariant((long)1)));
	if(!LoadShares(&sheet1, UniverseData))
	{
		COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
		wbook.Close(COleVariant((long)FALSE), covOptional, covOptional);
		return false;
	}

	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	wbook.Close(COleVariant((long)FALSE), covOptional, covOptional);

	return true;
}

BOOL CEngineMSExcel::LoadShares(_Worksheet* pSheet, SUniverseDataShares& UniverseData)
{
	char chLastCol = (char)(((int)'A')+NUM_YEARS_FOR_EPS+1); 
	CString sLastCell; 
	sLastCell.Format("%c%u", chLastCol, MAX_NUM_COMPANIES_IN_UNIVERSE+1);
	
	Range rAll; 
	COleVariant v;
	COleSafeArray sa;

	rAll.AttachDispatch(pSheet->GetRange( COleVariant("A1"),  COleVariant(sLastCell) ));
	sa = rAll.GetValue2(); 
	long index[2];

	// get Year 0
	index[0] = 1;
	index[1] = 2;
	sa.GetElement(index , &v);
	if(v.vt != VT_R8)
		return false;
	int nYear0 = (int)v.dblVal;

	int nStock;
	for(nStock = 1; nStock <= MAX_NUM_COMPANIES_IN_UNIVERSE; nStock++)
	{	
		// ticker
		index[0] = nStock+1;
		index[1] = 1;
		sa.GetElement(index , &v);
		if(v.vt != VT_BSTR)
			return true;

		SCompanyDataShares data;

		CString sTicker = v.bstrVal;
		strcpy(data.sTicker, sTicker);
		data.nYear0 = nYear0;

		for(int nCol=0; nCol < NUM_YEARS_FOR_EPS; nCol++)
		{
			index[1] = nCol+2;

			// Get Shares Number
			sa.GetElement(index , &v);
			if(v.vt == VT_R8)
				data.arShares[nCol] = v.dblVal;
			else
				data.arShares[nCol] = _INF_;
		}

		UniverseData.Add(data);
	}

	return true;
}

BOOL CEngineMSExcel::LoadPricesAndReturns(const char* sFilePrices, const char* sFileReturns, SUniverseDataPricesAndReturns& UniverseData)
{
	if(!m_pExcelApp)
	{
		m_pExcelApp = new _Application;
		BOOL bResult = m_pExcelApp->CreateDispatch("Excel.Application");
		if(!bResult)
		{
			::MessageBox(NULL, "Unable to run MS Excel. Please make sure that MS Office is properly installed.", "Cornerstone Fair Value Model", MB_ICONSTOP);
			delete m_pExcelApp;
			m_pExcelApp = NULL;
			return false;
		}
	}

	// get Workbooks interface
	Workbooks wbooks(m_pExcelApp->GetWorkbooks());

	////////////////////////////////////////////////////////////////////////////////
	// open the Prices file
	LPDISPATCH pD = wbooks.Open(sFilePrices);
	if(pD == NULL)
	{
		CString sErr; sErr.Format("Unable to open Excel file: '%s'.", sFilePrices);
		::MessageBox(NULL, sErr, "Cornerstone Fair Value Model", MB_ICONSTOP);
		m_pExcelApp->Quit();
		delete m_pExcelApp;
		m_pExcelApp = NULL;
		return false;
	}

	// Workbook and Worksheets
	_Workbook wbook(pD); 
	Worksheets sheets(wbook.GetWorksheets());

	// Load from Data from worksheet
	_Worksheet sheet1(sheets.GetItem(COleVariant((long)1)));
	if(!LoadPrices(&sheet1, UniverseData))
	{
		COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
		wbook.Close(COleVariant((long)FALSE), covOptional, covOptional);
		return false;
	}

	////////////////////////////////////////////////////////////////////////////////
	// open the Returns file
	pD = wbooks.Open(sFileReturns);
	if(pD == NULL)
	{
		CString sErr; sErr.Format("Unable to open Excel file: '%s'.", sFileReturns);
		::MessageBox(NULL, sErr, "Cornerstone Fair Value Model", MB_ICONSTOP);
		m_pExcelApp->Quit();
		delete m_pExcelApp;
		m_pExcelApp = NULL;
		return false;
	}

	// Workbook and Worksheets
	_Workbook wbook2(pD); 
	Worksheets sheets2(wbook2.GetWorksheets());

	// Load from Data from worksheet
	_Worksheet sheet2(sheets2.GetItem(COleVariant((long)1)));
	BOOL bRet = LoadReturnsIntoExistingData(&sheet2, UniverseData);

	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	wbook.Close(COleVariant((long)FALSE), covOptional, covOptional);

	return bRet;
}

BOOL CEngineMSExcel::LoadPrices(_Worksheet* pSheet, SUniverseDataPricesAndReturns& UniverseData)
{
	Range rAll; 
	COleVariant v;
	COleSafeArray sa;

	CString sLastCell; 
	sLastCell.Format("C%u", MAX_NUM_COMPANIES_IN_UNIVERSE+4);
	rAll.AttachDispatch(pSheet->GetRange( COleVariant("A1"),  COleVariant(sLastCell) ));
	sa = rAll.GetValue2(); 
	long index[2];

	// get price reporting date
	index[0] = 4; index[1] = 3;
	sa.GetElement(index , &v);
	if(v.vt != VT_R8)
		return false;

	COleDateTime date = v.date;

	int nRow;
	for(nRow = 5; nRow <= MAX_NUM_COMPANIES_IN_UNIVERSE+4; nRow++)
	{
		// Data line for each company
		SCompanyDataPricesAndReturns data;
		data.fReturn = 0;

		index[0] = nRow;
		
		// get ticker
		index[1] = 1;
		sa.GetElement(index , &v);
		if(v.vt != VT_BSTR)
			break;
		CString sTicker = v.bstrVal;
		strcpy(data.sTicker, sTicker);

		// get price
		index[1] = 3;
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data.fPrice = 0.0;
		else
			data.fPrice = v.dblVal;

		// reporting date
		data.date = date;

		// Add into array
		UniverseData.Add(data);
	}
	return true;
}

BOOL CEngineMSExcel::LoadReturnsIntoExistingData(_Worksheet* pSheet, SUniverseDataPricesAndReturns& UniverseData)
{
	Range rAll; 
	COleVariant v;
	COleSafeArray sa;

	CString sLastCell; 
	sLastCell.Format("C%u", MAX_NUM_COMPANIES_IN_UNIVERSE+4);
	rAll.AttachDispatch(pSheet->GetRange( COleVariant("A1"),  COleVariant(sLastCell) ));
	sa = rAll.GetValue2(); 
	long index[2];

	// get price reporting date
	index[0] = 4; index[1] = 3;
	sa.GetElement(index , &v);
	if(v.vt != VT_R8)
		return false;

	COleDateTime date = v.date;
	if(UniverseData.GetSize() > 0 && UniverseData[0].date != date)
	{
		::MessageBox(NULL, "The report dates for Prices and Returns are different. Make sure to select proper input files for Prices and Returns (d_prc.xls, d_ret.xls)", "Cornerstone Fair Value Model", MB_ICONSTOP);
		return false;
	}

	int nRow;
	for(nRow = 5; nRow <= UniverseData.GetSize()+4; nRow++)
	{
		// Data line for each company
		SCompanyDataPricesAndReturns* data = &UniverseData[nRow-5];

		index[0] = nRow;

		// get return
		index[1] = 3;
		sa.GetElement(index , &v);
		if(v.vt != VT_R8)
			data->fReturn = 0.0;
		else
			data->fReturn = v.dblVal;
	}
	return true;
}

BOOL CEngineMSExcel::LoadWeightsInIndices(const char* sFile, SUniverseWeightsInIndices& UniverseData)
{
	if(!m_pExcelApp)
	{
		m_pExcelApp = new _Application;
		BOOL bResult = m_pExcelApp->CreateDispatch("Excel.Application");
		if(!bResult)
		{
			::MessageBox(NULL, "Unable to run MS Excel. Please make sure that MS Office is properly installed.", "Cornerstone Fair Value Model", MB_ICONSTOP);
			delete m_pExcelApp;
			m_pExcelApp = NULL;
			return false;
		}
	}

	// get Workbooks interface
	Workbooks wbooks(m_pExcelApp->GetWorkbooks());

	LPDISPATCH pD = NULL;
	try // open the file
	{
		pD = wbooks.Open(sFile);
	}
	catch(...)
	{
		CString sErr; sErr.Format("Unable to open Excel file: '%s'.", sFile);
		::MessageBox(NULL, sErr, "Cornerstone Fair Value Model", MB_ICONSTOP);
		m_pExcelApp->Quit();
		delete m_pExcelApp;
		m_pExcelApp = NULL;
		return false;
	}

	if(pD == NULL)
	{
		CString sErr; sErr.Format("Unable to open Excel file: '%s'.", sFile);
		::MessageBox(NULL, sErr, "Cornerstone Fair Value Model", MB_ICONSTOP);
		m_pExcelApp->Quit();
		delete m_pExcelApp;
		m_pExcelApp = NULL;
		return false;
	}

	// Workbook and Worksheets
	_Workbook wbook(pD); 
	Worksheets sheets(wbook.GetWorksheets());

	// Load from Data from worksheet
	_Worksheet sheet1(sheets.GetItem(COleVariant((long)1)));
	BOOL bRet = LoadWeightsInIndices(&sheet1, UniverseData);

	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	wbook.Close(COleVariant((long)FALSE), covOptional, covOptional);

	return true;
}

BOOL CEngineMSExcel::LoadWeightsInIndices(_Worksheet* pSheet, SUniverseWeightsInIndices& UniverseData)
{
	Range rAll; 
	COleVariant v;
	COleSafeArray sa;

	rAll.AttachDispatch(pSheet->GetRange( COleVariant("A1"),  COleVariant("K2000") ));
	sa = rAll.GetValue2(); 
	long index[2];

	int nRow = 2;
	while(nRow <= 2000)  // max number of rows to be uploaded
	{
		SCompanyWeightsInIndices item;
		index[0] = nRow;

		// Ticker
		index[1] = 2;
		sa.GetElement(index , &v);
		if(v.vt != VT_BSTR)
			break;
		CString sTicker = v.bstrVal;

		// Truncate suffix (for example make GOOG-O  -->  GOOG)
		int n = sTicker.Find('-');
		if(n != -1)
			sTicker = sTicker.Left(n);
		strcpy(item.sTicker, sTicker);

		for(int nCol=0; nCol<NUM_WEIGHTS; nCol++)
		{
			index[1] = nCol+3; 
			sa.GetElement(index , &v);
			if(v.vt != VT_R8)
				item.data[nCol] = 0;
			else
				item.data[nCol] = v.dblVal / 100.0;
		}

		UniverseData.Add(item);
  		++nRow;
	}

	return true;	
}

BOOL CEngineMSExcel::LoadHistory(const char* sFile, SHistory& history)
{
	if(!m_pExcelApp)
	{
		m_pExcelApp = new _Application;
		BOOL bResult = m_pExcelApp->CreateDispatch("Excel.Application");
		if(!bResult)
		{
			::MessageBox(NULL, "Unable to run MS Excel. Please make sure that MS Office is properly installed.", "Cornerstone Fair Value Model", MB_ICONSTOP);
			delete m_pExcelApp;
			m_pExcelApp = NULL;
			return false;
		}
	}

	// get Workbooks interface
	Workbooks wbooks(m_pExcelApp->GetWorkbooks());

	// open the History file
	LPDISPATCH pD = NULL;
	try // open the file
	{
		pD = wbooks.Open(sFile);
	}
	catch(...)
	{
		CString sErr; sErr.Format("Unable to open Excel file: '%s'.", sFile);
		::MessageBox(NULL, sErr, "Cornerstone Fair Value Model", MB_ICONSTOP);
		m_pExcelApp->Quit();
		delete m_pExcelApp;
		m_pExcelApp = NULL;
		return false;
	}

	if(pD == NULL)
	{
		CString sErr; sErr.Format("Unable to open Excel file: '%s'.", sFile);
		::MessageBox(NULL, sErr, "Cornerstone Fair Value Model", MB_ICONSTOP);
		m_pExcelApp->Quit();
		delete m_pExcelApp;
		m_pExcelApp = NULL;
		return false;
	}

	// Workbook and Worksheets
	_Workbook wbook(pD);  
	Worksheets sheets(wbook.GetWorksheets());

	// Load from Data from worksheet
	_Worksheet sheet1(sheets.GetItem(COleVariant((long)1)));
	BOOL bRet = LoadHistory(&sheet1, history);

	COleVariant covOptional((long)DISP_E_PARAMNOTFOUND, VT_ERROR);
	wbook.Close(COleVariant((long)FALSE), covOptional, covOptional);

	return bRet;
}

BOOL CEngineMSExcel::LoadHistory(_Worksheet* pSheet, SHistory& history)
{
	Range rAll; 
	COleVariant v;
	COleSafeArray sa;

	rAll.AttachDispatch(pSheet->GetRange( COleVariant("A1"),  COleVariant("W1000") ));
	sa = rAll.GetValue2(); 
	long index[2];

	// get ticker name
	index[0] = 1; index[1] = 1;
	sa.GetElement(index , &v);
	if(v.vt != VT_BSTR)
		return false;

	CString sTicker = v.bstrVal;
	sTicker.TrimLeft(); sTicker.TrimRight();
	strcpy(history.sTicker, sTicker);

	int nRow = 5;
	while(true)
	{
		SHistoryItem item;
		index[0] = nRow;

		// date
		index[1] = 1;
		sa.GetElement(index , &v);
		if(v.vt != VT_BSTR)
			break;
		CString sDate = v.bstrVal;
		
		COleDateTime date = GetFromStr(sDate);
		if(date.GetStatus() != COleDateTime::valid)
			break;
		item.date = date;

		static int indexes[] = {2,3,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23};

		for(int nCol=0; nCol<NUM_COLUMNS_IN_HISTORY_EXCEL; nCol++)
		{
			index[1] = indexes[nCol]; 
			sa.GetElement(index , &v);
			if(v.vt != VT_R8)
				item.data[nCol] = _INF_;
			else
				item.data[nCol] = v.dblVal;
		}

		if(item.data[1] != _INF_)
			history.arItems.Add(item);
  		++nRow;
	}

	return true;
}

COleDateTime CEngineMSExcel::GetFromStr(const CString &sDate)
{
	COleDateTime dt;
	dt.SetStatus(COleDateTime::invalid);

	if(sDate.GetLength() != 10 || sDate[2] != '/' || sDate[5] != '/')
		return dt;

	CString sMonth = sDate.Left(2);
	CString sDay = sDate.Mid(3, 2);
	CString sYear = sDate.Mid(6);

	int nMonth = atoi(sMonth);
	int nDay = atoi(sDay);
	int nYear = atoi(sYear);

	dt = COleDateTime(nYear, nMonth, nDay, 0,0,0);
	return dt;
}

COleDateTime CEngineMSExcel::GetLastFriday(const COleDateTime &date)
{
	const int nFriday = 6;
	int nWeekday = date.GetDayOfWeek();
	if(nWeekday == nFriday)
		return date;

	COleDateTimeSpan sp;
	if(nWeekday > nFriday)
		sp = COleDateTimeSpan(nWeekday - nFriday, 0,0,0);
	else
		sp = COleDateTimeSpan(nWeekday + 1, 0,0,0);

	COleDateTime dt = date - sp;
	return dt;
}

