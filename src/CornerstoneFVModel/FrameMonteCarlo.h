#if !defined(AFX_FRAMEMONTECARLO_H__55BEF8CF_C2C4_4947_9AF2_58702D18E7E7__INCLUDED_)
#define AFX_FRAMEMONTECARLO_H__55BEF8CF_C2C4_4947_9AF2_58702D18E7E7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FrameMonteCarlo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFrameMonteCarlo frame

class CFrameMonteCarlo : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CFrameMonteCarlo)
protected:
	CFrameMonteCarlo();           // protected constructor used by dynamic creation

// Attributes
public:

	CToolBar	m_wndToolBar;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrameMonteCarlo)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFrameMonteCarlo();

	// Generated message map functions
	//{{AFX_MSG(CFrameMonteCarlo)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAMEMONTECARLO_H__55BEF8CF_C2C4_4947_9AF2_58702D18E7E7__INCLUDED_)
