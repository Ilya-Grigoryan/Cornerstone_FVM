#if !defined(AFX_DIALOGDATE_H__AF20F878_FF2B_4088_A9D9_3E798BFFDB11__INCLUDED_)
#define AFX_DIALOGDATE_H__AF20F878_FF2B_4088_A9D9_3E798BFFDB11__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogDate.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogDate dialog

class CDialogDate : public CDialog
{
// Construction
public:
	CDialogDate(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogDate)
	enum { IDD = IDD_DIALOG_DATE };
	CString	m_sD;
	CString	m_sM;
	CString	m_sY;
	//}}AFX_DATA
	COleDateTime	m_Date;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogDate)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogDate)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGDATE_H__AF20F878_FF2B_4088_A9D9_3E798BFFDB11__INCLUDED_)
