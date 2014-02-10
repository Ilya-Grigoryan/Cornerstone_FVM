#if !defined(AFX_VIEWGRAPH_H__C952AEF3_529E_4A74_A7D6_7B4A8C6E0C9E__INCLUDED_)
#define AFX_VIEWGRAPH_H__C952AEF3_529E_4A74_A7D6_7B4A8C6E0C9E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewGraph.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CSeries
#define SERIES_TYPE_LINE		1
#define SERIES_TYPE_BAR			2
#define SERIES_TYPE_PT_LINE		3
#define SERIES_TYPE_PT_LINE_1	4
#define SERIES_TYPE_PT_COLORED	5

#define LINE_TYPE_HORZ			1
#define LINE_TYPE_VERT			2

#define LINE_TEXT_LEFTBOTTOM	1
#define LINE_TEXT_TOPRIGHT		2

#define AXIS_TYPE_DOUBLE		1
#define AXIS_TYPE_PERCENT		2
#define AXIS_TYPE_DATE			3

#define WMU_CHART_POINTS_CALLBACK		WM_USER	+ 87

////////////////////////////////////////////////////////////////////////////
struct SPointsFromChart
{
	double fX;
	CArray<double,double>	arY;
};

/////////////////////////////////////////////////////////////////////////////
class CSeries
{
public:
	CSeries();
	~CSeries();

	void SetData(double* arX, double* arY, int nSize);
	void SetData(CArray<double,double>& arX, CArray<double,double>& arY);

public:
	int			m_nType;
	int			m_nWidth;
	int			m_nStyle;
	COLORREF	m_Color;
	CString		m_sName;

	CArray<double,double>	m_arX;
	CArray<double,double>	m_arY;

	int			m_nSel;
};

////////////////////////////////////////////////////////////////////////////////
class CLine
{
public:
	int			m_nType;
	int			m_nWidth;
	int			m_nStyle;
	COLORREF	m_Color;

	CString		m_sText;
	int			m_nTextAlign;
	double		m_fPos;
};

////////////////////////////////////////////////////////////////////////////////
class CRc
{
public:
	double		m_fLeft;
	double		m_fTop;
	double		m_fRight;
	double		m_fBottom;

	COLORREF	m_Color;

	//CString		m_sText;
	//int			m_nTextAlign;
};

/////////////////////////////////////////////////////////////////////////////
// CViewGraph view
class CViewGraph : public CView
{
public:
	CViewGraph();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewGraph)

// Attributes
public:
	CFont	m_FontNormal;
	CFont	m_FontVert;
	CFont	m_FontBold;

	int		m_nMarginLeft;
	int		m_nMarginTop;
	int		m_nMarginRight;
	int		m_nMarginBottom;

	BOOL	m_bMouseTrack;
	double	m_fMouseX;

	CArray<CSeries*,CSeries*>	m_arSeries;
	CArray<CLine*,CLine*>		m_arLines;
	CArray<CRc*,CRc*>			m_arRects;
	CStringArray				m_arLabels;

	static int LookupXIndex(double fX, CArray<double,double>& ar);

// Operations
public:
	void AddSeries(const char* szName, double* arX, double* arY, int nSize, 
		int type, int width, int style, COLORREF color);
	void AddSeries(const char* szName, CArray<double,double>& arX, CArray<double,double>& arY, 
		int type, int width, int style, COLORREF color);

	void AddLine(double pos, const char* szText, int textAlign, int type, int width, int style, COLORREF color);
	void AddRect(double left, double top, double right, double bottom, COLORREF color);

	void SetFont(LOGFONT &lf);

	void Draw(CDC* pDC, CRect rc);
	void DrawAxisX(CDC* pDC, CRect rc, CRect rcData);
	void DrawAxisY(CDC* pDC, CRect rc, CRect rcData);
	void DrawSeries(CDC* pDC, CRect rc, CSeries* pSeries);
	void DrawHeader(CDC* pDC, CRect rc);
	void DrawLine(CDC* pDC, CRect rc, CLine* pLine);
	void DrawRect(CDC* pDC, CRect rc, CRc* pRect);

	void CalcMargins();
	void GetTickSize(int nWidth, int nBlock, double fMin, double fMax, double &fStep);

	int	GetSeriesCount()				{ return m_arSeries.GetSize();				}
	int GetLineCount()					{ return m_arLines.GetSize();				}

	CSeries* GetSeries(int index)		{ return m_arSeries[index];					}
	CLine* GetLine(int index)			{ return m_arLines[index];					}

	void RemoveAll();

	double GetRealXFromCursorPos(int nX);

public:
	double m_fMinX;
	double m_fMaxX;
	double m_fMinY;
	double m_fMaxY;

	double m_fMinXData;
	double m_fMaxXData;
	double m_fMinYData;
	double m_fMaxYData;

	CString m_sLabelX;
	CString m_sLabelY;

	BOOL	m_bHeader;

	BOOL	m_bFixedMinX;
	BOOL	m_bFixedMaxX;
	BOOL	m_bFixedMinY;
	BOOL	m_bFixedMaxY;

	int		m_nTypeAxisX;
	int		m_nTypeAxisY;

	CArray<COLORREF, COLORREF> m_ColorTableArray;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewGraph)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewGraph();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewGraph)
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWGRAPH_H__C952AEF3_529E_4A74_A7D6_7B4A8C6E0C9E__INCLUDED_)
