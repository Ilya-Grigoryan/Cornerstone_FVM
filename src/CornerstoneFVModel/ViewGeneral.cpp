// ViewGeneral.cpp : implementation of the CViewGeneral class
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"

#include "Doc.h"
#include "ViewGeneral.h"
#include "WndGrid.h"
#include "FrameGeneral.h"
#include "History.h"
#include "ViewCharts.h"
#include "ViewMeetings.h"

#include "DialogLogin.h"
#include "DialogSensAnalysis.h"
#include "DialogLastReportYear.h"
#include "DialogDate.h"
#include "DialogEpsGraph.h"
#include "DialogSplit.h"
#include "Dialog2Minute.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

#define CLR_READONLY		RGB(220,220,220)
#define FONT_CLR_READONLY	RGB(130,130,130)

extern CApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CViewGeneral

IMPLEMENT_DYNCREATE(CViewGeneral, CFormView)

BEGIN_MESSAGE_MAP(CViewGeneral, CFormView)
	//{{AFX_MSG_MAP(CViewGeneral)
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_COMMAND(ID_GENERAL_CALC, OnGeneralCalc)
	ON_UPDATE_COMMAND_UI(ID_GENERAL_CALC, OnUpdateGeneralCalc)
	ON_COMMAND(ID_GENERAL_SAVE, OnGeneralSave)
	ON_UPDATE_COMMAND_UI(ID_GENERAL_SAVE, OnUpdateGeneralSave)
	ON_COMMAND(ID_GENERAL_SPLIT, OnGeneralSplit)
	ON_UPDATE_COMMAND_UI(ID_GENERAL_SPLIT, OnUpdateGeneralSplit)
	ON_COMMAND(ID_GENERAL_EXCEL, OnGeneralExcel)
	ON_UPDATE_COMMAND_UI(ID_GENERAL_EXCEL, OnUpdateGeneralExcel)
	ON_COMMAND(ID_GENERAL_WORD, OnGeneralWord)
	ON_UPDATE_COMMAND_UI(ID_GENERAL_WORD, OnUpdateGeneralWord)
	ON_COMMAND(ID_GENERAL_GRAPH, OnGeneralGraph)
	ON_UPDATE_COMMAND_UI(ID_GENERAL_GRAPH, OnUpdateGeneralGraph)
	ON_COMMAND(ID_GENERAL_OPEN, OnGeneralOpen)
	ON_UPDATE_COMMAND_UI(ID_GENERAL_OPEN, OnUpdateGeneralOpen)
	ON_COMMAND(ID_GENERAL_SAVETODB, OnGeneralSavetodb)
	ON_UPDATE_COMMAND_UI(ID_GENERAL_SAVETODB, OnUpdateGeneralSavetodb)
	ON_COMMAND(ID_GENERAL_SENS_ANALYSIS, OnGeneralSensAnalysis)
	ON_UPDATE_COMMAND_UI(ID_GENERAL_SENS_ANALYSIS, OnUpdateGeneralSensAnalysis)
	ON_COMMAND(ID_GENERAL_MONTECARLO, OnGeneralMontecarlo)
	ON_UPDATE_COMMAND_UI(ID_GENERAL_MONTECARLO, OnUpdateGeneralMontecarlo)
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, OnSelchangeComboType)
	ON_COMMAND(ID_GENERAL_LAST_EPS_YEAR, OnGeneralLastEpsYear)
	ON_BN_CLICKED(IDC_CHECK_DOWNLOAD_BS, OnCheckDownloadBs)
	ON_BN_CLICKED(IDC_CHECK_DOWNLOAD_EPS, OnCheckDownloadEps)
	ON_BN_CLICKED(IDC_CHECK_USE_DEBT_ADJUST, OnCheckUseDebtAdjust)
	ON_COMMAND(ID_GENERAL_HISTORY, OnGeneralHistory)
	ON_COMMAND(ID_GENERAL_ADD_TO_HISTORY, OnGeneralAddToHistory)
	ON_COMMAND(ID_GENERAL_DATE, OnGeneralDate)
	ON_COMMAND(ID_GENERAL_EPS, OnGeneralEps)
	ON_BN_CLICKED(IDC_CHECK_DOWNLOAD_ROCROE, OnCheckDownloadRocroe)
	ON_COMMAND(ID_GENERAL_MEETINGS, OnGeneralMeetings)
	ON_COMMAND(ID_GENERAL_2MINUTE, OnGeneral2minute)
	//}}AFX_MSG_MAP
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, CFormView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, CFormView::OnFilePrintPreview)
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewGeneral construction/destruction

CViewGeneral::CViewGeneral()
	: CFormView(CViewGeneral::IDD)
{
	//{{AFX_DATA_INIT(CViewGeneral)
	//}}AFX_DATA_INIT
	m_pCompany = NULL;

	m_pGridEps = NULL;
	m_pGridYrs = NULL;
	m_pGridBS = NULL;
	m_pGridInput2 = NULL;
	m_pGridRatios = NULL;
	m_pGridRatios2 = NULL;
	m_pGridRanks = NULL;
	m_pGridRanksHeader = NULL;

	m_pViewPercentActual = NULL;
	m_pViewPercentFore = NULL;
	m_pViewPercentRazor = NULL;
	m_pViewPercentRazorFore = NULL;

	m_bChanged = false;
}

CViewGeneral::~CViewGeneral()
{
}

void CViewGeneral::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewGeneral)
	DDX_Control(pDX, IDC_CHECK_DOWNLOAD_ROCROE, m_checkLoadROCROE);
	DDX_Control(pDX, IDC_COMBO_TYPE, m_comboType);
	DDX_Control(pDX, IDC_CHECK_DOWNLOAD_BS, m_checkLoadBS);
	DDX_Control(pDX, IDC_CHECK_DOWNLOAD_EPS, m_checkLoadEPS);
	DDX_Control(pDX, IDC_STATIC_DATE, m_stDate);
	DDX_Control(pDX, IDC_CHECK_USE_DEBT_ADJUST, m_checkUseDebtAdjust);
	DDX_Control(pDX, IDC_STATIC_CAPTION, m_stCaption);
	//}}AFX_DATA_MAP
}

BOOL CViewGeneral::PreCreateWindow(CREATESTRUCT& cs)
{
	return CFormView::PreCreateWindow(cs);
}

void CViewGeneral::OnInitialUpdate()
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();
	SetScrollSizes(MM_TEXT, CSize(0,0));

	LOGFONT lf;
	memset(&lf,0,sizeof(LOGFONT));
	strcpy(lf.lfFaceName,"Ms Sans Serif");
	lf.lfHeight = (long)13;
	lf.lfWeight = 400;
	m_font.CreateFontIndirect(&lf);

	lf.lfHeight = 13;
	lf.lfWeight = 600;
	m_font2.CreateFontIndirect(&lf);

	lf.lfHeight = 13;
	lf.lfWeight = 600;
	m_font3.CreateFontIndirect(&lf);

	// Use Debt Adjustment is true by default
	m_checkUseDebtAdjust.SetCheck(1);

	// EPS Grid
	m_pGridEps = new CWndGrid();
	m_pGridEps->SetEnableScrollBars(false);
	m_pGridEps->SetDefaultColumnSize(44);
	m_pGridEps->SetDefaultRowSize(20);
	m_pGridEps->SetTextAlign(DT_RIGHT);

	m_pGridEps->Create(this);

	m_pGridEps->SetRowCount(6);
	m_pGridEps->SetColumnCount(13);

	m_pGridEps->SetFixedColumnCount(1);
	m_pGridEps->SetFixedRowCount(1);

	m_pGridEps->SetItemText(1, 0, "ROC:");
	m_pGridEps->SetItemText(2, 0, "ROE:");
	m_pGridEps->SetItemText(3, 0, "EPS:");
	m_pGridEps->SetItemText(4, 0, "GAAP:");
	m_pGridEps->SetItemText(5, 0, "Shares:");
	m_pGridEps->SetItemText(0, 12, "Mean");

	m_pGridEps->SetFixedCellsBkColor(RGB(200,200,200));
	//m_pGridEps->SetFixedCellsForeColor(RGB(255,255,255));
	m_pGridEps->SetColumnBkColor(10, RGB(212,247,107));
	m_pGridEps->SetColumnBkColor(11, RGB(255,203,224));
	m_pGridEps->SetColumnBkColor(12, RGB(163,227,244));

	m_pGridEps->SetRowBkColor(4, RGB(252,250,253));
	for(int i=1; i<11; i++)
		m_pGridEps->SetItemText(4, i, "n/a");

	m_pGridEps->SetRowBkColor(5, RGB(230,250,250));
	for(int i=1; i<11; i++)
		m_pGridEps->SetItemText(5, i, "n/a");

	for(int y=2; y<=3; y++)
		for(int x=1; x<=3; x++)
			m_pGridEps->SetItemBkColor(y,x,CLR_READONLY);

	m_pGridEps->SetCellsFont(m_font);

	m_pGridEps->SetDefaultColorGridFixed(RGB(255,255,255));
	m_pGridEps->SetDefaultColorGridNonFixed(RGB(255,255,255));

	// Years small grid
	m_pGridYrs = new CWndGrid();
	m_pGridYrs->SetEnableScrollBars(false);
	m_pGridYrs->SetDefaultColumnSize(66);
	m_pGridYrs->SetDefaultRowSize(20);
	m_pGridYrs->SetTextAlign(DT_CENTER);

	m_pGridYrs->Create(this);

	m_pGridYrs->SetRowCount(4);
	m_pGridYrs->SetColumnCount(2);

	m_pGridYrs->SetFixedColumnCount(0);
	m_pGridYrs->SetFixedRowCount(1);
	m_pGridYrs->SetFixedCellsBkColor(RGB(200,200,200));

	m_pGridYrs->SetItemText(0, 0, "Yrs For M.");
	m_pGridYrs->SetItemText(0, 1, "10Yrs. M.");
	
	m_pGridYrs->SetColumnAllowEdit(0,false);
	m_pGridYrs->SetColumnAllowEdit(1,false);
	m_pGridYrs->SetColumnForeColor(0, FONT_CLR_READONLY);
	m_pGridYrs->SetColumnForeColor(1, FONT_CLR_READONLY);
	m_pGridYrs->SetCellsFont(m_font);

	m_pGridYrs->SetDefaultColorGridFixed(RGB(255,255,255));
	m_pGridYrs->SetDefaultColorGridNonFixed(RGB(255,255,255));

	// Balance Sheet Grid
	m_pGridBS = new CWndGrid();
	m_pGridBS->SetEnableScrollBars(false);
	m_pGridBS->SetDefaultColumnSize(60);
	m_pGridBS->SetDefaultRowSize(20);
	m_pGridBS->SetTextAlign(DT_RIGHT);

	m_pGridBS->Create(this);
	m_pGridBS->SetRowCount(16);
	m_pGridBS->SetColumnCount(2);

	m_pGridBS->SetFixedColumnCount(0);
	m_pGridBS->SetFixedRowCount(0);
	m_pGridBS->SetColumnSize(0, 90);

	m_pGridBS->SetItemText(0, 0, "Assets:");
	m_pGridBS->SetItemText(1, 0, "Net Plant:");
	m_pGridBS->SetItemText(2, 0, "Intangibles:");
	m_pGridBS->SetItemText(3, 0, "Cash:");
	m_pGridBS->SetItemText(4, 0, "Liabilities:");
	m_pGridBS->SetItemText(5, 0, "Long Term Debt:");
	m_pGridBS->SetItemText(6, 0, "Short Term Debt:");
	m_pGridBS->SetItemText(7, 0, "Total Debt:");
	m_pGridBS->SetItemText(8, 0, "Common Equity:");
	m_pGridBS->SetItemText(9, 0, "Preferred Stock:");
	m_pGridBS->SetItemText(10, 0, "Net Worth:");
	m_pGridBS->SetItemText(11, 0, "Debt Ratio:");
	m_pGridBS->SetItemText(12, 0, "Pre-Tax Income:");
	m_pGridBS->SetItemText(13, 0, "Interest Expense:");
	m_pGridBS->SetItemText(14, 0, "Depr && Amort:");
	m_pGridBS->SetItemText(15, 0, "Shares Out:");

	m_pGridBS->SetItemAllowEdit(7,1,false);
	m_pGridBS->SetRowForeColor(7,FONT_CLR_READONLY);
	m_pGridBS->SetItemAllowEdit(9,1,false);
	m_pGridBS->SetRowForeColor(9,FONT_CLR_READONLY);
	m_pGridBS->SetItemAllowEdit(10,1,false);
	m_pGridBS->SetRowForeColor(10,FONT_CLR_READONLY);
	m_pGridBS->SetItemAllowEdit(11,1,false);
	m_pGridBS->SetRowForeColor(11,FONT_CLR_READONLY);

	m_pGridBS->SetFixedCellsBkColor(RGB(255,255,255));
	m_pGridBS->SetCellsFont(m_font);

	m_pGridBS->SetDefaultColorGridFixed(RGB(255,255,255));
	m_pGridBS->SetDefaultColorGridNonFixed(RGB(255,255,255));

	// Inputs 2 (Price, Dividend, etc...)
	m_pGridInput2 = new CWndGrid();
	m_pGridInput2->SetEnableScrollBars(false);
	m_pGridInput2->SetDefaultColumnSize(72);
	m_pGridInput2->SetDefaultRowSize(20);
	m_pGridInput2->SetTextAlign(DT_RIGHT);

	m_pGridInput2->Create(this);
	m_pGridInput2->SetRowCount(12);
	m_pGridInput2->SetColumnCount(2);

	m_pGridInput2->SetFixedColumnCount(0);
	m_pGridInput2->SetFixedRowCount(0);
	m_pGridInput2->SetColumnSize(0, 90);

	m_pGridInput2->SetItemText(0, 0, "Price:");
	m_pGridInput2->SetItemText(1, 0, "Dividend:");
	m_pGridInput2->SetItemText(2, 0, "Asset Life:");
	m_pGridInput2->SetItemText(3, 0, "Book Value:");
	m_pGridInput2->SetItemText(4, 0, "Dilution:");
	m_pGridInput2->SetItemText(5, 0, "Mean ROI:");
	m_pGridInput2->SetItemText(6, 0, "EPS Adjustment:");
	m_pGridInput2->SetItemText(7, 0, "EPS Next Yr:");
	m_pGridInput2->SetItemText(8, 0, "EPS This Yr:");
	m_pGridInput2->SetItemText(9, 0, "EPS Last Yr:");
	m_pGridInput2->SetItemText(10, 0, "Book EPS:");
	m_pGridInput2->SetItemText(11, 0, "Norm. EPS:");

	m_pGridInput2->SetFixedCellsBkColor(RGB(200,200,200));
	
	m_pGridInput2->SetItemAllowEdit(10, 1,false);
	m_pGridInput2->SetRowForeColor(10,FONT_CLR_READONLY);
	m_pGridInput2->SetItemAllowEdit(11, 1,false);
	m_pGridInput2->SetRowForeColor(11,FONT_CLR_READONLY);

	m_pGridInput2->SetCellsFont(m_font);
	m_pGridInput2->SetItemFont(0,1,m_font2);

	m_pGridInput2->SetItemAllowEdit(5, 1,false);
	m_pGridInput2->SetItemForeColor(5,1, RGB(0,0,150));
	m_pGridInput2->SetItemFont(5,1,m_font2);
	m_pGridInput2->SetItemFont(5,0,m_font3);

	m_pGridInput2->SetDefaultColorGridFixed(RGB(255,255,255));
	m_pGridInput2->SetDefaultColorGridNonFixed(RGB(255,255,255));

	// Ratios (Yield, Multiple, etc...)
	m_pGridRatios = new CWndGrid();
	m_pGridRatios->SetEnableScrollBars(false);
	m_pGridRatios->SetDefaultColumnSize(72);
	m_pGridRatios->SetDefaultRowSize(20);
	m_pGridRatios->SetTextAlign(DT_RIGHT);

	m_pGridRatios->Create(this);
	m_pGridRatios->SetRowCount(12);
	m_pGridRatios->SetColumnCount(2);

	m_pGridRatios->SetFixedColumnCount(0);
	m_pGridRatios->SetFixedRowCount(0);
	m_pGridRatios->SetColumnSize(0, 110);

	m_pGridRatios->SetItemText(0, 0, "Risk Adjustment:");
	m_pGridRatios->SetItemText(1, 0, "Yield:");
	m_pGridRatios->SetItemText(2, 0, "Multiple:");
	m_pGridRatios->SetItemText(3, 0, "P/E:");
	m_pGridRatios->SetItemText(4, 0, "EV/EBITDA:");
	m_pGridRatios->SetItemText(5, 0, "Reinvest Rate:");
	m_pGridRatios->SetItemText(6, 0, "Max Reinvest Rate:");
	m_pGridRatios->SetItemText(7, 0, "Adjust For Payout:");
	m_pGridRatios->SetItemText(8, 0, "Growth Rate:");
	m_pGridRatios->SetItemText(9, 0, "Adj. Growth Rate:");
	m_pGridRatios->SetItemText(10, 0, "Oper. Fixed Cover:");
	m_pGridRatios->SetItemText(11, 0, "Interest Coverage:");

	m_pGridRatios->SetColumnForeColor(0,FONT_CLR_READONLY);
	m_pGridRatios->SetColumnForeColor(1,FONT_CLR_READONLY);
	m_pGridRatios->SetColumnAllowEdit(0, false);
	m_pGridRatios->SetColumnAllowEdit(1, false);
	m_pGridRatios->SetCellsFont(m_font);

	m_pGridRatios->SetItemForeColor(2,0, RGB(0,0,0));
	m_pGridRatios->SetItemForeColor(2,1, RGB(0,0,150));
	m_pGridRatios->SetItemForeColor(9,0, RGB(0,0,0));
	m_pGridRatios->SetItemForeColor(9,1, RGB(0,0,150));
	m_pGridRatios->SetItemFont(2,1,m_font2);
	m_pGridRatios->SetItemFont(9,1,m_font2);
	m_pGridRatios->SetItemFont(2,0,m_font3);
	m_pGridRatios->SetItemFont(9,0,m_font3);

	m_pGridRatios->SetDefaultColorGridFixed(RGB(255,255,255));
	m_pGridRatios->SetDefaultColorGridNonFixed(RGB(255,255,255));

	// Ratios2 (Adjusted Assets/Share, Net Chash Flow/Share, etc)
	m_pGridRatios2 = new CWndGrid();
	m_pGridRatios2->SetEnableScrollBars(false);
	m_pGridRatios2->SetDefaultColumnSize(67);
	m_pGridRatios2->SetDefaultRowSize(20);
	m_pGridRatios2->SetTextAlign(DT_RIGHT);

	m_pGridRatios2->Create(this);
	m_pGridRatios2->SetRowCount(12);
	m_pGridRatios2->SetColumnCount(2);

	m_pGridRatios2->SetFixedColumnCount(0);
	m_pGridRatios2->SetFixedRowCount(0);
	m_pGridRatios2->SetColumnSize(0, 120);

	m_pGridRatios2->SetItemText(0, 0, "Adjusted Asstes/Share:");
	m_pGridRatios2->SetItemText(1, 0, "Net Cash Flow/Share:");
	m_pGridRatios2->SetItemText(2, 0, "Depr && Amort/Share:");
	m_pGridRatios2->SetItemText(3, 0, "BEV:");
	m_pGridRatios2->SetItemText(4, 0, "BEV Rank:");
	m_pGridRatios2->SetItemText(5, 0, "Price/BEV:");
	m_pGridRatios2->SetItemText(6, 0, "Historical EPS Growth:");
	m_pGridRatios2->SetItemText(7, 0, "Indicated EPS Growth:");
	m_pGridRatios2->SetItemText(8, 0, "1 Yr Estimated Growth:");
	m_pGridRatios2->SetItemText(9, 0, "2 Yr Estimated Growth:");
	m_pGridRatios2->SetItemText(10, 0, "1 Yr Growth Rank:");
	m_pGridRatios2->SetItemText(11, 0, "2 Yr Growth Rank:");
	
	m_pGridRatios2->SetColumnForeColor(0,FONT_CLR_READONLY);
	m_pGridRatios2->SetColumnForeColor(1,FONT_CLR_READONLY);
	m_pGridRatios2->SetColumnAllowEdit(1, false);
	m_pGridRatios2->SetCellsFont(m_font);

	m_pGridRatios2->SetDefaultColorGridFixed(RGB(255,255,255));
	m_pGridRatios2->SetDefaultColorGridNonFixed(RGB(255,255,255));

	// Rankings Header
	m_pGridRanksHeader = new CWndGrid();
	m_pGridRanksHeader->SetEnableScrollBars(false);
	m_pGridRanksHeader->SetDefaultColumnSize(126);
	m_pGridRanksHeader->SetDefaultRowSize(20);
	m_pGridRanksHeader->SetTextAlign(DT_CENTER);

	m_pGridRanksHeader->Create(this);

	m_pGridRanksHeader->SetRowCount(1);
	m_pGridRanksHeader->SetColumnCount(8);

	m_pGridRanksHeader->SetFixedColumnCount(1);
	m_pGridRanksHeader->SetFixedRowCount(1);
	m_pGridRanksHeader->SetColumnSize(0, 42);
	m_pGridRanksHeader->SetColumnSize(2, 3);
	m_pGridRanksHeader->SetColumnSize(4, 3);
	m_pGridRanksHeader->SetColumnSize(6, 3);

	m_pGridRanksHeader->SetItemText(0, 1, "Normalized");
	m_pGridRanksHeader->SetItemText(0, 3, "Forecast");
	m_pGridRanksHeader->SetItemText(0, 5, "Razor");
	m_pGridRanksHeader->SetItemText(0, 7, "Razor Forecast");

	m_pGridRanksHeader->SetFixedCellsBkColor(RGB(200,200,200));
	m_pGridRanksHeader->SetCellsFont(m_font);

	m_pGridRanksHeader->SetDefaultColorGridFixed(RGB(255,255,255));
	m_pGridRanksHeader->SetDefaultColorGridNonFixed(RGB(255,255,255));

	// Rankings
	m_pGridRanks = new CWndGrid();
	m_pGridRanks->SetEnableScrollBars(false);
	m_pGridRanks->SetDefaultColumnSize(42);
	m_pGridRanks->SetDefaultRowSize(20);
	m_pGridRanks->SetTextAlign(DT_CENTER);

	m_pGridRanks->Create(this);

	m_pGridRanks->SetRowCount(4);
	m_pGridRanks->SetColumnCount(16);

	m_pGridRanks->SetFixedColumnCount(1);
	m_pGridRanks->SetFixedRowCount(1);

	m_pGridRanks->SetColumnSize(4, 3);
	m_pGridRanks->SetColumnSize(8, 3);
	m_pGridRanks->SetColumnSize(12, 3);
	m_pGridRanks->SetColumnBkColor(4, RGB(200,200,200));
	m_pGridRanks->SetColumnBkColor(8, RGB(200,200,200));
	m_pGridRanks->SetColumnBkColor(12, RGB(200,200,200));

	m_pGridRanks->SetItemText(1, 0, "EPV:");
	m_pGridRanks->SetItemText(2, 0, "RAV:");
	m_pGridRanks->SetItemText(3, 0, "PMV:");

	m_pGridRanks->SetItemText(0, 1, "Value");
	m_pGridRanks->SetItemText(0, 5, "Value");
	m_pGridRanks->SetItemText(0, 9, "Value");
	m_pGridRanks->SetItemText(0, 13, "Value");
	m_pGridRanks->SetItemText(0, 2, "Rank");
	m_pGridRanks->SetItemText(0, 6, "Rank");
	m_pGridRanks->SetItemText(0, 10, "Rank");
	m_pGridRanks->SetItemText(0, 14, "Rank");
	m_pGridRanks->SetItemText(0, 3, "P/Val");
	m_pGridRanks->SetItemText(0, 7, "P/Val");
	m_pGridRanks->SetItemText(0, 11, "P/Val");
	m_pGridRanks->SetItemText(0, 15, "P/Val");

	m_pGridRanks->SetFixedCellsBkColor(RGB(200,200,200));
	//m_pGridRanks->SetFixedCellsForeColor(RGB(255,255,255));

	m_pGridRanks->SetItemBkColor(1, 1, RGB(202,237,97));
	m_pGridRanks->SetColumnForeColor(2, RGB(0,0,150));
	//m_pGridRanks->SetColumnBkColor(3, RGB(193,247,254));

	m_pGridRanks->SetItemBkColor(1, 5, RGB(255,193,214));
	m_pGridRanks->SetColumnForeColor(6, RGB(0,0,150));
	//m_pGridRanks->SetColumnBkColor(6, RGB(193,247,254));

	m_pGridRanks->SetItemBkColor(1, 9, RGB(153,217,234));
	m_pGridRanks->SetColumnForeColor(10, RGB(0,0,150));
	//m_pGridRanks->SetColumnBkColor(9, RGB(193,247,254));

	m_pGridRanks->SetItemBkColor(1, 13, RGB(255,242,30));
	m_pGridRanks->SetColumnForeColor(14, RGB(0,0,150));
	//m_pGridRanks->SetColumnBkColor(12, RGB(193,247,254));

	m_pGridRanks->SetCellsFont(m_font);
	
	m_pGridRanks->SetDefaultColorGridFixed(RGB(255,255,255));
	m_pGridRanks->SetDefaultColorGridNonFixed(RGB(255,255,255));

	// Create percentile views
	m_pViewPercentActual = new CViewPercentile;
	m_pViewPercentActual->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0,0,1,1), this, 12213);
	m_pViewPercentActual->ShowWindow(SW_HIDE);

	m_pViewPercentFore = new CViewPercentile;
	m_pViewPercentFore->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0,0,1,1), this, 12213);
	m_pViewPercentFore->ShowWindow(SW_HIDE);

	m_pViewPercentRazor = new CViewPercentile;
	m_pViewPercentRazor->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0,0,1,1), this, 12213);
	m_pViewPercentRazor->ShowWindow(SW_HIDE);

	m_pViewPercentRazorFore = new CViewPercentile;
	m_pViewPercentRazorFore->Create(NULL, NULL, WS_CHILD|WS_VISIBLE, CRect(0,0,1,1), this, 12213);
	m_pViewPercentRazorFore->ShowWindow(SW_HIDE);
	
	Resize();
}

void CViewGeneral::SetBSNamesFromType(int nType)
{
	CString s;
	if(nType == 1) // Banks
	{
		m_pGridBS->SetItemText(0, 0, "Equity:");
		m_pGridBS->SetItemText(1, 0, "Deductions:");
		m_pGridBS->SetItemText(4, 0, "Non-Perfm Ins:");

		m_pGridBS->SetItemText(3, 1, "0.00");		// Cash is 0
		m_pGridBS->SetItemText(6, 1, "0.00");		// Short Term Debt is 0
		m_pGridBS->SetItemText(14, 1, "0.00");		// Depr and Amort is 0

		s.Format("%.0f", m_pCompany->GetNetPlant());
		m_pGridBS->SetItemText(1, 1, s);
		s.Format("%.0f", m_pCompany->GetLiabilities());
		m_pGridBS->SetItemText(4, 1, s);
		s.Format("%.0f", m_pCompany->GetNetWorth()); // Assets = NetWorth
		m_pGridBS->SetItemText(0, 1, s);
		
	} 
	else if(nType == 2 || nType == 4 || nType == 5) // Insurance, Homebuilders and Diverse Fins
	{
		m_pGridBS->SetItemText(0, 0, "Equity:");
		m_pGridBS->SetItemText(1, 0, "Deductions:");
		m_pGridBS->SetItemText(4, 0, "Non-Perfm Ins:");

		m_pGridBS->SetItemText(1, 1, "0.00");		// Deductions (Net Plant) is 0
		m_pGridBS->SetItemText(4, 1, "0.00");		// Non-prf (Liabilities) is 0
		m_pGridBS->SetItemText(14, 1, "0.00");		// Depr and Amort is 0

		s.Format("%.0f", m_pCompany->GetCash());
		m_pGridBS->SetItemText(3, 1, s);
		s.Format("%.0f", m_pCompany->GetShortTermDebt());
		m_pGridBS->SetItemText(6, 1, s);
		s.Format("%.0f", m_pCompany->GetNetWorth()); // Assets = NetWorth
		m_pGridBS->SetItemText(0, 1, s);
	}
	else
	{
		m_pGridBS->SetItemText(0, 0, "Assets:");
		m_pGridBS->SetItemText(1, 0, "Net Plant:");
		m_pGridBS->SetItemText(4, 0, "Liabilities:");

		s.Format("%.0f", m_pCompany->GetCash());
		m_pGridBS->SetItemText(3, 1, s);
		s.Format("%.0f", m_pCompany->GetShortTermDebt());
		m_pGridBS->SetItemText(6, 1, s);
		s.Format("%.0f", m_pCompany->GetDeprAndAmort());
		m_pGridBS->SetItemText(14, 1, s);
		s.Format("%.0f", m_pCompany->GetNetPlant());
		m_pGridBS->SetItemText(1, 1, s);
		s.Format("%.0f", m_pCompany->GetLiabilities());
		m_pGridBS->SetItemText(4, 1, s);
		s.Format("%.0f", m_pCompany->GetAssets());
		m_pGridBS->SetItemText(0, 1, s);
	}
	m_pGridBS->Invalidate();
}

void CViewGeneral::Resize()
{
	CRect rc;
	GetClientRect(rc);

	if(m_stCaption)
		m_stCaption.MoveWindow(10, 8, rc.Width()-20, 18);

	if(m_pGridEps)
		m_pGridEps->MoveWindow(10, 32, 13*44, 6*20);

	if(m_pGridYrs)
		m_pGridYrs->MoveWindow(10+13*44+10, 32, 133, 6*20);

	if(m_pGridBS)
		m_pGridBS->MoveWindow(10, 60+4*18+10+20, 90+60+1, 16*20);

	if(m_pGridInput2)
		m_pGridInput2->MoveWindow(10+90+60+1 + 10, 60+4*18+10+20, 90+72+1, 12*20);

	if(m_pGridRatios)
		m_pGridRatios->MoveWindow(10+90+60+90+72+10+2 + 10, 60+4*18+10+20, 110+72+1, 12*20);

	if(m_pGridRatios2)
		m_pGridRatios2->MoveWindow(10+90+60+90+72+10+110+72+10+3 + 10, 60+4*18+10+20, 120+67+1, 12*20);

	if(m_pGridRanksHeader)
		m_pGridRanksHeader->MoveWindow(10+90+60+1 + 10, 60+4*18+10+12*20+10+20, 4*126+42+3*3, 18);

	if(m_pGridRanks)
		m_pGridRanks->MoveWindow(10+90+60+1 + 10, 60+4*18+10+12*20+10+18+20, 13*42+3*3 , 4*20);

	///////////////////////////////////////////////////////////////////////////////////////
	//CWnd* pFrm1 = this->GetDlgItem(IDC_STATIC_FRM1);
	//if(pFrm1 && pFrm1->m_hWnd)
	//	pFrm1->MoveWindow(10, 40+4*18+10+15*20+5, 90+60+2, 44);

	//if(m_checkUseDebtAdjust.m_hWnd)
	//	m_checkUseDebtAdjust.MoveWindow(10+10, 40+4*18+10+15*20+5+13, 90+50, 25);

	///////////////////////////////////////////////////////////////////////////////////////
	CWnd* pFrm2 = this->GetDlgItem(IDC_STATIC_FRM2);
	if(pFrm2 && pFrm2->m_hWnd)
		pFrm2->MoveWindow(10, 60+4*18+10+16*20+20, 90+60+2, 98+30);

	///////////////////////////////////////////////////////////////////////////////////////
	if(m_pViewPercentActual)
		m_pViewPercentActual->MoveWindow(10+90+60+1 + 10, 40+4*18+10+15*20+5+44+5+20, 278, 46);

	if(m_pViewPercentFore)
		m_pViewPercentFore->MoveWindow(10+90+60+1 + 10, 40+4*18+10+15*20+5+44+5+46+20, 278, 46);

	if(m_pViewPercentRazor)
		m_pViewPercentRazor->MoveWindow(10+90+60+1 + 10 + 278, 40+4*18+10+15*20+5+44+5+20, 277 , 46);

	if(m_pViewPercentRazorFore)
		m_pViewPercentRazorFore->MoveWindow(10+90+60+1 + 10 + 278, 40+4*18+10+15*20+5+44+5+46+20, 277, 46);
}

/////////////////////////////////////////////////////////////////////////////
// CViewGeneral printing

void CViewGeneral::OnPrepareDC(CDC* pDC, CPrintInfo* pInfo) 
{
	if(pDC->IsPrinting()) // Is the DC a printer DC.
	{
		CRect rect;
		GetClientRect (&rect);

		int oldMapMode = pDC->SetMapMode(MM_ISOTROPIC);
		CSize ptOldWinExt = pDC->SetWindowExt(1000, 1000);
		ASSERT( ptOldWinExt.cx != 0 && ptOldWinExt.cy != 0 );
		CSize ptOldViewportExt = pDC->SetViewportExt(rect.Width(),
		  -rect.Height());
		ASSERT( ptOldViewportExt.cx != 0 && ptOldViewportExt.cy != 0 );
		//      CPoint ptOldOrigin = pDC->SetViewportOrg(rect.Width()/2,
		//         rect.Height()/2);
	}
	
	CFormView::OnPrepareDC(pDC, pInfo);
}

BOOL CViewGeneral::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CViewGeneral::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CViewGeneral::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
}

void CViewGeneral::OnPrint(CDC* pDC, CPrintInfo* pInfo)
{
	//pDC->SetMapMode(MM_TWIPS);

	::SelectObject(pDC->m_hDC, m_font);

	pDC->Rectangle(	pInfo->m_rectDraw.left+100, pInfo->m_rectDraw.top+100, 
					pInfo->m_rectDraw.right-100, pInfo->m_rectDraw.bottom-100);

	CString str = "Cornerstone Investment Partners \n Fair Valuation Model Version 1.0.0.1";
	CRect rc(pInfo->m_rectDraw);
	int nH = pDC->DrawText(str, &rc, DT_CALCRECT);

	rc.OffsetRect(0,-200);
	pDC->DrawText(str, &rc, DT_TABSTOP|DT_CALCRECT);
	pDC->MoveTo(rc.left+100, rc.top+nH);
	pDC->LineTo(rc.right-100, rc.top+nH);

	//pDC->TextOut(pInfo->m_rectDraw.left+200, pInfo->m_rectDraw.top+200, "Cornerstone Investment Partners \n Fair Valuation Model Version 1.0.0.1");

/*	CRect rc;
	GetClientRect(rc);

	CDC dcMem;
	dcMem.CreateCompatibleDC(NULL);
	CBitmap bm;
	bm.CreateCompatibleBitmap(GetDC(), rc.right, rc.bottom);
	dcMem.SelectObject(&bm);

//	m_pGridEps->Draw(&dcMem, rc);

	pDC->StretchBlt(pInfo->m_rectDraw.left,pInfo->m_rectDraw.top, pInfo->m_rectDraw.Width(),pInfo->m_rectDraw.Height(), 
				&dcMem, 0,0, rc.Width(), rc.Height(), SRCCOPY);
*/	


}

/////////////////////////////////////////////////////////////////////////////
// CViewGeneral diagnostics

#ifdef _DEBUG
void CViewGeneral::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewGeneral::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}

CDoc* CViewGeneral::GetDocument() // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CDoc)));
	return (CDoc*)m_pDocument;
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewGeneral message handlers

void CViewGeneral::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	Resize();		
}

void CViewGeneral::UpdateCompany()
{
	if(!m_pCompany)
		return;

	int i;
	double f;
	CString s;

	// Balance sheet
	s = m_pGridBS->GetItemText(0,1);
	if(s == "" || s == "-" || s == "n/a")	f = _INF_;	else	f = atof(s);
	m_pCompany->SetAssets(f);

	s = m_pGridBS->GetItemText(1,1);
	if(s == "" || s == "-" || s == "n/a")	f = _INF_;	else	f = atof(s);
	m_pCompany->SetNetPlant(f);

	s = m_pGridBS->GetItemText(2,1);
	if(s == "" || s == "-" || s == "n/a")	f = _INF_;	else	f = atof(s);
	m_pCompany->SetIntangibles(f);

	s = m_pGridBS->GetItemText(3,1);
	if(s == "" || s == "-" || s == "n/a")	f = _INF_;	else	f = atof(s);
	m_pCompany->SetCash(f);

	s = m_pGridBS->GetItemText(4,1);
	if(s == "" || s == "-" || s == "n/a")	f = _INF_;	else	f = atof(s);
	m_pCompany->SetLiabilities(f);

	s = m_pGridBS->GetItemText(5,1);
	if(s == "" || s == "-" || s == "n/a")	f = _INF_;	else	f = atof(s);
	m_pCompany->SetLongTermDebt(f);

	s = m_pGridBS->GetItemText(6,1);
	if(s == "" || s == "-" || s == "n/a")	f = _INF_;	else	f = atof(s);
	m_pCompany->SetShortTermDebt(f);

	s = m_pGridBS->GetItemText(8,1);
	if(s == "" || s == "-" || s == "n/a")	f = _INF_;	else	f = atof(s);
	m_pCompany->SetCommonEquity(f);

	s = m_pGridBS->GetItemText(12,1);
	if(s == "" || s == "-" || s == "n/a")	f = _INF_;	else	f = atof(s);
	m_pCompany->SetPreTaxIncome(f);

	s = m_pGridBS->GetItemText(13,1);
	if(s == "" || s == "-" || s == "n/a")	f = _INF_;	else	f = atof(s);
	m_pCompany->SetInterestExpense(f);

	s = m_pGridBS->GetItemText(14,1);
	if(s == "" || s == "-" || s == "n/a")	f = _INF_;	else	f = atof(s);
	m_pCompany->SetDeprAndAmort(f);

	s = m_pGridBS->GetItemText(15,1);
	if(s == "" || s == "-" || s == "n/a")	f = _INF_;	else	f = atof(s);
	m_pCompany->SetSharesOut(f);

	m_pCompany->SetType(m_comboType.GetCurSel());
	m_pCompany->SetLoadBS(m_checkLoadBS.GetCheck());
	m_pCompany->SetLoadEPS(m_checkLoadEPS.GetCheck());

	// Inputs2
	s = m_pGridInput2->GetItemText(0,1);
	if(s.GetLength() > 0 && s[0]=='$')	s.Delete(0);
	if(s == "" || s == "-" || s == "n/a")	f = _INF_;	else	f = atof(s);
	m_pCompany->SetPrice(f);

	s = m_pGridInput2->GetItemText(1,1);
	if(s == "" || s == "-" || s == "n/a")	f = _INF_;	else	f = atof(s);
	m_pCompany->SetDividend(f);

	s = m_pGridInput2->GetItemText(2,1);
	if(s == "" || s == "-" || s == "n/a")	f = _INF_;	else	f = atof(s);
	m_pCompany->SetAssetLife(f);

	s = m_pGridInput2->GetItemText(3,1);
	if(s == "" || s == "-" || s == "n/a")	f = _INF_;	else	f = atof(s);
	m_pCompany->SetBookValue(f);

	s = m_pGridInput2->GetItemText(4,1);
	if(s == "" || s == "-" || s == "n/a")	f = _INF_;	else	f = atof(s);
	m_pCompany->SetDilution(f);

	s = m_pGridInput2->GetItemText(6,1);
	if(s == "" || s == "-" || s == "n/a")
	{
		m_pCompany->SetEPSAdjustAdd(0);
		m_pCompany->SetEPSAdjustMultiply(1);
	}
	else if(s[0]=='*')
	{	
		m_pCompany->SetEPSAdjustAdd(0);
		s.Delete(0);
		f = atof(s);
		m_pCompany->SetEPSAdjustMultiply(f);
	} 
	else if(s[0] == '+' || s[0] == '-')
	{
		m_pCompany->SetEPSAdjustMultiply(1);
		f = atof(s);
		m_pCompany->SetEPSAdjustAdd(f);
	}
	else
	{
		m_pCompany->SetEPSAdjustAdd(0);
		m_pCompany->SetEPSAdjustMultiply(1);
	}

	s = m_pGridInput2->GetItemText(7,1);
	if(s == "" || s == "-" || s == "n/a")	f = _INF_;	else	f = atof(s);
	m_pCompany->SetEPSForecastNextYear(f);

	s = m_pGridInput2->GetItemText(8,1);
	if(s == "" || s == "-" || s == "n/a")	f = _INF_;	else	f = atof(s);
	m_pCompany->SetEPSForecastThisYear(f);

	s = m_pGridInput2->GetItemText(9,1);
	if(s == "" || s == "-" || s == "n/a")	f = _INF_;	else	f = atof(s);
	m_pCompany->SetEPSForecastLastYear(f);

	// ROC History
	double arRoc[11];
	for(i=0; i<11; i++)
	{
		s = m_pGridEps->GetItemText(1, i+1);
		if(s == "" || s == "-" || s == "n/a")
			f = _INF_;
		else
			f = atof(s);
		arRoc[i] = f;
	}
	m_pCompany->SetROCHistory(arRoc, 11);

	// ROE History
	double arRoe[11];
	for(i=0; i<11; i++)
	{
		s = m_pGridEps->GetItemText(2, i+1);
		if(s == "" || s == "-" || s == "n/a")
			f = _INF_;
		else
			f = atof(s);
		arRoe[i] = f;
	}
	m_pCompany->SetROEHistory(arRoe, 11);

	// EPS History
	double arEps[11];
	for(i=0; i<11; i++)
	{
		s = m_pGridEps->GetItemText(3, i+1);
		if(s == "" || s == "-" || s == "n/a")
			f = _INF_;
		else
			f = atof(s);
		arEps[i] = f;
	}
	m_pCompany->SetEPSHistory(arEps, 11);


	m_pCompany->SetUseDebtAdjust((BOOL)m_checkUseDebtAdjust.GetCheck());
	m_pCompany->SetLoadBS((BOOL)m_checkLoadBS.GetCheck());
	m_pCompany->SetLoadEPS((BOOL)m_checkLoadEPS.GetCheck());
	m_pCompany->SetLoadROCROE((BOOL)m_checkLoadROCROE.GetCheck());

	// set flag to false
	m_bChanged = false;
}

void CViewGeneral::SetCompany(CCompany* pCompany)
{
	m_pCompany = pCompany;

	// caption
	CString sCaption;
	sCaption.Format("[%s] %s, %s, %s", pCompany->GetTicker(), pCompany->GetName(), pCompany->GetIndustry(), pCompany->GetSector());
	m_stCaption.SetWindowText(sCaption);

	// setup ROC, ROE, EPS
	FillEpsRocRoe();

	// Balance Sheet
	FillBalanceSheet();

	// Input 2
	FillInput2();

	// Ratios
	FillRatios();
	FillRatios2();

	// Fill Ranks
	FillRanks();

	// set up the percentile graphs
	m_pViewPercentActual->SetName("EPV Actual Percentile");
	m_pViewPercentActual->SetPercentile( m_pCompany->GetRankEPV() / (double)m_pCompany->GetUniverse()->GetSize() );

	m_pViewPercentFore->SetName("EPV Forecast Percentile");
	m_pViewPercentFore->SetPercentile( m_pCompany->GetRankEPVFore() / (double)m_pCompany->GetUniverse()->GetSize() );

	m_pViewPercentRazor->SetName("EPV Razor Percentile");
	m_pViewPercentRazor->SetPercentile( m_pCompany->GetRankEPVRazor() / (double)m_pCompany->GetUniverse()->GetSize() );

	m_pViewPercentRazorFore->SetName("EPV Razor Forecast Percentile");
	m_pViewPercentRazorFore->SetPercentile( m_pCompany->GetRankEPVRazorFore() / (double)m_pCompany->GetUniverse()->GetSize() );

	// Price date
	CString sDate = "Reported Date: " + m_pCompany->GetReportDateForPriceAndReturn().Format("%m/%d/%Y");
	m_stDate.SetWindowText(sDate);

	// Use debt adjustment
	m_checkUseDebtAdjust.SetCheck(m_pCompany->GetUseDebtAdjust() ? 1 : 0);

	// Download BS and EPS
	m_checkLoadBS.SetCheck(m_pCompany->GetLoadBS() ? 1 : 0);
	m_checkLoadEPS.SetCheck(m_pCompany->GetLoadEPS() ? 1 : 0);

	m_bChanged = false;
}

void CViewGeneral::FillRanks()
{
	if(!m_pCompany)
		return;

	CString s;

	s.Format("%.2f", m_pCompany->GetEPV()); 
	m_pGridRanks->SetItemText(1,1,s);

	s.Format("%.2f", m_pCompany->GetRAV());
	m_pGridRanks->SetItemText(2,1,s);

	s.Format("%.2f", m_pCompany->GetPMV());
	m_pGridRanks->SetItemText(3,1,s);

	s.Format("%u", m_pCompany->GetRankEPV());
	m_pGridRanks->SetItemText(1,2,s);

	s.Format("%u", m_pCompany->GetRankRAV());
	m_pGridRanks->SetItemText(2,2,s);

	s.Format("%u", m_pCompany->GetRankPMV());
	m_pGridRanks->SetItemText(3,2,s);

	s.Format("%.2f", m_pCompany->GetPriceOverEPV());
	m_pGridRanks->SetItemText(1,3,s);

	s.Format("%.2f", m_pCompany->GetPriceOverRAV());
	m_pGridRanks->SetItemText(2,3,s);

	s.Format("%.2f", m_pCompany->GetPriceOverPMV());
	m_pGridRanks->SetItemText(3,3,s);

	///////////////////////////////////////
	s.Format("%.2f", m_pCompany->GetEPVFore()); 
	m_pGridRanks->SetItemText(1,5,s);

	s.Format("%.2f", m_pCompany->GetRAVFore());
	m_pGridRanks->SetItemText(2,5,s);

	s.Format("%.2f", m_pCompany->GetPMVFore());
	m_pGridRanks->SetItemText(3,5,s);

	s.Format("%u", m_pCompany->GetRankEPVFore());
	m_pGridRanks->SetItemText(1,6,s);

	s.Format("%u", m_pCompany->GetRankRAVFore());
	m_pGridRanks->SetItemText(2,6,s);

	s.Format("%u", m_pCompany->GetRankPMVFore());
	m_pGridRanks->SetItemText(3,6,s);

	s.Format("%.2f", m_pCompany->GetPriceOverEPVFore());
	m_pGridRanks->SetItemText(1,7,s);

	s.Format("%.2f", m_pCompany->GetPriceOverRAVFore());
	m_pGridRanks->SetItemText(2,7,s);

	s.Format("%.2f", m_pCompany->GetPriceOverPMVFore());
	m_pGridRanks->SetItemText(3,7,s);

	///////////////////////////////////////
	s.Format("%.2f", m_pCompany->GetEPVRazor()); 
	m_pGridRanks->SetItemText(1,9,s);

	s.Format("%.2f", m_pCompany->GetRAVRazor());
	m_pGridRanks->SetItemText(2,9,s);

	s.Format("%.2f", m_pCompany->GetPMVRazor());
	m_pGridRanks->SetItemText(3,9,s);

	s.Format("%u", m_pCompany->GetRankEPVRazor());
	m_pGridRanks->SetItemText(1,10,s);

	s.Format("%u", m_pCompany->GetRankRAVRazor());
	m_pGridRanks->SetItemText(2,10,s);

	s.Format("%u", m_pCompany->GetRankPMVRazor());
	m_pGridRanks->SetItemText(3,10,s);

	s.Format("%.2f", m_pCompany->GetPriceOverEPVRazor());
	m_pGridRanks->SetItemText(1,11,s);

	s.Format("%.2f", m_pCompany->GetPriceOverRAVRazor());
	m_pGridRanks->SetItemText(2,11,s);

	s.Format("%.2f", m_pCompany->GetPriceOverPMVRazor());
	m_pGridRanks->SetItemText(3,11,s);

		///////////////////////////////////////
	s.Format("%.2f", m_pCompany->GetEPVRazorFore()); 
	m_pGridRanks->SetItemText(1,13,s);

	s.Format("%.2f", m_pCompany->GetRAVRazorFore());
	m_pGridRanks->SetItemText(2,13,s);

	s.Format("%.2f", m_pCompany->GetPMVRazorFore());
	m_pGridRanks->SetItemText(3,13,s);

	s.Format("%u", m_pCompany->GetRankEPVRazorFore());
	m_pGridRanks->SetItemText(1,14,s);

	s.Format("%u", m_pCompany->GetRankRAVRazorFore());
	m_pGridRanks->SetItemText(2,14,s);

	s.Format("%u", m_pCompany->GetRankPMVRazorFore());
	m_pGridRanks->SetItemText(3,14,s);

	s.Format("%.2f", m_pCompany->GetPriceOverEPVRazorFore());
	m_pGridRanks->SetItemText(1,15,s);

	s.Format("%.2f", m_pCompany->GetPriceOverRAVRazorFore());
	m_pGridRanks->SetItemText(2,15,s);

	s.Format("%.2f", m_pCompany->GetPriceOverPMVRazorFore());
	m_pGridRanks->SetItemText(3,15,s);
}

void CViewGeneral::FillRatios2()
{
	if(!m_pCompany)
		return;

	CString s;

	s.Format("%.2f", m_pCompany->GetAdjustedAssetsOverShare());
	m_pGridRatios2->SetItemText(0, 1, s);

	s.Format("%.2f", m_pCompany->GetNetCashFlowOverShare());
	m_pGridRatios2->SetItemText(1, 1, s);

	s.Format("%.2f", m_pCompany->GetDeprAndAmortOverShare());
	m_pGridRatios2->SetItemText(2, 1, s);

	s.Format("%.2f", m_pCompany->GetBreakEvenValue());
	m_pGridRatios2->SetItemText(3, 1, s);

	s.Format("%u", m_pCompany->GetRankBEV());
	m_pGridRatios2->SetItemText(4, 1, s);

	s.Format("%.2f", m_pCompany->GetPriceOverBEV());
	m_pGridRatios2->SetItemText(5, 1, s);

	s.Format("%.2f%%", m_pCompany->GetHistoricalGrowth()*100);
	m_pGridRatios2->SetItemText(6, 1, s);

	m_pGridRatios2->SetItemText(7, 1, "n/a");

	s.Format("%.2f%%", m_pCompany->GetGrowthEstimate1()*100);
	m_pGridRatios2->SetItemText(8, 1, s);

	s.Format("%.2f%%", m_pCompany->GetGrowthEstimate2()*100);
	m_pGridRatios2->SetItemText(9, 1, s);

	s.Format("%u", m_pCompany->GetRankGrowth1());
	m_pGridRatios2->SetItemText(10, 1, s);

	s.Format("%u", m_pCompany->GetRankGrowth2());
	m_pGridRatios2->SetItemText(11, 1, s);
}

void CViewGeneral::FillRatios()
{
	if(!m_pCompany)
		return;

	CString s;

	s.Format("%.2f", m_pCompany->GetRiskAdjustment());
	m_pGridRatios->SetItemText(0, 1, s);

	s.Format("%.2f%%", m_pCompany->GetYield()*100.0);
	m_pGridRatios->SetItemText(1, 1, s);

	s.Format("%.2f", m_pCompany->GetMultiple()*100.0);
	m_pGridRatios->SetItemText(2, 1, s);

	s.Format("%.2f", m_pCompany->GetPE());
	m_pGridRatios->SetItemText(3, 1, s);

	s.Format("%.2f", m_pCompany->GetEVOverEBITDA());
	m_pGridRatios->SetItemText(4, 1, s);

	s.Format("%.2f", m_pCompany->GetReinvestmentRate());
	m_pGridRatios->SetItemText(5, 1, s);

	s.Format("%.2f", m_pCompany->GetMaxReinvestmentRate());
	m_pGridRatios->SetItemText(6, 1, s);

	s.Format("%.2f", m_pCompany->GetAdjustmentForPayout()*100);
	m_pGridRatios->SetItemText(7, 1, s);

	s.Format("%.2f%%", m_pCompany->GetGrowthRate()*100);
	m_pGridRatios->SetItemText(8, 1, s);

	s.Format("%.2f%%", m_pCompany->GetAdjustedGrowthRate()*100);
	m_pGridRatios->SetItemText(9, 1, s);

	s.Format("%.2f", m_pCompany->GetOperFixChargeCover());
	m_pGridRatios->SetItemText(10, 1, s);

	s.Format("%.2f", m_pCompany->GetInterestCover());
	m_pGridRatios->SetItemText(11, 1, s);
}

void CViewGeneral::FillInput2()
{
	if(!m_pCompany)
		return;
	
	CString sDate = m_pCompany->GetReportDateForPriceAndReturn().Format("%m/%d/%Y");
	CString sAdjust;
	if(m_pCompany->GetEPSAdjustAdd() != 0.0)
	{
		if(m_pCompany->GetEPSAdjustAdd() > 0.0)
			sAdjust.Format("+%.2f", m_pCompany->GetEPSAdjustAdd());
		else
			sAdjust.Format("%.2f", m_pCompany->GetEPSAdjustAdd());
	}
	else if(m_pCompany->GetEPSAdjustMultiply() != 1.0)
		sAdjust.Format("*%.2f", m_pCompany->GetEPSAdjustMultiply());
	else
		sAdjust = "0.00";

	CString s;
	
	s.Format("$%.2f", m_pCompany->GetPrice());
	m_pGridInput2->SetItemText(0, 1, s);
	
	s.Format("%.2f", m_pCompany->GetDividend());
	m_pGridInput2->SetItemText(1, 1, s);
	
	s.Format("%.2f", m_pCompany->GetAssetLife());
	m_pGridInput2->SetItemText(2, 1, s);
	
	s.Format("%.2f", m_pCompany->GetBookValue());
	m_pGridInput2->SetItemText(3, 1, s);
	
	m_pGridInput2->SetItemText(4, 1, "1.0");

	s.Format("%.2f%%", m_pCompany->GetMeanROI()*100);
	m_pGridInput2->SetItemText(5, 1, s);
	
	m_pGridInput2->SetItemText(6, 1, sAdjust);
	
	s.Format("%.2f", m_pCompany->GetEPSForecastNextYear());
	m_pGridInput2->SetItemText(7, 1, s);
	
	s.Format("%.2f", m_pCompany->GetEPSForecastThisYear());
	m_pGridInput2->SetItemText(8, 1, s);
	
	s.Format("%.2f", m_pCompany->GetEPSForecastLastYear());
	m_pGridInput2->SetItemText(9, 1, s);

	s.Format("%.2f", m_pCompany->GetBookEPS());
	m_pGridInput2->SetItemText(10, 1, s);

	s.Format("%.2f", m_pCompany->GetNormEPS());
	m_pGridInput2->SetItemText(11, 1, s);

}

void CViewGeneral::FillBalanceSheet()
{
	if(!m_pCompany)
		return;

	CString s;

	s.Format("%.0f", m_pCompany->GetAssets());
	m_pGridBS->SetItemText(0, 1, s);
	s.Format("%.0f", m_pCompany->GetNetPlant());
	m_pGridBS->SetItemText(1, 1, s);
	s.Format("%.0f", m_pCompany->GetIntangibles());
	m_pGridBS->SetItemText(2, 1, s);
	s.Format("%.0f", m_pCompany->GetCash());
	m_pGridBS->SetItemText(3, 1, s);
	s.Format("%.0f", m_pCompany->GetLiabilities());
	m_pGridBS->SetItemText(4, 1, s);
	s.Format("%.0f", m_pCompany->GetLongTermDebt());
	m_pGridBS->SetItemText(5, 1, s);
	s.Format("%.0f", m_pCompany->GetShortTermDebt());
	m_pGridBS->SetItemText(6, 1, s);
	s.Format("%.0f", m_pCompany->GetTotalDebt());
	m_pGridBS->SetItemText(7, 1, s);
	s.Format("%.0f", m_pCompany->GetCommonEquity());
	m_pGridBS->SetItemText(8, 1, s);
	s.Format("%.0f", m_pCompany->GetPreferred());
	m_pGridBS->SetItemText(9, 1, s);
	s.Format("%.0f", m_pCompany->GetNetWorth());
	m_pGridBS->SetItemText(10, 1, s);
	s.Format("%.2f", m_pCompany->GetDebtRatio());
	m_pGridBS->SetItemText(11, 1, s);
	s.Format("%.0f", m_pCompany->GetPreTaxIncome());
	m_pGridBS->SetItemText(12, 1, s);
	s.Format("%.0f", m_pCompany->GetInterestExpense());
	m_pGridBS->SetItemText(13, 1, s);
	s.Format("%.0f", m_pCompany->GetDeprAndAmort());
	m_pGridBS->SetItemText(14, 1, s);
	s.Format("%.0f", m_pCompany->GetSharesOut());
	m_pGridBS->SetItemText(15, 1, s);
	m_pGridEps->SetItemText(5, 10, s);

	m_comboType.SetCurSel(m_pCompany->GetType());

	SetBSNamesFromType(m_pCompany->GetType());

	m_checkLoadBS.SetCheck(m_pCompany->GetLoadBS());
	m_checkLoadEPS.SetCheck(m_pCompany->GetLoadEPS());
	m_checkLoadROCROE.SetCheck(m_pCompany->GetLoadROCROE());
}

void CViewGeneral::FillEpsRocRoe()
{
	if(!m_pCompany)
		return;

	int nYear0 = m_pCompany->GetYear()-9;
	double eps[ROCEPS_HYTORY_SIZE], roc[ROCEPS_HYTORY_SIZE], roe[ROCEPS_HYTORY_SIZE], shares[ROCEPS_HYTORY_SIZE];
	m_pCompany->GetEPSHistory(eps);
	m_pCompany->GetROCHistory(roc);
	m_pCompany->GetROEHistory(roe);
	m_pCompany->GetSharesHistory(shares);

	for(int i=0; i<=10; i++)
	{
		// year
		CString sYear; sYear.Format("%u", nYear0+i);
		m_pGridEps->SetItemText(0, i+1, sYear);

		// ROC
		double val = roc[ROCEPS_HYTORY_SIZE - 11 + i];
		CString sROC; if(val== _INF_) sROC="n/a"; else sROC.Format("%.2f", val);
		m_pGridEps->SetItemText(1, i+1, sROC);

		// ROE
		val = roe[ROCEPS_HYTORY_SIZE - 11 + i];
		CString sROE; if(val== _INF_) sROE="n/a"; else sROE.Format("%.2f", val);
		m_pGridEps->SetItemText(2, i+1, sROE);

		// EPS
		if(i<10)
		{
			val = eps[ROCEPS_HYTORY_SIZE - 11 + i];
			CString sEPS; if(val== _INF_) sEPS="n/a"; else sEPS.Format("%.2f", val);
			m_pGridEps->SetItemText(3, i+1, sEPS);
		}

		// Shares
		if(i<9)
		{
			val = shares[ROCEPS_HYTORY_SIZE - 11 + i];
			CString sShares; if(val== _INF_) sShares="n/a"; else sShares.Format("%.0f", val);
			m_pGridEps->SetItemText(5, i+1, sShares);
		}
	}

	CString s;
	s.Format("%.2f", m_pCompany->GetMeanROC());
	m_pGridEps->SetItemText(1, 12, s);
	s.Format("%.2f", m_pCompany->GetMeanROE());
	m_pGridEps->SetItemText(2, 12, s);
	s.Format("%.2f", m_pCompany->GetMeanEPS());
	m_pGridEps->SetItemText(3, 12, s);

	s.Format("%u", m_pCompany->GetYearsForROC());
	m_pGridYrs->SetItemText(1, 0, s);
	m_pGridYrs->SetItemText(2, 0, s);
	s.Format("%u", m_pCompany->GetYearsForEPS());
	m_pGridYrs->SetItemText(3, 0, s);
	s.Format("%.2f", m_pCompany->GetMeanROC10Years());
	m_pGridYrs->SetItemText(1, 1, s);
}

HBRUSH CViewGeneral::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CFormView::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_CAPTION)
	{
		pDC->SetTextColor(RGB(0, 255, 0));
		pDC->SetBkMode(TRANSPARENT);

		LOGFONT lf;
		memset(&lf,0,sizeof(LOGFONT));
		strcpy(lf.lfFaceName,"Courier New");
		lf.lfHeight = (long)16;
		lf.lfWeight = 400;
		CFont font; font.CreateFontIndirect(&lf);
		pDC->SelectObject(&font);


		static CBrush br(RGB(0,0,0));
		return br;

	}

	if (pWnd->GetDlgCtrlID() == IDC_STATIC_DATE)
	{
		pDC->SetTextColor(RGB(0, 0, 100));
		pDC->SetBkMode(TRANSPARENT);

		LOGFONT lf;
		memset(&lf,0,sizeof(LOGFONT));
		strcpy(lf.lfFaceName,"Arial");
		lf.lfHeight = (long)14;
		lf.lfWeight = 400;
		CFont font; font.CreateFontIndirect(&lf);
		pDC->SelectObject(&font);
		
		static CBrush br(RGB(210,210,210));
		return br;

	}

	return hbr;
}

void CViewGeneral::OnDestroy() 
{
	CFormView::OnDestroy();
	
	if(m_pGridEps)
		m_pGridEps->DestroyWindow();

	if(m_pGridYrs)
		m_pGridYrs->DestroyWindow();

	if(m_pGridBS)
		m_pGridBS->DestroyWindow();

	if(m_pGridInput2)
		m_pGridInput2->DestroyWindow();

	if(m_pGridRatios2)
		m_pGridRatios2->DestroyWindow();

	if(m_pGridRanksHeader)
		m_pGridRanksHeader->DestroyWindow();

	if(m_pGridRanks)
		m_pGridRanks->DestroyWindow();
}

void CViewGeneral::OnGeneralCalc() 
{
	UpdateCompany();

	// Set Next Year EPS Value to the history array (last item)
	//m_pCompany->SetEPS(ROCEPS_HYTORY_SIZE-1, m_pCompany->GetEPSForecastNextYear());

	// check the EPS adjustment
	CString sEpsAdjust = m_pGridInput2->GetItemText(6, 1);
	int nType = m_pCompany->GetType();

	if(nType == 1) // Banks
	{
		if(sEpsAdjust.GetLength() > 0 && sEpsAdjust[0] != '+' && sEpsAdjust[0] != '-' && sEpsAdjust != "0" &&  sEpsAdjust != "0.0" && sEpsAdjust != "0.00" )
			AfxMessageBox("For Banks EPS Adjustment should begin with '+' or '-'");
	}
	if(nType == 3) // Utilities
	{
		if(sEpsAdjust.GetLength() > 0 && sEpsAdjust[0] != '*' && sEpsAdjust != "0" &&  sEpsAdjust != "0.0" && sEpsAdjust != "0.00" )
			AfxMessageBox("For Utilities EPS Adjustment should begin with '*'");
	}

	// Returns can't be < 0 or > 50 %
	int i;
	double *values = new double[ROCEPS_HYTORY_SIZE];
	m_pCompany->GetROCHistory(values);
	for(i=0; i<ROCEPS_HYTORY_SIZE; i++)
		if(values[i] < _INF_)
		{
			if(values[i] < 0 || values[i] > 50)
				AfxMessageBox("ROC must be between 0% and 50%");
		}
	m_pCompany->GetROEHistory(values);
	for(i=0; i<ROCEPS_HYTORY_SIZE; i++)
		if(values[i] < _INF_)
		{
			if(values[i] < 0 || values[i] > 50)
				AfxMessageBox("ROE must be between 0% and 50%");
		}
	m_pCompany->GetEPSHistory(values);
	for(i=0; i<ROCEPS_HYTORY_SIZE; i++)
		if(values[i] < _INF_)
		{
			if(values[i] < 0)
				AfxMessageBox("EPS must be greater than 0.");
		}
	delete [] values;

	m_pCompany->CalcAll();
	theApp.m_Universe.RecalcRankings();

	SetCompany(m_pCompany);
	Invalidate(false);
}

void CViewGeneral::OnUpdateGeneralCalc(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CViewGeneral::OnGeneralOpen() 
{
	if(!m_pCompany)
		return;

	static char szFilters[] ="CIP Stock Info files (*.sti)|*.sti|All Files (*.*)|*.*||";

	CFileDialog fileDlg (true, "sti", "*.sti", OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, szFilters, this);

	if(fileDlg.DoModal() != IDOK )
		return;
	
	CString sName = fileDlg.GetPathName();
	m_pCompany->Load(sName, true);

	SetCompany(m_pCompany);
	Invalidate(false);
}

void CViewGeneral::OnUpdateGeneralOpen(CCmdUI* pCmdUI) 
{
		
}


void CViewGeneral::OnGeneralSave() 
{
	if(!m_pCompany)
		return;

	if(m_bChanged)
	{
		int nRes = AfxMessageBox("You have made some changes. Do you want to recalculate the Model before saving?", MB_YESNOCANCEL);
		if(nRes == IDCANCEL)
			return;

		if(nRes = IDYES)
		{
			OnGeneralCalc();
			m_bChanged = false;
		}
		
	}

	CTime date = CTime::GetCurrentTime();
	CString sTime = date.Format("%Y_%m_%d");
	CString sFileName;
	sFileName.Format("%s_%s.sti", m_pCompany->GetTicker(), sTime);

	static char szFilters[] ="CIP Stock Info files (*.sti)|*.sti|All Files (*.*)|*.*||";
	CFileDialog fileDlg (false, "sti", sFileName, OFN_OVERWRITEPROMPT|OFN_HIDEREADONLY, szFilters, this);

	if(fileDlg.DoModal() != IDOK )
		return;
	
	CString sName = fileDlg.GetPathName();
	m_pCompany->Save(sName);
}

void CViewGeneral::OnUpdateGeneralSave(CCmdUI* pCmdUI) 
{
	//pCmdUI->Enable(m_bChanged);
}

void CViewGeneral::OnGeneralSplit() 
{ 
	if(!m_pCompany)
		return;

	int nRet = AfxMessageBox("Are you sure you want to make Stick Split?  The History of the Stock will be modified and you will not be able to undo the changes.", MB_ICONQUESTION|MB_YESNO);
	if(nRet != IDYES)
		return;

	CDialogSplit dlg;
	if(dlg.DoModal() != IDOK)
		return;

	double fRate = dlg.m_fRate;
	if(!m_pCompany->Split(fRate))
	{
		AfxMessageBox("Unable to make the split and update the history.  Please try again.", MB_ICONINFORMATION);
		return;
	}
	
	m_pCompany->CalcAll();
	theApp.m_Universe.RecalcRankings();

	SetCompany(m_pCompany);
	Invalidate(false);

	AfxMessageBox("Stock split was successfully made; and History was updated.", MB_ICONINFORMATION);
}

void CViewGeneral::OnUpdateGeneralSplit(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CViewGeneral::OnGeneralExcel() 
{
	if(!m_pCompany)
		return;

	CString sTime = COleDateTime::GetCurrentTime().Format("%Y_%m_%d");
	CString sFileName;
	sFileName.Format("%s_Snapshot_%s.xls", m_pCompany->GetTicker(), sTime);
	static char szFilters[] ="Excel Files (*.xls)|*.xls|All Files (*.*)|*.*||";
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

	int nRow, nCol;
	
	// Header
	CString sLine;
	m_stCaption.GetWindowText(sLine);
	sLine = "\n" + sLine;
	sLine += "\n\n";
	file.WriteString(sLine);

	// EPS ROC ROE and Years
	for(nRow = 0; nRow < m_pGridEps->GetRowCount(); nRow++)
	{
		CString sLine;
		for(nCol=0; nCol<m_pGridEps->GetColumnCount(); nCol++)
			sLine += m_pGridEps->GetItemText(nRow, nCol) + "\t";
		
		sLine += "\t";
		for(nCol=0; nCol<m_pGridYrs->GetColumnCount(); nCol++)
			sLine += m_pGridYrs->GetItemText(nRow, nCol) + "\t";
		sLine += "\n";

		file.WriteString(sLine);
	}

	file.WriteString("\n");

	// BS and Ratios
	for(nRow = 0; nRow < m_pGridInput2->GetRowCount(); nRow++)
	{
		CString sLine;
		for(nCol=0; nCol<m_pGridBS->GetColumnCount(); nCol++)
			sLine += m_pGridBS->GetItemText(nRow, nCol) + "\t\t";
		
		sLine += "\t";
		for(nCol=0; nCol<m_pGridInput2->GetColumnCount(); nCol++)
			sLine += m_pGridInput2->GetItemText(nRow, nCol) + "\t\t";

		sLine += "\t";
		for(nCol=0; nCol<m_pGridRatios->GetColumnCount(); nCol++)
			sLine += m_pGridRatios->GetItemText(nRow, nCol) + "\t\t";

		sLine += "\t";
		for(nCol=0; nCol<m_pGridRatios2->GetColumnCount(); nCol++)
			sLine += m_pGridRatios2->GetItemText(nRow, nCol) + "\t\t\t";

		sLine += "\n";
		file.WriteString(sLine);
	}
	// BS and Ratios
	for(nRow = m_pGridInput2->GetRowCount(); nRow < m_pGridBS->GetRowCount(); nRow++)
	{
		CString sLine;
		for(nCol=0; nCol<m_pGridBS->GetColumnCount(); nCol++)
			sLine += m_pGridBS->GetItemText(nRow, nCol) + "\t\t";

		if(nRow == m_pGridInput2->GetRowCount()+2)
		{
			sLine += "\t\t\t";
			for(int i=1; i<=7; i+= 2)
				sLine += m_pGridRanksHeader->GetItemText(0, i)+"\t\t\t\t";
		}
		else if(nRow == m_pGridInput2->GetRowCount()+3)
		{
			sLine += "\t\t";
			for(int i=1; i<=m_pGridRanks->GetColumnCount(); i++)
					sLine += m_pGridRanks->GetItemText(0, i)+"\t";
		}
		sLine += "\n";
		file.WriteString(sLine);
	}

	for(nRow = 1; nRow < m_pGridRanks->GetRowCount(); nRow++)
	{
		CString sLine = "\t\t\t\t\t";
		for(int i=0; i<=m_pGridRanks->GetColumnCount(); i++)
			sLine += m_pGridRanks->GetItemText(nRow, i)+"\t";

		sLine += "\n";
		file.WriteString(sLine);
	}

	file.WriteString("\n");

	// company type
	sLine.Empty();
	int n = m_comboType.GetCurSel();
	if(n != -1)
		m_comboType.GetLBText(n, sLine);
	sLine += "\n\n";
	file.WriteString(sLine);

	// Debt Adjustment
	sLine = "Debt Adjustment: ";
	if(m_checkUseDebtAdjust.GetCheck()) 
		sLine += "YES";
	else
		sLine += "NO";
	sLine += "\n";
	file.WriteString(sLine);

	// Load Balance Sheet
	sLine = "Load Balance Sheet: ";
	if(m_checkLoadBS.GetCheck()) 
		sLine += "YES";
	else
		sLine += "NO";
	sLine += "\n";
	file.WriteString(sLine);

	// Load EPS 1, 2, & 3
	sLine = "Load EPS 1, 2, 3: ";
	if(m_checkLoadEPS.GetCheck()) 
		sLine += "YES";
	else
		sLine += "NO";
	sLine += "\n";
	file.WriteString(sLine);

	// Load ROC & ROE
	sLine = "Load ROC & ROE: ";
	if(m_checkLoadROCROE.GetCheck()) 
		sLine += "YES";
	else
		sLine += "NO";
	sLine += "\n";
	file.WriteString(sLine);

	// Load ROC & ROE
	file.WriteString("\n");
	m_stDate.GetWindowText(sLine);
	sLine += "\n";
	file.WriteString(sLine);

	file.Close();
	ShellExecute(m_hWnd, "open", sName, NULL, NULL, SW_SHOW);
}

void CViewGeneral::OnUpdateGeneralExcel(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CViewGeneral::OnGeneralWord() 
{
	// TODO: Add your command handler code here
	
}

void CViewGeneral::OnUpdateGeneralWord(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CViewGeneral::OnGeneralGraph() 
{
	if(!m_pCompany)
		return;

	CViewCharts* pView = theApp.NewViewCharts();

	CString sTokens;
	CString sTicker = m_pCompany->GetTicker();

	sTokens.Format("%s_REPV %s_NEPV %s_Price", sTicker, sTicker, sTicker);

	pView->SetCharts(sTokens, "", true);
}

void CViewGeneral::OnUpdateGeneralGraph(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CViewGeneral::OnGeneralSavetodb() 
{
	if(!m_pCompany)
		return;

//	CDialogLogin dlg;
//	if(dlg.DoModal() != IDOK)
//		return;

	if(m_bChanged)
	{
		int nRes = AfxMessageBox("You have made some changes. Do you want to recalculate the Model before saving?", MB_YESNOCANCEL);
		if(nRes == IDCANCEL)
			return;

		if(nRes = IDYES)
		{
			OnGeneralCalc();
			m_bChanged = false;
		}
		
	}

	SCompanyRawData data;
	if(!m_pCompany->GetRawData(data))
	{
		AfxMessageBox("Unable to update Data-Base.  Please try again.");
		return;
	}

	if(!CSQLCompany::UpdateCompany(data, true, true))
	{
		AfxMessageBox("Unable to update Data-Base.  Please try again.");
		return;
	}

	AfxMessageBox("The information is successfully saved into Data-Base.");
}

void CViewGeneral::OnUpdateGeneralSavetodb(CCmdUI* pCmdUI) 
{
	//pCmdUI->Enable(m_bChanged);
}

LRESULT CViewGeneral::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	switch(message)
	{
	case WMU_GRID_CELLCHANGED:
		m_bChanged = true;
		break;

	case WMU_CLOSING_FRAME:
		{
			if(m_bChanged)
			{
				int nRes = AfxMessageBox("You have made some changes. Do you want to recalculate the Model before closing?", MB_YESNO);
				if(nRes = IDYES)
				{
					OnGeneralCalc();
					m_bChanged = false;
				}		
			}
		}
	}

	return CFormView::WindowProc(message, wParam, lParam);
}


void CViewGeneral::OnGeneralSensAnalysis() 
{
	CDialogSensAnalysis dlg(m_pCompany);
	dlg.DoModal();
}

void CViewGeneral::OnUpdateGeneralSensAnalysis(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CViewGeneral::OnGeneralMontecarlo() 
{
	theApp.NewViewMonteCarlo(m_pCompany);
	
}

void CViewGeneral::OnUpdateGeneralMontecarlo(CCmdUI* pCmdUI) 
{
	// TODO: Add your command update UI handler code here
	
}

void CViewGeneral::OnSelchangeComboType() 
{
	SetBSNamesFromType(m_comboType.GetCurSel());
}

void CViewGeneral::OnGeneralLastEpsYear() 
{
	if(!m_pCompany)
		return;

	CDialogLastReportYear dlg;
	dlg.m_nOption = (m_pCompany->GetYear() == -1) ? 0 : 1;
	dlg.m_nYear = m_pCompany->GetYear();

	if(dlg.DoModal() != IDOK)
		return;

	int nYear = dlg.m_nYear;

	COleDateTime now = COleDateTime::GetCurrentTime();
	SCompanyRawData CompanyData;
	CSQLCompany::GetCompanyRawData(nYear, m_pCompany->GetTicker(), now, CompanyData);
	
	m_pCompany->CreateFromRawData(CompanyData);
	if(nYear != -1)
		m_pCompany->SetYear(nYear);

	theApp.m_Universe.CalcAll();

	SetCompany(m_pCompany);
	Invalidate();
}

void CViewGeneral::OnCheckDownloadBs() 
{
	BOOL bCheck = m_checkLoadBS.GetCheck();
	m_pCompany->SetLoadBS(bCheck);
}

void CViewGeneral::OnCheckDownloadEps() 
{
	BOOL bCheck = m_checkLoadEPS.GetCheck();
	m_pCompany->SetLoadEPS(bCheck);
}

void CViewGeneral::OnCheckDownloadRocroe() 
{
	BOOL bCheck = m_checkLoadROCROE.GetCheck();
	m_pCompany->SetLoadROCROE(bCheck);		
}

void CViewGeneral::OnCheckUseDebtAdjust() 
{
	BOOL bCheck = m_checkUseDebtAdjust.GetCheck();
	m_pCompany->SetUseDebtAdjust(bCheck);
}

void CViewGeneral::OnGeneralHistory() 
{
	if(!m_pCompany)
		return;

	theApp.NewViewHistory(m_pCompany);
}

void CViewGeneral::OnGeneralAddToHistory() 
{
	if(!m_pCompany)
		return;

	COleDateTime date;
	CHistory* pHist = m_pCompany->GetHistory();
	if(!pHist)
	{
		COleDateTime null; null.SetStatus(COleDateTime::null);
		m_pCompany->LoadHistory(null, null);
		pHist = m_pCompany->GetHistory();
	}

	if(pHist->GetSize() == 0)
	{
		COleDateTime now(COleDateTime::GetCurrentTime());
		date = COleDateTime(now.GetYear(), now.GetMonth(), now.GetDay(), 0,0,0);
	}
	else
	{
		date = pHist->GetItemPtr(0)->date;
		COleDateTimeSpan sp(7,0,0,0);
		date += sp;
	}

	CDialogDate dlg;
	dlg.m_Date = date;
	if(dlg.DoModal() != IDOK)
		return;

	// New record
	SHistoryItem item;
	item.date = dlg.m_Date;
	item.data[0] = _INF_;
	m_pCompany->GetHistoryItem(item);
	CSQLCompany::UpdateHistoryItem(m_pCompany->GetTicker(), item, NUM_COLUMNS_IN_HISTORY_DB);

	// Modify the last one
	if(pHist->GetSize() > 0)
	{
		pHist->GetItemPtr(0)->data[0] = 100.0 * ( item.data[1] / pHist->GetItemPtr(0)->data[1]  -  1 );
		CSQLCompany::UpdateHistoryItem(m_pCompany->GetTicker(), *pHist->GetItemPtr(0), NUM_COLUMNS_IN_HISTORY_DB);
	}

	AfxMessageBox("A new History Record is successfully added.");
}


void CViewGeneral::OnGeneralDate() 
{
	if(!m_pCompany)
		return;

	CDialogDate dlg;
	dlg.m_Date = m_pCompany->GetReportDateForPriceAndReturn();
	if(dlg.DoModal() != IDOK)
		return;

	COleDateTime date = dlg.m_Date;
	int nYear = m_pCompany->GetYear();

	SCompanyRawData CompanyData;
	CSQLCompany::GetCompanyRawData(nYear, m_pCompany->GetTicker(), date, CompanyData);
	
	m_pCompany->CreateFromRawData(CompanyData);
	if(nYear != -1)
		m_pCompany->SetYear(nYear);

	theApp.m_Universe.CalcAll();

	SetCompany(m_pCompany);
	Invalidate();	
}

void CViewGeneral::OnGeneralEps() 
{
	if(!m_pCompany)
		return;

	CDialogEpsGraph dlg(m_pCompany);
	dlg.DoModal();
}

void CViewGeneral::OnGeneralMeetings() 
{
	if(!m_pCompany)
		return;

	CViewMeetings* pView = theApp.NewViewMeetings();
	if(pView)
		pView->FilterByCompany(m_pCompany->GetTicker());
}

void CViewGeneral::OnGeneral2minute() 
{
	if(!m_pCompany)
		return;

	//CDialog2Minute dlg(m_pCompany->m_nID());
	//dlg.DoModal();
}
