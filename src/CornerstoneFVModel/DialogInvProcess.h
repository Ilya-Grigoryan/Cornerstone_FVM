#if !defined(AFX_DIALOGINVPROCESS_H__BB46D134_5A27_4895_9D18_2E6A459DCD07__INCLUDED_)
#define AFX_DIALOGINVPROCESS_H__BB46D134_5A27_4895_9D18_2E6A459DCD07__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogInvProcess.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogInvProcess dialog

class CDialogInvProcess : public CDialog
{
// Construction
public:
	CDialogInvProcess(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogInvProcess)
	enum { IDD = IDD_DIALOG_INVPROCESS };
	CListBox	m_listRW;
	CListBox	m_listSD;
	CListBox	m_listTruck;
	CListBox	m_listWIP;
	CListBox	m_listPort;
	CListBox	m_listAW;
	CListBox	m_listQC;
	CListBox	m_listRaw;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogInvProcess)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogInvProcess)
	afx_msg void OnButtonRawAdd();
	virtual BOOL OnInitDialog();
	afx_msg void OnButtonRawDel();
	afx_msg void OnButtonWipAdd();
	afx_msg void OnButtonWipDel();
	afx_msg void OnButtonQcAdd();
	afx_msg void OnButtonQcDel();
	afx_msg void OnButtonPortAdd();
	afx_msg void OnButtonPortDel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGINVPROCESS_H__BB46D134_5A27_4895_9D18_2E6A459DCD07__INCLUDED_)
