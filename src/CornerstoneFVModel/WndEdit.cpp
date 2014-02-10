// WndEdit.cpp : implementation file
//

#include "stdafx.h"
//#include "HPLDMEGrid.h"
#include "WndEdit.h"
#include "wndgrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CWndEdit

CWndEdit::CWndEdit()
{
}

CWndEdit::~CWndEdit()
{
}

BOOL CWndEdit::Show(CRect rc, LPCTSTR lpszText)
{
	SetWindowText(lpszText);
	MoveWindow(rc);
	ShowWindow(SW_SHOW);
	SetFocus();
//	SetCapture();

	return true;
}


BEGIN_MESSAGE_MAP(CWndEdit, CEdit)
	//{{AFX_MSG_MAP(CWndEdit)
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_WM_KEYDOWN()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CWndEdit message handlers

void CWndEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
/*	CRect rc;
	GetClientRect(rc);
	
	if(!rc.PtInRect(point))
	{
		char szText[500];
		GetWindowText(szText, 500);
		ReleaseCapture();
		GetParent()->SendMessage(WMU_SETTEXT, (WPARAM)szText, (LPARAM)true);
		ShowWindow(SW_HIDE);
	}
*/
	CEdit::OnLButtonDown(nFlags, point);
}

void CWndEdit::OnKillFocus(CWnd* pNewWnd) 
{
	CEdit::OnKillFocus(pNewWnd);
	ShowWindow(SW_HIDE);

	CWndGrid* pWnd = (CWndGrid*)GetParent();
	if(pWnd->m_bEditing)
	{
		ShowWindow(SW_HIDE);
		pWnd->m_bEditing = false;
		if(!pWnd->m_bLocked && pWnd->m_nActiveCellRow != -1 && pWnd->m_nActiveCellCol != -1)
		{
			CString sText; GetWindowText(sText);	
			pWnd->m_pCells[pWnd->m_nActiveCellRow*pWnd->m_nColCount+pWnd->m_nActiveCellCol].m_nType = CELL_TEXT;
			pWnd->m_pCells[pWnd->m_nActiveCellRow*pWnd->m_nColCount+pWnd->m_nActiveCellCol].m_sText = sText;
		}
		pWnd->Invalidate();
	}

	/*char szText[500];
	GetWindowText(szText, 500);
	ReleaseCapture();
	GetParent()->SendMessage(WMU_SETTEXT, (WPARAM)szText, (LPARAM)true);
	ShowWindow(SW_HIDE);*/
}

void CWndEdit::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags) 
{
	if(nChar == VK_RETURN)
	{
		TCHAR szText[500];
		GetWindowText(szText, 500);
		GetParent()->SendMessage(WMU_SETTEXT, (WPARAM)szText, (LPARAM)true);
		return;
	}

	if(nChar == VK_ESCAPE)
	{
		GetParent()->SendMessage(WMU_SETTEXT, (WPARAM)0, (LPARAM)false);
		return;
	}
	
	CEdit::OnKeyDown(nChar, nRepCnt, nFlags);
}

BOOL CWndEdit::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN)
	{
		int iii = 0;
	}

	return CEdit::PreTranslateMessage(pMsg);
}