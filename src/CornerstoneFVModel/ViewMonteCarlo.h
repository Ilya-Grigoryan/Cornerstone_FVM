#if !defined(AFX_VIEWMONTECARLO_H__FB2DC8BA_6E1F_4910_97BB_9ECC3549CE05__INCLUDED_)
#define AFX_VIEWMONTECARLO_H__FB2DC8BA_6E1F_4910_97BB_9ECC3549CE05__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewMonteCarlo.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewMonteCarlo form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CCompany;
class CDialogMonteCarloInputs;
class CWndGrid;
class CViewGraph;
class CViewMonteCarlo : public CFormView
{
protected:
	CViewMonteCarlo();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewMonteCarlo)

// Form Data
public:
	//{{AFX_DATA(CViewMonteCarlo)
	enum { IDD = IDD_FORM_MONTECARLO };
	CStatic	m_stPercent;
	CEdit	m_editPercent;
	CSliderCtrl	m_sliderLeft;
	CTabCtrl	m_TabOuts;
	//}}AFX_DATA

// Attributes
public:
	CCompany*		m_pCompany;

	double		m_fCumProb;

	CDialogMonteCarloInputs*	m_pDialogInputs;
	CViewGraph*					m_pGraph;
	CWndGrid*					m_pGrid;
	CFont						m_Font;

// Operations
public:
	void SetCompany(CCompany* pCompany);
	void SetCharts();
	void SetOutStats();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewMonteCarlo)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

	void Resize();

// Implementation
protected:
	virtual ~CViewMonteCarlo();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewMonteCarlo)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnSelchangeTabOuts(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWMONTECARLO_H__FB2DC8BA_6E1F_4910_97BB_9ECC3549CE05__INCLUDED_)
