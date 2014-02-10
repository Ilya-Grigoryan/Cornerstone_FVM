// ViewHTML.cpp : implementation file
//

#include "stdafx.h"
#include "cornerstonefvmodel.h"
#include "ViewHTML.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewHTML

IMPLEMENT_DYNCREATE(CViewHTML, CHtmlView)

CViewHTML::CViewHTML()
{
	//{{AFX_DATA_INIT(CViewHTML)
		// NOTE: the ClassWizard will add member initialization here
	//}}AFX_DATA_INIT
}

CViewHTML::~CViewHTML()
{
}

void CViewHTML::DoDataExchange(CDataExchange* pDX)
{
	CHtmlView::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CViewHTML)
		// NOTE: the ClassWizard will add DDX and DDV calls here
	//}}AFX_DATA_MAP
}


BEGIN_MESSAGE_MAP(CViewHTML, CHtmlView)
	//{{AFX_MSG_MAP(CViewHTML)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewHTML diagnostics

#ifdef _DEBUG
void CViewHTML::AssertValid() const
{
	CHtmlView::AssertValid();
}

void CViewHTML::Dump(CDumpContext& dc) const
{
	CHtmlView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewHTML message handlers
