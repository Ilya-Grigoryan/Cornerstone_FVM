#if !defined(AFX_DIALOGCONFIRMWEIGHT_H__E2AF94FE_1ED9_4DF2_979C_BDB1F53B134A__INCLUDED_)
#define AFX_DIALOGCONFIRMWEIGHT_H__E2AF94FE_1ED9_4DF2_979C_BDB1F53B134A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogConfirmWeight.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogConfirmWeight dialog

class CDialogConfirmWeight : public CDialog
{
// Construction
public:
	CDialogConfirmWeight(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogConfirmWeight)
	enum { IDD = IDD_DIALOG_CONFIRM_WEIGHT };
	CComboBox	m_comboBias;
	CComboBox	m_comboTarget;
	//}}AFX_DATA
	double	m_fWeight;
	CString m_sBias;

	CString m_sWeight;


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogConfirmWeight)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogConfirmWeight)
	virtual void OnOK();
	virtual BOOL OnInitDialog();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGCONFIRMWEIGHT_H__E2AF94FE_1ED9_4DF2_979C_BDB1F53B134A__INCLUDED_)
