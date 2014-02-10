#if !defined(AFX_DIALOGSELECTCOMPANY_H__52DAFA97_309C_41C2_86BE_80E4FA941AA9__INCLUDED_)
#define AFX_DIALOGSELECTCOMPANY_H__52DAFA97_309C_41C2_86BE_80E4FA941AA9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogSelectCompany.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSelectCompany dialog

class CDialogSelectCompany : public CDialog
{
// Construction
public:
	CDialogSelectCompany(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogSelectCompany)
	enum { IDD = IDD_DIALOG_SELECT_COMP };
	CComboBox	m_comboCompany;
	//}}AFX_DATA
	int		m_nId;
	CString m_sTicker;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSelectCompany)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogSelectCompany)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSELECTCOMPANY_H__52DAFA97_309C_41C2_86BE_80E4FA941AA9__INCLUDED_)
