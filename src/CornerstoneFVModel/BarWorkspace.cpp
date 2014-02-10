// BarWorkspace.cpp : implementation file
//

#include "stdafx.h"
#include "BarWorkspace.h"
#include "mainfrm.h"
#include "resource.h"

#include "DialogStocksAlphabetical.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBarWorkspace

CBarWorkspace::CBarWorkspace() 
{
	m_bMouseOnClose = false;
	m_bClosePressed = false;

	m_pDlg = NULL;
}

CBarWorkspace::~CBarWorkspace()
{
}


BEGIN_MESSAGE_MAP(CBarWorkspace, CSizingControlBar)
	//{{AFX_MSG_MAP(CBarWorkspace)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_DESTROY()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CBarWorkspace message handlers

int CBarWorkspace::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CSizingControlBar::OnCreate(lpCreateStruct) == -1)
		return -1;

	COLORMAP cm;
	cm.from = RGB(255,0,0);
	cm.to = GetSysColor(COLOR_BTNFACE);

	CBitmap bm;
	bm.LoadMappedBitmap(IDB_CLOSE, 0, &cm, 1);
	m_dcClose[0].CreateCompatibleDC(NULL);
	m_dcClose[0].SelectObject(&bm);
	bm.DeleteObject();

	bm.LoadMappedBitmap(IDB_CLOSE_ON, 0, &cm, 1);
	m_dcClose[1].CreateCompatibleDC(NULL);
	m_dcClose[1].SelectObject(&bm);
	bm.DeleteObject();

	bm.LoadMappedBitmap(IDB_CLOSE_PRESS, 0, &cm, 1);
	m_dcClose[2].CreateCompatibleDC(NULL);
	m_dcClose[2].SelectObject(&bm);
	bm.DeleteObject();

	m_pDlg = new CDialogStocksAlphabetical(NULL);
	m_pDlg->Create(IDD_DIALOG_STOCK_APLHABETICAL, this);
	m_pDlg->ShowWindow(SW_SHOW);

/*	m_pAnalisys = new CDialogAnalisys;
	m_pAnalisys->Create(IDD_DIALOG_ANALISYS, this);
	m_pAnalisys->ShowWindow(SW_HIDE);
*/	
	Resize();
	return 0;
}

void CBarWorkspace::OnSize(UINT nType, int cx, int cy) 
{
	CSizingControlBar::OnSize(nType, cx, cy);
	
	Resize();
}

void CBarWorkspace::Resize()
{
	CRect rc;
	GetClientRect(rc);
	int nDefTop = 5;

	if(m_pDlg)
		m_pDlg->MoveWindow(5, 20, rc.Width()-10, rc.Height()-20-10);

	Invalidate();
}

void CBarWorkspace::DrawCaption(CDC* pDC)
{
	CRect rc; GetClientRect(rc);

	pDC->Draw3dRect(7,7,rc.right-30,3,RGB(250,250,250),RGB(150,150,150));
	pDC->Draw3dRect(7,11,rc.right-30,3,RGB(250,250,250),RGB(150,150,150));

	pDC->BitBlt(rc.right-5-14, 4, 14,14, &m_dcClose[0], 0,0,SRCCOPY);
}

void CBarWorkspace::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
/*	CRect rc;
	GetClientRect(rc);
	dc.FillRect(rc, &CBrush(RGB(176,177,199)));*/

	if(!IsFloating())
		DrawCaption(&dc);
}

void CBarWorkspace::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_bClosePressed)
		return;

	CRect rc; GetClientRect(rc);
	CRect rcClose(rc.right-5-14, 4, rc.right-5, 4+14);
	if(rcClose.PtInRect(point))
	{
		if(!m_bMouseOnClose)
		{
			CClientDC dc(this);
			SetCapture();
			m_bMouseOnClose = true;
			dc.BitBlt(rc.right-5-14, 4, 14,14, &m_dcClose[1], 0,0,SRCCOPY);
		}
	}
	else
	{
		if(m_bMouseOnClose)
		{
			CClientDC dc(this);
			ReleaseCapture();
			m_bMouseOnClose = false;
			dc.BitBlt(rc.right-5-14, 4, 14,14, &m_dcClose[0], 0,0,SRCCOPY);
		}
	}
	
	CSizingControlBar::OnMouseMove(nFlags, point);
}

void CBarWorkspace::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CRect rc; GetClientRect(rc);
	CRect rcClose(rc.right-5-14, 4, rc.right-5, 4+14);
	if(rcClose.PtInRect(point))
	{
		if(!m_bClosePressed)
		{
			CClientDC dc(this);
			SetCapture();
			m_bClosePressed = true;
			dc.BitBlt(rc.right-5-14, 4, 14,14, &m_dcClose[2], 0,0,SRCCOPY);
			return;
		}
	}

	
	CSizingControlBar::OnLButtonDown(nFlags, point);
}

void CBarWorkspace::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if(m_bClosePressed)
	{
		ReleaseCapture();
		m_bClosePressed = false;	
		CRect rc; GetClientRect(rc);
		CRect rcClose(rc.right-5-14, 4, rc.right-5, 4+14);
		CClientDC dc(this);
		dc.BitBlt(rc.right-5-14, 4, 14,14, &m_dcClose[0], 0,0,SRCCOPY);

		if(rcClose.PtInRect(point))
		{
			CMainFrame* pFrame = (CMainFrame*)AfxGetMainWnd();
			if(!pFrame)
				return;
			ShowWindow(SW_HIDE);
			pFrame->RecalcLayout();
			return;
		}
	}	

	CSizingControlBar::OnLButtonUp(nFlags, point);
}

void CBarWorkspace::OnDestroy() 
{
	CSizingControlBar::OnDestroy();
	
	if(m_pDlg)
	{
		m_pDlg->DestroyWindow();
		delete m_pDlg;
	}
}
