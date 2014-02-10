#if !defined(AFX_DIALOGLASTREPORTYEAR_H__5FBC1FFE_0D62_40E3_8A44_A8495C20BD45__INCLUDED_)
#define AFX_DIALOGLASTREPORTYEAR_H__5FBC1FFE_0D62_40E3_8A44_A8495C20BD45__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogLastReportYear.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogLastReportYear dialog

class CDialogLastReportYear : public CDialog
{
// Construction
public:
	CDialogLastReportYear(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogLastReportYear)
	enum { IDD = IDD_DIALOG_LAST_REPORT_YEAR };
	int		m_nYear;
	int		m_nOption;
	//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogLastReportYear)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogLastReportYear)
	afx_msg void OnRadioOption1();
	afx_msg void OnRadioOption2();
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGLASTREPORTYEAR_H__5FBC1FFE_0D62_40E3_8A44_A8495C20BD45__INCLUDED_)
