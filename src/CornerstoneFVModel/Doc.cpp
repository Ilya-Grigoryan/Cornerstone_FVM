// Doc.cpp : implementation of the CDoc class
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"

#include "Doc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CDoc

IMPLEMENT_DYNCREATE(CDoc, CDocument)

BEGIN_MESSAGE_MAP(CDoc, CDocument)
	//{{AFX_MSG_MAP(CDoc)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CDoc, CDocument)
	//{{AFX_DISPATCH_MAP(CDoc)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//      DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

static const IID IID_ICornerstoneFVModel =
{ 0xdbe8718d, 0x7465, 0x4457, { 0x95, 0xc8, 0x62, 0xc, 0x7b, 0xf4, 0xff, 0x45 } };

BEGIN_INTERFACE_MAP(CDoc, CDocument)
	INTERFACE_PART(CDoc, IID_ICornerstoneFVModel, Dispatch)
END_INTERFACE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CDoc construction/destruction

CDoc::CDoc()
{
	EnableAutomation();

	AfxOleLockApp();
}

CDoc::~CDoc()
{
	AfxOleUnlockApp();
}

BOOL CDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	return TRUE;
}



/////////////////////////////////////////////////////////////////////////////
// CDoc serialization

void CDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
	}
	else
	{
	}
}

/////////////////////////////////////////////////////////////////////////////
// CDoc diagnostics

#ifdef _DEBUG
void CDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CDoc commands
