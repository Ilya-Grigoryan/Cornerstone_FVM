// ViewMonteCarlo.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "ViewMonteCarlo.h"

#include "WndGrid.h"
#include "ViewGraph.h"
#include "DialogMonteCarloInputs.h"

#include "Universe.h"
#include "Company.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewMonteCarlo

IMPLEMENT_DYNCREATE(CViewMonteCarlo, CFormView)

CViewMonteCarlo::CViewMonteCarlo()
	: CFormView(CViewMonteCarlo::IDD)
{
	//{{AFX_DATA_INIT(CViewMonteCarlo)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pCompany = NULL;
	m_pDialogInputs = NULL;
	m_pGraph = NULL;
	m_pGrid = NULL;
}

CViewMonteCarlo::~CViewMonteCarlo()
{
}

void CViewMonteCarlo::Resize()
{
	CRect rc;
	GetClientRect(rc);

	CRect rcTab(rc.left+5, rc.top+5, rc.right-200-5, 30);
	CRect rcInputs(rc.right-200, rc.top+5, rc.right-5, rc.bottom-5);
	CRect rcGrid(rc.left+5, rc.bottom-5-21*5, rcInputs.left-5, rc.bottom-5);
	CRect rcGraph(rc.left+5, rcTab.bottom+1, rcGrid.right, rcGrid.top-25);

	CRect rcSlider(rcGraph.left+45, rcGrid.top-24, rcGraph.right-3, rcGrid.top-3);
	CRect rcPercent(rcGraph.left+8, rcGrid.top-23, rcGraph.left+45, rcGrid.top-2);

	if(m_TabOuts)
		m_TabOuts.MoveWindow(rcTab);

	if(m_pDialogInputs)
		m_pDialogInputs->MoveWindow(rcInputs);

	if(m_pGraph)
		m_pGraph->MoveWindow(rcGraph);

	if(m_pGrid)
		m_pGrid->MoveWindow(rcGrid);

	if(m_sliderLeft)
		m_sliderLeft.MoveWindow(rcSlider);

	if(m_editPercent)
		m_editPercent.MoveWindow(rcPercent);

	if(m_stPercent)
		m_stPercent.MoveWindow(rcPercent.left-8, rcPercent.top+1, 8, 20);
}

void CViewMonteCarlo::SetCompany(CCompany* pCompany)
{
	m_pCompany = pCompany;
	m_pDialogInputs->m_pCompany = pCompany;

	// Init simulation
	m_pDialogInputs->SetupSimulation();
}

void CViewMonteCarlo::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewMonteCarlo)
	DDX_Control(pDX, IDC_STATIC_PERCENT, m_stPercent);
	DDX_Control(pDX, IDC_EDIT_PERCENT, m_editPercent);
	DDX_Control(pDX, IDC_SLIDER_LEFT, m_sliderLeft);
	DDX_Control(pDX, IDC_TAB_OUTS, m_TabOuts);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewMonteCarlo, CFormView)
	//{{AFX_MSG_MAP(CViewMonteCarlo)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB_OUTS, OnSelchangeTabOuts)
	ON_WM_HSCROLL()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewMonteCarlo diagnostics

#ifdef _DEBUG
void CViewMonteCarlo::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewMonteCarlo::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewMonteCarlo message handlers

void CViewMonteCarlo::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	ResizeParentToFit();

	SetScrollSizes(MM_TEXT, CSize(0,0));

	LOGFONT lf;
	memset(&lf,0,sizeof(LOGFONT));
	strcpy(lf.lfFaceName,"Ms Sans Serif");
	lf.lfHeight = (long)13;
	lf.lfWeight = 400;
	m_Font.CreateFontIndirect(&lf);

	CImageList* pImageList = new CImageList();
	pImageList->Create(16,16,ILC_COLOR|ILC_MASK, 4, 4);

	pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_GREEN));
	pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_RED));
	pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_BLUE));
	pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_YELLOW));
	m_TabOuts.SetImageList(pImageList);

	m_TabOuts.InsertItem(0, "Actual", 0);
	m_TabOuts.InsertItem(1, "Forecast", 1);
	m_TabOuts.InsertItem(2, "Razor", 2);
	m_TabOuts.InsertItem(3, "Razor Forecast", 3);
	
	m_pDialogInputs = new CDialogMonteCarloInputs(m_pCompany, this);
	m_pDialogInputs->Create(IDD_DIALOG_MONTECARLO_INPUTS, this);
	m_pDialogInputs->ShowWindow(SW_SHOW);

	m_pGraph = new CViewGraph();
	m_pGraph->Create(NULL, NULL, WS_CHILD, CRect(0,0,1,1), this, 23457);
	m_pGraph->ShowWindow(SW_SHOW);

	m_pGrid = new CWndGrid();
	m_pGrid->SetEnableScrollBars(false);
	m_pGrid->SetDefaultColumnSize(60);
	m_pGrid->SetDefaultRowSize(21);
	m_pGrid->SetTextAlign(DT_RIGHT);

	m_pGrid->Create(this);

	m_pGrid->SetColumnCount(10);
	m_pGrid->SetRowCount(5);

	m_pGrid->SetCellsFont(m_Font);

	m_pGrid->SetFixedColumnCount(0);
	m_pGrid->SetFixedRowCount(0);
	m_pGrid->SetDefaultColorGridFixed(RGB(255,255,255));
	m_pGrid->SetDefaultColorGridNonFixed(RGB(255,255,255));

	for(int i=0; i<=4; i+=2)
		m_pGrid->SetRowBkColor(i, RGB(230,230,230));

	m_pGrid->SetColumnSize(0, 60);
	m_pGrid->SetColumnSize(2, 2);
	m_pGrid->SetColumnSize(3, 90);
	m_pGrid->SetColumnSize(5, 2);
	m_pGrid->SetColumnSize(6, 140);
	m_pGrid->SetColumnSize(8, 2);
	m_pGrid->SetColumnSize(9, 800);

	m_pGrid->SetColumnBkColor(2, RGB(100,100,100));
	m_pGrid->SetColumnBkColor(5, RGB(100,100,100));
	m_pGrid->SetColumnBkColor(8, RGB(100,100,100));

	m_pGrid->SetItemText(0,0,"Min:");
	m_pGrid->SetItemText(1,0,"Max:");
	m_pGrid->SetItemText(2,0,"Mean:");
	m_pGrid->SetItemText(3,0,"Median:");
	m_pGrid->SetItemText(4,0,"Mode:");

	m_pGrid->SetItemText(0,3,"St. Dev:");
	m_pGrid->SetItemText(1,3,"Variance:");
	m_pGrid->SetItemText(2,3,"Coeff. of Var:");
	m_pGrid->SetItemText(3,3,"Skewness:");
	m_pGrid->SetItemText(4,3,"Kurtosis:");

	m_pGrid->SetItemText(0,6,"Low 2.5% Probability:");
	m_pGrid->SetItemText(1,6,"High 2.5% Probability:");
	m_pGrid->SetItemText(2,6,"10% Downside Risk:");

	m_sliderLeft.SetRange(0,100*100);
	m_sliderLeft.SetPos(0);

	m_fCumProb = 0;
	m_editPercent.SetWindowText("0.00");

	Resize();
}

void CViewMonteCarlo::SetOutStats()
{
	int nTab = m_TabOuts.GetCurSel();
	CMonteCarloOut* pOut = m_pDialogInputs->m_Simulation.GetOut(nTab);

	int nHisto = 100;
	pOut->CreateHistogram(nHisto);
	CHistogram* pHisto = pOut->GetHistogram(); 
	nHisto = pHisto->GetItemCount();
	
	double f;
	CString s;

	f = pHisto->GetMin();
	s.Format("%.2f", f);
	m_pGrid->SetItemText(0,1,s);

	f = pHisto->GetMax();
	s.Format("%.2f", f);
	m_pGrid->SetItemText(1,1,s);

	f = pHisto->GetMean();
	s.Format("%.2f", f);
	m_pGrid->SetItemText(2,1,s);

	f = pHisto->GetMedian();
	s.Format("%.2f", f);
	m_pGrid->SetItemText(3,1,s);

	CHistogram::Item item = pHisto->GetMode();
	s.Format("%.2f", (item.low + item.high)/2.0 );
	m_pGrid->SetItemText(4,1,s);

	f = pHisto->GetStDev();
	s.Format("%.2f", f);
	m_pGrid->SetItemText(0,4,s);

	f = pHisto->GetVariance();
	s.Format("%.2f", f);
	m_pGrid->SetItemText(1,4,s);

	f = pHisto->GetCoeffOfVar();
	s.Format("%.2f", f);
	m_pGrid->SetItemText(2,4,s);

	f = pHisto->GetSkewness();
	s.Format("%.2f", f);
	m_pGrid->SetItemText(3,4,s);

	f = pHisto->GetKurtosis();
	s.Format("%.2f", f);
	m_pGrid->SetItemText(4,4,s);

	f = pHisto->GetInverseCumulativeProb(0.025);
	s.Format("%.2f", f);
	m_pGrid->SetItemText(0,7,s);

	f = pHisto->GetInverseCumulativeProb(0.975);
	s.Format("%.2f", f);
	m_pGrid->SetItemText(1,7,s);

	f = pHisto->GetInverseCumulativeProb(0.1);
	s.Format("%.2f", f);
	m_pGrid->SetItemText(2,7,s);

	m_pGrid->Invalidate();
}

void CViewMonteCarlo::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	Resize();
}

void CViewMonteCarlo::OnDestroy() 
{
	CFormView::OnDestroy();

	CImageList* pImageList = m_TabOuts.GetImageList();
	if(pImageList)
		delete pImageList;	
	
	if(m_pDialogInputs)
	{
		m_pDialogInputs->DestroyWindow();
		delete m_pDialogInputs;
	}

	if(m_pGrid)
		m_pGrid->DestroyWindow();
}

void CViewMonteCarlo::SetCharts()
{
	int nTab = m_TabOuts.GetCurSel();

	CString sCaption;
	switch(nTab)
	{
	case 0: sCaption = "Actual"; break;
	case 1: sCaption = "Forecast"; break;
	case 2: sCaption = "Razor"; break;
	case 3: sCaption = "Razor Forecast"; break;
	}

	m_pGraph->m_sLabelX = sCaption + " Value ($)";
	m_pGraph->m_sLabelY = "Probability";
	CMonteCarloOut* pOut = m_pDialogInputs->m_Simulation.GetOut(nTab);

	int nHisto = 100;
	pOut->CreateHistogram(nHisto);
	CHistogram* pHisto = pOut->GetHistogram(); 
	nHisto = pHisto->GetItemCount();
	
	double* x = new double[nHisto];
	double* y = new double[nHisto];

	for(int i=0; i<nHisto; i++)
	{
		CHistogram::Item item = pHisto->GetItem(i);
		x[i] = (item.low + item.high) / 2.0;
		y[i] = item.prob;
	}

	CString sX = sCaption + " EPV";
	m_pGraph->RemoveAll();
	m_pGraph->AddSeries(sX, x, y, nHisto, SERIES_TYPE_BAR, 2, PS_SOLID, RGB(0,174,249));

	double fMean = pHisto->GetMean();
	CString s; s.Format("Mean: $%.2f", fMean);
	m_pGraph->AddLine(fMean, s, LINE_TEXT_TOPRIGHT, LINE_TYPE_VERT, 2, PS_SOLID, RGB(0,192,0));

	double val1 = pHisto->GetInverseCumulativeProb(0.025);
	CString s1; s1.Format("$%.2f (%.1f%%)", val1, 2.5);
	m_pGraph->AddLine(val1, s1, LINE_TEXT_TOPRIGHT, LINE_TYPE_VERT, 1, PS_SOLID, RGB(100,100,100));
	
	double val2 = pHisto->GetInverseCumulativeProb(0.975);
	CString s2; s2.Format("$%.2f (%.1f%%)", val2, 97.5);
	m_pGraph->AddLine(val2, s2, LINE_TEXT_TOPRIGHT, LINE_TYPE_VERT, 1, PS_SOLID, RGB(100,100,100));

	/*double val3 = pHisto->GetInverseCumulativeProb(0);
	CString s3; s3.Format("$%.2f (%.1f%%)", val3, 0);
	m_pGraph->AddLine(val3, s3, LINE_TEXT_TOPRIGHT, LINE_TYPE_VERT, 2, PS_SOLID, RGB(255,0,0));
*/
	m_pGraph->m_fMinY = 0;
	m_pGraph->m_bFixedMinY = true;
	m_pGraph->m_nTypeAxisY = AXIS_TYPE_PERCENT;
	m_pGraph->RedrawWindow();

	delete [] x;
	delete [] y;

	SetOutStats();
}

void CViewMonteCarlo::OnSelchangeTabOuts(NMHDR* pNMHDR, LRESULT* pResult) 
{
	SetCharts();
	
	*pResult = 0;
}

void CViewMonteCarlo::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if((CSliderCtrl*)pScrollBar == &m_sliderLeft)
	{
		m_fCumProb = (double)(m_sliderLeft.GetPos()) / 100.0;
		
		CString s; s.Format("%.2f", m_fCumProb);
		m_editPercent.SetWindowText(s);
	}	

	CFormView::OnHScroll(nSBCode, nPos, pScrollBar);
}
 