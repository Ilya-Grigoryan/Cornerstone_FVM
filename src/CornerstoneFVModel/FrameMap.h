#if !defined(AFX_FRAMEMAP_H__78DAC7BC_A4A0_4766_B75E_716E2E51C7AA__INCLUDED_)
#define AFX_FRAMEMAP_H__78DAC7BC_A4A0_4766_B75E_716E2E51C7AA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FrameMap.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFrameMap frame

class CFrameMap : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CFrameMap)
protected:
	CFrameMap();           // protected constructor used by dynamic creation

// Attributes
public:
	CToolBar	m_wndToolBar;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrameMap)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFrameMap();

	// Generated message map functions
	//{{AFX_MSG(CFrameMap)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAMEMAP_H__78DAC7BC_A4A0_4766_B75E_716E2E51C7AA__INCLUDED_)
