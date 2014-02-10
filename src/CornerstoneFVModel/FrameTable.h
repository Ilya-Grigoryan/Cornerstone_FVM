#if !defined(AFX_FRAMETABLE_H__AE73F0A9_70F3_472A_BBF0_ADFE38AE2B09__INCLUDED_)
#define AFX_FRAMETABLE_H__AE73F0A9_70F3_472A_BBF0_ADFE38AE2B09__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// FrameTable.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CFrameTable frame

class CFrameTable : public CMDIChildWnd
{
	DECLARE_DYNCREATE(CFrameTable)
protected:
	CFrameTable();           // protected constructor used by dynamic creation

// Attributes
public:
	CToolBar	m_wndToolBar;

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CFrameTable)
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CFrameTable();

	// Generated message map functions
	//{{AFX_MSG(CFrameTable)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_FRAMETABLE_H__AE73F0A9_70F3_472A_BBF0_ADFE38AE2B09__INCLUDED_)
