// DialogEpsGraph.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "DialogEpsGraph.h"

#include "ViewGraph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogEpsGraph dialog


CDialogEpsGraph::CDialogEpsGraph(CCompany* pCompany, CWnd* pParent /*=NULL*/)
	: CDialog(CDialogEpsGraph::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogEpsGraph)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pGraph = NULL;
	m_pCompany = pCompany;
}


void CDialogEpsGraph::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogEpsGraph)
	DDX_Control(pDX, IDC_TAB, m_Tab);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogEpsGraph, CDialog)
	//{{AFX_MSG_MAP(CDialogEpsGraph)
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, OnSelchangeTab)
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogEpsGraph message handlers

void CDialogEpsGraph::OnSelchangeTab(NMHDR* pNMHDR, LRESULT* pResult) 
{
	m_pGraph->RemoveAll();

	int nSel = m_Tab.GetCurSel();
	switch(nSel)
	{
	case 0: InitChartEPS(); break;
	case 1: InitChartROC(); break;
	case 2: InitChartROE(); break;
	}
	m_pGraph->Invalidate();
	
	*pResult = 0;
}

BOOL CDialogEpsGraph::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	CImageList* pImageList = new CImageList();
	pImageList->Create(16,16,ILC_COLOR|ILC_MASK, 4, 4);

	pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_GREEN));
	pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_RED));
	pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_BLUE));
	pImageList->Add(AfxGetApp()->LoadIcon(IDI_ICON_YELLOW));
	m_Tab.SetImageList(pImageList);

	m_Tab.InsertItem(0, "EPS", 0);
	m_Tab.InsertItem(1, "ROC", 1);
	m_Tab.InsertItem(2, "ROE", 2);

	m_pGraph = new CViewGraph();
	m_pGraph->Create(NULL, NULL, WS_CHILD, CRect(0,0,1,1), this, 23457);
	m_pGraph->ShowWindow(SW_SHOW);

	InitChartEPS();	

	Resize();
	return TRUE; 
}

void CDialogEpsGraph::Resize()
{
	CRect rc;
	GetClientRect(rc);

	CRect rcTab(rc.left+5, rc.top+5, rc.right-5, 29);
	CRect rcGraph(rc.left+5, rcTab.bottom+1, rc.right-5, rc.bottom-5-18-1);
	CRect rcFooter(rc.left+5, rcGraph.bottom+3, rc.right-5, rc.bottom-3);

	if(m_Tab.m_hWnd)
		m_Tab.MoveWindow(rcTab);

	if(m_pGraph)
		m_pGraph->MoveWindow(rcGraph);

	if(GetDlgItem(IDC_STATIC_FOOTER))
		GetDlgItem(IDC_STATIC_FOOTER)->MoveWindow(rcFooter);
}

void CDialogEpsGraph::OnDestroy() 
{	
	CImageList* pImageList = m_Tab.GetImageList();
	if(pImageList)
		delete pImageList;		

	CDialog::OnDestroy();
}

void CDialogEpsGraph::OnSize(UINT nType, int cx, int cy) 
{
	CDialog::OnSize(nType, cx, cy);
	
	Resize();	
}

HBRUSH CDialogEpsGraph::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_FOOTER)
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
	
	return hbr;
}

void CDialogEpsGraph::InitChartEPS()
{
	int nYear0 = m_pCompany->GetYear()-9;
	double x[11], y[11];
	for(int i=0; i<=10; i++)
	{
		x[i] = nYear0 + i;
		y[i] = m_pCompany->GetEPS(ROCEPS_HYTORY_SIZE - 11 + i);
	}

	m_pGraph->AddSeries("EPS", x, y, 11, SERIES_TYPE_PT_LINE, 1, PS_SOLID, RGB(0,0,200));

	double rx[2], ry[2], slope, intercept, coeff;
	CMath::SimpleRegression(x,y,11, slope, intercept, coeff);
	
	rx[0] = x[0];
	rx[1] = x[10];
	ry[0] = slope*x[0]+intercept;
	ry[1] = slope*x[10]+intercept;
	
	CString sName;
	if(intercept > 0.0)
		sName.Format("y = %.3f*x + %.3f", slope, intercept);
	else
		sName.Format("y = %.3f*x - %.3f", slope, -intercept);

	m_pGraph->AddSeries(sName, rx, ry, 2, SERIES_TYPE_LINE, 1, PS_SOLID, RGB(200,0,0));

	m_pGraph->m_sLabelX = "Years";
	m_pGraph->m_sLabelY = "EPS $";

	CString sCoeff;
	sCoeff.Format("Regression Coefficient: %.3f", coeff);
	GetDlgItem(IDC_STATIC_FOOTER)->SetWindowText(sCoeff);	
}

void CDialogEpsGraph::InitChartROC()
{
	int nYear0 = m_pCompany->GetYear()-9;
	double x[11], y[11];
	for(int i=0; i<=10; i++)
	{
		x[i] = nYear0 + i;
		y[i] = m_pCompany->GetROC(ROCEPS_HYTORY_SIZE - 11 + i);
	}

	m_pGraph->AddSeries("ROC", x, y, 11, SERIES_TYPE_PT_LINE, 1, PS_SOLID, RGB(0,0,200));

	double rx[2], ry[2], slope, intercept, coeff;
	CMath::SimpleRegression(x,y,11, slope, intercept, coeff);
	
	rx[0] = x[0];
	rx[1] = x[10];
	ry[0] = slope*x[0]+intercept;
	ry[1] = slope*x[10]+intercept;
	
	CString sName;
	if(intercept > 0.0)
		sName.Format("y = %.3f*x + %.3f", slope, intercept);
	else
		sName.Format("y = %.3f*x - %.3f", slope, -intercept);

	m_pGraph->AddSeries(sName, rx, ry, 2, SERIES_TYPE_LINE, 1, PS_SOLID, RGB(200,0,0));

	m_pGraph->m_sLabelX = "Years";
	m_pGraph->m_sLabelY = "ROC %";

	CString sCoeff;
	sCoeff.Format("Regression Coefficient: %.3f", coeff);
	GetDlgItem(IDC_STATIC_FOOTER)->SetWindowText(sCoeff);	
}

void CDialogEpsGraph::InitChartROE()
{
	int nYear0 = m_pCompany->GetYear()-9;
	double x[11], y[11];
	for(int i=0; i<=10; i++)
	{
		x[i] = nYear0 + i;
		y[i] = m_pCompany->GetROE(ROCEPS_HYTORY_SIZE - 11 + i);
	}

	m_pGraph->AddSeries("ROE", x, y, 11, SERIES_TYPE_PT_LINE, 1, PS_SOLID, RGB(0,0,200));

	double rx[2], ry[2], slope, intercept, coeff;
	CMath::SimpleRegression(x,y,11, slope, intercept, coeff);
	
	rx[0] = x[0];
	rx[1] = x[10];
	ry[0] = slope*x[0]+intercept;
	ry[1] = slope*x[10]+intercept;
	
	CString sName;
	if(intercept > 0.0)
		sName.Format("y = %.3f*x + %.3f", slope, intercept);
	else
		sName.Format("y = %.3f*x - %.3f", slope, -intercept);

	m_pGraph->AddSeries(sName, rx, ry, 2, SERIES_TYPE_LINE, 1, PS_SOLID, RGB(200,0,0));

	m_pGraph->m_sLabelX = "Years";
	m_pGraph->m_sLabelY = "ROE %";

	CString sCoeff;
	sCoeff.Format("Regression Coefficient: %.3f", coeff);
	GetDlgItem(IDC_STATIC_FOOTER)->SetWindowText(sCoeff);	
}