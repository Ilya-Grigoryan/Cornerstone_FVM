#if !defined(AFX_FRAMEHTML_H__8476DF96_1849_4519_BD47_2B39ED8CEE46__INCLUDED_)
#define AFX_FRAMEHTML_H__8476DF96_1849_4519_BD47_2B39ED8CEE46__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FrameHTML.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFrameHTML frame

class CFrameHTML : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CFrameHTML)
protected:
	CFrameHTML();           // protected constructor used by dynamic creation

// Attributes
public:
	CToolBar	m_wndToolBar;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrameHTML)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFrameHTML();

	// Generated message map functions
	//{{AFX_MSG(CFrameHTML)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAMEHTML_H__8476DF96_1849_4519_BD47_2B39ED8CEE46__INCLUDED_)
