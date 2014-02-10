#if !defined(AFX_FRAMECHARTS_H__7B96C18D_3BDD_4F8E_AEBF_6630CAC213F9__INCLUDED_)
#define AFX_FRAMECHARTS_H__7B96C18D_3BDD_4F8E_AEBF_6630CAC213F9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FrameCharts.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFrameCharts frame

class CFrameCharts : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CFrameCharts)
protected:
	CFrameCharts();           // protected constructor used by dynamic creation

// Attributes
public:
	CToolBar	m_wndToolBar;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrameCharts)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFrameCharts();

	// Generated message map functions
	//{{AFX_MSG(CFrameCharts)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAMECHARTS_H__7B96C18D_3BDD_4F8E_AEBF_6630CAC213F9__INCLUDED_)
