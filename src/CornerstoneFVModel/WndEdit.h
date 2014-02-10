#if !defined(AFX_WNDEDIT_H__3CD5A7A2_211C_4A86_BA30_C97F2857E425__INCLUDED_)
#define AFX_WNDEDIT_H__3CD5A7A2_211C_4A86_BA30_C97F2857E425__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// WndEdit.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CWndEdit window

#define WMU_SETTEXT		WM_USER + 121

class CWndEdit : public CEdit
{
// Construction
public:
	CWndEdit();

// Attributes
public:

// Operations
public:
	BOOL	Show(CRect rc, LPCTSTR lpszText);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CWndEdit)
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CWndEdit();

	// Generated message map functions
protected:
	//{{AFX_MSG(CWndEdit)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WNDEDIT_H__3CD5A7A2_211C_4A86_BA30_C97F2857E425__INCLUDED_)
