#if !defined(AFX_DIALOGREPORTEBITDALEVERAGE_H__EF5275E9_A892_4832_A270_67664285FE94__INCLUDED_)
#define AFX_DIALOGREPORTEBITDALEVERAGE_H__EF5275E9_A892_4832_A270_67664285FE94__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogReportEBITDALeverage.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogReportEBITDALeverage dialog

class CDialogReportEBITDALeverage : public CDialog
{
// Construction
public:
	CDialogReportEBITDALeverage(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogReportEBITDALeverage)
	enum { IDD = IDD_DIALOG_REP_EBITDA_LEV };
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
	//{{AFX_VIRTUAL(CDialogReportEBITDALeverage)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogReportEBITDALeverage)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGREPORTEBITDALEVERAGE_H__EF5275E9_A892_4832_A270_67664285FE94__INCLUDED_)
