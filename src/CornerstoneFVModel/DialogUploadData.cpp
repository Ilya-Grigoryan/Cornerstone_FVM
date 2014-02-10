// DialogUploadData.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "DialogUploadData.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogUploadData dialog


CDialogUploadData::CDialogUploadData(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogUploadData::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogUploadData)
	m_sBS = _T("");
	m_sEPS = _T("");
	m_bForceBS = FALSE;
	m_bForceEPS = FALSE;
	m_sPrice = _T("");
	m_sRet = _T("");
	m_sShares = _T("");
	//}}AFX_DATA_INIT
}


void CDialogUploadData::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogUploadData)
	DDX_Text(pDX, IDC_EDIT_BS_FILE, m_sBS);
	DDX_Text(pDX, IDC_EDIT_EPS_FILE, m_sEPS);
	DDX_Check(pDX, IDC_CHECK_FORCE_BS, m_bForceBS);
	DDX_Check(pDX, IDC_CHECK_FORCE_EPS, m_bForceEPS);
	DDX_Text(pDX, IDC_EDIT_PR_FILE, m_sPrice);
	DDX_Text(pDX, IDC_EDIT_RET_FILE, m_sRet);
	DDX_Text(pDX, IDC_EDIT_SHARES_FILE, m_sShares);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogUploadData, CDialog)
	//{{AFX_MSG_MAP(CDialogUploadData)
	ON_BN_CLICKED(IDC_BUTTON_BS_BROWSE, OnButtonBsBrowse)
	ON_BN_CLICKED(IDC_BUTTON_PR_BROWSE, OnButtonPrBrowse)
	ON_BN_CLICKED(IDC_BUTTON_EPS_BROWSE, OnButtonEpsBrowse)
	ON_BN_CLICKED(IDC_BUTTON_RET_BROWSE, OnButtonRetBrowse)
	ON_BN_CLICKED(IDC_BUTTON_SHARES_BROWSE, OnButtonSharesBrowse)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogUploadData message handlers

void CDialogUploadData::OnButtonBsBrowse() 
{
	static char szFilters[] ="Excel Files (*.xls)|*.xls|All Files (*.*)|*.*||";

	CFileDialog fileDlg (true, "xls", "d_fun.xls", OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, szFilters, this);
	if(fileDlg.DoModal() != IDOK )
		return;

	GetDlgItem(IDC_EDIT_BS_FILE)->SetWindowText(fileDlg.GetPathName());
}

void CDialogUploadData::OnButtonPrBrowse() 
{
	static char szFilters[] ="Excel Files (*.xls)|*.xls|All Files (*.*)|*.*||";

	CFileDialog fileDlg (true, "xls", "d_prc.xls", OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, szFilters, this);
	if(fileDlg.DoModal() != IDOK )
		return;

	GetDlgItem(IDC_EDIT_PR_FILE)->SetWindowText(fileDlg.GetPathName());
}

void CDialogUploadData::OnButtonRetBrowse() 
{
	static char szFilters[] ="Excel Files (*.xls)|*.xls|All Files (*.*)|*.*||";

	CFileDialog fileDlg (true, "xls", "d_ret.xls", OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, szFilters, this);
	if(fileDlg.DoModal() != IDOK )
		return;

	GetDlgItem(IDC_EDIT_RET_FILE)->SetWindowText(fileDlg.GetPathName());
}


void CDialogUploadData::OnButtonEpsBrowse() 
{
	static char szFilters[] ="Excel Files (*.xls)|*.xls|All Files (*.*)|*.*||";

	CFileDialog fileDlg (true, "xls", "eps.xls", OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, szFilters, this);
	if(fileDlg.DoModal() != IDOK )
		return;

	GetDlgItem(IDC_EDIT_EPS_FILE)->SetWindowText(fileDlg.GetPathName());	
}

void CDialogUploadData::OnOK() 
{
	UpdateData();

	if( (!m_sPrice.IsEmpty() && m_sRet.IsEmpty()) || (m_sPrice.IsEmpty() && !m_sRet.IsEmpty()) )
	{
		AfxMessageBox("Prices and Returns must be downloaded together. Please provide file path for both (d_prc.xls, d_ret.xls)");
		return;
	}
	
	CDialog::OnOK();
}

void CDialogUploadData::OnButtonSharesBrowse() 
{
	static char szFilters[] ="Excel Files (*.xls)|*.xls|All Files (*.*)|*.*||";

	CFileDialog fileDlg (true, "xls", "SharesOut.xls", OFN_FILEMUSTEXIST|OFN_HIDEREADONLY, szFilters, this);
	if(fileDlg.DoModal() != IDOK )
		return;

	GetDlgItem(IDC_EDIT_SHARES_FILE)->SetWindowText(fileDlg.GetPathName());		
}
