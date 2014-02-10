#if !defined(AFX_DIALOGREPORTGENERAL_H__36680F3F_38E1_44E6_8C5D_0D9FDAD9CABE__INCLUDED_)
#define AFX_DIALOGREPORTGENERAL_H__36680F3F_38E1_44E6_8C5D_0D9FDAD9CABE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogReportGeneral.h : header file
//

#include "Report.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogReportGeneral dialog

class CDialogReportGeneral : public CDialog
{
// Construction
public:
	CDialogReportGeneral(SReportFilter* pFilter, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogReportGeneral)
	enum { IDD = IDD_DIALOG_REP_GENERAL };
	CButton	m_checkSort;
	CCheckListBox	m_ListItems;
	CCheckListBox	m_ListFilter;
	BOOL	m_bNoIndividualStocks;
	double	m_fMCapMax;
	double	m_fMCapMin;
	CString	m_sTitle;
	BOOL	m_bExcel;
	int		m_nGroup;
	//}}AFX_DATA
	CArray<int,int>	m_arFilter;
	int		m_nSort;

	SReportFilter*	m_pRepFilter;

	void SetControls();
	int  CalcSelections();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogReportGeneral)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogReportGeneral)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnCheckSortBy();
	afx_msg void OnSelchangeListItems();
	afx_msg void OnKillfocusEditMin();
	afx_msg void OnKillfocusEditMax();
	afx_msg void OnButtonIncAll();
	afx_msg void OnButtonExcAll();
	afx_msg void OnButtonInverse();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGREPORTGENERAL_H__36680F3F_38E1_44E6_8C5D_0D9FDAD9CABE__INCLUDED_)
