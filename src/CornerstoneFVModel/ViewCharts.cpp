// ViewCharts.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "ViewCharts.h"

#include "DataManager.h"

#include "ViewGraph.h"
#include "WndGrid.h"
#include "DialogDate.h"
#include "ViewTable.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CViewCharts

IMPLEMENT_DYNCREATE(CViewCharts, CFormView)

CViewCharts::CViewCharts()
	: CFormView(CViewCharts::IDD)
{
	//{{AFX_DATA_INIT(CViewCharts)
	//}}AFX_DATA_INIT
	m_pGraph = NULL;
	m_pGrid = NULL;

	m_From.SetStatus(COleDateTime::invalid);
	m_To.SetStatus(COleDateTime::invalid);

	m_ColorTableArray.Add(RGB(0,0,192));
	m_ColorTableArray.Add(RGB(0,192,0));		
	m_ColorTableArray.Add(RGB(192,0,0));

	m_ColorTableArray.Add(RGB(0,192,192));		
	m_ColorTableArray.Add(RGB(192,192,0));		
	m_ColorTableArray.Add(RGB(192,0,192));

	m_ColorTableArray.Add(RGB(0,0,0xff));
	m_ColorTableArray.Add(RGB(0,0xff,0));		
	m_ColorTableArray.Add(RGB(0xff,0,0));
	
	m_ColorTableArray.Add(RGB(0,0,128));		
	m_ColorTableArray.Add(RGB(0,128,0));		
	m_ColorTableArray.Add(RGB(128,0,0));

	m_ColorTableArray.Add(RGB(0xff,128,0));	
	m_ColorTableArray.Add(RGB(0xff,0,128));	
	m_ColorTableArray.Add(RGB(0xff,128,128));

	m_ColorTableArray.Add(RGB(192,0,0xff));	
	m_ColorTableArray.Add(RGB(0,192,0xff));	
	m_ColorTableArray.Add(RGB(192,192,0xff));
	
	m_ColorTableArray.Add(RGB(192,0xff,0));	
	m_ColorTableArray.Add(RGB(0,0xff,192));	
	m_ColorTableArray.Add(RGB(192,0xff,192));
	
	m_ColorTableArray.Add(RGB(0xff,192,0));	
	m_ColorTableArray.Add(RGB(0xff,0,192));	
	m_ColorTableArray.Add(RGB(0xff,192,192));
	
	m_ColorTableArray.Add(RGB(192,0xff,0xff));	
	m_ColorTableArray.Add(RGB(0xff,0xff,192));	
	m_ColorTableArray.Add(RGB(0xff,192,0xff));
}

CViewCharts::~CViewCharts()
{
}

void CViewCharts::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewCharts)
	DDX_Control(pDX, IDC_EDIT_BASE, m_editBase);
	DDX_Control(pDX, IDC_EDIT_SYMBOLS, m_editSymbols);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewCharts, CFormView)
	//{{AFX_MSG_MAP(CViewCharts)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_COMMAND(ID_CHARTS_FROM, OnChartsFrom)
	ON_COMMAND(ID_CHARTS_END, OnChartsEnd)
	ON_COMMAND(ID_CHARTS_TABLE, OnChartsTable)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewCharts diagnostics

#ifdef _DEBUG
void CViewCharts::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewCharts::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewCharts message handlers

void CViewCharts::OnInitialUpdate() 
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

	m_pGraph = new CViewGraph();
	m_pGraph->Create(NULL, NULL, WS_CHILD, CRect(0,0,1,1), this, 23457);
	m_pGraph->ShowWindow(SW_SHOW);

	m_pGrid = new CWndGrid();
	m_pGrid->SetEnableScrollBars(false);
	m_pGrid->SetDefaultColumnSize(75);
	m_pGrid->SetDefaultRowSize(20);
	m_pGrid->SetTextAlign(DT_LEFT);

	m_pGrid->Create(this);

	m_pGrid->SetRowCount(2);
	m_pGrid->SetColumnCount(2);

	m_pGrid->SetFixedColumnCount(0);
	m_pGrid->SetFixedRowCount(1);
	m_pGrid->SetFixedCellsBkColor(RGB(200,200,200));

	m_pGrid->SetItemText(0, 0, "Item");
	m_pGrid->SetItemText(0, 1, "Value");
	m_pGrid->SetItemText(1, 0, "Date");
	
	m_pGrid->SetColumnAllowEdit(0,false);
	m_pGrid->SetColumnAllowEdit(1,false);
	/*m_pGrid->SetColumnForeColor(0, FONT_CLR_READONLY);
	m_pGrid->SetColumnForeColor(1, FONT_CLR_READONLY);*/
	m_pGrid->SetCellsFont(m_font);

	m_pGrid->SetDefaultColorGridFixed(RGB(255,255,255));
	m_pGrid->SetDefaultColorGridNonFixed(RGB(255,255,255));
	
	Resize();
}

void CViewCharts::OnDestroy() 
{
	CFormView::OnDestroy();

	CDataManager dm(&theApp.m_Universe);
	dm.FreeDataCollection(m_DataCollect);
	
	if(m_pGraph)
		m_pGraph->DestroyWindow();
}

void CViewCharts::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	Resize();
}

void CViewCharts::Resize()
{
	CRect rc;
	GetClientRect(rc);

	if(m_pGraph)
		m_pGraph->MoveWindow(5,5,rc.right-10-150-5, rc.bottom-10-20-15);

	if(m_pGrid)
		m_pGrid->MoveWindow(rc.right-5-150, 5, 150, rc.bottom-10-20-15);

	int nW = rc.right/4;
	if(GetDlgItem(IDC_STATIC_TOKENS))
		GetDlgItem(IDC_STATIC_TOKENS)->MoveWindow(rc.right/2-nW-30-52, rc.bottom-10-17, 50, 20);

	if(m_editSymbols)
		m_editSymbols.MoveWindow(rc.right/2-nW-30, rc.bottom-10-20, 2*nW, 23);

	if(GetDlgItem(IDC_STATIC_BASE))
		GetDlgItem(IDC_STATIC_BASE)->MoveWindow(rc.right/2+nW+25-32, rc.bottom-10-17, 30, 20);

	if(m_editBase)
		m_editBase.MoveWindow(rc.right/2+nW+25, rc.bottom-10-20, 95, 23);
}

void CViewCharts::AddChart(CData* pData, int nIndex)
{
	int colorIndex = nIndex % m_ColorTableArray.GetSize();
	COLORREF color = m_ColorTableArray[colorIndex];

	if(pData->GetSize())
		m_pGraph->AddSeries(pData->m_szToken, pData->m_pDates, pData->m_pValues, pData->GetSize(), SERIES_TYPE_LINE, 1, PS_SOLID, color);
}

BOOL CViewCharts::PreTranslateMessage(MSG* pMsg) 
{
	if(	pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		CWnd* pFocus = GetFocus();
		if(	pFocus && ::IsWindow(pFocus->m_hWnd) && 
			(pFocus->GetDlgCtrlID() == IDC_EDIT_SYMBOLS || pFocus->GetDlgCtrlID() == IDC_EDIT_BASE) )
		{
			m_pGraph->RemoveAll();

			CString sTokens;
			m_editSymbols.GetWindowText(sTokens); 

			CString sBase;
			m_editBase.GetWindowText(sBase);
			sBase.TrimLeft(); sBase.TrimRight();

			SetCharts(sTokens, sBase);
	//		return true;
		}
	}

	return CFormView::PreTranslateMessage(pMsg);
}

void CViewCharts::SetCharts(const char* tokens, const char* szBase, BOOL bSetTextToEdit)
{
	BeginWaitCursor();

	CDataManager dm(&theApp.m_Universe);
	dm.FreeDataCollection(m_DataCollect);

	dm.GetDataCollection(tokens, szBase, m_DataCollect, m_From, m_To);

	m_pGrid->SetRowCount(m_DataCollect.GetSize()+2);
	m_pGrid->SetFixedColumnCount(0);
	m_pGrid->SetFixedRowCount(1);
	m_pGrid->SetFixedCellsBkColor(RGB(200,200,200));
	m_pGrid->SetCellsFont(m_font);		

	for(int i=0; i<m_DataCollect.GetSize(); i++)
	{
		CData* pData = m_DataCollect[i];
		AddChart(pData, i);

		int colorIndex = i % m_ColorTableArray.GetSize();
		COLORREF color = m_ColorTableArray[colorIndex];
		
		m_pGrid->SetItemText(i+2, 0, pData->m_szToken);
		m_pGrid->SetItemForeColor(i+2, 0, color);
		m_pGrid->SetItemForeColor(i+2, 1, color);
	}

	m_pGraph->m_sLabelX = "Date";
	m_pGraph->m_sLabelY = "Value";
	m_pGraph->m_nTypeAxisX = AXIS_TYPE_DATE;
	m_pGraph->m_bMouseTrack = true;
	m_pGraph->Invalidate();
	m_pGrid->Invalidate();

	if(bSetTextToEdit)
		m_editSymbols.SetWindowText(tokens); 

	CString sTitle = "Trendline Graphs - "; sTitle += tokens;
	CDocument* pDoc = GetDocument();
	pDoc->SetTitle(sTitle);

	EndWaitCursor();
}

void CViewCharts::OnChartsFrom() 
{
	CDialogDate dlg;
	dlg.m_Date = m_From;
	if(dlg.DoModal() != IDOK)
		return;

	m_From = dlg.m_Date;

	CString sTokens;
	m_editSymbols.GetWindowText(sTokens); 

	CString sBase;
	m_editBase.GetWindowText(sBase);
	sBase.TrimLeft(); sBase.TrimRight();

	m_pGraph->RemoveAll();
	SetCharts(sTokens, sBase);
}

void CViewCharts::OnChartsEnd() 
{
	CDialogDate dlg;
	dlg.m_Date = m_To;
	if(dlg.DoModal() != IDOK)
		return;

	m_To = dlg.m_Date;

	CString sTokens;
	m_editSymbols.GetWindowText(sTokens); 

	CString sBase;
	m_editBase.GetWindowText(sBase);
	sBase.TrimLeft(); sBase.TrimRight();

	m_pGraph->RemoveAll();
	SetCharts(sTokens, sBase);
}

LRESULT CViewCharts::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message == WMU_CHART_POINTS_CALLBACK)
	{
		SPointsFromChart* pST = (SPointsFromChart*)lParam;
		
		CString sDate;
		if(pST->fX < _INF_)
			sDate = COleDateTime(pST->fX).Format("%m/%d/%Y");
		
		m_pGrid->SetItemText(1, 1, sDate);

		for(int i=0; i<pST->arY.GetSize(); i++)
		{
			double f = pST->arY[i];
			CString s;
			if(f<_INF_)
				s.Format("%.2f", f);
			m_pGrid->SetItemText(i+2, 1, s);
		}

		m_pGrid->Invalidate();
		return true;
	}
	
	return CFormView::WindowProc(message, wParam, lParam);
}

void CViewCharts::OnChartsTable() 
{
	CViewTable* pView = theApp.NewViewTable();
	pView->SetData(m_DataCollect);
}
