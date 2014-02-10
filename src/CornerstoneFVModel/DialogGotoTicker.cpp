// DialogGotoTicker.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "DialogGotoTicker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDialogGotoTicker dialog


CDialogGotoTicker::CDialogGotoTicker(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogGotoTicker::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogGotoTicker)
	m_sTicker = _T("");
	//}}AFX_DATA_INIT
}


void CDialogGotoTicker::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogGotoTicker)
	DDX_Control(pDX, IDC_EDIT_TICKER, m_editTicker);
	DDX_Text(pDX, IDC_EDIT_TICKER, m_sTicker);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogGotoTicker, CDialog)
	//{{AFX_MSG_MAP(CDialogGotoTicker)
	ON_BN_CLICKED(IDC_BUTTON_OPEN, OnButtonOpen)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogGotoTicker message handlers

BOOL CDialogGotoTicker::PreTranslateMessage(MSG* pMsg) 
{
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN)
	{
		CWnd* pFocus = GetFocus();
		if(pFocus && ::IsWindow(pFocus->m_hWnd) && pFocus->GetDlgCtrlID() == IDC_EDIT_TICKER)
		{
			CString sTicker;
			pFocus->GetWindowText(sTicker);
			int nIndex = theApp.m_Universe.GetIndexByTicker(sTicker, false);
			if(nIndex != -1)
			{
				CCompany* pCompany = theApp.m_Universe.GetCompany(nIndex);
				if(pCompany)
					theApp.NewViewGeneral(pCompany);
			}
		}
		return false;
	}
	
	return CDialog::PreTranslateMessage(pMsg);
}

void CDialogGotoTicker::OnButtonOpen() 
{
	CString sTicker;
	GetDlgItem(IDC_EDIT_TICKER)->GetWindowText(sTicker);
	int nIndex = theApp.m_Universe.GetIndexByTicker(sTicker, false);
	if(nIndex != -1)
	{
		CCompany* pCompany = theApp.m_Universe.GetCompany(nIndex);
		if(pCompany)
			theApp.NewViewGeneral(pCompany);
	}	
}
