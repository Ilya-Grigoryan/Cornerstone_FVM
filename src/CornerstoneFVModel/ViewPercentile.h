#if !defined(AFX_VIEWPERCENTILE_H__FEC879B8_970F_46C8_8CE2_35DA45572544__INCLUDED_)
#define AFX_VIEWPERCENTILE_H__FEC879B8_970F_46C8_8CE2_35DA45572544__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewPercentile.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewPercentile view

class CViewPercentile : public CView
{
public:
	CViewPercentile();
	CViewPercentile(const CString& sName, double fPercent);
	DECLARE_DYNCREATE(CViewPercentile)

// Attributes
public:
	void SetName(const char* szName)			{ m_sName = szName;	}
	CString GetName()							{ return m_sName;	}

	void SetPercentile(double fPercent)			{ m_fPercentile = fPercent; if(m_fPercentile<0.01) m_fPercentile=0.01; if(m_fPercentile>0.99) m_fPercentile=0.99; }
	double GetPercentile()						{ return m_fPercentile;		}

// Operations
public:

protected:
	CString		m_sName;
	double		m_fPercentile;

	CFont		m_Font;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewPercentile)
	protected:
	virtual void OnDraw(CDC* pDC);      // overridden to draw this view
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewPercentile();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
protected:
	//{{AFX_MSG(CViewPercentile)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWPERCENTILE_H__FEC879B8_970F_46C8_8CE2_35DA45572544__INCLUDED_)
