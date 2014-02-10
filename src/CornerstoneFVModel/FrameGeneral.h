// FrameGeneral.h : interface of the CFrameGeneral class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_FRAMEGENERAL_H__81A6E036_837B_4C7E_A076_6D1D2274A094__INCLUDED_)
#define AFX_FRAMEGENERAL_H__81A6E036_837B_4C7E_A076_6D1D2274A094__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WMU_CLOSING_FRAME	WM_USER+8547


class CFrameGeneral : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CFrameGeneral)
public:
	CFrameGeneral();

// Attributes
public:

	CToolBar	m_wndToolBar;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrameGeneral)
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CFrameGeneral();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

// Generated message map functions
protected:
	//{{AFX_MSG(CFrameGeneral)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnClose();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAMEGENERAL_H__81A6E036_837B_4C7E_A076_6D1D2274A094__INCLUDED_)
