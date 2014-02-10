// DialogLastReportYear.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "DialogLastReportYear.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogLastReportYear dialog


CDialogLastReportYear::CDialogLastReportYear(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogLastReportYear::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogLastReportYear)
	m_nYear = 0;
	m_nOption = -1;
	//}}AFX_DATA_INIT
}


void CDialogLastReportYear::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogLastReportYear)
	DDX_Text(pDX, IDC_EDIT_YEAR, m_nYear);
	DDX_Radio(pDX, IDC_RADIO_OPTION_1, m_nOption);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogLastReportYear, CDialog)
	//{{AFX_MSG_MAP(CDialogLastReportYear)
	ON_BN_CLICKED(IDC_RADIO_OPTION_1, OnRadioOption1)
	ON_BN_CLICKED(IDC_RADIO_OPTION_2, OnRadioOption2)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogLastReportYear message handlers

void CDialogLastReportYear::OnRadioOption1() 
{
	GetDlgItem(IDC_EDIT_YEAR)->EnableWindow(false);
	GetDlgItem(IDC_EDIT_YEAR)->SetWindowText("-1");
}

void CDialogLastReportYear::OnRadioOption2() 
{
	GetDlgItem(IDC_EDIT_YEAR)->EnableWindow(true);	
}

BOOL CDialogLastReportYear::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	if(m_nOption == 0)
		GetDlgItem(IDC_EDIT_YEAR)->EnableWindow(false);
	else
		GetDlgItem(IDC_EDIT_YEAR)->EnableWindow(true);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogLastReportYear::OnOK() 
{

	CDialog::OnOK();
}
