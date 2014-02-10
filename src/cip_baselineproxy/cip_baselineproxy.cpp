// cip_baselineproxy.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "cip_baselineproxy.h"
#include "DDialogMain.h"

#include "BaseMath.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CApp

BEGIN_MESSAGE_MAP(CApp, CWinApp)
	//{{AFX_MSG_MAP(CApp)
	//}}AFX_MSG
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CApp construction

CApp::CApp()
{
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CApp object

CApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CApp initialization

#include "universe.h"
#include "sqlcompany.h"
#include "enginemsexcel.h"

CString CApp::CreateMainDirectory()
{
	CString sDate = COleDateTime::GetCurrentTime().Format("%Y_%m_%d");
	TCHAR szAppDataPath[_MAX_PATH];
	SHGetSpecialFolderPath(NULL, szAppDataPath, CSIDL_APPDATA, false);

	BOOL bRet;
	CString sDir; 
	
	sDir.Format("%s\\Cornerstone IP", szAppDataPath);
	bRet = ::CreateDirectory(sDir, NULL);
	if(!bRet && ::GetLastError() != ERROR_ALREADY_EXISTS)
		return "";

	sDir += "\\Fair Value Model";
	bRet = ::CreateDirectory(sDir, NULL);
	if(!bRet && ::GetLastError() != ERROR_ALREADY_EXISTS)
		return "";

	sDir += "\\Baseline";
	bRet = ::CreateDirectory(sDir, NULL);
	if(!bRet && ::GetLastError() != ERROR_ALREADY_EXISTS)
		return "";

	sDir += "\\" + sDate + "\\";
	bRet = ::CreateDirectory(sDir, NULL);
	if(!bRet && ::GetLastError() != ERROR_ALREADY_EXISTS)
		return "";

	return sDir;
}

BOOL CApp::CreateTemplateFromResource(unsigned int nRes, const char* szFile)
{
	const char* szRName = MAKEINTRESOURCE(nRes);
	HRSRC hrSrc = ::FindResource(m_hInstance, szRName, "XLS");
	if(!hrSrc)
		return false;

	DWORD nSize = ::SizeofResource(m_hInstance, hrSrc);
	if(nSize <= 0)
		return false;

	HGLOBAL hGlobal = ::LoadResource(m_hInstance, hrSrc);
	if(!hGlobal)
		return false;

	void* lpData = ::LockResource(hGlobal);
	if(!lpData)
		return false;

	CFile file;
	if(!file.Open(szFile, CFile::modeCreate|CFile::modeWrite)) // Any new development can be actually done here 
		return false;
	
	file.Write(lpData, nSize);
	file.Close();
	
	::UnlockResource(hGlobal);
	::FreeResource(hGlobal);

	return true;
}

COleDateTime CApp::CalcInterestRate(CEngineMSExcel &excel, const COleDateTime &date, double &fWeeklyAve, double &f3MAve, double &fFridayClose)
{
	COleDateTime friday = excel.GetLastFriday(date);

	BOOL bExist = CSQLCompany::DoesInterestRateRecordExist(friday);
	COleDateTime dtReq = friday;
	if(bExist)
		dtReq -= COleDateTimeSpan(7,0,0,0);

	double values[5];
	excel.GetLastWeekT10ValuesFromBaseLine(friday, values);

	fWeeklyAve = CMath::GetMean(values, 5);
	
	SInterestRateHistory hist;
	int nHist = CSQLCompany::GetInterestRateHistory(dtReq, 12, hist);

	double f3m[13];
	for(int i=0; i<13; i++)
		f3m[i] = _INF_;
	for(int i=0; i<nHist; i++)
		f3m[i] = hist[i].fWeeklyAve;
	f3m[nHist] = fWeeklyAve;

	f3MAve = CMath::GetMean(f3m, nHist+1);
	fFridayClose = values[4];

	return friday;
}

BOOL CApp::InitInstance()
{
	////////////////////////////
	CString sDir = CreateMainDirectory();
	::SetCurrentDirectory(sDir);

	CString sFileFundamental = sDir + "Fundamental.xls";
	CString sFilePrices = sDir + "Prices.xls";
	CString sFileEpsRocRoe = sDir + "EpsRocRoe.xls";

	 
	BOOL bReturn = CreateTemplateFromResource(IDR_XLS_FUNDAMENTAL, sFileFundamental);
	if(!bReturn)
	{
		AfxMessageBox("Enable to populate Fundamentals template files. Please check the current user privileges on this computer.");
		return false;
	}
	bReturn = CreateTemplateFromResource(IDR_XLS_PRICES, sFilePrices);
	if(!bReturn)
	{
		AfxMessageBox("Enable to populate Prices template files. Please check the current user privileges on this computer.");
		return false;
	}
	bReturn = CreateTemplateFromResource(IDR_XLS_EPSROCROE, sFileEpsRocRoe);
	if(!bReturn)
	{
		AfxMessageBox("Unable to populate EPS,ROC and ROE template files. Please check the current user privileges on this computer.");
		return false;
	}

	// Get the tickers for this date
	COleDateTime now = COleDateTime::GetCurrentTime();
	CArray<int,int> arIds;
	CStringArray	arTickers;
	if(!CSQLCompany::GetCompanyIDAndTickers(arIds, arTickers, now))
	{
		AfxMessageBox("Unable to get Ticker information from the Data-Base.");
		return false;
	}

	SUniverseDataFundamentals data;
	SUniverseDataPricesAndReturns prices;
	SUniverseDataEpsRocRoe epsrocroe;
	CEngineMSExcel excel;
	int nResult = 0;

	double fWeeklyAve, f3MAve, fFridayClose;
	COleDateTime friday = CalcInterestRate(excel, now, fWeeklyAve, f3MAve, fFridayClose);
	if(!CSQLCompany::UpdateInterestRate(friday, fWeeklyAve, f3MAve, fFridayClose))
	{
		AfxMessageBox("Unable to set Interest Rates in the Data-Base. Please try again."); 
		return false;
	}
	//AfxMessageBox("Interest Rate - UPDATED.");

	////////////////////////////////////////////////////////////////////////////////////////
	// Load Eps Roc Roe 
	nResult = excel.LoadEpsRocRoeFromBaseLine(sFileEpsRocRoe, arTickers, epsrocroe);
	switch(nResult)
	{
	case -1: AfxMessageBox("Unable to start MS Excel Application.  Please check whether MS Office is properly installed."); return false;
	case -2: AfxMessageBox("Baseline Excel Add-In not found.  Please make sure that Baseline is properly installed.");		return false;
	case -3: AfxMessageBox("Unable to read EPS, ROC and ROE from the specified Excel file.  Please check the file path.");			return false;
	}
	
	// Update Eps Roc Roe in the Data-Base
	if(!CSQLCompany::UpdateUniverseEpsRocRoe(epsrocroe, false, true))
	{
		AfxMessageBox("Unable to update the EPS, ROC and ROE in the Data-Base.");
		return false;
	}
	//AfxMessageBox("EPS, ROC and ROE - UPDATED.");

	////////////////////////////////////////////////////////////////////////////////////////
	// Load Fundamentals
	nResult = excel.LoadFundamentalsFromBaseLine(sFileFundamental, arTickers, data);
	switch(nResult)
	{
	case -1: AfxMessageBox("Unable to start MS Excel Application.  Please check whether MS Office is properly installed."); return false;
	case -2: AfxMessageBox("Baseline Excel Add-In not found.  Please make sure that Baseline is properly installed.");		return false;
	case -3: AfxMessageBox("Unable to read Fundamentals from the specified Excel file.  Please check the file path.");	return false;
	}
	
	// Update Fundamentals in the Data-Base
	if(!CSQLCompany::UpdateUniverseFundamentals(data, false))
	{
		AfxMessageBox("Unable to update the Fundamentals and Balance-Sheets in the Data-Base.");
		return false;
	}
//	AfxMessageBox("Fundamentals and Balance Sheets - UPDATED."); 

	////////////////////////////////////////////////////////////////////////////////////////
	// Load Prices 
	nResult = excel.LoadPricesFromBaseLine(sFilePrices, arTickers, prices);
	switch(nResult)
	{
	case -1: AfxMessageBox("Unable to start MS Excel Application.  Please check whether MS Office is properly installed."); return false;
	case -2: AfxMessageBox("Baseline Excel Add-In not found.  Please make sure that Baseline is properly installed.");		return false;
	case -3: AfxMessageBox("Unable to read Prices from the specified Excel file.  Please check the file path.");			return false;
	}
	
	// Update Prices in the Data-Base
	if(!CSQLCompany::UpdateUniversePricesAndReturns(prices, true))
	{
		AfxMessageBox("Unable to update the Prices in the Data-Base.");
		return false;
	}
	//AfxMessageBox("Prices - UPDATED.");

	AfxMessageBox("The Interest Rates, Fundamentals (Balance Sheets), EPS, ROC and ROE are updated in the Data-Base.");
	return FALSE;
}
