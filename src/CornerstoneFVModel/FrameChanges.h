#if !defined(AFX_FRAMECHANGES_H__49549D4C_8C44_49AD_AEF1_7D77D5A5D317__INCLUDED_)
#define AFX_FRAMECHANGES_H__49549D4C_8C44_49AD_AEF1_7D77D5A5D317__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FrameChanges.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFrameChanges frame

class CFrameChanges : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CFrameChanges)
protected:
	CFrameChanges();           // protected constructor used by dynamic creation

// Attributes
public:
	CToolBar	m_wndToolBar;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrameChanges)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFrameChanges();

	// Generated message map functions
	//{{AFX_MSG(CFrameChanges)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAMECHANGES_H__49549D4C_8C44_49AD_AEF1_7D77D5A5D317__INCLUDED_)
