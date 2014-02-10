// Combo.cpp : implementation file
//

#include "stdafx.h"
#include "Combo.h"
#include "wndgrid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CCombo

CCombo::CCombo()
{
}

CCombo::~CCombo()
{
}

BOOL CCombo::Create(CWnd* pParent,BOOL bEdit)
{
	static UINT nID = 45778;
	BOOL bRes;
	if(bEdit)
		bRes = CComboBox::Create(CBS_DROPDOWN|CBS_DISABLENOSCROLL|CBS_AUTOHSCROLL, CRect(0,0,0,0), pParent, nID);
	else
		bRes = CComboBox::Create(CBS_DROPDOWNLIST|CBS_DISABLENOSCROLL, CRect(0,0,0,0), pParent, nID);

	SetFont(&((CWndGrid*)pParent)->m_DefaultFont);


	return bRes;
}
void CCombo::CreateCorrectSize(CStringArray* pArray)
{
	int     dx=0;
	CDC*    pDC = GetDC();
	CFont* pFont = GetFont();       
	CFont* oldFont = pDC -> SelectObject(pFont);
    	
	for(int i = 0; i < pArray->GetSize();i++)
	{
		CSize sz = pDC->GetTextExtent(pArray->GetAt(i));
		if (sz.cx > dx)      
			dx = sz.cx;
	}
	pDC -> SelectObject(oldFont);
	ReleaseDC(pDC);
	dx += ::GetSystemMetrics(SM_CXVSCROLL) + 2 * ::GetSystemMetrics(SM_CXEDGE);
	SetDroppedWidth(dx);
}
BOOL CCombo::Show(CWnd* pParent, CRect& rc, CStringArray* pArray, LPCTSTR szCurrent, BOOL bEdit)
{
	Create(pParent, bEdit);

	CreateCorrectSize(pArray);

	int nSel = -1;
	for(int i=0; i<pArray->GetSize(); i++)
	{
		CString s = pArray->GetAt(i);
		if(s==szCurrent)
			nSel = i;
		AddString(s);
	}
	SetCurSel(nSel);

	int nHeight = 40;
	if(pArray->GetSize())
	{
		int nH = GetItemHeight(0);
		nHeight = nH*(pArray->GetSize()+1);
	}

	if(bEdit)
		SetWindowText(szCurrent);

	MoveWindow(rc.left, rc.top, rc.Width(), nHeight+100); // raf 12/08/04
	ShowWindow(SW_SHOW);
	if(!bEdit)
		ShowDropDown();
	
	return true;
}


BEGIN_MESSAGE_MAP(CCombo, CComboBox)
	//{{AFX_MSG_MAP(CCombo)
	ON_CONTROL_REFLECT(CBN_SELCHANGE, OnSelchange)
	ON_WM_KILLFOCUS()
	ON_WM_LBUTTONDOWN()
	ON_CONTROL_REFLECT(CBN_SELENDCANCEL, OnSelendcancel)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCombo message handlers

void CCombo::OnSelchange() 
{
	CWnd* pParent = GetParent();
	int nIndex = GetCurSel();
	CString sSel;
	if(nIndex != -1)
		GetLBText(nIndex, sSel);

	GetParent()->SendMessage(WMU_SETTEXT, (WPARAM)sSel.GetBuffer(0), (LPARAM)true);
	sSel.ReleaseBuffer();
	ShowWindow(SW_HIDE);
}

void CCombo::OnKillFocus(CWnd* pNewWnd) 
{
	//CComboBox::OnKillFocus(pNewWnd);
	//ShowWindow(SW_HIDE);	
}

void CCombo::OnLButtonDown(UINT nFlags, CPoint point) 
{	
	CComboBox::OnLButtonDown(nFlags, point);
}

LRESULT CCombo::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	if(message==WM_KILLFOCUS)
	{
		ShowWindow(SW_HIDE);
		return 1;
	}
	return CComboBox::WindowProc(message, wParam, lParam);
}

void CCombo::OnSelendcancel() 
{
	CString sSel; GetWindowText(sSel);
	GetParent()->SendMessage(WMU_SETTEXT, (WPARAM)sSel.GetBuffer(0), (LPARAM)true);
	sSel.ReleaseBuffer();

	ShowWindow(SW_HIDE);
}
