#if !defined(AFX_VIEW2X2METRICS_H__99D5DC47_E539_4E9D_8DA6_57E11C324B86__INCLUDED_)
#define AFX_VIEW2X2METRICS_H__99D5DC47_E539_4E9D_8DA6_57E11C324B86__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// View2x2Metrics.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CView2x2Metrics view

class CView2x2Metrics : public CView
{
public:
	CView2x2Metrics();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CView2x2Metrics)

// Attributes
public:
	double	m_fX;
	double	m_fY;

	int		m_nSizeX; // 1 - 5 indexed sizes
	int		m_nSizeY;

	bool	m_bMouseOn;
	CPoint	m_ptOld;

	CFont	m_FontNormal;
	CFont	m_FontVert;

// Operations
public:
	void	SetConfidenceLevels(int nValue, int nFund);
	void	SetConfidenceValue(int nValue);
	void	SetConfidenceFund(int nFund);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CView2x2Metrics)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	virtual void OnActivateView(BOOL bActivate, CView* pActivateView, CView* pDeactiveView);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CView2x2Metrics();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif
	
	void Draw(CDC* pDC, CRect &rc);
	void DrawCaptions(CDC* pDC, CRect rc);
	void DrawRects(CDC* pDC, CRect &rect);
	void DrawPoint(CDC* pDC, CRect &rc);

	// Generated message map functions
protected:
	//{{AFX_MSG(CView2x2Metrics)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEW2X2METRICS_H__99D5DC47_E539_4E9D_8DA6_57E11C324B86__INCLUDED_)
