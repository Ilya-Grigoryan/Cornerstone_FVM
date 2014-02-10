#if !defined(AFX_DIALOGNEWMEETING_H__16EBDCB8_16AF_4BD5_8B24_ECFB8771AC5B__INCLUDED_)
#define AFX_DIALOGNEWMEETING_H__16EBDCB8_16AF_4BD5_8B24_ECFB8771AC5B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogNewMeeting.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogNewMeeting dialog

class CDialogNewMeeting : public CDialog
{
// Construction
public:
	CDialogNewMeeting(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogNewMeeting)
	enum { IDD = IDD_DIALOG_ADD_MEETING };
	CListBox	m_ListFiles;
	CComboBox	m_comboRec;
	CComboBox	m_comboDec;
	CEdit	m_editDesc;
	CEdit	m_editAnalyst;
	CEdit	m_editDate;
	CComboBox	m_comboCompany;
	//}}AFX_DATA
	int				m_nCompanyID;
	COleDateTime	m_Date;
	CString			m_sAnalyst;
	int				m_nRecommend;
	int				m_nDecision;
	CString			m_sDesc;
	CStringArray	m_arFiles;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogNewMeeting)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogNewMeeting)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonAddFile();
	afx_msg void OnButtonDeleteFile();
	afx_msg void OnButtonDeleteAllfiles();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGNEWMEETING_H__16EBDCB8_16AF_4BD5_8B24_ECFB8771AC5B__INCLUDED_)
