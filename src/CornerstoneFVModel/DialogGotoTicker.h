#if !defined(AFX_DIALOGGOTOTICKER_H__BD8212DE_23EA_46D9_B213_5479DECA2B41__INCLUDED_)
#define AFX_DIALOGGOTOTICKER_H__BD8212DE_23EA_46D9_B213_5479DECA2B41__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogGotoTicker.h : header file
//

#include "resource.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogGotoTicker dialog

class CDialogGotoTicker : public CDialog
{
// Construction
public:
	CDialogGotoTicker(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogGotoTicker)
	enum { IDD = IDD_DIALOGBAR_GOTOTICKER };
	CEdit	m_editTicker;
	CString	m_sTicker;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogGotoTicker)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogGotoTicker)
	afx_msg void OnButtonOpen();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGGOTOTICKER_H__BD8212DE_23EA_46D9_B213_5479DECA2B41__INCLUDED_)
