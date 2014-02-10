// DialogReportGeneral.cpp : implementation file
//

#include "stdafx.h"
#include "cornerstonefvmodel.h"
#include "DialogReportGeneral.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogReportGeneral dialog


CDialogReportGeneral::CDialogReportGeneral(SReportFilter* pFilter, CWnd* pParent /*=NULL*/)
	: CDialog(CDialogReportGeneral::IDD, pParent)
{
	m_pRepFilter = pFilter;

	//{{AFX_DATA_INIT(CDialogReportGeneral)
	m_bNoIndividualStocks = FALSE;
	m_fMCapMax = 1000000.0;
	m_fMCapMin = 0.0;
	m_sTitle = _T("");
	m_bExcel = FALSE;
	m_nGroup = 0;
	//}}AFX_DATA_INIT

	m_nSort = -1;
}


void CDialogReportGeneral::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogReportGeneral)
	DDX_Control(pDX, IDC_CHECK_SORT_BY, m_checkSort);
	DDX_Control(pDX, IDC_LIST_ITEMS, m_ListItems);
	DDX_Control(pDX, IDC_LIST_FILTER, m_ListFilter);
	DDX_Check(pDX, IDC_CHECK_NO_INDIVIDUAL_STOCKS, m_bNoIndividualStocks);
	DDX_Text(pDX, IDC_EDIT_MCAP_MAX, m_fMCapMax);
	DDX_Text(pDX, IDC_EDIT_MCAP_MIN, m_fMCapMin);
	DDX_Text(pDX, IDC_EDIT_TITLE, m_sTitle);
	DDX_Check(pDX, IDC_CHECK_EXPORT_TO_EXCEL, m_bExcel);
	DDX_Radio(pDX, IDC_RADIO_GROUP_1, m_nGroup);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogReportGeneral, CDialog)
	//{{AFX_MSG_MAP(CDialogReportGeneral)
	ON_BN_CLICKED(IDC_CHECK_SORT_BY, OnCheckSortBy)
	ON_LBN_SELCHANGE(IDC_LIST_ITEMS, OnSelchangeListItems)
	ON_EN_KILLFOCUS(IDC_EDIT_MIN, OnKillfocusEditMin)
	ON_EN_KILLFOCUS(IDC_EDIT_MAX, OnKillfocusEditMax)
	ON_BN_CLICKED(IDC_BUTTON_INC_ALL, OnButtonIncAll)
	ON_BN_CLICKED(IDC_BUTTON_EXC_ALL, OnButtonExcAll)
	ON_BN_CLICKED(IDC_BUTTON_INVERSE, OnButtonInverse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogReportGeneral message handlers

BOOL CDialogReportGeneral::OnInitDialog() 
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

	/////////////////////////////////////////////////////////////////////////////
	for(int i=0; i<m_pRepFilter->GetSize(); i++)
	{
		SReportFilterItem item = m_pRepFilter->GetAt(i);
		m_ListItems.AddString(item.szName);

		if(item.bInclude)
			m_ListItems.SetCheck(i, 1);
	}

	m_ListItems.SetCurSel(0);
	SetControls();

	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogReportGeneral::OnOK() 
{
	UpdateData();
	
	m_arFilter.RemoveAll();
	for(int i=0; i<m_ListFilter.GetCount(); i++)
		m_arFilter.Add(m_ListFilter.GetCheck(i));

	int nCheckedIndex = -1;
	for(int i=0; i<m_ListItems.GetCount(); i++)
	{
		BOOL check = m_ListItems.GetCheck(i);
		(*m_pRepFilter)[i].bInclude = check;

		if(check)
		{
			++nCheckedIndex;
			if(i == m_nSort)
				m_nSort = nCheckedIndex;
		}
	}

	if(CalcSelections() == 0)
	{
		AfxMessageBox("No items selected.", MB_ICONINFORMATION);
		return;
	}

	CDialog::OnOK();
}

void CDialogReportGeneral::OnCheckSortBy() 
{
	int nSel = m_ListItems.GetCurSel();
	if(nSel == -1)
		return;

	BOOL bCheck = m_ListItems.GetCheck(nSel);
	if(!bCheck)
	{
		AfxMessageBox("The item is not selected, so the Report cannot by sorted by this item. \n\nPlease include it in before clicking 'Sort By' check box.", MB_ICONINFORMATION);
		return;
	}

	m_nSort = nSel;
}

void CDialogReportGeneral::OnSelchangeListItems() 
{
	SetControls();
}

void CDialogReportGeneral::OnKillfocusEditMin() 
{
	int nSel = m_ListItems.GetCurSel();
	if(nSel == -1)
		return;

	CString s;
	GetDlgItem(IDC_EDIT_MIN)->GetWindowText(s);

	double f = atof(s);
	(*m_pRepFilter)[nSel].fMin = f;
}

void CDialogReportGeneral::OnKillfocusEditMax() 
{
	int nSel = m_ListItems.GetCurSel();
	if(nSel == -1)
		return;

	CString s;
	GetDlgItem(IDC_EDIT_MAX)->GetWindowText(s);

	double f = atof(s);
	(*m_pRepFilter)[nSel].fMax = f;
}

void CDialogReportGeneral::SetControls()
{
	int nSel = m_ListItems.GetCurSel();
	if(nSel == -1)
		return;

	if(m_nSort == nSel)
		m_checkSort.SetCheck(1);
	else
		m_checkSort.SetCheck(0);

	SReportFilterItem item = m_pRepFilter->GetAt(nSel);

	CString sMin; sMin.Format("%.2f", item.fMin);
	GetDlgItem(IDC_EDIT_MIN)->SetWindowText(sMin);

	CString sMax; sMax.Format("%.2f", item.fMax);
	GetDlgItem(IDC_EDIT_MAX)->SetWindowText(sMax);

	int nCount = CalcSelections();
	if(nCount > 18)
	{
		AfxMessageBox("You have selected too many items to be included in the report. For best viewing results maximum number of items must be 18.", MB_ICONINFORMATION);
	}

	CString s; s.Format("%u", nCount);
	GetDlgItem(IDC_STATIC_NUMBER)->SetWindowText(s);

	GetDlgItem(IDC_STATIC_MINMAX)->SetWindowText(item.szNameShort);
}

int CDialogReportGeneral::CalcSelections()
{
	int nCount = 0;
	for(int i=0; i<m_ListItems.GetCount(); i++)
	{
		BOOL check = m_ListItems.GetCheck(i);
		if(check)
			++nCount;
	}
	return nCount;
}

void CDialogReportGeneral::OnButtonIncAll() 
{
	for(int i=0; i<m_ListItems.GetCount(); i++)
		m_ListItems.SetCheck(i, 1);

	int nCount = CalcSelections();
	if(nCount > 18)
	{
		AfxMessageBox("You have selected too many items to be included in the report. For best viewing results maximum number of items must be 18.", MB_ICONINFORMATION);
	}

	CString s; s.Format("%u", nCount);
	GetDlgItem(IDC_STATIC_NUMBER)->SetWindowText(s);
}

void CDialogReportGeneral::OnButtonExcAll() 
{
	for(int i=0; i<m_ListItems.GetCount(); i++)
		m_ListItems.SetCheck(i, 0);

	int nCount = CalcSelections();
	if(nCount > 18)
	{
		AfxMessageBox("You have selected too many items to be included in the report. For best viewing results maximum number of items must be 18.", MB_ICONINFORMATION);
	}

	CString s; s.Format("%u", nCount);
	GetDlgItem(IDC_STATIC_NUMBER)->SetWindowText(s);
}

void CDialogReportGeneral::OnButtonInverse() 
{
	for(int i=0; i<m_ListItems.GetCount(); i++)
	{
		BOOL check = m_ListItems.GetCheck(i);
		if(check)
			m_ListItems.SetCheck(i, 0);
		else
			m_ListItems.SetCheck(i, 1);
	}

	int nCount = CalcSelections();
	if(nCount > 18)
	{
		AfxMessageBox("You have selected too many items to be included in the report. For best viewing results maximum number of items must be 18.", MB_ICONINFORMATION);
	}

	CString s; s.Format("%u", nCount);
	GetDlgItem(IDC_STATIC_NUMBER)->SetWindowText(s);
}
