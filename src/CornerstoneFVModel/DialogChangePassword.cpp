// DialogChangePassword.cpp : implementation file
//

#include "stdafx.h"
#include "cornerstonefvmodel.h"
#include "DialogChangePassword.h"

#include "SQLUser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogChangePassword dialog


CDialogChangePassword::CDialogChangePassword(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogChangePassword::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogChangePassword)
	m_sName = _T("");
	m_sPwdOld = _T("");
	m_sPwd1 = _T("");
	m_sPwd2 = _T("");
	//}}AFX_DATA_INIT
}


void CDialogChangePassword::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogChangePassword)
	DDX_Text(pDX, IDC_EDIT_NAME, m_sName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD_OLD, m_sPwdOld);
	DDX_Text(pDX, IDC_EDIT_PASSWORD_1, m_sPwd1);
	DDX_Text(pDX, IDC_EDIT_PASSWORD_2, m_sPwd2);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogChangePassword, CDialog)
	//{{AFX_MSG_MAP(CDialogChangePassword)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogChangePassword message handlers

void CDialogChangePassword::OnOK() 
{
	UpdateData();
	if(m_sPwd1 != m_sPwd2)
	{
		MessageBox("Passwords don't match. Please try again.", "Cornerstone FV Model", MB_ICONSTOP);
		return;
	}

	if(m_sPwd1.IsEmpty())
	{
		MessageBox("The new Password cannot be empty. Please try again.", "Cornerstone FV Model", MB_ICONSTOP);
		return;
	}

	int n = CSQLUser::ChangePassword(m_sName, m_sPwdOld, m_sPwd1);
	if(n == -1)
	{
		MessageBox("Invalid User Name. Please enter a correct User Name/Password and try again.", "Cornerstone FV Model", MB_ICONSTOP);
		return;
	}

	if(n == -2)
	{
		MessageBox("Invalid Old Password. Please enter a Old Password and try again.", "Cornerstone FV Model", MB_ICONSTOP);
		return;
	}

	MessageBox("The password has been successfully changed.", "Cornerstone FV Model", MB_ICONINFORMATION);
	CDialog::OnOK();
}
