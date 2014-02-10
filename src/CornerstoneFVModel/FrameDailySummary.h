#if !defined(AFX_FRAMEDAILYSUMMARY_H__DD59F918_B058_4BDD_8A30_735B52638656__INCLUDED_)
#define AFX_FRAMEDAILYSUMMARY_H__DD59F918_B058_4BDD_8A30_735B52638656__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FrameDailySummary.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFrameDailySummary frame

class CFrameDailySummary : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CFrameDailySummary)
protected:
	CFrameDailySummary();           // protected constructor used by dynamic creation

// Attributes
public:
	CToolBar	m_wndToolBar;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrameDailySummary)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFrameDailySummary();

	// Generated message map functions
	//{{AFX_MSG(CFrameDailySummary)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAMEDAILYSUMMARY_H__DD59F918_B058_4BDD_8A30_735B52638656__INCLUDED_)
