// FrameMonteCarlo.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "FrameMonteCarlo.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CFrameMonteCarlo

IMPLEMENT_DYNCREATE(CFrameMonteCarlo, CMDIChildWnd)

CFrameMonteCarlo::CFrameMonteCarlo()
{
}

CFrameMonteCarlo::~CFrameMonteCarlo()
{
}


BEGIN_MESSAGE_MAP(CFrameMonteCarlo, CMDIChildWnd)
	//{{AFX_MSG_MAP(CFrameMonteCarlo)
	ON_WM_CREATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CFrameMonteCarlo message handlers

int CFrameMonteCarlo::OnCreate(LPCREATESTRUCT lpCreateStruct) 
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
