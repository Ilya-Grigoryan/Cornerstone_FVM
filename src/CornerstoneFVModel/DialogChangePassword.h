#if !defined(AFX_DIALOGCHANGEPASSWORD_H__6790D79B_F50B_47DA_A814_64862440D042__INCLUDED_)
#define AFX_DIALOGCHANGEPASSWORD_H__6790D79B_F50B_47DA_A814_64862440D042__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogChangePassword.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogChangePassword dialog

class CDialogChangePassword : public CDialog
{
// Construction
public:
	CDialogChangePassword(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogChangePassword)
	enum { IDD = IDD_DIALOG_CHANGE_PASSWORD };
	CString	m_sName;
	CString	m_sPwdOld;
	CString	m_sPwd1;
	CString	m_sPwd2;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogChangePassword)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogChangePassword)
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGCHANGEPASSWORD_H__6790D79B_F50B_47DA_A814_64862440D042__INCLUDED_)
