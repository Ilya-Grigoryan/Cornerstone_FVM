// DialogNameAndTicker.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "DialogNameAndTicker.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogNameAndTicker dialog


CDialogNameAndTicker::CDialogNameAndTicker(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogNameAndTicker::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogNameAndTicker)
	m_sName = _T("");
	m_sTicker = _T("");
	//}}AFX_DATA_INIT
}


void CDialogNameAndTicker::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogNameAndTicker)
	DDX_Text(pDX, IDC_EDIT_NAME, m_sName);
	DDX_Text(pDX, IDC_EDIT_TICKER, m_sTicker);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogNameAndTicker, CDialog)
	//{{AFX_MSG_MAP(CDialogNameAndTicker)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogNameAndTicker message handlers

void CDialogNameAndTicker::OnOK() 
{
	UpdateData();
	
	if(m_sName.IsEmpty() || m_sTicker.IsEmpty())
	{
		AfxMessageBox("Please prove the Ticker and the Name.");
		return;
	}

	CDialog::OnOK();
}
