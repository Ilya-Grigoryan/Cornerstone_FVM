#if !defined(AFX_DIALOGMONTECARLOINPUTS_H__33914B67_68C3_46E0_BEC6_B4E80C0D0734__INCLUDED_)
#define AFX_DIALOGMONTECARLOINPUTS_H__33914B67_68C3_46E0_BEC6_B4E80C0D0734__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogMonteCarloInputs.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogMonteCarloInputs dialog

#include "MonteCarlo.h"

class CCompany;
class CViewMonteCarlo;
class CMonteCarloSimulation;
class CDialogMonteCarloInputs : public CDialog
{
// Construction
public:
	CDialogMonteCarloInputs(CCompany* pCompany, CWnd* pParent = NULL);   // standard constructor

// Dialog Data
	//{{AFX_DATA(CDialogMonteCarloInputs)
	enum { IDD = IDD_DIALOG_MONTECARLO_INPUTS };
	CEdit	m_editF2;
	CEdit	m_editF1;
	CComboBox	m_comboDistrib;
	CListBox	m_ListParams;
	int		m_nMarketDirection;
	int		m_nMarketVariance;
	int		m_nDistrib;
	double	m_fF1;
	double	m_fF2;
	int		m_nIterations;
	BOOL	m_bInteractive;
	BOOL	m_bOutActual;
	BOOL	m_bOutFore;
	BOOL	m_bOutRazor;
	BOOL	m_bOutForeRazor;
	//}}AFX_DATA

	CFont		m_Font;

	CViewMonteCarlo*	m_pParent;
	CCompany*			m_pCompany;
	CMonteCarloSimulation m_Simulation;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogMonteCarloInputs)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnOK() {}
	virtual void OnCancel() {}
	//}}AFX_VIRTUAL
	
public:
	void SetupSimulation();
	void SetDistributions(int nDirection, int nVariance);
	void ApplyMarketConditions(double& fMean, double &fStdev);

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogMonteCarloInputs)
	virtual BOOL OnInitDialog();
	afx_msg void OnSelchangeList();
	afx_msg void OnButtonSetparams();
	afx_msg void OnButtonRun();
	afx_msg void OnKillfocusEditF1();
	afx_msg void OnKillfocusEditF2();
	afx_msg void OnSelchangeComboDistrib();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGMONTECARLOINPUTS_H__33914B67_68C3_46E0_BEC6_B4E80C0D0734__INCLUDED_)
