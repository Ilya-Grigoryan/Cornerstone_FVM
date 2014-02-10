// Bloomberg.cpp : implementation file
//

#include "stdafx.h"
#include "cip_fileproxy.h"
#include "Bloomberg.h"

#include "input.h"
#include "excel.h"
#include "sqlcompany.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CCip_fileproxyApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CBloomberg

IMPLEMENT_DYNCREATE(CBloomberg, CCmdTarget)

CBloomberg::CBloomberg()
{
	EnableAutomation();
	
	// To keep the application running as long as an OLE automation 
	//	object is active, the constructor calls AfxOleLockApp.
	
	AfxOleLockApp();
}

CBloomberg::~CBloomberg()
{
	// To terminate the application when all objects created with
	// 	with OLE automation, the destructor calls AfxOleUnlockApp.
	
	AfxOleUnlockApp();
}


void CBloomberg::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CBloomberg, CCmdTarget)
	//{{AFX_MSG_MAP(CBloomberg)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CBloomberg, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CBloomberg)
	DISP_FUNCTION(CBloomberg, "LoadReturns", LoadReturns, VT_I4, VTS_DISPATCH)
	DISP_FUNCTION(CBloomberg, "SetTicker", SetTicker, VT_I4, VTS_DISPATCH)
	DISP_FUNCTION(CBloomberg, "PrepareTickers", PrepareTickers, VT_I4, VTS_NONE)
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IBloomberg to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {D2D82D43-851F-4FCA-833B-862DD4641FB7}
static const IID IID_IBloomberg =
{ 0xd2d82d43, 0x851f, 0x4fca, { 0x83, 0x3b, 0x86, 0x2d, 0xd4, 0x64, 0x1f, 0xb7 } };

BEGIN_INTERFACE_MAP(CBloomberg, CCmdTarget)
	INTERFACE_PART(CBloomberg, IID_IBloomberg, Dispatch)
END_INTERFACE_MAP()

// {4FE15ED2-711D-4B42-AD79-6FF56C2B8878}
IMPLEMENT_OLECREATE(CBloomberg, "CIPFileProxy.Bloomberg", 0x4fe15ed2, 0x711d, 0x4b42, 0xad, 0x79, 0x6f, 0xf5, 0x6c, 0x2b, 0x88, 0x78)

/////////////////////////////////////////////////////////////////////////////
// CBloomberg message handlers

long CBloomberg::LoadReturns(LPDISPATCH lpSheet) 
{
	_Worksheet sheet(lpSheet);

	long lRet;
	lRet = LoadEPS(&sheet);
	if(lRet != 1)
		return lRet;

	lRet = LoadBalanceSheet(&sheet);
	return lRet;
}

long CBloomberg::SetTicker(LPDISPATCH lpSheet) 
{
	_Worksheet sheet(lpSheet);

	if(theApp.m_nCurrent >= theApp.m_arTickers.GetSize())
		return 0;

	CString sTicker = theApp.m_arTickers[theApp.m_nCurrent];
	Range rgTicker;

	rgTicker.AttachDispatch(sheet.GetRange(COleVariant("A3"), COleVariant("A3")));	

	rgTicker.SetValue2(COleVariant((LPCTSTR)sTicker));
	theApp.m_nAttempt = 0;

	theApp.m_nCurrent ++;
	if(theApp.m_nCurrent >= theApp.m_arTickers.GetSize())
		return 0;

	return 1;
}

long CBloomberg::PrepareTickers() 
{
	CString sDb = CSQLCompany::GetDatabase();
	if(sDb != "cip_universe_blp")
	{
		AfxMessageBox("Bloomberg must be selected as current Data-Base. Unable to continue. Please change the Data-Base and try again.");
		return 0;
	}

	theApp.m_nCurrent = 0;
	theApp.m_arIds.RemoveAll();
	theApp.m_arTickers.RemoveAll();

	// Get the tickers for this date
	COleDateTime now = COleDateTime::GetCurrentTime();
	CArray<int,int> arIds;
	CStringArray	arTickers;
	if(!CSQLCompany::GetCompanyIDAndTickers(theApp.m_arIds, theApp.m_arTickers, now))
	{
		AfxMessageBox("Unable to get Ticker information from the Data-Base.");
		return 0;
	}

	return 0;
}

long CBloomberg::LoadEPS(_Worksheet *sheet)
{
	Range rgCheck, rgYear, rgTicker, rgEps, rgRoe, rgRoe1, rgRoc, rgRoc1, rgShares, rgAdjustAdd;
	COleVariant v;

	rgCheck.AttachDispatch(sheet->GetRange(COleVariant("M5"), COleVariant("M5")));
	rgYear.AttachDispatch(sheet->GetRange(COleVariant("E1"), COleVariant("E1")));
	rgTicker.AttachDispatch(sheet->GetRange(COleVariant("A3"), COleVariant("A3")));
	rgEps.AttachDispatch(sheet->GetRange(COleVariant("B6"), COleVariant("N6")));
	rgRoe.AttachDispatch(sheet->GetRange(COleVariant("B14"), COleVariant("N14")));
	rgRoe1.AttachDispatch(sheet->GetRange(COleVariant("B15"), COleVariant("N15")));
	rgRoc.AttachDispatch(sheet->GetRange(COleVariant("B20"), COleVariant("N20")));
	rgRoc1.AttachDispatch(sheet->GetRange(COleVariant("B21"), COleVariant("N21")));
	rgShares.AttachDispatch(sheet->GetRange(COleVariant("B23"), COleVariant("N23")));
	rgAdjustAdd.AttachDispatch(sheet->GetRange(COleVariant("K59"), COleVariant("K59")));
	SCompanyDataEpsRocRoe data;

	v = rgCheck.GetValue2();
	if(v.vt != VT_R8)
	{
		if(theApp.m_nAttempt < 4)
		{
			theApp.m_nAttempt ++;
			return 0;
		}
		else
			return -1;
	}

	v = rgTicker.GetValue2();
	if(v.vt != VT_BSTR)		
		return -1;				
	CString sTicker = v.bstrVal;
	strcpy(data.sTicker, sTicker);

	v = rgYear.GetValue2();
	if(v.vt != VT_R8)
		return -1;
	
	//strcpy(data.sTicker, sTicker);
	data.nYear0 = (int)v.dblVal - 10;

	v = rgAdjustAdd.GetValue2();
	if(v.vt == VT_R8)
	{
		data.fAdjustAdd = v.dblVal;
		data.fAdjustMult = 1.0;
	}

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

	if(!CSQLCompany::UpdateCompanyEpsRocRoe(data, true, false))
	{
		//AfxMessageBox("Unable to update the EPS, ROC and ROE in the Data-Base.");
		return 1;
	}

	return 1;
}

long CBloomberg::LoadBalanceSheet(_Worksheet *sheet)
{
	Range rAll, rTicker, rPrice; 
	COleVariant v;
	COleSafeArray sa;

	rTicker.AttachDispatch(sheet->GetRange( COleVariant("A3"),  COleVariant("A3") ));
	rAll.AttachDispatch(sheet->GetRange( COleVariant("A40"),  COleVariant("AB40") ));
	rPrice.AttachDispatch(sheet->GetRange( COleVariant("P3"),  COleVariant("P3") ));

	long index[2];
	index[0] = 1;

	// Data line for each company
	SCompanyDataFundamentals data;
	data.bLoadBS = data.bLoadEPS = data.bLoadROCROE = -1;
	data.type = -1;
	data.bUseDebtAdjust = -1;

	// get ticker
	v = rTicker.GetValue2();
	if(v.vt != VT_BSTR)		
		return 1;				
	CString sTicker = v.bstrVal;
	strcpy(data.sTicker, sTicker);
	
	sa = rAll.GetValue2(); 

	// get name
	index[1] = 2; 
	sa.GetElement(index , &v);
	if(v.vt != VT_BSTR)
		return 1;
	CString sName = v.bstrVal;
	strcpy(data.sName, sName);

	// get report date
	index[1] = 3; 
	sa.GetElement(index , &v);
	if(v.vt != VT_BSTR)
		return 1;
	CString sDate = v.bstrVal;
	data.dtReport = GetDateFromString(sDate);

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
		if(sSplit[0] == '#')
			sSplit = "-";
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
		return -1;
	CString sIndustry = v.bstrVal;
	//sIndustry.Replace("\'", "\\\'");
	strcpy(data.sIndustry, sIndustry);

	// get sector
	index[1] = 25; 
	sa.GetElement(index , &v);
	if(v.vt != VT_BSTR)
		return -1;
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

	// Update Database
	if(!CSQLCompany::UpdateCompanyFundamentals(data, true))
	{
		//AfxMessageBox("Unable to update the Fundamentals and Balance-Sheets in the Data-Base.");
		//return 1;
	}

	// Get price
	COleDateTime today(COleDateTime::GetCurrentTime());
	COleDateTime date = GetLastFriday(today);		

	v = rPrice.GetValue2();
	if(v.vt == VT_R8)
	{
		SCompanyDataPricesAndReturns data;
		
		strcpy(data.sTicker, sTicker);
		data.date = date;
		data.fReturn = 0;

		data.fPrice = v.dblVal;
		CSQLCompany::UpdateCompanyPricesAndReturns(data, true);
	}

	return 1;
}

COleDateTime CBloomberg::GetLastFriday(const COleDateTime &date)
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

COleDateTime CBloomberg::GetDateFromString(const CString& str)
{
	COleDateTime dt;
	dt.SetStatus(COleDateTime::invalid);

	int n1 = str.Find('/');
	if(n1 == -1)
		return dt;
	CString sm = str.Left(n1);

	int n2 = str.Find('/', n1+1);
	if(n2 == -1)
		return dt;
	CString sd = str.Mid(n1+1, n2-n1-1);

	CString sy = str.Mid(n2+1);

	int nM = atoi(sm);
	int nD = atoi(sd);
	int nY = atoi(sy);

	dt = COleDateTime(nY, nM, nD, 0,0,0);
	return dt;

	return dt;
}