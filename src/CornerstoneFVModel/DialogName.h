#if !defined(AFX_DIALOGNAME_H__E2CF7145_8D9C_4295_8DD1_AB6CC28BA04E__INCLUDED_)
#define AFX_DIALOGNAME_H__E2CF7145_8D9C_4295_8DD1_AB6CC28BA04E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogName.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogName dialog

class CDialogName : public CDialog
{
// Construction
public:
	CDialogName(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogName)
	enum { IDD = IDD_DIALOG_NAME };
	CString	m_sName;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogName)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogName)
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGNAME_H__E2CF7145_8D9C_4295_8DD1_AB6CC28BA04E__INCLUDED_)
