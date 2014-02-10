// FrameGeneral.cpp : implementation of the CFrameGeneral class
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"

#include "FrameGeneral.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrameGeneral

IMPLEMENT_DYNCREATE(CFrameGeneral, CMDIChildWnd)

BEGIN_MESSAGE_MAP(CFrameGeneral, CMDIChildWnd)
	//{{AFX_MSG_MAP(CFrameGeneral)
	ON_WM_CREATE()
	ON_WM_CLOSE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrameGeneral construction/destruction

CFrameGeneral::CFrameGeneral()
{
}

CFrameGeneral::~CFrameGeneral()
{
}

BOOL CFrameGeneral::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIChildWnd::PreCreateWindow(cs) )
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CFrameGeneral diagnostics

#ifdef _DEBUG
void CFrameGeneral::AssertValid() const
{
	CMDIChildWnd::AssertValid();
}

void CFrameGeneral::Dump(CDumpContext& dc) const
{
	CMDIChildWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CFrameGeneral message handlers

int CFrameGeneral::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CMDIChildWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP
		| CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_GENERAL))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar, AFX_IDW_DOCKBAR_LEFT);
	
	return 0;
}

void CFrameGeneral::OnClose() 
{
	CView* pView = GetActiveView();
	if(pView && pView->m_hWnd)
		pView->SendMessage(WMU_CLOSING_FRAME, 0,0);

	CMDIChildWnd::OnClose();
}
