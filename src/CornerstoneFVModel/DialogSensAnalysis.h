#if !defined(AFX_DIALOGSENSANALYSIS_H__99D6493B_164F_4D53_81DD_CDEA9F2B5CFF__INCLUDED_)
#define AFX_DIALOGSENSANALYSIS_H__99D6493B_164F_4D53_81DD_CDEA9F2B5CFF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// DialogSensAnalysis.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CDialogSensAnalysis dialog

class CCompany;
class CWndGrid;
class CViewGraph;
class CDialogSensAnalysis : public CDialog
{
// Construction
public:
	CDialogSensAnalysis(CCompany* pCompany, CWnd* pParent = NULL);   // standard constructor
	
	static void CallBack(int nInputs, double* inputs, int nOuts, double* outs);

// Dialog Data
	//{{AFX_DATA(CDialogSensAnalysis)
	enum { IDD = IDD_DIALOG_SENS_ANALYSIS };
	CComboBox	m_comboView;
	CStatic	m_stGrowthRazorFore;
	CStatic	m_stGrowthRazor;
	CStatic	m_stGrowthFore;
	CStatic	m_stGrowth;
	CSliderCtrl	m_slEPSThis;
	CSliderCtrl	m_slEPSNext;
	CSliderCtrl	m_slEPSLast;
	CEdit	m_editEPSThis;
	CEdit	m_editEPSNext;
	CEdit	m_editEPSLast;
	CSliderCtrl	m_slROE;
	CSliderCtrl	m_slROC;
	CSliderCtrl	m_slPrice;
	CSliderCtrl	m_slEPS;
	CSliderCtrl	m_slDiv;
	CSliderCtrl	m_slDiscount;
	CSliderCtrl	m_slDebt;
	CEdit	m_editROE;
	CEdit	m_editROC;
	CEdit	m_editPrice;
	CEdit	m_editEPS;
	CEdit	m_editDiv;
	CEdit	m_editDiscount;
	CEdit	m_editDebt;
	CStatic	m_stROIRazorFore;
	CStatic	m_stROIRazor;
	CStatic	m_stROIFore;
	CStatic	m_stMultipleRazorFore;
	CStatic	m_stMultipleRazor;
	CStatic	m_stMultipleFore;
	CStatic	m_stROI;
	CStatic	m_stMultiple;
	double	m_fDebt;
	double	m_fDiscount;
	double	m_fDiv;
	double	m_fEPS;
	double	m_fEPSLast;
	double	m_fEPSNext;
	double	m_fEPSThis;
	double	m_fPrice;
	double	m_fROC;
	double	m_fROE;
	//}}AFX_DATA

	CCompany*	m_pCompany;
	CCompany	m_OriginalCompany;
	double		m_fOriginalDiscount;

	CWndGrid*	m_pGrid;
	CViewGraph*	m_pGraph;

	CFont		m_font;
	

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CDialogSensAnalysis)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL
	
	void Resize();

	void SetGridData();
	void SetInputs();

	void DrawRankGraph(CDC* pDC, CRect rc, double fPercent);
	void Calc();
	void CalcForDiscount();

	void SetupChartROCEPV();
	void SetupChartROEEPV();
	void SetupChartEPSEPV();
	void SetupChartDebtEPV();
	void SetupChartDivEPV();
	void SetupChartEPSNextYEPV();
	void SetupChartEPSThisYEPV();
	void SetupChartEPSLastYEPV();

	void SetupChartDiscountEPV();

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CDialogSensAnalysis)
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnKillfocusEditRoc();
	afx_msg void OnButtonReset();
	afx_msg void OnSelchangeComboView();
	afx_msg void OnKillfocusEditEpsThis();
	afx_msg void OnKillfocusEditEpsNext();
	afx_msg void OnButtonExportRankings();
	afx_msg void OnKillfocusEditRoe();
	afx_msg void OnKillfocusEditEps();
	afx_msg void OnKillfocusEditDebt();
	afx_msg void OnKillfocusEditPrice();
	afx_msg void OnKillfocusEditDiscount();
	afx_msg void OnKillfocusEditDiv();
	afx_msg void OnKillfocusEditEpsLast();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_DIALOGSENSANALYSIS_H__99D6493B_164F_4D53_81DD_CDEA9F2B5CFF__INCLUDED_)
