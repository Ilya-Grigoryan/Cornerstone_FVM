// DialogSelectUniverse.cpp : implementation file
//

#include "stdafx.h"
#include "cornerstonefvmodel.h"
#include "DialogSelectUniverse.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogSelectUniverse dialog


CDialogSelectUniverse::CDialogSelectUniverse(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogSelectUniverse::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogSelectUniverse)
	m_nSel = -1;
	//}}AFX_DATA_INIT
}


void CDialogSelectUniverse::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogSelectUniverse)
	DDX_Radio(pDX, IDC_RADIO_1, m_nSel);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogSelectUniverse, CDialog)
	//{{AFX_MSG_MAP(CDialogSelectUniverse)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogSelectUniverse message handlers
