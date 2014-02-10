#if !defined(AFX_DIALOGDATACHECK_H__205EE648_51BF_4FAF_86B6_23A3BC2F4E92__INCLUDED_)
#define AFX_DIALOGDATACHECK_H__205EE648_51BF_4FAF_86B6_23A3BC2F4E92__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogDataCheck.h : header file
//

#include "Company.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogDataCheck dialog

class CDialogDataCheck : public CDialog
{
// Construction
public:
	CDialogDataCheck(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogDataCheck)
	enum { IDD = IDD_DIALOG_DATA_CHECK };
	CButton	m_radioRazor;
	CButton	m_radioPrice;
	CButton	m_radioNorm;
	CButton	m_radioEPSThis;
	CButton	m_radioEPSNext;
	CButton	m_radioDebt;
	CButton	m_radioBev;
	int		m_nBEV;
	double	m_fDebt;
	double	m_fEPSNext;
	double	m_fEPSThis;
	int		m_nNorm;
	int		m_nPeriods;
	double	m_fPrice;
	int		m_nRazor;
	int		m_nRadioBEV;
	int		m_nRadioDebt;
	int		m_nRadioEPSNext;
	int		m_nRadioEPSThis;
	int		m_nRadioNorm;
	int		m_nRadioPrice;
	int		m_nRadioRazor;
	//}}AFX_DATA

	SChangeCriteria	m_CH;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogDataCheck)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogDataCheck)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnButtonIncludeAll();
	afx_msg void OnButtonExcludeAll();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGDATACHECK_H__205EE648_51BF_4FAF_86B6_23A3BC2F4E92__INCLUDED_)
