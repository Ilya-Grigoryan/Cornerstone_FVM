// CornerstoneFVModel.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"

#include "EngineMSExcel.h"
#include "History.h"
#include "Report.h"

#include "MainFrm.h"
#include "Doc.h"

#include "FrameGeneral.h"
#include "ViewGeneral.h"
#include "FrameMonteCarlo.h"
#include "ViewMonteCarlo.h"
#include "FrameHistory.h"
#include "ViewHistory.h"
#include "FrameCharts.h"
#include "ViewCharts.h"
#include "FrameMeetings.h"
#include "ViewMeetings.h"
#include "FrameTable.h"
#include "ViewTable.h"
#include "FrameChanges.h"
#include "ViewChanges.h"
#include "FrameHTML.h"
#include "ViewHTML.h"
#include "FrameMap.h"
#include "ViewMap.h"
#include "FrameMapSectors.h"
#include "ViewMapSectors.h"
#include "FrameDailySummary.h"
#include "ViewDailySummary.h"

#include "DialogLastReportYear.h"
#include "DialogMacro.h"
#include "DialogUploadData.h"
#include "DialogDate.h"
#include "DialogSectors.h"
#include "DialogCompanies.h"
#include "DialogStocksAlphabetical.h"
#include "DialogDataCheck.h"
#include "DialogConnections.h"
#include "DialogChangePassword.h"
#include "DialogInvProcess.h"

#include "DialogReportEBITDALeverage.h"
#include "DialogEarningsMomentum.h"
#include "DialogReportValueMomentum1.h"
#include "DialogReportGeneral.h"

#include "DialogSelectUniverse.h"
#include "DialogLogin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



////////////////////////////


/////////////////////////////////////////////////////////////////////////////
// CApp

BEGIN_MESSAGE_MAP(CApp, CWinApp)
	//{{AFX_MSG_MAP(CApp)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
	ON_COMMAND(ID_FILE_EXPORT_VALUES, OnFileExportValues)
	ON_COMMAND(ID_APP_LAST_EPS_YEAR, OnAppLastEpsYear)
	ON_COMMAND(ID_APP_MACROPARAMS, OnAppMacroparams)
	ON_COMMAND(ID_APP_UPLOAD_DATA, OnAppUploadData)
	ON_COMMAND(ID_APP_COMPANIES, OnAppCompanies)
	ON_COMMAND(ID_APP_SECTORS, OnAppSectors)
	ON_COMMAND(ID_APP_INDUSTRIES, OnAppIndustries)
	ON_COMMAND(ID_APP_UPLOAD_HISTORY, OnAppUploadHistory)
	ON_COMMAND(ID_APP_ADD_TO_HISTORY, OnAppAddToHistory)
	ON_COMMAND(ID_APP_DELETE_HISTORY_ITEM, OnAppDeleteHistoryItem)
	ON_COMMAND(ID_APP_CHARTS, OnAppCharts)
	ON_COMMAND(ID_APP_UPLOAD_WEIGHTS, OnAppUploadWeights)
	ON_COMMAND(ID_APP_PRICE_DATE, OnAppPriceDate)
	ON_COMMAND(ID_APP_MEETINGS, OnAppMeetings)
	ON_COMMAND(ID_APP_CHECK_SPLITS, OnAppCheckSplits)
	ON_COMMAND(ID_APP_CHECK_DATA, OnAppCheckData)
	ON_COMMAND(ID_APP_REPORT_EBITDA_LEVERAGE, OnAppReportEbitdaLeverage)
	ON_COMMAND(ID_APP_MAP, OnAppMap)
	ON_COMMAND(ID_APP_REPORT_EARNINGS_MOMENTUM, OnAppReportEarningsMomentum)
	ON_COMMAND(ID_APP_REPORT_VALUE_MOMENTUM, OnAppReportValueMomentum)
	ON_COMMAND(ID_APP_REPORT_GENERAL, OnAppReportGeneral)
	ON_COMMAND(ID_CONNECT_SERVERS, OnConnectServers)
	ON_COMMAND(ID_APP_MAPSECTORS, OnAppMapsectors)
	ON_COMMAND(ID_APP_SELECT_UNIVERSE, OnAppSelectUniverse)
	ON_COMMAND(ID_APP_SUMMARIES, OnAppSummaries)
	ON_COMMAND(ID_APP_CHANGE_PASSWORD, OnAppChangePassword)
	ON_COMMAND(ID_APP_INVPROCESS, OnAppInvprocess)
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
	// Standard print setup command
	ON_COMMAND(ID_FILE_PRINT_SETUP, CWinApp::OnFilePrintSetup)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CApp construction

CApp::CApp()
{
	m_nCurrentYear = -1;
	m_pDialogStocks = NULL;
	m_nUniverse = 1;

	m_nUserId = -1;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CApp object

CApp theApp;

static const CLSID clsid =
{ 0xbda9d066, 0xeb57, 0x48f8, { 0xb6, 0xd, 0xa7, 0xe0, 0xe6, 0x56, 0xb9, 0x31 } };

/////////////////////////////////////////////////////////////////////////////
// CApp initialization

#include "SQLMeetings.h"


BOOL CApp::InitInstance()
{
	CCommandLineInfo cmdInfo;
	ParseCommandLine(cmdInfo);
	CSplashWnd::EnableSplashScreen(cmdInfo.m_bShowSplash);

	// Initialize OLE libraries
	/*if (!AfxOleInit())
	{
		AfxMessageBox(IDP_OLE_INIT_FAILED);
		return FALSE;
	}*/

	AfxEnableControlContainer();

	// get the database (currently Baseline and Bloomberg)
	CString sDb = GetProfileString("Server Connections", "db");
	if(sDb.IsEmpty())
		m_nDatabase = 0;
	else
		m_nDatabase = atoi(sDb);

	// select universe
	CDialogSelectUniverse dlg;
	dlg.m_nSel = m_nUniverse-1;
	if(dlg.DoModal() != IDOK)
		return false;
	m_nUniverse = dlg.m_nSel+1;

	CSplashWnd::ShowSplashScreen(NULL);

	// Standard initialization

#ifdef _AFXDLL
	Enable3dControls();			// Call this when using MFC in a shared DLL
#else
	Enable3dControlsStatic();	// Call this when linking to MFC statically
#endif

	// Change the registry key under which our settings are stored.
	SetRegistryKey(_T("Cornerstone"));
	strcpy((char*)m_pszProfileName , "FV Model");

	LoadStdProfileSettings(5);  // Load standard INI file options (including MRU)

	// Register document templates
	m_pTemplateGeneral = new CMultiDocTemplate(
		IDR_MODELTYPE,
		RUNTIME_CLASS(CDoc),
		RUNTIME_CLASS(CFrameGeneral), // custom MDI child frame
		RUNTIME_CLASS(CViewGeneral));
	AddDocTemplate(m_pTemplateGeneral);
	m_server.ConnectTemplate(clsid, m_pTemplateGeneral, FALSE);

	m_pTemplateMonteCarlo = new CMultiDocTemplate(
		IDR_MODELTYPE,
		RUNTIME_CLASS(CDoc),
		RUNTIME_CLASS(CFrameMonteCarlo), // custom MDI child frame
		RUNTIME_CLASS(CViewMonteCarlo));
	AddDocTemplate(m_pTemplateMonteCarlo);
	m_server.ConnectTemplate(clsid, m_pTemplateMonteCarlo, FALSE);

	m_pTemplateHistory = new CMultiDocTemplate(
		IDR_MODELTYPE,
		RUNTIME_CLASS(CDoc),
		RUNTIME_CLASS(CFrameHistory), // custom MDI child frame
		RUNTIME_CLASS(CViewHistory));
	AddDocTemplate(m_pTemplateHistory);
	m_server.ConnectTemplate(clsid, m_pTemplateHistory, FALSE); 

	m_pTemplateCharts = new CMultiDocTemplate(
		IDR_MODELTYPE,
		RUNTIME_CLASS(CDoc),
		RUNTIME_CLASS(CFrameCharts), // custom MDI child frame
		RUNTIME_CLASS(CViewCharts));
	AddDocTemplate(m_pTemplateCharts);
	m_server.ConnectTemplate(clsid, m_pTemplateCharts, FALSE);

	m_pTemplateMeetings = new CMultiDocTemplate(
		IDR_MODELTYPE,
		RUNTIME_CLASS(CDoc),
		RUNTIME_CLASS(CFrameMeetings), // custom MDI child frame
		RUNTIME_CLASS(CViewMeetings));
	AddDocTemplate(m_pTemplateMeetings);
	m_server.ConnectTemplate(clsid, m_pTemplateMeetings, FALSE);

	m_pTemplateTable = new CMultiDocTemplate(
		IDR_MODELTYPE,
		RUNTIME_CLASS(CDoc),
		RUNTIME_CLASS(CFrameTable), // custom MDI child frame
		RUNTIME_CLASS(CViewTable));
	AddDocTemplate(m_pTemplateTable);
	m_server.ConnectTemplate(clsid, m_pTemplateTable, FALSE);

	m_pTemplateChanges = new CMultiDocTemplate(
		IDR_MODELTYPE,
		RUNTIME_CLASS(CDoc),
		RUNTIME_CLASS(CFrameChanges), // custom MDI child frame
		RUNTIME_CLASS(CViewChanges));
	AddDocTemplate(m_pTemplateChanges);
	m_server.ConnectTemplate(clsid, m_pTemplateChanges, FALSE);

	m_pTemplateHTML = new CMultiDocTemplate(
		IDR_MODELTYPE,
		RUNTIME_CLASS(CDoc),
		RUNTIME_CLASS(CFrameHTML), // custom MDI child frame
		RUNTIME_CLASS(CViewHTML));
	AddDocTemplate(m_pTemplateHTML);
	m_server.ConnectTemplate(clsid, m_pTemplateHTML, FALSE);

	m_pTemplateMap = new CMultiDocTemplate(
		IDR_MODELTYPE,
		RUNTIME_CLASS(CDoc),
		RUNTIME_CLASS(CFrameMap), // custom MDI child frame
		RUNTIME_CLASS(CViewMap));
	AddDocTemplate(m_pTemplateMap);
	m_server.ConnectTemplate(clsid, m_pTemplateMap, FALSE);

	m_pTemplateMapSectors = new CMultiDocTemplate(
		IDR_MODELTYPE,
		RUNTIME_CLASS(CDoc),
		RUNTIME_CLASS(CFrameMapSectors), // custom MDI child frame
		RUNTIME_CLASS(CViewMapSectors));
	AddDocTemplate(m_pTemplateMapSectors);
	m_server.ConnectTemplate(clsid, m_pTemplateMapSectors, FALSE);

	m_pTemplateSummary = new CMultiDocTemplate(
	IDR_MODELTYPE,
	RUNTIME_CLASS(CDoc),
	RUNTIME_CLASS(CFrameDailySummary), // custom MDI child frame
	RUNTIME_CLASS(CViewDailySummary));
	AddDocTemplate(m_pTemplateSummary);
	m_server.ConnectTemplate(clsid, m_pTemplateSummary, FALSE);

	COleTemplateServer::RegisterAll();

	// load and calculate stock universe information
	COleDateTime now = COleDateTime::GetCurrentTime();
	CSQLCompany::SetDate(now);
	m_Universe.LoadAll(m_nUniverse, m_nCurrentYear, now);

	// create main MDI Frame window
	CMainFrame* pMainFrame = new CMainFrame;
	if (!pMainFrame->LoadFrame(IDR_MAINFRAME))
		return FALSE;
	m_pMainWnd = pMainFrame;

	// Enable drag/drop open
	m_pMainWnd->DragAcceptFiles();

	// Enable DDE Execute open
	EnableShellOpen();
	RegisterShellFileTypes(TRUE);

	if (cmdInfo.m_bRunEmbedded || cmdInfo.m_bRunAutomated)
	{
		// Application was run with /Embedding or /Automation.  Don't show the
		//  main window in this case.
		return TRUE;
	}

	pMainFrame->ShowWindow(SW_SHOWMAXIMIZED);
	pMainFrame->UpdateWindow();

	// Check for Interest rate not to be more than week old
	COleDateTime friday = CEngineMSExcel::GetLastFriday(now);
	if(!CSQLCompany::DoesInterestRateRecordExist(friday))
	{
		CString sMessage;
		sMessage.Format("Interest Rate information might be outdated (more than a week old).\n\nThe Interest Rate record for last Friday, %s doesn't exist.  Please check the Model for updates.\n\nFor now, the most recent Rate of %.2f%% will be used.", 
			friday.Format("%m/%d/%Y"), m_Universe.GetMacroParametersPointer()->GetDiscountRate()*100);
		AfxMessageBox(sMessage, MB_ICONINFORMATION);
	}

	// Check for the companies with RollOver reporting year
	CArray<CCompany*,CCompany*>	arCompanies;
	int nRollOvers = m_Universe.GetCompaniesWithRollOverYear(arCompanies);
	if(nRollOvers > 0)
	{
		CString sComps;
		for(int i=0; i<min(7,arCompanies.GetSize()); i++)
		{
			sComps += arCompanies[i]->GetTicker(); 
			sComps += "\n";
		}
		if(arCompanies.GetSize() > 7)
		{
			CString sEnd; sEnd.Format("and %u more...\n", arCompanies.GetSize()-7);
			sComps += sEnd;
		}
		
		CString sMessage; sMessage.Format("The following %u companies have rolled over reporting years. \n\n"
			"%s\nEPS, ROC and ROE must be updated for these companies.  \nDo you want to update them now?", nRollOvers, sComps);
		int nRes = AfxMessageBox(sMessage, MB_ICONQUESTION|MB_YESNO);
		if(nRes == IDYES)
		{
			for(int i=0; i<arCompanies.GetSize(); i++)
			{
				NewViewGeneral(arCompanies[i]);
				CSQLCompany::SetCompanyRollOver(arCompanies[i]->GetTicker(), 0);
			}
		}
	}

	// Check for the companies with No Shares Outstanding
	arCompanies.RemoveAll();
	int nNoShares = m_Universe.GetCompaniesWithNoSharesOut(arCompanies);
	if(nNoShares > 0)
	{
		CString sComps;
		for(int i=0; i<min(7,arCompanies.GetSize()); i++)
		{
			sComps += arCompanies[i]->GetTicker(); 
			sComps += "\n";
		}
		if(arCompanies.GetSize() > 7)
		{
			CString sEnd; sEnd.Format("and %u more...\n", arCompanies.GetSize()-7);
			sComps += sEnd;
		}
		
		CString sMessage; sMessage.Format("The following %u companies have no Shares Outstanding. \n\n"
			"%s\nPerhaps, these companies no longer exist.  \nDo you want to check them now?", nNoShares, sComps);
		int nRes = AfxMessageBox(sMessage, MB_ICONQUESTION|MB_YESNO);
		if(nRes == IDYES)
		{
			for(int i=0; i<arCompanies.GetSize(); i++)
				NewViewGeneral(arCompanies[i]);
		}
	}

	// Check for the companies with SP500 index controversy
	if(m_nDatabase == 0) 
	{
		arCompanies.RemoveAll();
		int nSP500 = m_Universe.GetCompaniesWithSP500Controversy(arCompanies);
		if(nSP500 > 0)
		{
			CString sComps;
			for(int i=0; i<min(7,arCompanies.GetSize()); i++)
			{
				sComps += arCompanies[i]->GetTicker(); 
				sComps += "\n";
			}
			if(arCompanies.GetSize() > 7)
			{
				CString sEnd; sEnd.Format("and %u more...\n", arCompanies.GetSize()-7);
				sComps += sEnd;
			}
			
			CString sMessage; sMessage.Format("The following %u companies are no longer in the S&P 500 index, but have positive weights in the Data-Base. \n\n"
				"%s\nPerhaps, these companies have been just removed from S&P 500.  \nDo you want to update index weights now?", nSP500, sComps);
			int nRes = AfxMessageBox(sMessage, MB_ICONQUESTION|MB_YESNO);
			if(nRes == IDYES)
				OnAppCompanies();
		}
	}

	CReport::ConstructDefaultFilter(m_RepFilter);

	return TRUE;
}


/////////////////////////////////////////////////////////////////////////////
// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	//{{AFX_DATA(CAboutDlg)
	enum { IDD = IDD_ABOUTBOX };
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CAboutDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	//{{AFX_MSG(CAboutDlg)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
	//{{AFX_DATA_INIT(CAboutDlg)
	//}}AFX_DATA_INIT
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CAboutDlg)
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
	//{{AFX_MSG_MAP(CAboutDlg)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

// App command to run the dialog
void CApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

/////////////////////////////////////////////////////////////////////////////
// CApp message handlers


BOOL CApp::PreTranslateMessage(MSG* pMsg) 
{
	if (CSplashWnd::PreTranslateAppMessage(pMsg))
		return TRUE;
	
	return CWinApp::PreTranslateMessage(pMsg);
}

void CApp::NewViewGeneral(CCompany* pCompany)
{
	CDocument* pDoc = m_pTemplateGeneral->OpenDocumentFile(NULL);
	if(!pDoc)
		return;

	CString sTitle;
	sTitle.Format("%s  -  %s", pCompany->GetTicker(), pCompany->GetName());
	pDoc->SetTitle(sTitle);

	POSITION pos = pDoc->GetFirstViewPosition();
	if (pos != NULL)
	{
		CViewGeneral* pView = (CViewGeneral*)pDoc->GetNextView(pos);
		pView->SetCompany(pCompany);
	}   
}

void CApp::NewViewMonteCarlo(CCompany* pCompany)
{
	CDocument* pDoc = m_pTemplateMonteCarlo->OpenDocumentFile(NULL);
	if(!pDoc)
		return;

	CString sTitle;
	sTitle.Format("%s - %s - Monte-Carlo Simulation", pCompany->GetTicker(), pCompany->GetName());
	pDoc->SetTitle(sTitle);

	POSITION pos = pDoc->GetFirstViewPosition();
	if (pos != NULL)
	{
		CViewMonteCarlo* pView = (CViewMonteCarlo*)pDoc->GetNextView(pos);
		pView->SetCompany(pCompany);
	}
}

void CApp::NewViewHistory(CCompany* pCompany)
{
	CDocument* pDoc = m_pTemplateHistory->OpenDocumentFile(NULL);
	if(!pDoc)
		return;

	CString sTitle;
	sTitle.Format("%s - %s - History", pCompany->GetTicker(), pCompany->GetName());
	pDoc->SetTitle(sTitle);

	POSITION pos = pDoc->GetFirstViewPosition();
	if (pos != NULL)
	{
		CViewHistory* pView = (CViewHistory*)pDoc->GetNextView(pos);
		pView->SetCompany(pCompany);
	}
}

CViewCharts* CApp::NewViewCharts()
{
	CDocument* pDoc = m_pTemplateCharts->OpenDocumentFile(NULL);
	if(!pDoc)
		return NULL;

	CString sTitle;
	pDoc->SetTitle("Trendline Graphs");

	POSITION pos = pDoc->GetFirstViewPosition();
	if (pos != NULL)
	{
		CViewCharts* pView = (CViewCharts*)pDoc->GetNextView(pos);
		return pView;
	}

	return NULL;
}

CViewTable* CApp::NewViewTable()
{
	CDocument* pDoc = m_pTemplateTable->OpenDocumentFile(NULL);
	if(!pDoc)
		return NULL;

	CString sTitle;
	pDoc->SetTitle("Data");

	POSITION pos = pDoc->GetFirstViewPosition();
	if (pos != NULL)
	{
		CViewTable* pView = (CViewTable*)pDoc->GetNextView(pos);
		return pView;
	}

	return NULL;
}

CViewChanges* CApp::NewViewChanges()
{
	CDocument* pDoc = m_pTemplateChanges->OpenDocumentFile(NULL);
	if(!pDoc)
		return NULL;

	CString sTitle;
	pDoc->SetTitle("Companies with significant changes in data");

	POSITION pos = pDoc->GetFirstViewPosition();
	if (pos != NULL)
	{
		CViewChanges* pView = (CViewChanges*)pDoc->GetNextView(pos);
		return pView;
	}

	return NULL;
}

CViewHTML* CApp::NewViewHTML()
{
	CDocument* pDoc = m_pTemplateHTML->OpenDocumentFile(NULL);
	if(!pDoc)
		return NULL;

	CString sTitle;
	pDoc->SetTitle("Print Preiew");

	POSITION pos = pDoc->GetFirstViewPosition();
	if (pos != NULL)
	{
		CViewHTML* pView = (CViewHTML*)pDoc->GetNextView(pos);
		return pView;
	}

	return NULL;
}

CViewMap* CApp::NewViewMap()
{
	CDocument* pDoc = m_pTemplateMap->OpenDocumentFile(NULL);
	if(!pDoc)
		return NULL;

	CString sTitle;
	pDoc->SetTitle("Stock Migration Map");

	POSITION pos = pDoc->GetFirstViewPosition();
	if (pos != NULL)
	{
		CViewMap* pView = (CViewMap*)pDoc->GetNextView(pos);
		return pView;
	}

	return NULL;
}

CViewMapSectors* CApp::NewViewMapSectors()
{
	CDocument* pDoc = m_pTemplateMapSectors->OpenDocumentFile(NULL);
	if(!pDoc)
		return NULL;

	CString sTitle;
	pDoc->SetTitle("Sector Ranking Map");

	POSITION pos = pDoc->GetFirstViewPosition();
	if (pos != NULL)
	{
		CViewMapSectors* pView = (CViewMapSectors*)pDoc->GetNextView(pos);
		return pView;
	}

	return NULL;
}

CViewDailySummary*	CApp::NewViewSummaries()
{
	if(theApp.m_nUserId == -1)
	{
		CDialogLogin dlg;
		dlg.DoModal();

		if(dlg.m_nUserId == -1)
			return NULL;

		theApp.m_nUserId = dlg.m_nUserId;
	}


	CDocument* pDoc = m_pTemplateSummary->OpenDocumentFile(NULL);
	if(!pDoc)
		return NULL;

	CString sTitle;
	pDoc->SetTitle("Daily Summaries on Valuation and Funadamentals");

	POSITION pos = pDoc->GetFirstViewPosition();
	if (pos != NULL)
	{
		CViewDailySummary* pView = (CViewDailySummary*)pDoc->GetNextView(pos);
		return pView;
	}

	return NULL;
}

CViewMeetings* CApp::NewViewMeetings()
{
	CDocument* pDoc = m_pTemplateMeetings->OpenDocumentFile(NULL);
	if(!pDoc)
		return NULL;

	CString sTitle;
	pDoc->SetTitle("Investment Meetings");

	POSITION pos = pDoc->GetFirstViewPosition();
	if (pos != NULL)
	{
		CViewMeetings* pView = (CViewMeetings*)pDoc->GetNextView(pos);
		return pView;
	}

	return NULL;
}

int CApp::ExitInstance() 
{
	//m_Universe.SaveLocalCopyInAppData();	 
	return CWinApp::ExitInstance();
}

void CApp::OnFileExportValues() 
{
	CTime date = CTime::GetCurrentTime();
	CString sTime = date.Format("%Y_%m_%d");
	CString sFileName;
	sFileName.Format("CIP_Universe_Values_%s.xls", sTime);

	static char szFilters[] ="Excel Files (*.xls)|*.txt|All Files (*.*)|*.*||";
	CFileDialog fileDlg (false, "xls", sFileName, OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY, szFilters, NULL);

	if(fileDlg.DoModal() != IDOK )
		return;
	
	CString sName = fileDlg.GetPathName();
	
	CStdioFile file;
	if(!file.Open(sName, CFile::modeCreate|CFile::modeWrite|CFile::typeText))
	{
		CString sErr; sErr.Format("Unable to create file: '%s'", sName);
		::MessageBox(NULL, sErr, "Cornerstone Fair Value Model", MB_ICONSTOP);
		return;
	}

	for(int i=0; i<m_Universe.GetSize(); i++)
	{
		CCompany* pCompany = m_Universe.GetCompany(i);
		CString sTicker = pCompany->GetTicker();
		CString s;
		s.Format("%s-Normalized\t%.2f\n%s-Forecast\t%.2f\n%s-Razor\t%.2f\n%s-Razor Forecast\t%.2f\n",
				
				sTicker, pCompany->GetEPV(),
				sTicker, pCompany->GetEPVFore(),
				sTicker, pCompany->GetEPVRazor(),
				sTicker, pCompany->GetEPVRazorFore());
		file.WriteString(s);
	}

	file.Close();
	ShellExecute(AfxGetMainWnd()->m_hWnd, "open", sName, NULL, NULL, SW_SHOW);
}

void CApp::OnAppLastEpsYear() 
{
	CDialogLastReportYear dlg;
	dlg.m_nOption = (m_nCurrentYear == -1) ? 0 : 1;
	dlg.m_nYear = m_nCurrentYear;

	if(dlg.DoModal() != IDOK)
		return;

	m_nCurrentYear = dlg.m_nYear;

	// load and calculate stock universe information
	COleDateTime now = COleDateTime::GetCurrentTime();

	for(int i=0; i<m_Universe.GetSize(); i++)
	{
		CCompany* pCompany = m_Universe.GetCompany(i);
		
		SCompanyRawData CompanyData;
		CSQLCompany::GetCompanyRawData(m_nCurrentYear, pCompany->GetTicker(), now, CompanyData);
		
		pCompany->CreateFromRawData(CompanyData);
		pCompany->CalcAll();
		if(m_nCurrentYear != -1)
			pCompany->SetYear(m_nCurrentYear);
	}

	m_Universe.RecalcRankings();

	CMainFrame* pMainFrame = (CMainFrame*)m_pMainWnd;
	CWnd* pMDI = CWnd::FromHandle(pMainFrame->m_hWndMDIClient);

	CWnd* pChild = pMDI->GetWindow(GW_CHILD);
	while(pChild)
	{
		if(pChild->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)))
		{
			CMDIChildWnd* pChildFrame = (CMDIChildWnd*)pChild;
			CView* pView = pChildFrame->GetActiveView();
			if(pView->IsKindOf(RUNTIME_CLASS(CViewGeneral)))
			{
				CViewGeneral* pMainView = (CViewGeneral*)pView;
				pMainView->SetCompany(pMainView->m_pCompany);
				pMainView->Invalidate();
			}
		}

		pChild = pChild->GetWindow(GW_HWNDNEXT);
	}

	m_Universe.CalcAll();
}

void CApp::OnAppMacroparams() 
{
	CDialogMacro dlg;
	dlg.m_Macro = m_Universe.GetMacroParameters();
	if(dlg.DoModal() != IDOK)
		return;

	m_Universe.SetMacroParameters(dlg.m_Macro);

	m_Universe.CalcAll();

	CMainFrame* pMainFrame = (CMainFrame*)m_pMainWnd;
	CWnd* pMDI = CWnd::FromHandle(pMainFrame->m_hWndMDIClient);

	CWnd* pChild = pMDI->GetWindow(GW_CHILD);
	while(pChild)
	{
		if(pChild->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)))
		{
			CMDIChildWnd* pChildFrame = (CMDIChildWnd*)pChild;
			pChild = pChild->GetWindow(GW_HWNDNEXT);
			pChildFrame->MDIDestroy();

			/*CView* pView = pChildFrame->GetActiveView();
			if(pView->IsKindOf(RUNTIME_CLASS(CViewGeneral)))
			{
				CViewGeneral* pMainView = (CViewGeneral*)pView;
				pMainView->SetCompany(pMainView->m_pCompany);
				pMainView->Invalidate();
			}*/
		}
		else
			pChild = pChild->GetWindow(GW_HWNDNEXT);
	}	
}

void CApp::OnAppUploadData() 
{
	CDialogUploadData dlg;
	if(dlg.DoModal() != IDOK)
		return;

	CEngineMSExcel excel;
	SUniverseDataFundamentals dataFun;
	SUniverseDataEpsRocRoe dataEps;
	SUniverseDataPricesAndReturns dataPriceRet;
	SUniverseDataShares dataShares;

	// Balance Sheet
	if(!dlg.m_sBS.IsEmpty())
	{
		if(!excel.LoadFundamentals(dlg.m_sBS, dataFun))
		{
			AfxMessageBox("Unable to load Fundamental data. Please check the file format.", MB_ICONINFORMATION);
			return;
		}

		int nUpdated = CSQLCompany::UpdateUniverseFundamentals(dataFun, dlg.m_bForceBS);
		if(nUpdated == dataFun.GetSize())
		{
			CString s;
			s.Format("Fundamentals for all %u companies are successfully updated in the Data-Base.", nUpdated);
			AfxMessageBox(s, MB_ICONINFORMATION);
		}
		else
		{
			CString s;
			s.Format("Fundamentals for only %u out of %u companies are updated. \nAn error occured while updating the other %u companies.\n\nPlease check the Input File format and try again.", 
					nUpdated, dataFun.GetSize(), dataFun.GetSize() - nUpdated);
			AfxMessageBox(s, MB_ICONINFORMATION);
		}

	}

	// EPS, ROC, ROE
	if(!dlg.m_sEPS.IsEmpty())
	{
		if(!excel.LoadEpsRocAndRoe(dlg.m_sEPS, dataEps))
		{
			AfxMessageBox("Unable to load EPS, ROC and ROE data. Please check the file format.");
			return;
		}
		
		int nUpdated = CSQLCompany::UpdateUniverseEpsRocRoe(dataEps, dlg.m_bForceEPS);
		if(nUpdated == dataEps.GetSize())
		{
			CString s;
			s.Format("EPS, ROC and ROE for all %u companies are successfully updated in the Data-Base.", nUpdated);
			AfxMessageBox(s, MB_ICONINFORMATION);
		}
		else
		{
			CString s;
			s.Format("EPS, ROC and ROE for only %u out of %u companies are updated. \nAn error occured while updating the other %u companies.\n\nPlease check the Input File format and try again.", 
					nUpdated, dataEps.GetSize(), dataEps.GetSize() - nUpdated);
			AfxMessageBox(s, MB_ICONINFORMATION);
		}
	}
	
	if(!dlg.m_sPrice.IsEmpty() || !dlg.m_sRet.IsEmpty())
	{
		if(!excel.LoadPricesAndReturns(dlg.m_sPrice, dlg.m_sRet, dataPriceRet))
		{
			AfxMessageBox("Unable to load Price/Return data. Please check the file format.");
			return;
		}

		int nUpdated = CSQLCompany::UpdateUniversePricesAndReturns(dataPriceRet, dlg.m_bForceBS);
		if(nUpdated == dataPriceRet.GetSize())
		{
			CString s;
			s.Format("Prices for all %u companies are successfully updated in the Data-Base.", nUpdated);
			AfxMessageBox(s, MB_ICONINFORMATION);
		}
		else
		{
			CString s;
			s.Format("Prices for only %u out of %u companies are updated. \nAn error occured while updating the other %u companies.\n\nPlease check the Input File format and try again.", 
					nUpdated, dataPriceRet.GetSize(), dataPriceRet.GetSize() - nUpdated);
			AfxMessageBox(s, MB_ICONINFORMATION);
		}
	}

	// Shares Outstanding
	if(!dlg.m_sShares.IsEmpty())
	{
		if(!excel.LoadShares(dlg.m_sShares, dataShares))
		{
			AfxMessageBox("Unable to load Shares Outstanding data. Please check the file format.");
			return;
		}
		
		int nUpdated = CSQLCompany::UpdateUniverseShares(dataShares, dlg.m_bForceEPS);
		if(nUpdated == dataShares.GetSize())
		{
			CString s;
			s.Format("Shares Outstanding for all %u companies are successfully updated in the Data-Base.", nUpdated);
			AfxMessageBox(s, MB_ICONINFORMATION);
		}
		else
		{
			CString s;
			s.Format("Shares Outstanding for only %u out of %u companies are updated. \nAn error occured while updating the other %u companies.\n\nPlease check the Input File format and try again.", 
					nUpdated, dataEps.GetSize(), dataEps.GetSize() - nUpdated);
			AfxMessageBox(s, MB_ICONINFORMATION);
		}
	}

	//AfxMessageBox("The Information is successfully uploaded into the Data-Base. Please restart the program to see the changes.");
}

void CApp::OnAppCompanies()   
{
	CDialogCompanies dlg;
	dlg.DoModal();

	BeginWaitCursor();

	m_Universe.LoadAll(m_nUniverse, m_nCurrentYear, m_Universe.GetDate());
	m_Universe.CalcAll();
	m_pDialogStocks->SetupData();

	CMainFrame* pMainFrame = (CMainFrame*)m_pMainWnd;
	CWnd* pMDI = CWnd::FromHandle(pMainFrame->m_hWndMDIClient);

	CWnd* pChild = pMDI->GetWindow(GW_CHILD);
	while(pChild)
	{
		if(pChild->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)))
		{
			CMDIChildWnd* pChildFrame = (CMDIChildWnd*)pChild;
			pChild = pChild->GetWindow(GW_HWNDNEXT);
			pChildFrame->MDIDestroy();

			/*CView* pView = pChildFrame->GetActiveView();
			if(pView->IsKindOf(RUNTIME_CLASS(CViewGeneral)))
			{
				CViewGeneral* pMainView = (CViewGeneral*)pView;
				pMainView->SetCompany(pMainView->m_pCompany);
				pMainView->Invalidate();
			}*/

			
		}
		else
			pChild = pChild->GetWindow(GW_HWNDNEXT);
	}

	EndWaitCursor();
}

void CApp::OnAppSectors() 
{
	CDialogSectors dlg(0);
	if(dlg.DoModal() != IDOK)
		return;
}

void CApp::OnAppIndustries() 
{
	CDialogSectors dlg(1);
	if(dlg.DoModal() != IDOK)
		return;
}

void CApp::OnAppUploadHistory() 
{
	static int nLargeNum = 1000*(_MAX_PATH + 1) + 1;
	static char szFilters[] ="Excel Files (*.xls)|*.xls|All Files (*.*)|*.*||";

	CFileDialog fileDlg (true, "xls", "", OFN_ALLOWMULTISELECT|OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, szFilters, NULL);
	CString fileName;
	fileDlg.m_ofn.lpstrFile = fileName.GetBuffer(nLargeNum);
	fileDlg.m_ofn.nMaxFile = nLargeNum;
	if(fileDlg.DoModal() != IDOK )
		return;		

	POSITION pos = fileDlg.GetStartPosition( );
	int nCount = 0;
	while(pos)
	{
		CString sPath = fileDlg.GetNextPathName(pos);

		CEngineMSExcel excel; 
		SHistory history;

 		if(!excel.LoadHistory(sPath, history))
		{
			CString s; s.Format("Unable to load History data from file '%s'. Please check the file format.", sPath);
			AfxMessageBox(s);
			continue;
		}

		if(!CSQLCompany::UpdateHistory(history, NUM_COLUMNS_IN_HISTORY_EXCEL))
		{
			CString s; s.Format("Unable to update History in the Data-Base for file '%s'. Please check the file format.", sPath);
			AfxMessageBox(s);
			continue;
		}

		++nCount;
	}
	fileName.ReleaseBuffer();

	CString s; s.Format("The History for '%u' companiy(ies) is successfully uploaded into the Data-Base. Please restart the program to see the changes.", nCount);
	AfxMessageBox(s);
}

void CApp::OnAppAddToHistory() 
{
	COleDateTime now(COleDateTime::GetCurrentTime());
	COleDateTime date = COleDateTime(now.GetYear(), now.GetMonth(), now.GetDay(), 0,0,0);

	int nRes = AfxMessageBox("Are you sure you want to add all current records into the History?", MB_YESNOCANCEL);
	if(nRes != IDYES)
		return;
	
	CDialogDate dlg;
	dlg.m_Date = date;
	if(dlg.DoModal() != IDOK)
		return;		

	for(int i=0; i<m_Universe.GetSize(); i++)
	{
		CCompany* pCompany = m_Universe.GetCompany(i);
		pCompany->LoadHistory(m_Universe.GetMacroParametersPointer()->GetPeriodsMA()+1);

		CHistory* pHist = pCompany->GetHistory();

		// New record
		SHistoryItem item;
		item.date = dlg.m_Date;
		item.data[0] = _INF_;
		pCompany->GetHistoryItem(item);
		CSQLCompany::UpdateHistoryItem(pCompany->GetTicker(), item, NUM_COLUMNS_IN_HISTORY_DB);

		// Modify the last one
		if(pHist->GetSize() > 0)
		{
			pHist->GetItemPtr(0)->data[0] = 100.0 * ( item.data[1] / pHist->GetItemPtr(0)->data[1]  -  1 );
			CSQLCompany::UpdateHistoryItem(pCompany->GetTicker(), *pHist->GetItemPtr(0), NUM_COLUMNS_IN_HISTORY_DB);
		}
	}

	AfxMessageBox("History Records are successfully added for all Companies in the Universe.");
}

void CApp::OnAppDeleteHistoryItem() 
{
	COleDateTime now(COleDateTime::GetCurrentTime());
	COleDateTime date = COleDateTime(now.GetYear(), now.GetMonth(), now.GetDay(), 0,0,0);
	
	CDialogDate dlg;
	dlg.m_Date = date;
	if(dlg.DoModal() != IDOK)
		return;				

	CString sMessage; sMessage.Format("Are you sure you want to permanently delete all '%s' History records from the Data-Base? You cannot undo this operation.", dlg.m_Date.Format("%m/%d/%Y"));
	int nRes = AfxMessageBox(sMessage, MB_YESNOCANCEL);
	if(nRes != IDYES)
		return;

	CSQLCompany::DeleteAllHistoryItems(dlg.m_Date);

	sMessage.Format("'%s' History Records are successfully deleted for all Companies in the Universe.", dlg.m_Date.Format("%m/%d/%Y"));
	AfxMessageBox(sMessage);

}

void CApp::OnAppCharts() 
{
	NewViewCharts();	
}


void CApp::OnAppUploadWeights() 
{
	static char szFilters[] ="Excel Files (*.xls)|*.xls|All Files (*.*)|*.*||";

	CFileDialog fileDlg (true, "xls", "", OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, szFilters, NULL);
	if(fileDlg.DoModal() != IDOK )
		return;		

	CEngineMSExcel excel;
	SUniverseWeightsInIndices data;

	if(!excel.LoadWeightsInIndices(fileDlg.GetPathName(), data))
	{
		AfxMessageBox("Unable to load Indices/Weights data. Please check the file format.");
		return;
	}

	if(!CSQLCompany::UpdateUniverseWeightsInIndices(data))
	{
		AfxMessageBox("Unable to update information in the Data-Base. Please check the file format.");
		return;
	}

	AfxMessageBox("The Information is uploaded into the Data-Base. Please restart the program to see the changes.", MB_ICONINFORMATION);	
}

void CApp::OnAppPriceDate() 
{
	CDialogDate dlg;
	dlg.m_Date = m_Universe.GetDate();
	if(dlg.DoModal() != IDOK)
		return;

	BeginWaitCursor();

	COleDateTime date = dlg.m_Date;

	CSQLCompany::SetDate(date);
	m_Universe.LoadAll(m_nUniverse, m_nCurrentYear, date);
	m_Universe.CalcAll();
	m_pDialogStocks->SetupData();

	CMainFrame* pMainFrame = (CMainFrame*)m_pMainWnd;
	CWnd* pMDI = CWnd::FromHandle(pMainFrame->m_hWndMDIClient);

	CWnd* pChild = pMDI->GetWindow(GW_CHILD);
	while(pChild)
	{
		if(pChild->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)))
		{
			CMDIChildWnd* pChildFrame = (CMDIChildWnd*)pChild;
			pChild = pChild->GetWindow(GW_HWNDNEXT);
			pChildFrame->MDIDestroy();

			/*CView* pView = pChildFrame->GetActiveView();
			if(pView->IsKindOf(RUNTIME_CLASS(CViewGeneral)))
			{
				CViewGeneral* pMainView = (CViewGeneral*)pView;
				pMainView->SetCompany(pMainView->m_pCompany);
				pMainView->Invalidate();
			}*/
		}
		else
			pChild = pChild->GetWindow(GW_HWNDNEXT);
	}	

	EndWaitCursor();
}

void CApp::OnAppMeetings() 
{
	NewViewMeetings();		
}

void CApp::OnAppCheckSplits() 
{
	CDialogDate dlg;
	dlg.m_Date = CEngineMSExcel::GetLastFriday(COleDateTime::GetCurrentTime()) - COleDateTimeSpan(7,0,0,0);
	if(dlg.DoModal() != IDOK)
		return;

	COleDateTime date = dlg.m_Date;
	COleDateTime dtLastAutoSplit = CSQLCompany::GetLastAutoSplitDate();
	if(dtLastAutoSplit > date)
		date = dtLastAutoSplit;

	CArray<CCompany*,CCompany*>	arCompanies;
	int nSplits = m_Universe.GetCompaniesWithRecentSplit(date, arCompanies);
	if(nSplits > 0)
	{
		CString sComps;
		for(int i=0; i<min(7,arCompanies.GetSize()); i++)
		{
			sComps += arCompanies[i]->GetTicker() + " - ";
			sComps += arCompanies[i]->GetLastSplitDate().Format("%m/%d/%Y") + ", ";
			sComps += arCompanies[i]->GetSplitRatio();
			sComps += "\n";
		}
		if(arCompanies.GetSize() > 7)
		{
			CString sEnd; sEnd.Format("and %u more...\n", arCompanies.GetSize()-7);
			sComps += sEnd;
		}
		
		CString sMessage; sMessage.Format("The following %u companies have recent Stock Splits. \n\n"
			"%s\nPrice, Shares Outstanding, EPS, Dividend, Book Value and History need updating for these companies.  \nDo you want to make automatic split now?", nSplits, sComps);
		int nRes = AfxMessageBox(sMessage, MB_ICONQUESTION|MB_YESNO);
		if(nRes == IDYES)
		{
			for(int i=0; i<arCompanies.GetSize(); i++)
			{
				// Get the ratio
				double fRatio = arCompanies[i]->GetSplitRatioDouble();
				if(fRatio <= 0)
				{
					CString sMessage; sMessage.Format("The company '%s' has an invalid Split Ratio '%s'", 
						arCompanies[i]->GetTicker(), arCompanies[i]->GetSplitRatio());
					AfxMessageBox(sMessage, MB_ICONINFORMATION); 
				}

				// Split
				arCompanies[i]->Split(fRatio);

				// Open the General View
				NewViewGeneral(arCompanies[i]);
			}

			CSQLCompany::SetLastAutoSplitDate(COleDateTime::GetCurrentTime());
		}
	}
	else
		AfxMessageBox("There are no companies with recent Stock Split that need handling.", MB_ICONINFORMATION);
}

void CApp::OnAppCheckData() 
{
	CDialogDataCheck dlg;
	if(dlg.DoModal() != IDOK)
		return;

	BeginWaitCursor();

	CArray<CCompany*,CCompany*>	arCompanies;
	CArray<SChangeCriteria,SChangeCriteria> arCH;
	for(int i=0; i<m_Universe.GetSize(); i++)
	{
		CCompany* pCompany = m_Universe.GetCompany(i);
		
		SChangeCriteria ch = dlg.m_CH;
		if(pCompany->CheckForChanges(ch))
		{
			arCompanies.Add(pCompany);
			arCH.Add(ch);
		}
	}

	if(arCompanies.GetSize() == 0)
	{
		AfxMessageBox("There are no companies that meet with the change criteria you have specified.", MB_ICONINFORMATION);
		return;
	}

	CString sMessage;
	sMessage.Format("The following %u companies meet the change criteria you specified over the last %u periods:\n\n", arCompanies.GetSize(), dlg.m_CH.nPeriods);
	
	for(int i=0; i<min(7,arCompanies.GetSize()); i++)
	{
		CCompany* pCompany = arCompanies[i];
		SChangeCriteria ch = arCH[i];

		CString sLine;
		
		//= pCompany->GetTicker() + " - ";
		//if(ch.nDeltaRankN) {
		//	sLine += "Norm Rank: " + itoa(ch.nDeltaRankN); }

		sLine.Format("%s - Norm: %u, Razor: %u, BEV: %u, Price: %.2f%%, EPSNext: %.2f%%, EPSThis: %.2f%%, Debt: %.2f%%\n\n",
			pCompany->GetTicker(),
			ch.nDeltaRankN, ch.nDeltaRankR, ch.nDeltaBEV, ch.fDeltaPrice*100.0, ch.fDeltaEPSNext*100.0, ch.fDeltaEPSThis*100.0, ch.fDeltaDebt*100.0);

		sMessage += sLine;
	}
	if(arCompanies.GetSize() > 7)
	{
		CString sEnd; sEnd.Format("and %u more...\n", arCompanies.GetSize()-7);
		sMessage += sEnd;
	}

	sMessage += "These changes may be caused by companies' fundamental factors, or by errors in data since last upload. Do you want to review them now?";

	EndWaitCursor();

	int nRes = AfxMessageBox(sMessage, MB_ICONQUESTION|MB_YESNO);
	if(nRes == IDYES)
	{
		CViewChanges* pView = NewViewChanges();
		pView->SetData(arCompanies, arCH);

		//for(i=0; i<min(7,arCompanies.GetSize()); i++)
		//	NewViewGeneral(arCompanies[i]);
	}
}

CString CApp::CreateTempDirectory()
{
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

	sDir += "\\Temp\\";
	bRet = ::CreateDirectory(sDir, NULL);
	if(!bRet && ::GetLastError() != ERROR_ALREADY_EXISTS)
		return "";

	return sDir;
}

void CApp::OnAppReportEbitdaLeverage() 
{
	CDialogReportEBITDALeverage dlg;
	if(dlg.DoModal() != IDOK)
		return;

	SReport rep;
	CReport::GetEBITDA_Leverage(&m_Universe, dlg.m_nGroup, dlg.m_bNoIndividualStocks, dlg.m_nSort, dlg.m_sTitle, dlg.m_fMCapMin, dlg.m_fMCapMax, dlg.m_arFilter, rep);

	if(!dlg.m_bExcel)
	{
		CString sDir = theApp.CreateTempDirectory();	
		CString sFile; sFile.Format("Report_EBITDA_%s.html", COleDateTime::GetCurrentTime().Format("%Y_%m_%d"));
		CString sPath = sDir + sFile;

		CreateReportHTML_1(sPath, &rep);
	}
	else
	{
		CString sTime = COleDateTime::GetCurrentTime().Format("%Y_%m_%d");
		CString sFileName;
		sFileName.Format("Report_EBITDA_Leverage_%s.xls", sTime);
		static char szFilters[] ="Excel Files (*.xls)|*.xls|All Files (*.*)|*.*||";
		CFileDialog fileDlg (false, "xls", sFileName, OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY, szFilters, NULL);

		if(fileDlg.DoModal() != IDOK )
			return;

		CreateReportExcel_1(fileDlg.GetPathName(), &rep);
	}
}

void CApp::OnAppMap() 
{
	NewViewMap();	
}

void CApp::OnAppMapsectors() 
{
	NewViewMapSectors();
}


void CApp::OnAppSummaries() 
{
	NewViewSummaries();
}


BOOL CApp::CreateReportHTML_4(const char* szFile, SReport* pRep, SReportFilter& filter)
{
	CStdioFile file;
	if(!file.Open(szFile, CFile::modeCreate|CFile::modeWrite|CFile::typeText))
	{
		CString sMessage; sMessage.Format("Unable to create the file: '%s'", szFile);
		AfxMessageBox(sMessage, MB_ICONINFORMATION);
		return false;
	}

	CString sLine;
	sLine = "<table align='center' cellpadding='0' cellspacing='0' border='1'><tr><td>";
	file.WriteString(sLine);

	sLine.Format("<div align='center' style='font-size: 20px;'><b>%s</b></div>", pRep->sName);
	file.WriteString(sLine);

	sLine.Format("<div align='center' style='font-size: 14px;'>%s</div>", pRep->sSortAndDate);
	file.WriteString(sLine);

	sLine.Format("<div align='center' style='font-size: 16px;'>%s</div>", pRep->sOptCaption);
	file.WriteString(sLine);

	sLine.Format("<div align='center' style='font-size: 16px;'>%s</div>", "Cornerstone Investment Partners, LLC<br><br>");
	file.WriteString(sLine);

	CArray<int,int>	arStyles;
	for(int i=0; i<filter.GetSize(); i++)
		if(filter[i].bInclude)
			arStyles.Add(filter[i].nStyle);

	for(int i=0; i<pRep->arGroups.GetSize(); i++)
	{
		SReportGroup* pGroup = pRep->arGroups[i];
		
		sLine.Format("<div align='center' style='font-family: \"Ms Sans Serif\"; font-size: 13px;'><i>%s</i></div>", pGroup->sGroupName);
		file.WriteString(sLine);

		sLine = "<table width='100%' cellpadding='0' cellspacing='6px' border='0'>";
		file.WriteString(sLine);

		// Names
		sLine = "<tr>";
		for(int i=0; i<pRep->arColNames.GetSize(); i++)
		{
			if(i<1)
				sLine += "<td align='left' style='font-family: \"Ms Sans Serif\"; font-size:11px; padding-left: 3px;'><b>";
			else
				sLine += "<td align='right' style='font-family: \"Ms Sans Serif\"; font-size:11px; padding-left: 3px;'><b>";
			sLine += pRep->arColNames[i];
			sLine += "</b></td>";
		}
		sLine += "</tr>";
		file.WriteString(sLine);

		// Rows
		for(int r=0; r<pGroup->arItems.GetSize(); r++)
		{
			SReportItem* pItem = pGroup->arItems[r];
			sLine = "<tr>";

			sLine += "<td nowrap align='left' style='font-family: \"Ms Sans Serif\"; font-size:11px; padding-left: 3px;'><b>";
			sLine += pItem->sTicker;
			sLine += "</b></td>";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				sLine += "<td nowrap align='right' style='font-family: \"Ms Sans Serif\"; font-size:11px; padding-left: 3px;'>";
				CString s; 
				if(pItem->arData[c] < _INF_)
				{
					int nStyle = arStyles[c];
					if(nStyle == 0)
						s.Format("%.2f", pItem->arData[c]);
					else if(nStyle == 1)
						s.Format("%.0f", pItem->arData[c]);
					else if(nStyle == 2)
						s.Format("%.2f%%", pItem->arData[c]*100);
				}
				else
					s = "n/a";

				sLine += s;
				sLine += "</td>";
			}
			sLine += "</tr>";
			file.WriteString(sLine);
		}

		// Simple Average
		SReportItem* pItem = pGroup->pAve;
		if(pItem)
		{
			sLine = "<tr>";

			sLine += "<td nowrap align='left' style='font-family: \"Ms Sans Serif\"; font-size:11px; padding-left: 3px;'><b>";
			sLine += pItem->sTicker;
			sLine += "</b></td>";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				sLine += "<td nowrap align='right' style='font-family: \"Ms Sans Serif\"; font-size:11px; padding-left: 3px;'><b>";
				CString s; 
				if(pItem->arData[c] < _INF_)
				{
					int nStyle = arStyles[c];
					if(nStyle == 0)
						s.Format("%.2f", pItem->arData[c]);
					else if(nStyle == 1)
						s.Format("%.0f", pItem->arData[c]);
					else if(nStyle == 2)
						s.Format("%.2f%%", pItem->arData[c]*100);
				}
				else
					s = "n/a";

				sLine += s;
				sLine += "</b></td>";
			}
			sLine += "</tr>";
			file.WriteString(sLine);
		}

		// Cap Wgt Average
		pItem = pGroup->pCapAve;
		if(pItem)
		{
			sLine = "<tr>";

			sLine += "<td nowrap align='left' style='font-family: \"Ms Sans Serif\"; font-size:11px; padding-left: 3px;'><b>";
			sLine += pItem->sTicker;
			sLine += "</b></td>";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				sLine += "<td nowrap align='right' style='font-family: \"Ms Sans Serif\"; font-size:11px; padding-left: 3px;'><b>";
				CString s; 
				if(pItem->arData[c] < _INF_)
				{
					int nStyle = arStyles[c];
					if(nStyle == 0)
						s.Format("%.2f", pItem->arData[c]);
					else if(nStyle == 1)
						s.Format("%.0f", pItem->arData[c]);
					else if(nStyle == 2)
						s.Format("%.2f%%", pItem->arData[c]*100);
				}
				else
					s = "n/a";

				sLine += s;
				sLine += "</b></td>";
			}
			sLine += "</tr>";
			file.WriteString(sLine);
		}

		// Price Wgt Average
		pItem = pGroup->pPriceAve;
		if(pItem)
		{
			sLine = "<tr>";

			sLine += "<td nowrap align='left' style='font-family: \"Ms Sans Serif\"; font-size:11px; padding-left: 3px;'><b>";
			sLine += pItem->sTicker;
			sLine += "</b></td>";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				sLine += "<td nowrap align='right' style='font-family: \"Ms Sans Serif\"; font-size:11px; padding-left: 3px;'><b>";
				CString s; 
				if(pItem->arData[c] < _INF_)
				{
					int nStyle = arStyles[c];
					if(nStyle == 0)
						s.Format("%.2f", pItem->arData[c]);
					else if(nStyle == 1)
						s.Format("%.0f", pItem->arData[c]);
					else if(nStyle == 2)
						s.Format("%.2f%%", pItem->arData[c]*100);
				}
				else
					s = "n/a";

				sLine += s;
				sLine += "</b></td>";
			}
			sLine += "</tr>";
			file.WriteString(sLine);
		}

		// END of Row
		sLine = "</table><br><br>";
		file.WriteString(sLine);
	}

	
	sLine = "</td></tr></table>";
	file.WriteString(sLine);
	file.Close();

	//::ShellExecute(m_hWnd, "open", sPath, NULL, NULL, SW_SHOW);
	CViewHTML* pView = NewViewHTML();
	pView->Navigate(szFile);

	return true;
}

BOOL CApp::CreateReportHTML_1(const char* szFile, SReport* pRep)
{
	CStdioFile file;
	if(!file.Open(szFile, CFile::modeCreate|CFile::modeWrite|CFile::typeText))
	{
		CString sMessage; sMessage.Format("Unable to create the file: '%s'", szFile);
		AfxMessageBox(sMessage, MB_ICONINFORMATION);
		return false;
	}

	CString sLine;
	sLine = "<table align='center' cellpadding='0' cellspacing='0' border='1'><tr><td>";
	file.WriteString(sLine);

	sLine.Format("<div align='center' style='font-size: 20px;'><b>%s</b></div>", pRep->sName);
	file.WriteString(sLine);

	sLine.Format("<div align='center' style='font-size: 14px;'>%s</div>", pRep->sSortAndDate);
	file.WriteString(sLine);

	sLine.Format("<div align='center' style='font-size: 16px;'>%s</div>", pRep->sOptCaption);
	file.WriteString(sLine);

	sLine.Format("<div align='center' style='font-size: 16px;'>%s</div>", "Cornerstone Investment Partners, LLC<br><br>");
	file.WriteString(sLine);

	for(int i=0; i<pRep->arGroups.GetSize(); i++)
	{
		SReportGroup* pGroup = pRep->arGroups[i];
		
		sLine.Format("<div align='center' style='font-family: \"Ms Sans Serif\"; font-size: 13px;'><i>%s</i></div>", pGroup->sGroupName);
		file.WriteString(sLine);

		sLine = "<table width='100%' cellpadding='0' cellspacing='6px' border='0'>";
		file.WriteString(sLine);

		// Names
		sLine = "<tr>";
		for(int i=0; i<pRep->arColNames.GetSize(); i++)
		{
			if(i<1)
				sLine += "<td align='left' style='font-family: \"Ms Sans Serif\"; font-size:11px; padding-left: 3px;'><b>";
			else
				sLine += "<td align='right' style='font-family: \"Ms Sans Serif\"; font-size:11px; padding-left: 3px;'><b>";
			sLine += pRep->arColNames[i];
			sLine += "</b></td>";
		}
		sLine += "</tr>";
		file.WriteString(sLine);

		// Rows
		for(int r=0; r<pGroup->arItems.GetSize(); r++)
		{
			SReportItem* pItem = pGroup->arItems[r];
			sLine = "<tr>";

			sLine += "<td nowrap align='left' style='font-family: \"Ms Sans Serif\"; font-size:11px; padding-left: 3px;'><b>";
			sLine += pItem->sTicker;
			sLine += "</b></td>";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				sLine += "<td nowrap align='right' style='font-family: \"Ms Sans Serif\"; font-size:11px; padding-left: 3px;'>";
				CString s; 
				if(pItem->arData[c] < _INF_)
					s.Format("%.2f", pItem->arData[c]);
				else
					s = "n/a";
				sLine += s;
				sLine += "</td>";
			}
			sLine += "</tr>";
			file.WriteString(sLine);
		}

		// Simple Average
		SReportItem* pItem = pGroup->pAve;
		if(pItem)
		{
			sLine = "<tr>";

			sLine += "<td nowrap align='left' style='font-family: \"Ms Sans Serif\"; font-size:11px; padding-left: 3px;'><b>";
			sLine += pItem->sTicker;
			sLine += "</b></td>";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				sLine += "<td nowrap align='right' style='font-family: \"Ms Sans Serif\"; font-size:11px; padding-left: 3px;'><b>";
				CString s; 
				if(pItem->arData[c] < _INF_)
					s.Format("%.2f", pItem->arData[c]);
				else
					s = "n/a";
				sLine += s;
				sLine += "</b></td>";
			}
			sLine += "</tr>";
			file.WriteString(sLine);
		}

		// Cap Wgt Average
		pItem = pGroup->pCapAve;
		if(pItem)
		{
			sLine = "<tr>";

			sLine += "<td nowrap align='left' style='font-family: \"Ms Sans Serif\"; font-size:11px; padding-left: 3px;'><b>";
			sLine += pItem->sTicker;
			sLine += "</b></td>";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				sLine += "<td nowrap align='right' style='font-family: \"Ms Sans Serif\"; font-size:11px; padding-left: 3px;'><b>";
				CString s; 
				if(pItem->arData[c] < _INF_)
					s.Format("%.2f", pItem->arData[c]);
				else
					s = "n/a";
				sLine += s;
				sLine += "</b></td>";
			}
			sLine += "</tr>";
			file.WriteString(sLine);
		}

		// Price Wgt Average
		pItem = pGroup->pPriceAve;
		if(pItem)
		{
			sLine = "<tr>";

			sLine += "<td nowrap align='left' style='font-family: \"Ms Sans Serif\"; font-size:11px; padding-left: 3px;'><b>";
			sLine += pItem->sTicker;
			sLine += "</b></td>";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				sLine += "<td nowrap align='right' style='font-family: \"Ms Sans Serif\"; font-size:11px; padding-left: 3px;'><b>";
				CString s; 
				if(pItem->arData[c] < _INF_)
					s.Format("%.2f", pItem->arData[c]);
				else
					s = "n/a";
				sLine += s;
				sLine += "</b></td>";
			}
			sLine += "</tr>";
			file.WriteString(sLine);
		}

		// END of Row
		sLine = "</table><br><br>";
		file.WriteString(sLine);
	}

	
	sLine = "</td></tr></table>";
	file.WriteString(sLine);
	file.Close();

	//::ShellExecute(m_hWnd, "open", sPath, NULL, NULL, SW_SHOW);
	CViewHTML* pView = NewViewHTML();
	pView->Navigate(szFile);

	return true;
}

BOOL CApp::CreateReportHTML_2(const char* szFile, SReport* pRep)
{
	CStdioFile file;
	if(!file.Open(szFile, CFile::modeCreate|CFile::modeWrite|CFile::typeText))
	{
		CString sMessage; sMessage.Format("Unable to create the file: '%s'", szFile);
		AfxMessageBox(sMessage, MB_ICONINFORMATION);
		return false;
	}

	CString sLine;
	sLine = "<table align='center' cellpadding='0' cellspacing='0' border='1'><tr><td>";
	file.WriteString(sLine);

	sLine.Format("<div align='center' style='font-size: 20px;'><b>%s</b></div>", pRep->sName);
	file.WriteString(sLine);

	sLine.Format("<div align='center' style='font-size: 14px;'>%s</div>", pRep->sSortAndDate);
	file.WriteString(sLine);

	sLine.Format("<div align='center' style='font-size: 16px;'>%s</div>", pRep->sOptCaption);
	file.WriteString(sLine);

	sLine.Format("<div align='center' style='font-size: 16px;'>%s</div>", "Cornerstone Investment Partners, LLC<br><br>");
	file.WriteString(sLine);

	for(int i=0; i<pRep->arGroups.GetSize(); i++)
	{
		SReportGroup* pGroup = pRep->arGroups[i];
		
		sLine.Format("<div align='center' style='font-family: \"Ms Sans Serif\"; font-size: 15px;'><i>%s</i></div>", pGroup->sGroupName);
		file.WriteString(sLine);

		sLine = "<table width='100%' cellpadding='0' cellspacing='6px' border='0'>";
		file.WriteString(sLine);

		// Names
		sLine = "<tr>";
		for(int i=0; i<pRep->arColNames.GetSize(); i++)
		{
			if(i<2)
				sLine += "<td align='left' style='font-family: \"Ms Sans Serif\"; font-size:15px; padding-left: 3px;'><b>";
			else
				sLine += "<td align='right' style='font-family: \"Ms Sans Serif\"; font-size:15px; padding-left: 3px;'><b>";
			sLine += pRep->arColNames[i];
			sLine += "</b></td>";
		}
		sLine += "</tr>";
		file.WriteString(sLine);

		// Rows
		for(int r=0; r<pGroup->arItems.GetSize(); r++)
		{
			SReportItem* pItem = pGroup->arItems[r];
			sLine = "<tr>";

			sLine += "<td nowrap align='left' style='font-family: \"Ms Sans Serif\"; font-size:15px; padding-left: 3px;'><b>";
			sLine += pItem->sTicker;
			sLine += "</b></td>";

			sLine += "<td nowrap align='left' style='font-family: \"Ms Sans Serif\"; font-size:15px; padding-left: 3px;'><b>";
			sLine += pItem->sName;
			sLine += "</b></td>";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				sLine += "<td nowrap align='right' style='font-family: \"Ms Sans Serif\"; font-size:15px; padding-left: 3px;'>";
				CString s; 
				if(pItem->arData[c] < _INF_)
					if(c==3 || c==6 || c==8 || c==10 || c==14)
						s.Format("%.0f", pItem->arData[c]);
					else
						s.Format("%.2f", pItem->arData[c]);
				else
					s = "n/a";
				sLine += s;
				sLine += "</td>";
			}
			sLine += "</tr>";
			file.WriteString(sLine);
		}

		// Simple Average
		SReportItem* pItem = pGroup->pAve;
		if(pItem)
		{
			sLine = "<tr>";

			sLine += "<td nowrap align='left' style='font-family: \"Ms Sans Serif\"; font-size:15px; padding-left: 3px;'><b>";
			sLine += pItem->sTicker;
			sLine += "</b></td>";
			sLine += "<td nowrap align='left' style='font-family: \"Ms Sans Serif\"; font-size:15px; padding-left: 3px;'><b>Average</b></td>";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				sLine += "<td nowrap align='right' style='font-family: \"Ms Sans Serif\"; font-size:15px; padding-left: 3px;'><b>";
				CString s; 
				if(pItem->arData[c] < _INF_)
					if(c==3 || c==6 || c==8 || c==10 || c==14)
						s.Format("%.0f", pItem->arData[c]);
					else
						s.Format("%.2f", pItem->arData[c]);
				else
					s = "n/a";
				sLine += s;
				sLine += "</b></td>";
			}
			sLine += "</tr>";
			file.WriteString(sLine);
		}

		// Cap Wgt Average
		pItem = pGroup->pCapAve;
		if(pItem)
		{
			sLine = "<tr>";

			sLine += "<td nowrap align='left' style='font-family: \"Ms Sans Serif\"; font-size:15px; padding-left: 3px;'><b>";
			sLine += pItem->sTicker;
			sLine += "</b></td>";
			sLine += "<td nowrap align='left' style='font-family: \"Ms Sans Serif\"; font-size:15px; padding-left: 3px;'><b>Cap Weighted</b></td>";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				sLine += "<td nowrap align='right' style='font-family: \"Ms Sans Serif\"; font-size:15px; padding-left: 3px;'><b>";
				CString s; 
				if(pItem->arData[c] < _INF_)
					if(c==3 || c==6 || c==8 || c==10 || c==14)
						s.Format("%.0f", pItem->arData[c]);
					else
						s.Format("%.2f", pItem->arData[c]);
				else
					s = "n/a";
				sLine += s;
				sLine += "</b></td>";
			}
			sLine += "</tr>";
			file.WriteString(sLine);
		}

		// Price Wgt Average
		pItem = pGroup->pPriceAve;
		if(pItem)
		{
			sLine = "<tr>";

			sLine += "<td nowrap align='left' style='font-family: \"Ms Sans Serif\"; font-size:15px; padding-left: 3px;'><b>";
			sLine += pItem->sTicker;
			sLine += "</b></td>";
			sLine += "<td nowrap align='left' style='font-family: \"Ms Sans Serif\"; font-size:15px; padding-left: 3px;'><b>Price Weighted</b></td>";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				sLine += "<td nowrap align='right' style='font-family: \"Ms Sans Serif\"; font-size:15px; padding-left: 3px;'><b>";
				CString s; 
				if(pItem->arData[c] < _INF_)
					if(c==3 || c==6 || c==8 || c==10 || c==14)
						s.Format("%.0f", pItem->arData[c]);
					else
						s.Format("%.2f", pItem->arData[c]);
				else
					s = "n/a";
				sLine += s;
				sLine += "</b></td>";
			}
			sLine += "</tr>";
			file.WriteString(sLine);
		}

		// END of Row
		sLine = "</table><br><br>";
		file.WriteString(sLine);
	}

	
	sLine = "</td></tr></table>";
	file.WriteString(sLine);
	file.Close();

	//::ShellExecute(m_hWnd, "open", sPath, NULL, NULL, SW_SHOW);
	CViewHTML* pView = NewViewHTML();
	pView->Navigate(szFile);

	return true;
}

BOOL CApp::CreateReportExcel_4(const char* szFile, SReport* pRep, SReportFilter& filter)
{
	CStdioFile file;
	if(!file.Open(szFile, CFile::modeCreate|CFile::modeWrite|CFile::typeText))
	{
		CString sMessage; sMessage.Format("Unable to create the file: '%s'", szFile);
		AfxMessageBox(sMessage, MB_ICONINFORMATION);
		return false;
	}

	CString sLine;
	sLine.Format("%s\n", pRep->sName);
	file.WriteString(sLine);

	sLine.Format("%s\n", pRep->sSortAndDate);
	file.WriteString(sLine);

	sLine.Format("%s\n", pRep->sOptCaption);
	file.WriteString(sLine);

	sLine.Format("Cornerstone Investment Partners, LLC\n\n\n");
	file.WriteString(sLine);

	CArray<int,int>	arStyles;
	for(int i=0; i<filter.GetSize(); i++)
		if(filter[i].bInclude)
			arStyles.Add(filter[i].nStyle);

	for(int i=0; i<pRep->arGroups.GetSize(); i++)
	{
		SReportGroup* pGroup = pRep->arGroups[i];
		
		sLine.Format("%s\n", pGroup->sGroupName);
		file.WriteString(sLine);

		// Names
		sLine = "";
		for(int i=0; i<pRep->arColNames.GetSize(); i++)
		{
			sLine += pRep->arColNames[i];
			sLine += "\t";
		}
		sLine += "\n";
		file.WriteString(sLine);

		// Rows
		for(int r=0; r<pGroup->arItems.GetSize(); r++)
		{
			SReportItem* pItem = pGroup->arItems[r];
			sLine = "";

			sLine += pItem->sTicker;
			sLine += "\t";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				CString s; 
				if(pItem->arData[c] < _INF_)
				{
					int nStyle = arStyles[c];
					if(nStyle == 0)
						s.Format("%.2f", pItem->arData[c]);
					else if(nStyle == 1)
						s.Format("%.0f", pItem->arData[c]);
					else if(nStyle == 2)
						s.Format("%.2f%%", pItem->arData[c]*100);
				}
				else
					s = "n/a";

				sLine += s;
				sLine += "\t";
			}
			sLine += "\n";
			file.WriteString(sLine);
		}

		// Simple Average
		SReportItem* pItem = pGroup->pAve;
		if(pItem)
		{
			sLine = "";

			sLine += pItem->sTicker;
			sLine += "\t";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				CString s; 
				if(pItem->arData[c] < _INF_)
				{
					int nStyle = arStyles[c];
					if(nStyle == 0)
						s.Format("%.2f", pItem->arData[c]);
					else if(nStyle == 1)
						s.Format("%.0f", pItem->arData[c]);
					else if(nStyle == 2)
						s.Format("%.2f%%", pItem->arData[c]*100);
				}
				else
					s = "n/a";

				sLine += s;
				sLine += "\t";
			}
			sLine += "\n";
			file.WriteString(sLine);
		}

		// Cap Wgt Average
		pItem = pGroup->pCapAve;
		if(pItem)
		{
			sLine = "";

			sLine += pItem->sTicker;
			sLine += "\t";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				CString s; 
				if(pItem->arData[c] < _INF_)
				{
					int nStyle = arStyles[c];
					if(nStyle == 0)
						s.Format("%.2f", pItem->arData[c]);
					else if(nStyle == 1)
						s.Format("%.0f", pItem->arData[c]);
					else if(nStyle == 2)
						s.Format("%.2f%%", pItem->arData[c]*100);
				}
				else
					s = "n/a";

				sLine += s;
				sLine += "\t";
			}
			sLine += "\n";
			file.WriteString(sLine);
		}

		// Price Wgt Average
		pItem = pGroup->pPriceAve;
		if(pItem)
		{
			sLine = "";

			sLine += pItem->sTicker;
			sLine += "\t";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				CString s; 
				if(pItem->arData[c] < _INF_)
				{
					int nStyle = arStyles[c];
					if(nStyle == 0)
						s.Format("%.2f", pItem->arData[c]);
					else if(nStyle == 1)
						s.Format("%.0f", pItem->arData[c]);
					else if(nStyle == 2)
						s.Format("%.2f%%", pItem->arData[c]*100);
				}
				else
					s = "n/a";

				sLine += s;
				sLine += "\t";
			}
			sLine += "\n\n";
			file.WriteString(sLine);
		}
	}

	file.Close();
	::ShellExecute(AfxGetMainWnd()->m_hWnd, "open", szFile, NULL, NULL, SW_SHOW);
	return true;
}

BOOL CApp::CreateReportExcel_1(const char* szFile, SReport* pRep)
{
	CStdioFile file;
	if(!file.Open(szFile, CFile::modeCreate|CFile::modeWrite|CFile::typeText))
	{
		CString sMessage; sMessage.Format("Unable to create the file: '%s'", szFile);
		AfxMessageBox(sMessage, MB_ICONINFORMATION);
		return false;
	}

	CString sLine;
	sLine.Format("%s\n", pRep->sName);
	file.WriteString(sLine);

	sLine.Format("%s\n", pRep->sSortAndDate);
	file.WriteString(sLine);

	sLine.Format("%s\n", pRep->sOptCaption);
	file.WriteString(sLine);

	sLine.Format("Cornerstone Investment Partners, LLC\n\n\n");
	file.WriteString(sLine);

	for(int i=0; i<pRep->arGroups.GetSize(); i++)
	{
		SReportGroup* pGroup = pRep->arGroups[i];
		
		sLine.Format("%s\n", pGroup->sGroupName);
		file.WriteString(sLine);

		// Names
		sLine = "";
		for(int i=0; i<pRep->arColNames.GetSize(); i++)
		{
			sLine += pRep->arColNames[i];
			sLine += "\t";
		}
		sLine += "\n";
		file.WriteString(sLine);

		// Rows
		for(int r=0; r<pGroup->arItems.GetSize(); r++)
		{
			SReportItem* pItem = pGroup->arItems[r];
			sLine = "";

			sLine += pItem->sTicker;
			sLine += "\t";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				CString s; 
				if(pItem->arData[c] < _INF_)
					s.Format("%.2f", pItem->arData[c]);
				else
					s = "n/a";
				sLine += s;
				sLine += "\t";
			}
			sLine += "\n";
			file.WriteString(sLine);
		}

		// Simple Average
		SReportItem* pItem = pGroup->pAve;
		if(pItem)
		{
			sLine = "";

			sLine += pItem->sTicker;
			sLine += "\t";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				CString s; 
				if(pItem->arData[c] < _INF_)
					s.Format("%.2f", pItem->arData[c]);
				else
					s = "n/a";
				sLine += s;
				sLine += "\t";
			}
			sLine += "\n";
			file.WriteString(sLine);
		}

		// Cap Wgt Average
		pItem = pGroup->pCapAve;
		if(pItem)
		{
			sLine = "";

			sLine += pItem->sTicker;
			sLine += "\t";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				CString s; 
				if(pItem->arData[c] < _INF_)
					s.Format("%.2f", pItem->arData[c]);
				else
					s = "n/a";
				sLine += s;
				sLine += "\t";
			}
			sLine += "\n";
			file.WriteString(sLine);
		}

		// Price Wgt Average
		pItem = pGroup->pPriceAve;
		if(pItem)
		{
			sLine = "";

			sLine += pItem->sTicker;
			sLine += "\t";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				CString s; 
				if(pItem->arData[c] < _INF_)
					s.Format("%.2f", pItem->arData[c]);
				else
					s = "n/a";
				sLine += s;
				sLine += "\t";
			}
			sLine += "\n\n";
			file.WriteString(sLine);
		}
	}

	file.Close();
	::ShellExecute(AfxGetMainWnd()->m_hWnd, "open", szFile, NULL, NULL, SW_SHOW);
	return true;
}

BOOL CApp::CreateReportExcel_2(const char* szFile, SReport* pRep)
{
	CStdioFile file;
	if(!file.Open(szFile, CFile::modeCreate|CFile::modeWrite|CFile::typeText))
	{
		CString sMessage; sMessage.Format("Unable to create the file: '%s'", szFile);
		AfxMessageBox(sMessage, MB_ICONINFORMATION);
		return false;
	}

	CString sLine;
	sLine.Format("%s\n", pRep->sName);
	file.WriteString(sLine);

	sLine.Format("%s\n", pRep->sSortAndDate);
	file.WriteString(sLine);

	sLine.Format("%s\n", pRep->sOptCaption);
	file.WriteString(sLine);

	sLine.Format("Cornerstone Investment Partners, LLC\n\n\n");
	file.WriteString(sLine);

	for(int i=0; i<pRep->arGroups.GetSize(); i++)
	{
		SReportGroup* pGroup = pRep->arGroups[i];
		
		sLine.Format("%s\n", pGroup->sGroupName);
		file.WriteString(sLine);

		// Names
		sLine = "";
		for(int i=0; i<pRep->arColNames.GetSize(); i++)
		{
			sLine += pRep->arColNames[i];
			sLine += "\t";
		}
		sLine += "\n";
		file.WriteString(sLine);

		// Rows
		for(int r=0; r<pGroup->arItems.GetSize(); r++)
		{
			SReportItem* pItem = pGroup->arItems[r];
			sLine = "";

			sLine += pItem->sTicker + "\t";
			sLine += pItem->sName + "\t";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				CString s; 
				if(pItem->arData[c] < _INF_)
					if(c==3 || c==6 || c==8 || c==10 || c==14)
						s.Format("%.0f", pItem->arData[c]);
					else
						s.Format("%.2f", pItem->arData[c]);
				else
					s = "n/a";
				sLine += s;
				sLine += "\t";
			}
			sLine += "\n";
			file.WriteString(sLine);
		}

		// Simple Average
		SReportItem* pItem = pGroup->pAve;
		if(pItem)
		{
			sLine = "";

			sLine += pItem->sTicker;
			sLine += "\t\t";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				CString s; 
				if(pItem->arData[c] < _INF_)
					if(c==3 || c==6 || c==8 || c==10 || c==14)
						s.Format("%.0f", pItem->arData[c]);
					else
						s.Format("%.2f", pItem->arData[c]);
				else
					s = "n/a";
				sLine += s;
				sLine += "\t";
			}
			sLine += "\n";
			file.WriteString(sLine);
		}

		// Cap Wgt Average
		pItem = pGroup->pCapAve;
		if(pItem)
		{
			sLine = "";

			sLine += pItem->sTicker;
			sLine += "\t\t";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				CString s; 
				if(pItem->arData[c] < _INF_)
					if(c==3 || c==6 || c==8 || c==10 || c==14)
						s.Format("%.0f", pItem->arData[c]);
					else
						s.Format("%.2f", pItem->arData[c]);
				else
					s = "n/a";
				sLine += s;
				sLine += "\t";
			}
			sLine += "\n";
			file.WriteString(sLine);
		}

		// Price Wgt Average
		pItem = pGroup->pPriceAve;
		if(pItem)
		{
			sLine = "";

			sLine += pItem->sTicker;
			sLine += "\t\t";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				CString s; 
				if(pItem->arData[c] < _INF_)
					if(c==3 || c==6 || c==8 || c==10 || c==14)
						s.Format("%.0f", pItem->arData[c]);
					else
						s.Format("%.2f", pItem->arData[c]);
				else
					s = "n/a";
				sLine += s;
				sLine += "\t";
			}
			sLine += "\n\n";
			file.WriteString(sLine);
		}
	}

	file.Close();
	::ShellExecute(AfxGetMainWnd()->m_hWnd, "open", szFile, NULL, NULL, SW_SHOW);
	return true;
}

void CApp::OnAppReportEarningsMomentum() 
{
	CDialogEarningsMomentum dlg;
	if(dlg.DoModal() != IDOK)
		return;

	SReport rep;
	CReport::GetEarningsMomentum(&m_Universe, dlg.m_nGroup, dlg.m_bNoIndividualStocks, dlg.m_nSort, dlg.m_sTitle, dlg.m_fMCapMin, dlg.m_fMCapMax, dlg.m_arFilter, rep);

	if(!dlg.m_bExcel)
	{
		CString sDir = theApp.CreateTempDirectory();	
		CString sFile; sFile.Format("Report_EarningsMomentum_%s.html", COleDateTime::GetCurrentTime().Format("%Y_%m_%d"));
		CString sPath = sDir + sFile;

		CreateReportHTML_2(sPath, &rep);
	}
	else
	{
		CString sTime = COleDateTime::GetCurrentTime().Format("%Y_%m_%d");
		CString sFileName;
		sFileName.Format("Report_EarningsMomentum_%s.xls", sTime);
		static char szFilters[] ="Excel Files (*.xls)|*.xls|All Files (*.*)|*.*||";
		CFileDialog fileDlg (false, "xls", sFileName, OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY, szFilters, NULL);

		if(fileDlg.DoModal() != IDOK )
			return;

		CreateReportExcel_2(fileDlg.GetPathName(), &rep);
	}		
}

void CApp::OnAppReportValueMomentum() 
{
	CDialogReportValueMomentum1 dlg;
	if(dlg.DoModal() != IDOK)
		return;

	SReport rep;
	CReport::GetValueMomentum1(&m_Universe, dlg.m_nGroup, dlg.m_bNoIndividualStocks, dlg.m_nSort, dlg.m_sTitle, dlg.m_fMCapMin, dlg.m_fMCapMax, dlg.m_arFilter, rep);

	if(!dlg.m_bExcel)
	{
		CString sDir = theApp.CreateTempDirectory();	
		CString sFile; sFile.Format("Report_ValueMomentum_%s.html", COleDateTime::GetCurrentTime().Format("%Y_%m_%d"));
		CString sPath = sDir + sFile;

		CreateReportHTML_3(sPath, &rep);
	}
	else
	{
		CString sTime = COleDateTime::GetCurrentTime().Format("%Y_%m_%d");
		CString sFileName;
		sFileName.Format("Report_ValueMomentum_%s.xls", sTime);
		static char szFilters[] ="Excel Files (*.xls)|*.xls|All Files (*.*)|*.*||";
		CFileDialog fileDlg (false, "xls", sFileName, OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY, szFilters, NULL);

		if(fileDlg.DoModal() != IDOK )
			return;

		CreateReportExcel_3(fileDlg.GetPathName(), &rep);
	}	
}

BOOL CApp::CreateReportHTML_3(const char* szFile, SReport* pRep)
{
	CStdioFile file;
	if(!file.Open(szFile, CFile::modeCreate|CFile::modeWrite|CFile::typeText))
	{
		CString sMessage; sMessage.Format("Unable to create the file: '%s'", szFile);
		AfxMessageBox(sMessage, MB_ICONINFORMATION);
		return false;
	}

	CString sLine;
	sLine = "<table align='center' cellpadding='0' cellspacing='0' border='1'><tr><td>";
	file.WriteString(sLine);

	sLine.Format("<div align='center' style='font-size: 20px;'><b>%s</b></div>", pRep->sName);
	file.WriteString(sLine);

	sLine.Format("<div align='center' style='font-size: 14px;'>%s</div>", pRep->sSortAndDate);
	file.WriteString(sLine);

	sLine.Format("<div align='center' style='font-size: 16px;'>%s</div>", pRep->sOptCaption);
	file.WriteString(sLine);

	sLine.Format("<div align='center' style='font-size: 16px;'>%s</div>", "Cornerstone Investment Partners, LLC<br><br>");
	file.WriteString(sLine);

	for(int i=0; i<pRep->arGroups.GetSize(); i++)
	{
		SReportGroup* pGroup = pRep->arGroups[i];
		
		sLine.Format("<div align='center' style='font-family: \"Ms Sans Serif\"; font-size: 13px;'><i>%s</i></div>", pGroup->sGroupName);
		file.WriteString(sLine);

		sLine = "<table width='100%' cellpadding='0' cellspacing='6px' border='0'>";
		file.WriteString(sLine);

		// Names
		sLine = "<tr>";
		for(int i=0; i<pRep->arColNames.GetSize(); i++)
		{
			if(i<2)
				sLine += "<td align='left' style='font-family: \"Ms Sans Serif\"; font-size:12px; padding-left: 3px;'><b>";
			else
				sLine += "<td align='right' style='font-family: \"Ms Sans Serif\"; font-size:12px; padding-left: 3px;'><b>";
			sLine += pRep->arColNames[i];
			sLine += "</b></td>";
		}
		sLine += "</tr>";
		file.WriteString(sLine);

		// Rows
		for(int r=0; r<pGroup->arItems.GetSize(); r++)
		{
			SReportItem* pItem = pGroup->arItems[r];
			sLine = "<tr>";

			sLine += "<td nowrap align='left' style='font-family: \"Ms Sans Serif\"; font-size:12px; padding-left: 3px;'><b>";
			sLine += pItem->sTicker;
			sLine += "</b></td>";

			sLine += "<td nowrap align='left' style='font-family: \"Ms Sans Serif\"; font-size:12px; padding-left: 3px;'><b>";
			sLine += pItem->sName;
			sLine += "</b></td>";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				sLine += "<td nowrap align='right' style='font-family: \"Ms Sans Serif\"; font-size:12px; padding-left: 3px;'>";
				CString s; 
				if(pItem->arData[c] < _INF_)
					if(c==3 || c==6 || c==9 || c==12 || c==16)
						s.Format("%.0f", pItem->arData[c]);
					else
						s.Format("%.2f", pItem->arData[c]);
				else
					s = "n/a";
				sLine += s;
				sLine += "</td>";
			}
			sLine += "</tr>";
			file.WriteString(sLine);
		}

		// Simple Average
		SReportItem* pItem = pGroup->pAve;
		if(pItem)
		{
			sLine = "<tr>";

			sLine += "<td nowrap align='left' style='font-family: \"Ms Sans Serif\"; font-size:12px; padding-left: 3px;'><b>";
			sLine += pItem->sTicker;
			sLine += "</b></td>";
			sLine += "<td nowrap align='left' style='font-family: \"Ms Sans Serif\"; font-size:12px; padding-left: 3px;'><b>Average</b></td>";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				sLine += "<td nowrap align='right' style='font-family: \"Ms Sans Serif\"; font-size:12px; padding-left: 3px;'><b>";
				CString s; 
				if(pItem->arData[c] < _INF_)
					if(c==3 || c==6 || c==9 || c==12 || c==16)
						s.Format("%.0f", pItem->arData[c]);
					else
						s.Format("%.2f", pItem->arData[c]);
				else
					s = "n/a";
				sLine += s;
				sLine += "</b></td>";
			}
			sLine += "</tr>";
			file.WriteString(sLine);
		}

		// Cap Wgt Average
		pItem = pGroup->pCapAve;
		if(pItem)
		{
			sLine = "<tr>";

			sLine += "<td nowrap align='left' style='font-family: \"Ms Sans Serif\"; font-size:12px; padding-left: 3px;'><b>";
			sLine += pItem->sTicker;
			sLine += "</b></td>";
			sLine += "<td nowrap align='left' style='font-family: \"Ms Sans Serif\"; font-size:12px; padding-left: 3px;'><b>Cap Weighted</b></td>";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				sLine += "<td nowrap align='right' style='font-family: \"Ms Sans Serif\"; font-size:15px; padding-left: 3px;'><b>";
				CString s; 
				if(pItem->arData[c] < _INF_)
					if(c==3 || c==6 || c==9 || c==12 || c==16)
						s.Format("%.0f", pItem->arData[c]);
					else
						s.Format("%.2f", pItem->arData[c]);
				else
					s = "n/a";
				sLine += s;
				sLine += "</b></td>";
			}
			sLine += "</tr>";
			file.WriteString(sLine);
		}

		// Price Wgt Average
		pItem = pGroup->pPriceAve;
		if(pItem)
		{
			sLine = "<tr>";

			sLine += "<td nowrap align='left' style='font-family: \"Ms Sans Serif\"; font-size:12px; padding-left: 3px;'><b>";
			sLine += pItem->sTicker;
			sLine += "</b></td>";
			sLine += "<td nowrap align='left' style='font-family: \"Ms Sans Serif\"; font-size:12px; padding-left: 3px;'><b>Price Weighted</b></td>";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				sLine += "<td nowrap align='right' style='font-family: \"Ms Sans Serif\"; font-size:12px; padding-left: 3px;'><b>";
				CString s; 
				if(pItem->arData[c] < _INF_)
					if(c==3 || c==6 || c==9 || c==12 || c==16)
						s.Format("%.0f", pItem->arData[c]);
					else
						s.Format("%.2f", pItem->arData[c]);
				else
					s = "n/a";
				sLine += s;
				sLine += "</b></td>";
			}
			sLine += "</tr>";
			file.WriteString(sLine);
		}

		// END of Row
		sLine = "</table><br><br>";
		file.WriteString(sLine);
	}

	
	sLine = "</td></tr></table>";
	file.WriteString(sLine);
	file.Close();

	//::ShellExecute(m_hWnd, "open", sPath, NULL, NULL, SW_SHOW);
	CViewHTML* pView = NewViewHTML();
	pView->Navigate(szFile);

	return true;
}

BOOL CApp::CreateReportExcel_3(const char* szFile, SReport* pRep)
{
	CStdioFile file;
	if(!file.Open(szFile, CFile::modeCreate|CFile::modeWrite|CFile::typeText))
	{
		CString sMessage; sMessage.Format("Unable to create the file: '%s'", szFile);
		AfxMessageBox(sMessage, MB_ICONINFORMATION);
		return false;
	}

	CString sLine;
	sLine.Format("%s\n", pRep->sName);
	file.WriteString(sLine);

	sLine.Format("%s\n", pRep->sSortAndDate);
	file.WriteString(sLine);

	sLine.Format("%s\n", pRep->sOptCaption);
	file.WriteString(sLine);

	sLine.Format("Cornerstone Investment Partners, LLC\n\n\n");
	file.WriteString(sLine);

	for(int i=0; i<pRep->arGroups.GetSize(); i++)
	{
		SReportGroup* pGroup = pRep->arGroups[i];
		
		sLine.Format("%s\n", pGroup->sGroupName);
		file.WriteString(sLine);

		// Names
		sLine = "";
		for(int i=0; i<pRep->arColNames.GetSize(); i++)
		{
			sLine += pRep->arColNames[i];
			sLine += "\t";
		}
		sLine += "\n";
		file.WriteString(sLine);

		// Rows
		for(int r=0; r<pGroup->arItems.GetSize(); r++)
		{
			SReportItem* pItem = pGroup->arItems[r];
			sLine = "";

			sLine += pItem->sTicker + "\t";
			sLine += pItem->sName + "\t";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				CString s; 
				if(pItem->arData[c] < _INF_)
					if(c==3 || c==6 || c==9 || c==12 || c==16)
						s.Format("%.0f", pItem->arData[c]);
					else
						s.Format("%.2f", pItem->arData[c]);
				else
					s = "n/a";
				sLine += s;
				sLine += "\t";
			}
			sLine += "\n";
			file.WriteString(sLine);
		}

		// Simple Average
		SReportItem* pItem = pGroup->pAve;
		if(pItem)
		{
			sLine = "";

			sLine += pItem->sTicker;
			sLine += "\t\t";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				CString s; 
				if(pItem->arData[c] < _INF_)
					if(c==3 || c==6 || c==9 || c==12 || c==16)
						s.Format("%.0f", pItem->arData[c]);
					else
						s.Format("%.2f", pItem->arData[c]);
				else
					s = "n/a";
				sLine += s;
				sLine += "\t";
			}
			sLine += "\n";
			file.WriteString(sLine);
		}

		// Cap Wgt Average
		pItem = pGroup->pCapAve;
		if(pItem)
		{
			sLine = "";

			sLine += pItem->sTicker;
			sLine += "\t\t";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				CString s; 
				if(pItem->arData[c] < _INF_)
					if(c==3 || c==6 || c==9 || c==12 || c==16)
						s.Format("%.0f", pItem->arData[c]);
					else
						s.Format("%.2f", pItem->arData[c]);
				else
					s = "n/a";
				sLine += s;
				sLine += "\t";
			}
			sLine += "\n";
			file.WriteString(sLine);
		}

		// Price Wgt Average
		pItem = pGroup->pPriceAve;
		if(pItem)
		{
			sLine = "";

			sLine += pItem->sTicker;
			sLine += "\t\t";
			
			for(int c=0; c<pItem->arData.GetSize(); c++)
			{
				CString s; 
				if(pItem->arData[c] < _INF_)
					if(c==3 || c==6 || c==9 || c==12 || c==16)
						s.Format("%.0f", pItem->arData[c]);
					else
						s.Format("%.2f", pItem->arData[c]);
				else
					s = "n/a";
				sLine += s;
				sLine += "\t";
			}
			sLine += "\n\n";
			file.WriteString(sLine);
		}
	}

	file.Close();
	::ShellExecute(AfxGetMainWnd()->m_hWnd, "open", szFile, NULL, NULL, SW_SHOW);
	return true;
}

void CApp::OnAppReportGeneral() 
{
	CDialogReportGeneral dlg(&m_RepFilter);
	if(dlg.DoModal() != IDOK)
		return;


	SReport rep;
	CReport::GetGeneral(&m_Universe, dlg.m_nGroup, dlg.m_bNoIndividualStocks, dlg.m_nSort, dlg.m_sTitle, dlg.m_fMCapMin, dlg.m_fMCapMax, dlg.m_arFilter, m_RepFilter, rep);

	if(!dlg.m_bExcel)
	{
		CString sDir = theApp.CreateTempDirectory();	
		CString sFile; sFile.Format("Report_EBITDA_%s.html", COleDateTime::GetCurrentTime().Format("%Y_%m_%d"));
		CString sPath = sDir + sFile;

		CreateReportHTML_4(sPath, &rep, m_RepFilter);
	}
	else
	{
		CString sTime = COleDateTime::GetCurrentTime().Format("%Y_%m_%d");
		CString sFileName;
		sFileName.Format("Report_General_%s.xls", sTime);
		static char szFilters[] ="Excel Files (*.xls)|*.xls|All Files (*.*)|*.*||";
		CFileDialog fileDlg (false, "xls", sFileName, OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY, szFilters, NULL);

		if(fileDlg.DoModal() != IDOK )
			return;

		CreateReportExcel_4(fileDlg.GetPathName(), &rep, m_RepFilter);
	}
}

void CApp::OnConnectServers() 
{
	const char szKey[] = "HKEY_CURRENT_USER\\Software\\Cornerstone\\Fair Value Model\\Server Connections\\";
	CDialogConnections dlg;
	if(dlg.DoModal() == IDOK)
	{
		WriteProfileString("Server Connections", "server1", dlg.m_sServer1);
		WriteProfileString("Server Connections", "server2", dlg.m_sServer2);
		WriteProfileString("Server Connections", "server3", dlg.m_sServer3);

		WriteProfileString("Server Connections", "login1", dlg.m_sLogin1);
		WriteProfileString("Server Connections", "login2", dlg.m_sLogin2);
		WriteProfileString("Server Connections", "login3", dlg.m_sLogin3);

		WriteProfileString("Server Connections", "psw1", dlg.m_sPassw1);
		WriteProfileString("Server Connections", "psw2", dlg.m_sPassw2);
		WriteProfileString("Server Connections", "psw3", dlg.m_sPassw3);

		CString sDb; sDb.Format("%u", dlg.m_nDatabase);
		WriteProfileString("Server Connections", "db", sDb);
	}
}


void CApp::OnAppSelectUniverse() 
{
	// select universe
	CDialogSelectUniverse dlg;
	dlg.m_nSel = m_nUniverse-1;
	if(dlg.DoModal() != IDOK)
		return;
	m_nUniverse = dlg.m_nSel+1;	

	BeginWaitCursor();

	m_Universe.LoadAll(m_nUniverse, m_nCurrentYear, m_Universe.GetDate());
	m_Universe.CalcAll();
	m_pDialogStocks->SetupData();

	CMainFrame* pMainFrame = (CMainFrame*)m_pMainWnd;
	CWnd* pMDI = CWnd::FromHandle(pMainFrame->m_hWndMDIClient);

	CWnd* pChild = pMDI->GetWindow(GW_CHILD);
	while(pChild)
	{
		if(pChild->IsKindOf(RUNTIME_CLASS(CMDIChildWnd)))
		{
			CMDIChildWnd* pChildFrame = (CMDIChildWnd*)pChild;
			pChild = pChild->GetWindow(GW_HWNDNEXT);
			pChildFrame->MDIDestroy();

			/*CView* pView = pChildFrame->GetActiveView();
			if(pView->IsKindOf(RUNTIME_CLASS(CViewGeneral)))
			{
				CViewGeneral* pMainView = (CViewGeneral*)pView;
				pMainView->SetCompany(pMainView->m_pCompany);
				pMainView->Invalidate();
			}*/

			
		}
		else
			pChild = pChild->GetWindow(GW_HWNDNEXT);
	}

	EndWaitCursor();
}


void CApp::OnAppChangePassword() 
{
	CDialogChangePassword dlg;
	dlg.DoModal();
}

void CApp::OnAppInvprocess() 
{
	CDialogInvProcess dlg;
	dlg.DoModal();
}
