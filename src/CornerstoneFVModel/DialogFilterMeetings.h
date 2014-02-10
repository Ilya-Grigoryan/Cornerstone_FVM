#if !defined(AFX_DIALOGFILTERMEETINGS_H__31C3EF56_3B61_4028_A851_274512A23F7C__INCLUDED_)
#define AFX_DIALOGFILTERMEETINGS_H__31C3EF56_3B61_4028_A851_274512A23F7C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogFilterMeetings.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogFilterMeetings dialog

class CDialogFilterMeetings : public CDialog
{
// Construction
public:
	CDialogFilterMeetings(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogFilterMeetings)
	enum { IDD = IDD_DIALOG_FILTER_MEETINGS };
	CEdit	m_editToY;
	CEdit	m_editToM;
	CEdit	m_editToD;
	CEdit	m_editFromY;
	CEdit	m_editFromM;
	CEdit	m_editFromD;
	CButton	m_checkTo;
	CButton	m_checkFrom;
	CComboBox	m_comboRec;
	CComboBox	m_comboDec;
	CComboBox	m_comboCompany;
	int		m_nSort;
	//}}AFX_DATA
	int				m_nCompanyID;
	COleDateTime	m_From;
	COleDateTime	m_To;
	int				m_nRec;
	int				m_nDec;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogFilterMeetings)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogFilterMeetings)
	virtual BOOL OnInitDialog();
	afx_msg void OnCheckFrom();
	afx_msg void OnCheckTo();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGFILTERMEETINGS_H__31C3EF56_3B61_4028_A851_274512A23F7C__INCLUDED_)
