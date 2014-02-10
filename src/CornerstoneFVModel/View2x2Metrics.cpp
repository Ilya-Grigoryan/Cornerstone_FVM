// View2x2Metrics.cpp : implementation file
//

#include "stdafx.h"
#include "cornerstonefvmodel.h"
#include "View2x2Metrics.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CView2x2Metrics

IMPLEMENT_DYNCREATE(CView2x2Metrics, CView)

CView2x2Metrics::CView2x2Metrics()
{
	m_fX = 50;
	m_fY = 50;

	m_nSizeX = 3;
	m_nSizeY = 3;

	m_bMouseOn = false;
}

CView2x2Metrics::~CView2x2Metrics()
{
}


BEGIN_MESSAGE_MAP(CView2x2Metrics, CView)
	//{{AFX_MSG_MAP(CView2x2Metrics)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_MOUSEACTIVATE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CView2x2Metrics drawing

void CView2x2Metrics::DrawCaptions(CDC* pDC, CRect rc)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(58,96,143));

	CBrush br(RGB(64,158,204));

	CPen* pOldPen = (CPen*) pDC->SelectObject(&pen);
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&br);
	CFont* pOldFont = (CFont*)pDC->SelectObject(&m_FontNormal);

	pDC->SetTextColor(RGB(255,255,255));
	pDC->SetBkMode(TRANSPARENT);

	// top headers
	CRect rcTop(rc.left+40, rc.top, rc.right, rc.top+20);
	pDC->Rectangle(rcTop);
	pDC->DrawText("Valuation", rcTop, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	CRect rcTop1(rc.left+60, rc.top+22, rc.left+60+80, rc.top+22+20);
	pDC->Rectangle(rcTop1);
	pDC->DrawText("Good", rcTop1, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	CRect rcTop2(rc.left+170, rc.top+22, rc.left+170+80, rc.top+22+20);
	pDC->Rectangle(rcTop2);
	pDC->DrawText("Bad", rcTop2, DT_CENTER|DT_VCENTER|DT_SINGLELINE);

	if(pOldFont)
		pDC->SelectObject(pOldFont);
	
	// left headers
	pOldFont = (CFont*)pDC->SelectObject(&m_FontVert);
	CRect rcLeft(rc.left, rc.top+40, rc.left+20, rc.bottom);
	pDC->Rectangle(rcLeft);
	pDC->TextOut(rcLeft.left+2, (rcLeft.top+rcLeft.bottom)/2+40, CString("Fundamentals"));
	
	CRect rcLeft1(rc.left+22, rc.top+55, rc.left+22+20, rc.top+55+70);
	pDC->Rectangle(rcLeft1);
	pDC->TextOut(rcLeft1.left+2, (rcLeft1.top+rcLeft1.bottom)/2+18, CString("Good"));

	CRect rcLeft2(rc.left+22, rc.top+150, rc.left+22+20, rc.top+150+70);
	pDC->Rectangle(rcLeft2);
	pDC->TextOut(rcLeft2.left+2, (rcLeft2.top+rcLeft2.bottom)/2+11, CString("Bad"));

	if(pOldPen)
		pDC->SelectObject(pOldPen);
	if(pOldBrush)
		pDC->SelectObject(pOldBrush);
	if(pOldFont)
		pDC->SelectObject(pOldFont);
}

void CView2x2Metrics::DrawPoint(CDC* pDC, CRect &rc)
{
	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(120,0,0));

	CBrush br(RGB(255,25,0));
	CPen* pOldPen = (CPen*) pDC->SelectObject(&pen);
	CBrush* pOldBrush = (CBrush*)pDC->SelectObject(&br);

	int x = (int)(40.0 + ( (double)m_fX/100.0) * (double)(rc.Width()) );
	int y = (int)(40.0 + ( (double)m_fY/100.0) * (double)(rc.Height()) );

	/*int dx,dy;
	switch(m_nSizeX)
	{
	case 1: dx = 26; break;
	case 2: dx = 22; break;
	case 3: dx = 18; break;
	case 4: dx = 14; break;
	case 5: dx = 10; break;
	}

	switch(m_nSizeY)
	{
	case 1: dy = 26; break;
	case 2: dy = 22; break;
	case 3: dy = 18; break;
	case 4: dy = 14; break;
	case 5: dy = 10; break;
	}*/

	int dx,dy;
	switch(m_nSizeX)
	{
	case 1: dx = 40; break;
	case 2: dx = 29; break;
	case 3: dx = 18; break;
	case 4: dx = 10; break;
	case 5: dx = 5; break;
	}

	switch(m_nSizeY)
	{
	case 1: dy = 40; break;
	case 2: dy = 29; break;
	case 3: dy = 18; break;
	case 4: dy = 10; break;
	case 5: dy = 5; break;
	}

	pDC->Ellipse(x-dx, y-dy, x+dx, y+dy);

	if(pOldPen)
		pDC->SelectObject(pOldPen);
	if(pOldBrush)
		pDC->SelectObject(pOldBrush);
} 

void CView2x2Metrics::DrawRects(CDC* pDC, CRect &rc)
{
	CPen* pOldPen = NULL;
	CBrush* pOldBrush = NULL;

	int nRX;
	int nRY;
	static COLORREF cColors[10] = 
	{	
		RGB(255,180,200), RGB(255,190,215), RGB(255,210,230), RGB(222,215,150), RGB(200,242,150), 
		RGB(160,222,140), RGB(120,255,120), RGB(50,255,50), RGB(0,222,0), RGB(0,180,0)
		
	};
	static double cRadius[10] = 
	{
		1.1, 0.8, 0.6, 0.5, 0.4, 0.3, 0.25, 0.2, 0.15, 0.1
	};

	// select the clipping region
	CRgn rgn;
	rgn.CreateRectRgn(rc.left+5, rc.top+5, rc.right-5, rc.bottom-3);
	pDC->SelectClipRgn(&rgn);

	pOldPen = (CPen*)pDC->SelectStockObject(NULL_PEN);
	int nCX = rc.left+4;
	int nCY = rc.top+4;

	pDC->FillSolidRect(rc.left+5, rc.top+5, rc.right-5, rc.bottom-3, RGB(255,140,160));

	double fRadius = 1.1;
	for(int i=0; i<10; i++) 
	{
		double fRad = cRadius[i];
		COLORREF color = cColors[i];

		nRX = (int)(fRad*(double)rc.Width()-2);
		nRY = (int)(fRad*(double)rc.Height()-2);

		CBrush br0(color);
		pOldBrush = (CBrush*)pDC->SelectObject(&br0);

		CRect rc10(nCX - nRX, nCY - nRY, nCX + nRX, nCY + nRY);
		pDC->Ellipse(rc10);
		if(pOldBrush)
			pDC->SelectObject(pOldBrush);
	}

	// select all as a clipping region
	pDC->SelectClipRgn(NULL);
	if(pOldPen)
		pDC->SelectObject(pOldPen);
	if(pOldBrush)
		pDC->SelectObject(pOldBrush);

	CPen pen;
	pen.CreatePen(PS_SOLID, 1, RGB(58,96,143));
	pOldPen = (CPen*) pDC->SelectObject(&pen);
	pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);

	CRect rc1(rc.left+4, rc.top+4, rc.left+rc.Width()/2-2, rc.top+rc.Height()/2-2); 
	CRect rc2(rc.left+rc.Width()/2+2, rc.top+4, rc.right-4, rc.top+rc.Height()/2-2); 
	CRect rc3(rc.left+4, rc.top+rc.Height()/2+2, rc.left+rc.Width()/2-2, rc.bottom-2);
	CRect rc4(rc.left+rc.Width()/2+2, rc.top+rc.Height()/2+2, rc.right-4, rc.bottom-2);
	
	pDC->Rectangle(rc1);
	pDC->Rectangle(rc2);
	pDC->Rectangle(rc3);
	pDC->Rectangle(rc4);

	if(pOldPen)
		pDC->SelectObject(pOldPen);
	if(pOldBrush)
		pDC->SelectObject(pOldBrush);
}

void CView2x2Metrics::Draw(CDC* pDC, CRect &rcClient)
{
	pDC->FillSolidRect(rcClient, RGB(255,255,255));
	DrawCaptions(pDC, rcClient);

	CRect rc(rcClient);
	rc.top += 40;
	rc.left += 40;

	DrawRects(pDC, rc);
	DrawPoint(pDC, rc);
}

void CView2x2Metrics::OnDraw(CDC* pDC)
{
	CRect rc;
	GetClientRect(rc);

	CDC dc;
	dc.CreateCompatibleDC(NULL);
	CBitmap bm;
	bm.CreateCompatibleBitmap(pDC, rc.right, rc.bottom);
	dc.SelectObject(&bm);

	Draw(&dc, rc);

	pDC->BitBlt(0,0,rc.right,rc.bottom,&dc,0,0,SRCCOPY);
}

void CView2x2Metrics::SetConfidenceLevels(int nValue, int nFund)
{
	if(nValue < 1)
		nValue = 1;
	if(nValue > 5)
		nValue = 5;

	if(nFund < 1)
		nFund = 1;
	if(nFund > 5)
		nFund = 5;

	m_nSizeX = nValue;
	m_nSizeY = nFund;

	Invalidate(false);
}

void CView2x2Metrics::SetConfidenceValue(int nValue)
{
	if(nValue < 1)
		nValue = 1;
	if(nValue > 5)
		nValue = 5;

	m_nSizeX = nValue;
	Invalidate(false);
}

void CView2x2Metrics::SetConfidenceFund(int nFund)
{
	if(nFund < 1)
		nFund = 1;
	if(nFund > 5)
		nFund = 5;

	m_nSizeY = nFund;
	Invalidate(false);
}

/////////////////////////////////////////////////////////////////////////////
// CView2x2Metrics diagnostics

#ifdef _DEBUG
void CView2x2Metrics::AssertValid() const
{
	CView::AssertValid();
}

void CView2x2Metrics::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CView2x2Metrics message handlers

void CView2x2Metrics::OnLButtonDown(UINT nFlags, CPoint point) 
{
	m_bMouseOn = true;
	m_ptOld = point;
	
	CView::OnLButtonDown(nFlags, point);
}

void CView2x2Metrics::OnLButtonUp(UINT nFlags, CPoint point) 
{
	m_bMouseOn = false;
	CView::OnLButtonUp(nFlags, point);
}

void CView2x2Metrics::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_bMouseOn)
	{
		CRect rc;
		GetClientRect(rc);

		int x = point.x;
		int y = point.y;

		m_fX = 100.0*(double)(x - 40) / (double) (rc.Width()-40);
		m_fY = 100.0*(double)(y - 40) / (double) (rc.Height()-40);
		

		Invalidate(false);
		m_ptOld = point;
	}
	
	CView::OnMouseMove(nFlags, point);
}

BOOL CView2x2Metrics::OnEraseBkgnd(CDC* pDC) 
{	
	return true; //CView::OnEraseBkgnd(pDC);
}

int CView2x2Metrics::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// fonts
	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	_tcscpy(lf.lfFaceName, _T("Arial"));
	lf.lfHeight = 13;
	m_FontNormal.CreateFontIndirect(&lf);

	lf.lfEscapement = 900;
	m_FontVert.CreateFontIndirect(&lf);
	
	return 0;
}

void CView2x2Metrics::OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView) 
{
	
	//CView::OnActivateView(bActivate, pActivateView, pDeactiveView);
}

int CView2x2Metrics::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{	
	return MA_NOACTIVATE;//CView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}
