#if !defined(AFX_FRAMEHISTORY_H__72583969_EF86_41DA_BFE7_8857390C325C__INCLUDED_)
#define AFX_FRAMEHISTORY_H__72583969_EF86_41DA_BFE7_8857390C325C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FrameHistory.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFrameHistory frame

class CFrameHistory : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CFrameHistory)
protected:
	CFrameHistory();           // protected constructor used by dynamic creation

// Attributes
public:
	CToolBar	m_wndToolBar;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrameHistory)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFrameHistory();

	// Generated message map functions
	//{{AFX_MSG(CFrameHistory)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAMEHISTORY_H__72583969_EF86_41DA_BFE7_8857390C325C__INCLUDED_)
