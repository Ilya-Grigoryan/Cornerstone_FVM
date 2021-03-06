// DialogReportEBITDALeverage.cpp : implementation file
//

#include "stdafx.h"
#include "cornerstonefvmodel.h"
#include "DialogReportEBITDALeverage.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogReportEBITDALeverage dialog


CDialogReportEBITDALeverage::CDialogReportEBITDALeverage(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogReportEBITDALeverage::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogReportEBITDALeverage)
	m_bNoIndividualStocks = FALSE;
	m_fMCapMax = 1000000.0;
	m_fMCapMin = 0.0;
	m_sTitle = _T("");
	m_nSort = 0;
	m_bExcel = FALSE;
	m_nGroup = 0;
	//}}AFX_DATA_INIT
}


void CDialogReportEBITDALeverage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogReportEBITDALeverage)
	DDX_Control(pDX, IDC_LIST_FILTER, m_ListFilter);
	DDX_Check(pDX, IDC_CHECK_NO_INDIVIDUAL_STOCKS, m_bNoIndividualStocks);
	DDX_Text(pDX, IDC_EDIT_MCAP_MAX, m_fMCapMax);
	DDX_Text(pDX, IDC_EDIT_MCAP_MIN, m_fMCapMin);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_sTitle);
	DDX_Radio(pDX, IDC_RADIO_SORT_1, m_nSort);
	DDX_Check(pDX, IDC_CHECK_EXPORT_TO_EXCEL, m_bExcel);
	DDX_Radio(pDX, IDC_RADIO_GROUP_1, m_nGroup);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogReportEBITDALeverage, CDialog)
	//{{AFX_MSG_MAP(CDialogReportEBITDALeverage)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogReportEBITDALeverage message handlers

BOOL CDialogReportEBITDALeverage::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	static char *szGroups[] = {
			"All Stocks",
			"c30 Stocks",
			"r1000v Stocks",
			"lcv Stocks",
			"sp500 Stocks",
			"bev Stocks",
			"c20 Stocks",
			"rmid Stocks",
			"r1000 Stocks",
			"r1000g Stocks"
	};

	for(int i=0; i<10; i++)
		m_ListFilter.AddString(szGroups[i]);
	m_ListFilter.SetCheck(0, 1);

	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogReportEBITDALeverage::OnOK() 
{
	UpdateData();
	
	m_arFilter.RemoveAll();
	for(int i=0; i<m_ListFilter.GetCount(); i++)
		m_arFilter.Add(m_ListFilter.GetCheck(i));

	CDialog::OnOK();
}
