// DialogLogin.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "DialogLogin.h"
#include "DialogChangePassword.h"

#include "SQLUser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogLogin dialog


CDialogLogin::CDialogLogin(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogLogin::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogLogin)
	m_sName = _T("");
	m_sPassword = _T("");
	//}}AFX_DATA_INIT
	m_nUserId = -1;
}


void CDialogLogin::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);

	//{{AFX_DATA_MAP(CDialogLogin)
	DDX_Text(pDX, IDC_EDIT_NAME, m_sName);
	DDX_Text(pDX, IDC_EDIT_PASSWORD, m_sPassword);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogLogin, CDialog)
	//{{AFX_MSG_MAP(CDialogLogin)
	ON_BN_CLICKED(IDC_CHANGE_PASSWORD, OnChangePassword)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogLogin message handlers

void CDialogLogin::OnOK() 
{
	UpdateData();
	
	m_nUserId = CSQLUser::Login(m_sName, m_sPassword);
	if(m_nUserId < 0)
	{
		MessageBox("Invalid Name or Password. Please try again.", "Cornerstone Investment Partners", MB_ICONSTOP);
		return;
	}
	
	CDialog::OnOK();
}

void CDialogLogin::OnChangePassword() 
{
	CDialogChangePassword dlg;
	dlg.DoModal();
}
