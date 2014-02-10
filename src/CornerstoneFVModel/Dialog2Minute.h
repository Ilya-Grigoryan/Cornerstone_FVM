#if !defined(AFX_DIALOG2MINUTE_H__287BB053_754A_4E9A_A530_A57C4856BEB2__INCLUDED_)
#define AFX_DIALOG2MINUTE_H__287BB053_754A_4E9A_A530_A57C4856BEB2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Dialog2Minute.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialog2Minute dialog

class CDialog2Minute : public CDialog
{
// Construction
public:
	CDialog2Minute(int nCompanyID, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialog2Minute)
	enum { IDD = IDD_DIALOG_2MINUTE };
	CString	m_s1;
	CString	m_s2;
	CString	m_s3;
	CString	m_s4;
	CString	m_s5;
	CString	m_s6;
	CString	m_s7;
	CString	m_s8;
	CString	m_s9;
	int		m_n1;
	int		m_n2;
	int		m_n3;
	int		m_n4;
	int		m_n5;
	int		m_n6;
	int		m_n7;
	int		m_n8;
	int		m_n9;
	CString	m_sDate;
	CString	m_sThesis;
	int		m_n10;
	CString	m_s10;
	//}}AFX_DATA
	int		m_nCompanyID;
	int		*m_arChecks[10];


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialog2Minute)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialog2Minute)
	afx_msg void OnCancel();
	afx_msg void OnSave();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOG2MINUTE_H__287BB053_754A_4E9A_A530_A57C4856BEB2__INCLUDED_)
