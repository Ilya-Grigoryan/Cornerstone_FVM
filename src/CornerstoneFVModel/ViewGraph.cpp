// ViewGraph.cpp : implementation file
//

#include "stdafx.h"
#include "CornerstoneFVModel.h"
#include "ViewGraph.h"
#include <cmath>
using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CSeries::CSeries()
{
	m_nType = SERIES_TYPE_LINE;
	m_Color = RGB(0,0,0);
	m_nWidth =1;
	m_nStyle = PS_SOLID;
	m_sName = "Unknown";
	m_nSel = -1;
}

CSeries::~CSeries()
{
	
}

void CSeries::SetData(double* arX, double* arY, int nSize)
{
	m_arX.RemoveAll();
	m_arY.RemoveAll();

	m_arX.SetSize(nSize);
	memcpy(m_arX.GetData(), arX, nSize*sizeof(double));

	m_arY.SetSize(nSize);
	memcpy(m_arY.GetData(), arY, nSize*sizeof(double));
}

void CSeries::SetData(CArray<double,double>& arX, CArray<double,double>& arY)
{
	m_arX.RemoveAll();
	m_arY.RemoveAll();

	int nSize = arX.GetSize();

	m_arX.SetSize(nSize);
	memcpy(m_arX.GetData(), arX.GetData(), nSize*sizeof(double));

	m_arY.SetSize(nSize);
	memcpy(m_arY.GetData(), arY.GetData(), nSize*sizeof(double));
}


/////////////////////////////////////////////////////////////////////////////
// CViewGraph

IMPLEMENT_DYNCREATE(CViewGraph, CView)

CViewGraph::CViewGraph()
{
	// Default min max
	m_fMinX = m_fMinXData = _INF_;
	m_fMaxX = m_fMaxXData = -_INF_;

	m_fMinY = m_fMinYData = _INF_;
	m_fMaxY = m_fMaxYData = -_INF_;

	m_sLabelX = "Axis X";
	m_sLabelY = "Axis Y";

	m_bHeader = true;

	m_bFixedMinX = false;
	m_bFixedMaxX = false;
	m_bFixedMinY = false;
	m_bFixedMaxY = false;

	m_nTypeAxisX = AXIS_TYPE_DOUBLE;
	m_nTypeAxisY = AXIS_TYPE_DOUBLE;

	m_bMouseTrack = false;
	m_fMouseX = _INF_;

	m_ColorTableArray.Add(RGB(0,0,192));
	m_ColorTableArray.Add(RGB(0,192,0));		
	m_ColorTableArray.Add(RGB(192,0,0));

	m_ColorTableArray.Add(RGB(0,192,192));		
	m_ColorTableArray.Add(RGB(192,192,0));		
	m_ColorTableArray.Add(RGB(192,0,192));

	m_ColorTableArray.Add(RGB(0,0,0xff));
	m_ColorTableArray.Add(RGB(0,0xff,0));		
	m_ColorTableArray.Add(RGB(0xff,0,0));
	
	m_ColorTableArray.Add(RGB(0,0,128));		
	m_ColorTableArray.Add(RGB(0,128,0));		
	m_ColorTableArray.Add(RGB(128,0,0));

	m_ColorTableArray.Add(RGB(0xff,128,0));	
	m_ColorTableArray.Add(RGB(0xff,0,128));	
	m_ColorTableArray.Add(RGB(0xff,128,128));

	m_ColorTableArray.Add(RGB(192,0,0xff));	
	m_ColorTableArray.Add(RGB(0,192,0xff));	
	m_ColorTableArray.Add(RGB(192,192,0xff));
	
	m_ColorTableArray.Add(RGB(192,0xff,0));	
	m_ColorTableArray.Add(RGB(0,0xff,192));	
	m_ColorTableArray.Add(RGB(192,0xff,192));
	
	m_ColorTableArray.Add(RGB(0xff,192,0));	
	m_ColorTableArray.Add(RGB(0xff,0,192));	
	m_ColorTableArray.Add(RGB(0xff,192,192));
	
	m_ColorTableArray.Add(RGB(192,0xff,0xff));	
	m_ColorTableArray.Add(RGB(0xff,0xff,192));	
	m_ColorTableArray.Add(RGB(0xff,192,0xff));
}

CViewGraph::~CViewGraph()
{
}


BEGIN_MESSAGE_MAP(CViewGraph, CView)
	//{{AFX_MSG_MAP(CViewGraph)
	ON_WM_MOUSEACTIVATE()
	ON_WM_ERASEBKGND()
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CViewGraph drawing

void CViewGraph::OnDraw(CDC* pDC)
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

#define LINE_COLOR_0		RGB(100,100,100)
#define LINE_COLOR_1		RGB(190,190,190)
#define LINE_COLOR_2		RGB(220,220,220)

//////////////////////////////////////////////////////////////
void CViewGraph::DrawAxisX(CDC* pDC, CRect rc, CRect rcData)
{
	CFont* pOldFont = pDC->SelectObject(&m_FontNormal);
	pDC->SetBkMode(TRANSPARENT);

	CPen pen(PS_DOT, 1, LINE_COLOR_1);
	CPen* pOldPen = pDC->SelectObject(&pen);

	CPen pen2(PS_DOT, 1, LINE_COLOR_2);
	
	int nWidth = rc.Width();
	double fStep;
	GetTickSize(nWidth, 20, m_fMinXData, m_fMaxXData, fStep);

	if(!m_bFixedMinX)
	{
		int n1 = (int) floor(m_fMinXData/fStep) - 1;
		m_fMinX = n1*fStep;
	}

	if(!m_bFixedMaxX)
	{
		int n1 = (int) ceil(m_fMaxXData/fStep) + 1;
		m_fMaxX = n1*fStep;
	}
	
	double fRange = m_fMaxX-m_fMinX;

	int nStep = (int)( fStep * (double)nWidth / fRange );
	double fx = 0;
	int nTextEnd = -10000;
	while( !(fx > fRange) )
	{
		int x = rc.left + (int)( fx * (double)nWidth / fRange );
		
		CString s; 
		switch(m_nTypeAxisX)
		{
		case AXIS_TYPE_DOUBLE:	
			s.Format("%g", m_fMinX+fx);
			break;

		case AXIS_TYPE_PERCENT:
			s.Format("%.1f%%", (m_fMinX+fx)*100.0);
			break;

		case AXIS_TYPE_DATE:
			{
				COleDateTime date(m_fMinX+fx);
				s = date.Format("%m/%d/%y");
			}
			break;
		}
		int w = pDC->GetTextExtent(s).cx;

		pDC->MoveTo(x, rc.top+3);
		pDC->LineTo(x, rcData.top);

		pDC->SelectObject(pen2);

		int x2 = x+nStep/2;
		if(x2 <= rcData.right)
		{
			pDC->MoveTo(x2, rc.top+1);
			pDC->LineTo(x2, rcData.top);
		}
		 
		pDC->SelectObject(pen);
		
		int nTextS = x - w/2;
		if(nTextS > nTextEnd+3)
		{
			pDC->TextOut(x-w/2, rc.top+3, s); 
			nTextEnd = nTextS + w;
		}
		fx += fStep;
	}

	LOGFONT lf; 
	m_FontNormal.GetLogFont(&lf);

	pDC->DrawText(m_sLabelX, CRect(rc.left, rc.top+3+lf.lfHeight, rc.right, rc.bottom), DT_CENTER|DT_TOP);

	if(pOldPen)
		pDC->SelectObject(pOldPen);
	if(pOldFont)
		pDC->SelectObject(pOldFont);
}

//////////////////////////////////////////////////////////////
void CViewGraph::DrawAxisY(CDC* pDC, CRect rc, CRect rcData)
{
	CFont* pOldFont = pDC->SelectObject(&m_FontNormal);
	pDC->SetBkMode(TRANSPARENT);

	CPen pen(PS_DOT, 1, LINE_COLOR_1);
	CPen* pOldPen = pDC->SelectObject(&pen);

	CPen pen2(PS_DOT, 1, LINE_COLOR_2);

	int nHeight = rc.Height();
	double fStep;
	GetTickSize(nHeight, 20, m_fMinYData, m_fMaxYData, fStep);

	if(!m_bFixedMinY)
	{
		int n1 = (int) floor(m_fMinYData/fStep) - 1;
		m_fMinY = n1*fStep;
	}

	if(!m_bFixedMaxY)
	{
		int n1 = (int) ceil(m_fMaxYData/fStep) + 1;
		m_fMaxY = n1*fStep;
	}

	double fRange = m_fMaxY-m_fMinY;

	int nStep = (int)( fStep * (double)nHeight / fRange );
	double fy = 0;
	while( !(fy > fRange) )
	{
		int y = rc.bottom - (int)( fy * (double)nHeight / fRange );
		
		CString s; 
		switch(m_nTypeAxisY)
		{
		case AXIS_TYPE_DOUBLE:	
		case AXIS_TYPE_DATE:
			s.Format("%g", m_fMinY+fy);
			break;

		case AXIS_TYPE_PERCENT:
			s.Format("%.1f%%", (m_fMinY+fy)*100.0);
			break;
		}
		int w = pDC->GetTextExtent(s).cx;

		pDC->MoveTo(rc.right-3, y);
		pDC->LineTo(rcData.right, y);

		pDC->SelectObject(pen2);

		int y2 = y-nStep/2;
		if(y2 >= rcData.top)
		{
			pDC->MoveTo(rc.right-1, y2);
			pDC->LineTo(rcData.right, y2);
		}
		 
		pDC->SelectObject(pen);

		pDC->DrawText(s, CRect(rc.left, y-20, rc.right-3, y+20), DT_RIGHT|DT_VCENTER|DT_SINGLELINE);
		fy += fStep;
	}

	LOGFONT lf; 
	m_FontNormal.GetLogFont(&lf);

	int n = pDC->GetTextExtent(m_sLabelY).cx;
	pDC->SelectObject(&m_FontVert);

	pDC->TextOut(rc.left+3, rc.CenterPoint().y+n/2, m_sLabelY);

	if(pOldPen)
		pDC->SelectObject(pOldPen);
	if(pOldFont)
		pDC->SelectObject(pOldFont);
}

/////////////////////////////////////////////////////////////////
void CViewGraph::DrawLine(CDC* pDC, CRect rc, CLine* pLine)
{
	double fRangeX = m_fMaxX - m_fMinX;
	double fRangeY = m_fMaxY - m_fMinY;

	CPen pen(pLine->m_nStyle, pLine->m_nWidth, pLine->m_Color);
	CPen* pOldPen = pDC->SelectObject(&pen);

	LOGFONT lf; m_FontNormal.GetLogFont(&lf);
	if(pLine->m_nType == LINE_TYPE_HORZ)
	{
		int y = rc.bottom - (int)( (pLine->m_fPos-m_fMinY) * (double)rc.Height() / fRangeY );

		pDC->MoveTo(rc.left, y);
		pDC->LineTo(rc.right, y);

		CFont* pOldFont = pDC->SelectObject(&m_FontNormal);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(pLine->m_Color);
		
		if(pLine->m_nTextAlign == LINE_TEXT_LEFTBOTTOM)
		{
			pDC->TextOut(rc.left+3, y-lf.lfHeight, pLine->m_sText);
		}
		else
		{
			CRect r(rc.left+3, y-lf.lfHeight, rc.right-2, y+2);
			pDC->DrawText(pLine->m_sText, r, DT_RIGHT|DT_TOP);
		}

		if(pOldFont)
			pDC->SelectObject(pOldFont);

	}
	else if(pLine->m_nType == LINE_TYPE_VERT)
	{
		int x = rc.left + (int)( (pLine->m_fPos-m_fMinX) * (double)rc.Width() / fRangeX );

		pDC->MoveTo(x, rc.top);
		pDC->LineTo(x, rc.bottom);

		CFont* pOldFont = pDC->SelectObject(&m_FontNormal);
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(pLine->m_Color);

		if(pLine->m_nTextAlign == LINE_TEXT_LEFTBOTTOM)
		{
			pDC->TextOut(x+2, rc.bottom-lf.lfHeight, pLine->m_sText);
		}
		else
		{
			pDC->TextOut(x+2, rc.top+2, pLine->m_sText);
		}

		if(pOldFont)
			pDC->SelectObject(pOldFont);
	}

	if(pOldPen)
		pDC->SelectObject(pOldPen);
}

void CViewGraph::DrawRect(CDC* pDC, CRect rc, CRc* pRect)
{
	double fRangeX = m_fMaxX - m_fMinX;
	double fRangeY = m_fMaxY - m_fMinY;

	int left = rc.left + (int)( (pRect->m_fLeft-m_fMinX) * (double)rc.Width() / fRangeX );
	int right = rc.left + (int)( (pRect->m_fRight-m_fMinX) * (double)rc.Width() / fRangeX );
	int top = rc.bottom - (int)( (pRect->m_fTop-m_fMinY) * (double)rc.Height() / fRangeY );
	int bottom = rc.bottom - (int)( (pRect->m_fBottom-m_fMinY) * (double)rc.Height() / fRangeY );

	pDC->FillSolidRect(left, top, (right-left), (bottom-top), pRect->m_Color);
}

/////////////////////////////////////////////////////////////////
void CViewGraph::DrawSeries(CDC* pDC, CRect rc, CSeries* pSeries)
{
	if(pSeries->m_arX.GetSize() < 1)
		return;

	double fRangeX = m_fMaxX - m_fMinX;
	double fRangeY = m_fMaxY - m_fMinY;

	int nStart = 0;
	while(nStart < pSeries->m_arX.GetSize() && (pSeries->m_arX[nStart] >= _INF_ || pSeries->m_arY[nStart] >= _INF_) )
		++nStart;

	if(nStart >= pSeries->m_arX.GetSize())  // all data points were unknown
		return;

	double fxOld = pSeries->m_arX[nStart];
	double fyOld = pSeries->m_arY[nStart];
	int xOld = rc.left + (int)( (fxOld-m_fMinX) * (double)rc.Width() / fRangeX );
	int yOld = rc.bottom - (int)( (fyOld-m_fMinY) * (double)rc.Height() / fRangeY );
	
	++nStart;
	int x, y;
	switch(pSeries->m_nType)
	{
	case SERIES_TYPE_PT_COLORED:
		{
			CPen* pOldPen = (CPen*)pDC->SelectStockObject(NULL_PEN);

			int nSize = pSeries->m_arX.GetSize();
			for(int i=0; i<nSize; i++)
			{
				double fx = pSeries->m_arX[i];
				double fy = pSeries->m_arY[i];

				if(fx < _INF_ && fy < _INF_)
				{
					x = rc.left + (int)( (fx-m_fMinX) * (double)rc.Width() / fRangeX );
					y = rc.bottom - (int)( (fy-m_fMinY) * (double)rc.Height() / fRangeY );

					// points
					int colorIndex = i % m_ColorTableArray.GetSize();
					COLORREF color = m_ColorTableArray[colorIndex];

					CBrush br(color);
					CBrush* pOldBrush = pDC->SelectObject(&br);

					static int nW = 7;
					pDC->Ellipse(x-nW, y-nW, x+nW, y+nW);	

					CFont* pOldFont = pDC->SelectObject(&m_FontBold);
					pDC->SetTextColor(color);
					pDC->DrawText(m_arLabels[i], CRect(x-100, y+nW, x+100, y+nW+30), DT_CENTER|DT_SINGLELINE|DT_TOP);
					
					if(pOldBrush)
						pDC->SelectObject(pOldBrush);
					if(pOldFont)
						pDC->SelectObject(pOldFont);

					// selection
					if(i == pSeries->m_nSel)
					{
						static int nW = 5;
						pDC->Ellipse(x-nW, y-nW, x+nW, y+nW);	
					}
				}
			}

			if(pOldPen)
				pDC->SelectObject(pOldPen);
		}
		break;

	case SERIES_TYPE_LINE:
	case SERIES_TYPE_PT_LINE:
	case SERIES_TYPE_PT_LINE_1:
		{
			CPen pen(pSeries->m_nStyle, pSeries->m_nWidth, pSeries->m_Color);
			CPen* pOldPen = pDC->SelectObject(&pen);

			int nSize = pSeries->m_arX.GetSize();
			for(int i=nStart; i<nSize; i++)
			{
				double fx = pSeries->m_arX[i];
				double fy = pSeries->m_arY[i];

				x = rc.left + (int)( (fx-m_fMinX) * (double)rc.Width() / fRangeX );
				y = rc.bottom - (int)( (fy-m_fMinY) * (double)rc.Height() / fRangeY );

				if(fx < _INF_ && fy < _INF_ && fxOld < _INF_ && fyOld < _INF_)
				{
					pDC->MoveTo(xOld, yOld);
					pDC->LineTo(x, y);

					// points
					if(pSeries->m_nType == SERIES_TYPE_PT_LINE)
					{
						CBrush br(pSeries->m_Color);
						CBrush* pOldBrush = pDC->SelectObject(&br);

						static int nW = 3;
						pDC->Ellipse(xOld-nW, yOld-nW, xOld+nW, yOld+nW);
						pDC->Ellipse(x-nW, y-nW, x+nW, y+nW);	
						
						if(pOldBrush)
							pDC->SelectObject(pOldBrush);
					}
					else if(pSeries->m_nType == SERIES_TYPE_PT_LINE_1)
					{
						CBrush br(pSeries->m_Color);
						//br.CreateHatchBrush(HS_CROSS, pSeries->m_Color);
						CBrush* pOldBrush = pDC->SelectObject(&br);

						CPen pen(PS_SOLID, 1, RGB(0,0,0));
						CPen* pOldPen = pDC->SelectObject(&pen);

						static int nMaxPt = 10;
						int nPeriod = pSeries->m_arX.GetSize()/nMaxPt;
						if(nPeriod == 0)
							nPeriod = 1;

						int nWO = (nSize-(i-1)-1)/nPeriod+2; 
						int nW = (nSize-i-1)/nPeriod+2;
						pDC->Ellipse(xOld-nWO, yOld-nWO, xOld+nWO, yOld+nWO);
						pDC->Ellipse(x-nW, y-nW, x+nW, y+nW);
						
						if(i==nStart || i==nStart+1 || i==nStart+2)
						{
							CString sT = (i==nStart) ? "1" : (i==nStart+1) ? "2" : "3";
							CFont* pOldFont = pDC->SelectObject(&m_FontBold);
							pDC->SetTextColor(RGB(255,255,255));
							pDC->DrawText(sT, CRect(xOld-nWO, yOld-nWO, xOld+nWO, yOld+nWO), DT_CENTER|DT_SINGLELINE|DT_VCENTER);
							if(pOldFont)
								pDC->SelectObject(pOldFont);
						}
						
						if(pOldBrush)
							pDC->SelectObject(pOldBrush);
						if(pOldPen)
							pDC->SelectObject(pOldPen);
					}

					// selection
					if(i == pSeries->m_nSel)
					{
						CBrush br(pSeries->m_Color);
						CBrush* pOldBrush = pDC->SelectObject(&br);

						static int nW = 5;
						pDC->Ellipse(x-nW, y-nW, x+nW, y+nW);	
						
						if(pOldBrush)
							pDC->SelectObject(pOldBrush);
					}
				}

				fxOld = fx;
				fyOld = fy;
				xOld = x;
				yOld = y;
			}

			if(pOldPen)
				pDC->SelectObject(pOldPen);
		}
		break;

	case SERIES_TYPE_BAR:
		{
			CPen pen(PS_SOLID, 1, RGB(0,116,166));
			CPen* pOldPen = pDC->SelectObject(&pen);

			CBrush br(pSeries->m_Color);
			CBrush* pOldBrush = pDC->SelectObject(&br);
			
			int xMin = rc.left + (int)( (m_fMinXData-m_fMinX) * (double)rc.Width() / fRangeX );
			int xMax = rc.left + (int)( (m_fMaxXData-m_fMinX) * (double)rc.Width() / fRangeX );
			int nW = (xMax-xMin) / (pSeries->m_arX.GetSize());

			int xOld = (int)_INF_;
			for(int i=0; i<pSeries->m_arX.GetSize(); i++)
			{
				double fx = pSeries->m_arX[i];
				double fy = pSeries->m_arY[i];

				if(fx < _INF_ && fy < _INF_)
				{
					int x = rc.left + (int)( (fx-m_fMinX) * (double)rc.Width() / fRangeX );
					int y = rc.bottom - (int)( (fy-m_fMinY) * (double)rc.Height() / fRangeY );

					int x1 = x-nW/2;
					int x2 = x+nW/2;

					if(xOld != (int)_INF_ && x1 != xOld-1)
						x1 = xOld-1;

					pDC->Rectangle(x1, y, x2, rc.bottom);

					xOld = x2;
				}
			}

			if(pOldPen)
				pDC->SelectObject(pOldPen);
			if(pOldBrush)
				pDC->SelectObject(pOldBrush);
		}
		break;
	}

}

/////////////////////////////////////////////////////////////////
void CViewGraph::Draw(CDC* pDC, CRect rc)
{
	CalcMargins();

	// background
	pDC->FillSolidRect(rc, LINE_COLOR_2);

	// chart frame
	CBrush br0(LINE_COLOR_0);
	pDC->FrameRect(rc, &br0);

	// data area
	CRect rcGraph(rc);
	rcGraph.DeflateRect(m_nMarginLeft, m_nMarginTop, m_nMarginRight, m_nMarginBottom);
	pDC->FillSolidRect(rcGraph, RGB(255,255,255));

	int nWidth = rcGraph.Width();
	int nHeight = rcGraph.Height();
	double fStep;
	GetTickSize(nWidth, 20, m_fMinXData, m_fMaxXData, fStep);
	GetTickSize(nHeight, 20, m_fMinYData, m_fMaxYData, fStep);
	if(!m_bFixedMinX)
	{
		int n1 = (int) floor(m_fMinXData/fStep) - 1;
		m_fMinX = n1*fStep;
	}
	if(!m_bFixedMaxX)
	{
		int n1 = (int) ceil(m_fMaxXData/fStep) + 1;
		m_fMaxX = n1*fStep;
	}
	if(!m_bFixedMinY)
	{
		int n1 = (int) floor(m_fMinYData/fStep) - 1;
		m_fMinY = n1*fStep;
	}

	if(!m_bFixedMaxY)
	{
		int n1 = (int) ceil(m_fMaxYData/fStep) + 1;
		m_fMaxY = n1*fStep;
	}

	// frame rect
	CBrush br1(LINE_COLOR_1);
	pDC->FrameRect(rcGraph, &br1);

	// Rects
	for(int i=0; i<m_arRects.GetSize(); i++)
		DrawRect(pDC, rcGraph, m_arRects[i]);

	// Axes
	CRect rcAxisX(m_nMarginLeft, rc.bottom-m_nMarginBottom, rc.right-m_nMarginRight, rc.bottom);
	DrawAxisX(pDC, rcAxisX, rcGraph);

	CRect rcAxisY(rc.left, rc.top+m_nMarginTop, rc.left+m_nMarginLeft, rc.bottom-m_nMarginBottom);
	DrawAxisY(pDC, rcAxisY, rcGraph);

	// Series
	for(int i=0; i<m_arSeries.GetSize(); i++)
		DrawSeries(pDC, rcGraph, m_arSeries[i]);

	// Lines
	for(int i=0; i<m_arLines.GetSize(); i++)
		DrawLine(pDC, rcGraph, m_arLines[i]);

	// header
	if(m_bHeader)
		DrawHeader(pDC, rcGraph);

	// tracking line
	if(m_bMouseTrack && m_fMouseX < _INF_)
	{
		CLine line;
		line.m_nType = LINE_TYPE_VERT;
		line.m_nWidth = 1;
		line.m_Color = RGB(100,100,100);
		line.m_nStyle = PS_SOLID;
		line.m_nTextAlign = LINE_TEXT_TOPRIGHT;
		if(m_nTypeAxisX == AXIS_TYPE_DATE)
			line.m_sText = COleDateTime(m_fMouseX).Format("%m/%d/%Y");
		line.m_fPos = m_fMouseX;
		DrawLine(pDC, rcGraph, &line);
	}
}

void CViewGraph::DrawHeader(CDC* pDC, CRect rc)
{
	pDC->SetBkMode(TRANSPARENT);

	LOGFONT lf; m_FontNormal.GetLogFont(&lf);
	CFont* pOldFont = pDC->SelectObject(&m_FontNormal);
	int d = lf.lfHeight-2;

	int nLeft = rc.left+2;
	for(int i=0; i<m_arSeries.GetSize(); i++)
	{
		pDC->SetTextColor(m_arSeries[i]->m_Color);

		pDC->FillSolidRect(nLeft, rc.top-d, d-3, d-3, m_arSeries[i]->m_Color);
		nLeft += d-1;

		CString s = m_arSeries[i]->m_sName;
		int nW = pDC->GetTextExtent(s).cx;

		pDC->TextOut(nLeft, rc.top-lf.lfHeight, s);
		nLeft += nW+7;
	}
	
	if(pOldFont)
		pDC->SelectObject(pOldFont);
}


double niceround(double f1, double f2)
{
	double factor = f1/f2;

	double f;
	if(factor <= 0.5)
		f = 0.5*f2;
	else if(factor <= 1.0)
		f = f2;
	else if(factor <= 1.5)
		f = 1.5*f2;
	else if(factor <= 2.0)
		f = 2.0*f2;
	else if(factor <= 3.0)
		f = 3.0*f2;
	else if(factor <= 5.0)
		f = 5.0*f2;
	else
		f = 10.0*f2;

	return f;
}

void CViewGraph::GetTickSize(int nWidth, int nBlock, double fMin, double fMax, double &fStep)
{
	double count = (double)nWidth / (double)nBlock;
	double range = fMax - fMin;
	double step0 = range/count;

	int n1 = (int) log10(step0);
	double f1 = pow(10, n1);
	
	fStep = niceround(step0, f1);
}

void CViewGraph::SetFont(LOGFONT &lf)
{
	if(m_FontNormal.m_hObject)
		m_FontNormal.DeleteObject();
	if(m_FontVert.m_hObject)
		m_FontVert.DeleteObject();

	m_FontNormal.CreateFontIndirect(&lf);

	lf.lfEscapement = -900;
	m_FontVert.CreateFontIndirect(&lf);
}

void CViewGraph::CalcMargins()
{
	LOGFONT lf;
	m_FontNormal.GetLogFont(&lf);

	m_nMarginBottom = (int)(2.5 * lf.lfHeight);
	m_nMarginTop = (int)(2 * lf.lfHeight);
	m_nMarginRight = 10;

	m_nMarginLeft = (int)(4 * lf.lfHeight);
/*	CDC* pDC = GetDC();
	CFont* pOldFont = pDC->SelectObject(&m_FontNormal);
	
	CString s1, s2;
	s1.Format("%g", m_fMinY);
	s2.Format("%g", m_fMaxY);

	int n1 = pDC->GetTextExtent(s1).cx;
	int n2 = pDC->GetTextExtent(s2).cx;
	m_nMarginLeft = max(n1, n2) + (int)(1.5*lf.lfHeight);

	if(pOldFont)
		pDC->SelectObject(pOldFont);
	ReleaseDC(pDC);*/
}

/////////////////////////////////////////////////////////////////////////////
// CViewGraph diagnostics

#ifdef _DEBUG
void CViewGraph::AssertValid() const
{
	CView::AssertValid();
}

void CViewGraph::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}
#endif //_DEBUG

/////////////////////////////////////////////////////////////////////////////
// CViewGraph message handlers

int CViewGraph::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message) 
{
	return MA_NOACTIVATE; //CView::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

BOOL CViewGraph::OnEraseBkgnd(CDC* pDC) 
{	
	return true;//CView::OnEraseBkgnd(pDC);
}

int CViewGraph::OnCreate(LPCREATESTRUCT lpCreateStruct) 
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	// fonts
	LOGFONT lf;
	memset(&lf, 0, sizeof(lf));
	_tcscpy(lf.lfFaceName, _T("Arial"));
	lf.lfHeight = 12;
	m_FontNormal.CreateFontIndirect(&lf);

	lf.lfEscapement = 900;
	m_FontVert.CreateFontIndirect(&lf);

	_tcscpy(lf.lfFaceName, _T("Arial"));
	lf.lfHeight = 15;
	lf.lfWeight = 600;
	lf.lfEscapement = 0;
	m_FontBold.CreateFontIndirect(&lf);

	// margins
	m_nMarginLeft = m_nMarginBottom = (int)(2.5 * lf.lfHeight);
	m_nMarginRight = m_nMarginTop = (int)(1.5 * lf.lfHeight);

	return 0;
}

void CViewGraph::OnDestroy() 
{
	CView::OnDestroy();
	
	RemoveAll();	
}

void CViewGraph::RemoveAll()
{
	for(int i=0; i<m_arSeries.GetSize(); i++)
		delete m_arSeries[i];
	m_arSeries.RemoveAll();

	for(int i=0; i<m_arLines.GetSize(); i++)
		delete m_arLines[i];
	m_arLines.RemoveAll();

	for(int i=0; i<m_arRects.GetSize(); i++)
		delete m_arRects[i];
	m_arRects.RemoveAll();


	// Default min max
	m_fMinX = m_fMinXData = _INF_;
	m_fMaxX = m_fMaxXData = -_INF_;

	m_fMinY = m_fMinYData = _INF_;
	m_fMaxY = m_fMaxYData = -_INF_;

}

void CViewGraph::AddLine(double pos, const char* szText, int textAlign, int type, int width, int style, COLORREF color)
{
	CLine* pLine = new CLine;
	pLine->m_fPos = pos;
	pLine->m_sText = szText;
	pLine->m_nTextAlign = textAlign;

	pLine->m_nType = type;
	pLine->m_Color = color;
	pLine->m_nWidth = width;
	pLine->m_nStyle = style;

	m_arLines.Add(pLine);

	if(type==LINE_TYPE_HORZ)
	{
		if(pos < m_fMinYData)
			m_fMinYData = pos;
		if(pos > m_fMaxYData)
			m_fMaxYData = pos;
	} 
	else
	{
		if(pos < m_fMinXData)
			m_fMinXData = pos;
		if(pos > m_fMaxXData)
			m_fMaxXData = pos;
	}
}

void CViewGraph::AddRect(double left, double top, double right, double bottom, COLORREF color)
{
	CRc *pRect = new CRc;

	pRect->m_fLeft = left;
	pRect->m_fTop = top;
	pRect->m_fRight = right;
	pRect->m_fBottom = bottom;

	pRect->m_Color = color;

	if(left < m_fMinXData)
		m_fMinXData = left;
	if(left > m_fMaxXData)
		m_fMaxXData = left;

	if(right < m_fMinXData)
		m_fMinXData = right;
 	if(right > m_fMaxXData)
		m_fMaxXData = right;

	if(top < m_fMinYData)
		m_fMinYData = top;
	if(top > m_fMaxYData)
		m_fMaxYData = top;

	if(bottom < m_fMinYData)
		m_fMinYData = bottom;
	if(bottom > m_fMaxYData)
		m_fMaxYData = bottom;

	m_arRects.Add(pRect);
}

void CViewGraph::AddSeries(	const char* szName, double* arX, double* arY, int nSize, 
							int type, int width, int style, COLORREF color)
{
	if(nSize < 1)
		return;

	CSeries* pSeries = new CSeries;
	pSeries->m_sName = szName;
	pSeries->m_nType = type;
	pSeries->m_Color = color;
	pSeries->m_nWidth = width;
	pSeries->m_nStyle = style;

	pSeries->SetData(arX, arY, nSize);
	m_arSeries.Add(pSeries);

	double fMinX = _INF_, fMaxX = -_INF_, fMinY = _INF_, fMaxY = -_INF_;
	for(int i=0; i<nSize; i++)
	{
		double x = arX[i];
		if(x < (double)_INF_)
		{
			if(x < fMinX)
				fMinX = x;
			if(x > fMaxX)
				fMaxX = x;
		}

		double y = arY[i];
		if(y < (double)_INF_)
		{
			if(y < fMinY)
				fMinY = y;
			if(y > fMaxY)
				fMaxY = y;
		}
	}

	if(fMinX < m_fMinXData)
		m_fMinXData = fMinX;
	if(fMaxX > m_fMaxXData)
		m_fMaxXData = fMaxX;
	if(fMinY < m_fMinYData)
		m_fMinYData = fMinY;
	if(fMaxY > m_fMaxYData)
		m_fMaxYData = fMaxY;
}

void CViewGraph::AddSeries(const char* szName, CArray<double,double>& arX, CArray<double,double>& arY, 
						   int type, int width, int style, COLORREF color)
{
	if(arX.GetSize() == 0 || arX.GetSize()  != arY.GetSize())
		return;

	AddSeries(szName, arX.GetData(), arY.GetData(), arX.GetSize(), type, width, style, color);
}

void CViewGraph::OnMouseMove(UINT nFlags, CPoint point) 
{
	if(m_bMouseTrack)
	{
		SPointsFromChart st;
		st.fX = _INF_;

		m_fMouseX = GetRealXFromCursorPos(point.x);
		//if(m_fMouseX < _INF_)
		{
			st.fX = m_fMouseX;
			for(int i=0; i<m_arSeries.GetSize(); i++)
			{
				CSeries* pSeries = m_arSeries[i];
				pSeries->m_nSel = LookupXIndex(m_fMouseX, pSeries->m_arX);

				if(pSeries->m_nSel >= 0 && pSeries->m_nSel < pSeries->m_arY.GetSize())
					st.arY.Add(pSeries->m_arY[pSeries->m_nSel]);
				else
					st.arY.Add(_INF_);
			}  
		}
		Invalidate();

		CWnd* pParent = GetParent(); 
		if(pParent && IsWindow(pParent->m_hWnd))
		{
			pParent->SendMessage(WMU_CHART_POINTS_CALLBACK, (WPARAM)this, (LPARAM)&st);
		}
	}

	CView::OnMouseMove(nFlags, point);
}

double CViewGraph::GetRealXFromCursorPos(int nX)
{
	CRect rc;
	GetClientRect(rc);

	CRect rcData(rc);
	rcData.DeflateRect(m_nMarginLeft, m_nMarginTop, m_nMarginRight, m_nMarginBottom);

	int nWidth = rcData.right - rcData.left;
	double fRange = m_fMaxX-m_fMinX;
	
	if(nX < rcData.left || nX > rcData.right)
		return _INF_;

	double fX = m_fMinX + ( (nX-rcData.left) * fRange/(double)nWidth );
	return fX;
}

int CViewGraph::LookupXIndex(double fX, CArray<double,double>& ar)
{
	if(ar.GetSize() > 0 && (fX < ar[0] || fX > ar[ar.GetUpperBound()]) )
		return -1;

	/* first, last and middle indexes*/
	int nFirst = 0;
	int nLast = ar.GetSize()-1;
	int nMiddle = (int)(nLast + nFirst)/2;	

	/* find element in array*/
	while(nFirst <= nLast)
	{
		double Temp = ar[nMiddle];

		/* check whether the element already exists*/ 
		if(fX == Temp)
			return nMiddle;

		/* check for greater or less*/
		if(fX < Temp)
			nLast = nMiddle - 1;
		else 
			nFirst = nMiddle + 1;
	
		/* calc new nMiddle*/
		nMiddle = (int)(nLast + nFirst)/2;	
	}

	return nFirst;
}