// ViewMapSectors.cpp : implementation file
//

#include "stdafx.h"
#include "cornerstonefvmodel.h"
#include "ViewMapSectors.h"
#include "ViewGraph.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CViewMapSectors

IMPLEMENT_DYNCREATE(CViewMapSectors, CFormView)

CViewMapSectors::CViewMapSectors()
	: CFormView(CViewMapSectors::IDD)
{
	//{{AFX_DATA_INIT(CViewMapSectors)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
	m_pGraph = NULL;
}

CViewMapSectors::~CViewMapSectors()
{
}

void CViewMapSectors::DoDataExchange(CDataExchange* pDX)
{
	CFormView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewMapSectors)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewMapSectors, CFormView)
	//{{AFX_MSG_MAP(CViewMapSectors)
	ON_WM_SIZE()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewMapSectors diagnostics

#ifdef _DEBUG
void CViewMapSectors::AssertValid() const
{
	CFormView::AssertValid();
}

void CViewMapSectors::Dump(CDumpContext& dc) const
{
	CFormView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewMapSectors message handlers

void CViewMapSectors::OnInitialUpdate() 
{
	CFormView::OnInitialUpdate();
	CFormView::OnInitialUpdate();
	ResizeParentToFit();
	SetScrollSizes(MM_TEXT, CSize(0,0));

	m_pGraph = new CViewGraph();
	m_pGraph->Create(NULL, NULL, WS_CHILD, CRect(0,0,1,1), this, 23457);
	m_pGraph->ShowWindow(SW_SHOW);

	m_pGraph->m_sLabelX = "Normalized Rank";
	m_pGraph->m_sLabelY = "Razor Rank";

	m_pGraph->AddRect(1,1,400,400,RGB(201,250,49));
	m_pGraph->AddRect(400,400,800,800,RGB(255,184,211));

	SetupMap();
	
	m_pGraph->Invalidate(false);
	Resize();
}

void CViewMapSectors::OnSize(UINT nType, int cx, int cy) 
{
	CFormView::OnSize(nType, cx, cy);
	
	Resize();
}

void CViewMapSectors::Resize()
{
	CRect rc;
	GetClientRect(rc);

	if(m_pGraph)
		m_pGraph->MoveWindow(5,5,rc.right-10, rc.bottom-10);
}

void CViewMapSectors::OnDestroy() 
{
	CFormView::OnDestroy();
	
	if(m_pGraph)
		m_pGraph->DestroyWindow();	
}

void CViewMapSectors::SetupMap()
{
	CUniverse* pUniverse = &theApp.m_Universe;
	int nSize = pUniverse->GetSectorsCount();

	double *x = new double[nSize];
	double *y = new double[nSize];
	for(int i=0; i<nSize; i++)
	{
		CString sName = pUniverse->GetSectorName(i);
		m_pGraph->m_arLabels.Add(sName);

		double fx = pUniverse->GetSectorNEPVRank(sName);
		double fy = pUniverse->GetSectorREPVRank(sName);

		x[i] = fx;
		y[i] = fy;
	}

	COLORREF color = RGB(255,100,100);//m_ColorTableArray[colorIndex];

	m_pGraph->AddSeries("Sector Rankings", x, y, nSize, SERIES_TYPE_PT_COLORED, 1, PS_SOLID, color);

	delete [] x;
	delete [] y;
}