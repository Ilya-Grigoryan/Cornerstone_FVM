#if !defined(AFX_DIALOGCOMPANIES_H__CA8BB36D_5D31_4EBD_B593_EA1414ABD4BF__INCLUDED_)
#define AFX_DIALOGCOMPANIES_H__CA8BB36D_5D31_4EBD_B593_EA1414ABD4BF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogCompanies.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogCompanies dialog

class CDialogCompanies : public CDialog
{
// Construction
public:
	CDialogCompanies(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogCompanies)
	enum { IDD = IDD_DIALOG_COMPANIES };
	CButton	m_checkLoadROCROE;
	CEdit	m_editLookup;
	CEdit	m_editLastYear;
	CEdit	m_editTicker;
	CEdit	m_editName;
	CEdit	m_editCIQTicker;
	CComboBox	m_comboType;
	CComboBox	m_comboSector;
	CComboBox	m_comboIndustry;
	CButton	m_checkDebtAdjust;
	CButton	m_checkLoadEPS;
	CButton	m_checkLoadBS;
	CListBox	m_List;
	//}}AFX_DATA
	CFont	m_Font;
	BOOL	m_bAdding;
	int		m_nIndexAdding;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogCompanies)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	void SetValues(int nIndex);
	CWnd*	m_arEditWeights[9];


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogCompanies)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeListMain();
	afx_msg void OnButtonNew();
	afx_msg void OnButtonDelete();
	afx_msg void OnButtonSave();
	afx_msg void OnChangeEditNameLookup();
	virtual void OnOK();
	virtual void OnCancel();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGCOMPANIES_H__CA8BB36D_5D31_4EBD_B593_EA1414ABD4BF__INCLUDED_)
