#if !defined(AFX_DIALOGUPLOADDATA_H__A1396A0F_7CFB_4AE9_88E2_45DFB900AB79__INCLUDED_)
#define AFX_DIALOGUPLOADDATA_H__A1396A0F_7CFB_4AE9_88E2_45DFB900AB79__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogUploadData.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogUploadData dialog

class CDialogUploadData : public CDialog
{
// Construction
public:
	CDialogUploadData(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogUploadData)
	enum { IDD = IDD_DIALOG_UPLOAD_DATA };
	CString	m_sBS;
	CString	m_sEPS;
	BOOL	m_bForceBS;
	BOOL	m_bForceEPS;
	CString	m_sPrice;
	CString	m_sRet;
	CString	m_sShares;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogUploadData)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogUploadData)
	afx_msg void OnButtonBsBrowse();
	afx_msg void OnButtonPrBrowse();
	afx_msg void OnButtonEpsBrowse();
	afx_msg void OnButtonRetBrowse();
	virtual void OnOK();
	afx_msg void OnButtonSharesBrowse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGUPLOADDATA_H__A1396A0F_7CFB_4AE9_88E2_45DFB900AB79__INCLUDED_)
