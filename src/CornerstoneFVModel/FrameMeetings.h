#if !defined(AFX_FRAMEMEETINGS_H__FA764706_E188_4B9A_8989_D64213A00747__INCLUDED_)
#define AFX_FRAMEMEETINGS_H__FA764706_E188_4B9A_8989_D64213A00747__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FrameMeetings.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFrameMeetings frame

class CFrameMeetings : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CFrameMeetings)
protected:
	CFrameMeetings();           // protected constructor used by dynamic creation

// Attributes
public:
	CToolBar	m_wndToolBar;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrameMeetings)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFrameMeetings();

	// Generated message map functions
	//{{AFX_MSG(CFrameMeetings)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAMEMEETINGS_H__FA764706_E188_4B9A_8989_D64213A00747__INCLUDED_)
