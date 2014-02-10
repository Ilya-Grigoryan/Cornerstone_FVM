#if !defined(AFX_VIEWDAILYSUMMARY_H__A8E2E8C9_5481_4572_B104_9E7DEC7A0EDC__INCLUDED_)
#define AFX_VIEWDAILYSUMMARY_H__A8E2E8C9_5481_4572_B104_9E7DEC7A0EDC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewDailySummary.h : header file
//

#include "SQLMeetings.h"
#include "SQLUser.h"
#include "SQLDailySum.h"

/////////////////////////////////////////////////////////////////////////////
// CViewDailySummary form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CView2x2Metrics;
class CViewDailySummary : public CFormView
{
protected:
	CViewDailySummary();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewDailySummary)

// Form Data
public:
	//{{AFX_DATA(CViewDailySummary)
	enum { IDD = IDD_DIALOG_DAILYSUMMARY };
	CEdit	m_editOneLiner;
	CComboBox	m_comboExecution;
	CComboBox	m_comboVision;
	CSliderCtrl	m_sliderFund;
	CSliderCtrl	m_sliderValue;
	CComboBox	m_comboBias;
	CComboBox	m_comboTarget;
	CEdit	m_editNotesValuation;
	CEdit	m_editNotesManagement;
	CEdit	m_editNotesFundamentals;
	CEdit	m_editDate;
	CComboBox	m_comboCompany;
	CComboBox	m_comboAnalysts;
	int		m_nExecution;
	int		m_nVision;
	//}}AFX_DATA
	CView2x2Metrics*	m_pViewMetrics;
	int		m_nID;

	int		m_nUserLevel;

	BOOL	m_bConfirmed;
	double	m_fWeight2;
	CString	m_sBias2;
	CString	m_sOneLiner;

	void Resize();
	void SetData(SDailySum& daily);
	
	void UpdateView();
	void SetDataToDefaults();
	BOOL SetDataCombineAllAnalysts(int nCompany, COleDateTime date);

	void SetPermissions();

	BOOL GetData();
	BOOL CreateTemplateFromResource(unsigned int nRes, const char* szFile);

	void SelectUserInCombo(int nUserId);
	void EnablePermissions();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewDailySummary)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewDailySummary();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewDailySummary)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnDailysumsSave();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnSelchangeComboAnalyst();
	afx_msg void OnSelchangeComboCompany();
	afx_msg void OnKillfocusEditItemdate();
	afx_msg void OnDailysums2minute();
	afx_msg void OnDailysumsPrint();
	afx_msg void OnDailysumsUnlock();
	afx_msg void OnDailysumsLock();
	afx_msg void OnConfirm();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWDAILYSUMMARY_H__A8E2E8C9_5481_4572_B104_9E7DEC7A0EDC__INCLUDED_)
