#if !defined(AFX_DIALOGREPORTVALUEMOMENTUM1_H__EDCD7315_4D16_432F_AF1C_3CD0722B7660__INCLUDED_)
#define AFX_DIALOGREPORTVALUEMOMENTUM1_H__EDCD7315_4D16_432F_AF1C_3CD0722B7660__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogReportValueMomentum1.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogReportValueMomentum1 dialog

class CDialogReportValueMomentum1 : public CDialog
{
// Construction
public:
	CDialogReportValueMomentum1(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogReportValueMomentum1)
	enum { IDD = IDD_DIALOG_REP_VALUE_MOMENTUM_1 };
	CCheckListBox	m_ListFilter;
	BOOL	m_bNoIndividualStocks;
	double	m_fMCapMax;
	double	m_fMCapMin;
	CString	m_sTitle;
	int		m_nSort;
	BOOL	m_bExcel;
	int		m_nGroup;
	//}}AFX_DATA
	CArray<int,int>	m_arFilter;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogReportValueMomentum1)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogReportValueMomentum1)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGREPORTVALUEMOMENTUM1_H__EDCD7315_4D16_432F_AF1C_3CD0722B7660__INCLUDED_)
