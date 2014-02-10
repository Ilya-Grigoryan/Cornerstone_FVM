#if !defined(AFX_BARWORKSPACE_H__C60A8AA5_D4A1_40C3_8A23_401EE846F2A9__INCLUDED_)
#define AFX_BARWORKSPACE_H__C60A8AA5_D4A1_40C3_8A23_401EE846F2A9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BarWorkspace.h : header file
//
#include "sizingcontrolbar.h"

/////////////////////////////////////////////////////////////////////////////
// CBarWorkspace window

class CDialogStocksAlphabetical;
class CBarWorkspace : public CSizingControlBar
{
	void Resize();
	void DrawCaption(CDC* pDC);
	void GetClosePos(int &x, int &y);

	BOOL m_bMouseOnClose;
	BOOL m_bClosePressed;
	CDC	m_dcClose[3];

// Construction
public:
	CBarWorkspace();

// Attributes
public:
	CDialog*	m_pDlg;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBarWorkspace)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CBarWorkspace();

	// Generated message map functions
protected:
	//{{AFX_MSG(CBarWorkspace)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BARWORKSPACE_H__C60A8AA5_D4A1_40C3_8A23_401EE846F2A9__INCLUDED_)
