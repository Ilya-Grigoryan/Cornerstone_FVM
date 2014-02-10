// ViewPercentile.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "ViewPercentile.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CViewPercentile

IMPLEMENT_DYNCREATE(CViewPercentile, CView)

CViewPercentile::CViewPercentile()
{
	m_sName = "Unknown";
	m_fPercentile = 0.0;

	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	strcpy(lf.lfFaceName, "Times New Roman");
	lf.lfHeight = 14;
	lf.lfWeight = 400;

	m_Font.CreateFontIndirect(&lf);
}

CViewPercentile::CViewPercentile(const CString& sName, double fPercent)
{
	m_sName = sName;
	m_fPercentile = fPercent;
}

CViewPercentile::~CViewPercentile()
{
}


BEGIN_MESSAGE_MAP(CViewPercentile, CView)
	//{{AFX_MSG_MAP(CViewPercentile)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewPercentile drawing

void CViewPercentile::OnDraw(CDC* pDC)
{
	static COLORREF palette[] = 
	{ 
		RGB(0,255,0), RGB(128,255,0), RGB(136,209,1), RGB(179,209,1), RGB(199,209,1), 
		RGB(255,255,0), RGB(208,162,2), RGB(209,111,1), RGB(209,48,1), RGB(255,0,0)
	};

	CRect rc;
	GetClientRect(rc);

	CBrush brBack(RGB(0,0,0));
	pDC->FillRect(rc, &brBack);
	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(255,255,255));

	//CBrush br(RGB(100,100,100));
	//pDC->FrameRect(rc, &br);

	//m_fPercentile = 0.79;

	//////////////////////////////////////////////
	int x0 = 15;
	int y0 = rc.bottom/2;
	int tick1 = 5;
	int tick2 = 2;

	//pDC->MoveTo(x0, y0);
	//pDC->LineTo(rc.right-x0, y0);

	CPen pen(PS_SOLID, 1, palette[0]);
	CPen* pOldPen = pDC->SelectObject(&pen);

	pDC->MoveTo(x0, y0-tick1);
	pDC->LineTo(x0, y0+tick1);

	int step = (rc.right-2*x0)/10;
	int nLength = step*10;
	for(int i=1; i<10; i++)
	{
		CPen pen(PS_SOLID, 1, palette[i]);
		pDC->SelectObject(&pen);

		int x = x0 + i*step;
		pDC->MoveTo(x-step, y0);
		pDC->LineTo(x, y0);

		pDC->MoveTo(x-step/2, y0-0);
		pDC->LineTo(x-step/2, y0+tick2);

		pDC->MoveTo(x, y0-tick2);
		pDC->LineTo(x, y0+tick2);
	}

	int x = x0 + 10*step;
	pDC->MoveTo(x-step, y0);
	pDC->LineTo(x, y0);
	pDC->MoveTo(x, y0-tick1);
	pDC->LineTo(x, y0+tick1);

	// output text
	CRect rcT(rc); rcT.top+=5;
	CFont* pOldFont = (CFont*)pDC->SelectObject(&m_Font);
	pDC->DrawText(m_sName, rcT, DT_CENTER);

	// calc the position of the arrow
	int nPos = (int)( (double)x0 + m_fPercentile * (double)nLength );

	int nSection = (int)(m_fPercentile*10.0);
	CPen pen2(PS_SOLID, 1, palette[nSection]);
	pDC->SelectObject(&pen2);

	// draw the arrow
	pDC->MoveTo(nPos, y0+tick2+2);
	pDC->LineTo(nPos-2, y0+tick2+2 + 4);
	pDC->LineTo(nPos+2, y0+tick2+2 + 4);
	pDC->LineTo(nPos, y0+tick2+2);

	// output percentage
	CString sPercent;
	sPercent.Format("%u%%", (int)((1.0-m_fPercentile)*100.0));

	pDC->SetTextColor(palette[nSection]);

	int nW = pDC->DrawText(sPercent, CRect(0,0,1,1), DT_LEFT|DT_CALCRECT);
	pDC->TextOut(nPos-nW/2, y0+tick2+4+2, sPercent);

	if(pOldPen)
			pDC->SelectObject(pOldPen);
	if(pOldFont)
		pDC->SelectObject(pOldFont);
}

/////////////////////////////////////////////////////////////////////////////
// CViewPercentile diagnostics

#ifdef _DEBUG
void CViewPercentile::AssertValid() const
{
	CView::AssertValid();
}

void CViewPercentile::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewPercentile message handlers
