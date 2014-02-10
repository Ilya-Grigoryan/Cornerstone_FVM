// DialogMapStep.cpp : implementation file
//

#include "stdafx.h"
#include "cornerstonefvmodel.h"
#include "DialogMapStep.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialogMapStep dialog


CDialogMapStep::CDialogMapStep(CWnd* pParent /*=NULL*/)
	: CDialog(CDialogMapStep::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialogMapStep)
	m_nStep = 0;
	//}}AFX_DATA_INIT
}


void CDialogMapStep::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialogMapStep)
	DDX_Text(pDX, IDC_EDIT_STEP, m_nStep);
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialogMapStep, CDialog)
	//{{AFX_MSG_MAP(CDialogMapStep)
		// NOTE: the ClassWizard will add message map macros here
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialogMapStep message handlers
