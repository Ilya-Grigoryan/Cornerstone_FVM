#if !defined(AFX_DIALOGNAMEANDTICKER_H__DF469B66_27AA_49F9_8F9A_DDAEB77B7EB6__INCLUDED_)
#define AFX_DIALOGNAMEANDTICKER_H__DF469B66_27AA_49F9_8F9A_DDAEB77B7EB6__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogNameAndTicker.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogNameAndTicker dialog

class CDialogNameAndTicker : public CDialog
{
// Construction
public:
	CDialogNameAndTicker(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogNameAndTicker)
	enum { IDD = IDD_DIALOG_NAME_AND_TICKER };
	CString	m_sName;
	CString	m_sTicker;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogNameAndTicker)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogNameAndTicker)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGNAMEANDTICKER_H__DF469B66_27AA_49F9_8F9A_DDAEB77B7EB6__INCLUDED_)
