#if !defined(AFX_DIALOGMACRO_H__5943A67F_CBBC_41E9_A77A_5F5F687C3E8C__INCLUDED_)
#define AFX_DIALOGMACRO_H__5943A67F_CBBC_41E9_A77A_5F5F687C3E8C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogMacro.h : header file
//
#include "MacroParameters.h"

/////////////////////////////////////////////////////////////////////////////
// CDialogMacro dialog

class CDialogMacro : public CDialog
{
// Construction
public:
	CDialogMacro(CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogMacro)
	enum { IDD = IDD_DIALOG_MACRO };
	CEdit	m_editMinGrowth;
	CEdit	m_editPeriodsMovingAve;
	CEdit	m_editYearsEPSRIOMeans;
	CEdit	m_editYearsDiscountEarnings;
	CEdit	m_editMaxReinvestment;
	CEdit	m_editTax;
	CEdit	m_editInterest;
	CEdit	m_edtInterestFactor;
	CEdit	m_editDiscount;
	double	m_fDiscount;
	double	m_fInterest;
	double	m_fInterestFactor;
	double	m_fMaxReinvest;
	int		m_nPeriodsMovingAve;
	double	m_fTax;
	int		m_nYearsDiscountEarnings;
	int		m_nYearsEPSROIMeans;
	double	m_fMinGrowth;
	//}}AFX_DATA

	CMacroParameters	m_Macro;

	void SetDataToControls();
	BOOL GetDataFromControls();


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogMacro)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogMacro)
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnKillfocusEditDiscount();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGMACRO_H__5943A67F_CBBC_41E9_A77A_5F5F687C3E8C__INCLUDED_)
