// ViewGeneral.h : interface of the CViewGeneral class
//
/////////////////////////////////////////////////////////////////////////////

#if !defined(AFX_VIEWGENERAL_H__0A7A317D_B54C_4336_9157_6831811F19F0__INCLUDED_)
#define AFX_VIEWGENERAL_H__0A7A317D_B54C_4336_9157_6831811F19F0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Universe.h"
#include "ViewPercentile.h"

class CWndGrid;
class CViewGeneral : public CFormView
{
protected: // create from serialization only
	CViewGeneral();
	DECLARE_DYNCREATE(CViewGeneral)

public:
	//{{AFX_DATA(CViewGeneral)
	enum { IDD = IDD_CORNERSTONEFVMODEL_FORM };
	CButton	m_checkLoadROCROE;
	CComboBox	m_comboType;
	CButton	m_checkLoadBS;
	CButton	m_checkLoadEPS;
	CStatic	m_stDate;
	CButton	m_checkUseDebtAdjust;
	CStatic	m_stCaption;
	//}}AFX_DATA
	CCompany*	m_pCompany;

	CWndGrid*	m_pGridEps;
	CWndGrid*	m_pGridYrs;
	CWndGrid*	m_pGridBS;
	CWndGrid*	m_pGridInput2;
	CWndGrid*	m_pGridRatios;
	CWndGrid*	m_pGridRatios2;
	CWndGrid*	m_pGridRanksHeader;
	CWndGrid*	m_pGridRanks;

	CViewPercentile*	m_pViewPercentActual;
	CViewPercentile*	m_pViewPercentFore;
	CViewPercentile*	m_pViewPercentRazor;
	CViewPercentile*	m_pViewPercentRazorFore;

	CFont		m_font;
	CFont		m_font2;
	CFont		m_font3;

	BOOL		m_bChanged;

// Attributes
public:
	CDoc* GetDocument();

protected:
	void FillEpsRocRoe();
	void FillBalanceSheet();
	void FillInput2();
	void FillRatios();
	void FillRatios2();
	void FillRanks();

	void UpdateCompany();

	void SetBSNamesFromType(int nType);

// Operations
public:
	void Resize();
	void SetCompany(CCompany* pCompany);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewGeneral)
	public:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	virtual void OnPrepareDC(CDC* pDC, CPrintInfo* pInfo = NULL);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual void OnInitialUpdate(); // called first time after construct
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnPrint(CDC* pDC, CPrintInfo* pInfo);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CViewGeneral();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	//{{AFX_MSG(CViewGeneral)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDestroy();
	afx_msg void OnGeneralCalc();
	afx_msg void OnUpdateGeneralCalc(CCmdUI* pCmdUI);
	afx_msg void OnGeneralSave();
	afx_msg void OnUpdateGeneralSave(CCmdUI* pCmdUI);
	afx_msg void OnGeneralSplit();
	afx_msg void OnUpdateGeneralSplit(CCmdUI* pCmdUI);
	afx_msg void OnGeneralExcel();
	afx_msg void OnUpdateGeneralExcel(CCmdUI* pCmdUI);
	afx_msg void OnGeneralWord();
	afx_msg void OnUpdateGeneralWord(CCmdUI* pCmdUI);
	afx_msg void OnGeneralGraph();
	afx_msg void OnUpdateGeneralGraph(CCmdUI* pCmdUI);
	afx_msg void OnGeneralOpen();
	afx_msg void OnUpdateGeneralOpen(CCmdUI* pCmdUI);
	afx_msg void OnGeneralSavetodb();
	afx_msg void OnUpdateGeneralSavetodb(CCmdUI* pCmdUI);
	afx_msg void OnGeneralSensAnalysis();
	afx_msg void OnUpdateGeneralSensAnalysis(CCmdUI* pCmdUI);
	afx_msg void OnGeneralMontecarlo();
	afx_msg void OnUpdateGeneralMontecarlo(CCmdUI* pCmdUI);
	afx_msg void OnSelchangeComboType();
	afx_msg void OnGeneralLastEpsYear();
	afx_msg void OnCheckDownloadBs();
	afx_msg void OnCheckDownloadEps();
	afx_msg void OnCheckUseDebtAdjust();
	afx_msg void OnGeneralHistory();
	afx_msg void OnGeneralAddToHistory();
	afx_msg void OnGeneralDate();
	afx_msg void OnGeneralEps();
	afx_msg void OnCheckDownloadRocroe();
	afx_msg void OnGeneralMeetings();
	afx_msg void OnGeneral2minute();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

#ifndef _DEBUG  // debug version in ViewGeneral.cpp
inline CDoc* CViewGeneral::GetDocument()
   { return (CDoc*)m_pDocument; }
#endif

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWGENERAL_H__0A7A317D_B54C_4336_9157_6831811F19F0__INCLUDED_)
