// Dialog2MinuteV2.cpp : implementation file
//

#include "stdafx.h"
#include "cornerstonefvmodel.h"
#include "Dialog2MinuteV2.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDialog2MinuteV2 dialog


CDialog2MinuteV2::CDialog2MinuteV2(CWnd* pParent /*=NULL*/)
	: CDialog(CDialog2MinuteV2::IDD, pParent)
{
	//{{AFX_DATA_INIT(CDialog2MinuteV2)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}


void CDialog2MinuteV2::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CDialog2MinuteV2)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CDialog2MinuteV2, CDialog)
	//{{AFX_MSG_MAP(CDialog2MinuteV2)
	ON_WM_CTLCOLOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDialog2MinuteV2 message handlers

HBRUSH CDialog2MinuteV2::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
	
		
	// TODO: Return a different brush if the default is not desired
	return hbr;
}
