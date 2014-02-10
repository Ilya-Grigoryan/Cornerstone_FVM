// DialogConnections.cpp : implementation file
//

#include "stdafx.h"
#include "cornerstonefvmodel.h"
#include "DialogConnections.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

extern CApp theApp;

/////////////////////////////////////////////////////////////////////////////
// CDialogConnections dialog


CDialogConnections::CDialogConnections(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogConnections::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogConnections)
	m_sLogin1 = _T("");
	m_sLogin2 = _T("");
	m_sLogin3 = _T("");
	m_sPassw1 = _T("");
	m_sPassw2 = _T("");
	m_sPassw3 = _T("");
	m_sServer1 = _T("");
	m_sServer2 = _T("");
	m_sServer3 = _T("");
	m_nDatabase = 0;
	//}}AFX_DATA_INIT
}


void CDialogConnections::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogConnections)
	DDX_Text(pDX, IDC_EDIT_LOGIN_1, m_sLogin1);
	DDX_Text(pDX, IDC_EDIT_LOGIN_2, m_sLogin2);
	DDX_Text(pDX, IDC_EDIT_LOGIN_3, m_sLogin3);
	DDX_Text(pDX, IDC_EDIT_PASSWORD_1, m_sPassw1);
	DDX_Text(pDX, IDC_EDIT_PASSWORD_2, m_sPassw2);
	DDX_Text(pDX, IDC_EDIT_PASSWORD_3, m_sPassw3);
	DDX_Text(pDX, IDC_EDIT_SERVER_1, m_sServer1);
	DDX_Text(pDX, IDC_EDIT_SERVER_2, m_sServer2);
	DDX_Text(pDX, IDC_EDIT_SERVER_3, m_sServer3);
	DDX_Radio(pDX, IDC_RADIO_DB, m_nDatabase);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogConnections, CDialog)
	//{{AFX_MSG_MAP(CDialogConnections)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogConnections message handlers

BOOL CDialogConnections::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	m_sServer1 = theApp.GetProfileString("Server Connections", "server1");
	m_sServer2 = theApp.GetProfileString("Server Connections", "server2");
	m_sServer3 = theApp.GetProfileString("Server Connections", "server3");

	m_sLogin1 = theApp.GetProfileString("Server Connections", "login1");
	m_sLogin2 = theApp.GetProfileString("Server Connections", "login2");
	m_sLogin3 = theApp.GetProfileString("Server Connections", "login3");
	
	m_sPassw1 = theApp.GetProfileString("Server Connections", "psw1");
	m_sPassw2 = theApp.GetProfileString("Server Connections", "psw2");
	m_sPassw3 = theApp.GetProfileString("Server Connections", "psw3");

	CString sDb = theApp.GetProfileString("Server Connections", "db");
	if(sDb.IsEmpty())
		m_nDatabase = 0;
	else
		m_nDatabase = atoi(sDb);

	if(m_sServer1.IsEmpty())
	{
		m_sServer1 = "cornerstone_dev";
		m_sLogin1 = "cip_client"; 
		m_sPassw1 = "cipclientresearch";
	}

	UpdateData(false);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogConnections::OnOK() 
{
	
	CDialog::OnOK();
}
