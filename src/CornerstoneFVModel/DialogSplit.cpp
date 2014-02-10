// DialogSplit.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "DialogSplit.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSplit dialog


CDialogSplit::CDialogSplit(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSplit::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSplit)
	m_nBefore = 0;
	m_nAfter = 0;
	//}}AFX_DATA_INIT
}


void CDialogSplit::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSplit)
	DDX_Text(pDX, IDC_EDIT_BEFORE, m_nBefore);
	DDX_Text(pDX, IDC_EDIT_AFTER, m_nAfter);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSplit, CDialog)
	//{{AFX_MSG_MAP(CDialogSplit)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSplit message handlers

BOOL CDialogSplit::OnInitDialog() 
{
	CDialog::OnInitDialog();
	
	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE unless you set the focus to a control
	              // EXCEPTION: OCX Property Pages should return FALSE
}

void CDialogSplit::OnOK() 
{
	UpdateData();
	if(m_nBefore <= 0 || m_nAfter <= 0)
	{
		AfxMessageBox("Please enter correct values for stock split.", MB_ICONINFORMATION);
		return;
	}
	m_fRate = (double)m_nAfter/(double)m_nBefore;

	CDialog::OnOK();
}
