// DialogDate.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "DialogDate.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogDate dialog


CDialogDate::CDialogDate(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogDate::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogDate)
	m_sD = _T("");
	m_sM = _T("");
	m_sY = _T("");
	//}}AFX_DATA_INIT
}


void CDialogDate::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogDate)
	DDX_Text(pDX, IDC_EDIT_D, m_sD);
	DDX_Text(pDX, IDC_EDIT_M, m_sM);
	DDX_Text(pDX, IDC_EDIT_Y, m_sY);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogDate, CDialog)
	//{{AFX_MSG_MAP(CDialogDate)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogDate message handlers

void CDialogDate::OnOK() 
{
	UpdateData();

	int nM = atoi(m_sM);
	int nD = atoi(m_sD);
	int nY = atoi(m_sY);

	m_Date = COleDateTime(nY, nM, nD, 0,0,0);
	if(m_Date.GetStatus() != COleDateTime::valid)
	{
		AfxMessageBox("Please enter a valid date");
		return;
	}
	
	CDialog::OnOK();
}

BOOL CDialogDate::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(m_Date.GetStatus() == COleDateTime::valid)
	{
		int nY = m_Date.GetYear();
		int nM = m_Date.GetMonth();
		int nD = m_Date.GetDay();

		m_sY.Format("%u", nY);
		m_sM.Format("%02u", nM);
		m_sD.Format("%02u", nD);

		UpdateData(false);
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}
