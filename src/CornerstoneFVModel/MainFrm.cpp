// MainFrm.cpp : implementation of the CMainFrame class
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "DialogStocksAlphabetical.h"
#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CMainFrame

IMPLEMENT_DYNAMIC(CMainFrame, CMDIFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CMDIFrameWnd)
	//{{AFX_MSG_MAP(CMainFrame)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_COMMAND(ID_VIEW_WORKSPACE, OnViewWorkspace)
	ON_UPDATE_COMMAND_UI(ID_VIEW_WORKSPACE, OnUpdateViewWorkspace)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // status line indicator
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

/////////////////////////////////////////////////////////////////////////////
// CMainFrame construction/destruction

CMainFrame::CMainFrame()
{
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CMDIFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		  sizeof(indicators)/sizeof(UINT)))
	{
		TRACE0("Failed to create status bar\n");
		return -1;      // fail to create
	}

	if (!m_wndToolBar.CreateEx(this) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("Failed to create toolbar\n");
		return -1;      // fail to create
	}

	if (!m_wndGotoBar.Create(IDD_DIALOGBAR_GOTOTICKER, this 
		/*CBRS_ALIGN_TOP, AFX_IDW_DIALOGBAR*/))
	{
		TRACE0("Failed to create dialogbar\n");
		return -1;		// fail to create
	}

	if (!m_wndReBar.Create(this) ||
		!m_wndReBar.AddBar(&m_wndToolBar) ||
		!m_wndReBar.AddBar(&m_wndGotoBar))
	{
		TRACE0("Failed to create rebar\n");
		return -1;      // fail to create
	}

	DWORD style = m_wndToolBar.GetBarStyle() |CBRS_TOOLTIPS | CBRS_FLYBY;
	style &= ~CBRS_BORDER_3D;
	m_wndToolBar.SetBarStyle(style);

	EnableDocking(CBRS_ALIGN_ANY);

	// create workspace bar
	if(!m_barWorkspace.Create( this, CSize(330,80), 111))
	{
		TRACE0("Failed to create workspace bar\n");
		return -1;      // fail to create
	}
	m_barWorkspace.EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_barWorkspace, AFX_IDW_DOCKBAR_LEFT);
	m_barWorkspace.SetWindowText(_T("Workspace"));

	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CMDIFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// CMainFrame diagnostics

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CMDIFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CMDIFrameWnd::Dump(dc);
}

#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CMainFrame message handlers


void CMainFrame::OnSize(UINT nType, int cx, int cy) 
{
	CMDIFrameWnd::OnSize(nType, cx, cy);
	
	RecalcLayout();	
}

void CMainFrame::OnViewWorkspace() 
{
	BOOL bShow = !m_barWorkspace.IsWindowVisible();
	ShowControlBar(&m_barWorkspace, bShow, false);	
}

void CMainFrame::OnUpdateViewWorkspace(CCmdUI* pCmdUI) 
{
	pCmdUI->SetCheck(m_barWorkspace.IsWindowVisible());
}
