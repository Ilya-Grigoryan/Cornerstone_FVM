// DialogName	.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "DialogName.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogName dialog


CDialogName::CDialogName(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogName::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogName)
	m_sName = _T("");
	//}}AFX_DATA_INIT
}


void CDialogName::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogName)
	DDX_Text(pDX, IDC_EDIT_NAME, m_sName);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogName, CDialog)
	//{{AFX_MSG_MAP(CDialogName)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogName message handlers

void CDialogName::OnOK() 
{
	UpdateData();	
	if(m_sName.IsEmpty())
	{
		AfxMessageBox("Please enter a name.");
		return;
	}

	CDialog::OnOK();
}

void CDialogName::OnCancel() 
{	
	CDialog::OnCancel();
}
