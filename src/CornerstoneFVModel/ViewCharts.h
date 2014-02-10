#if !defined(AFX_VIEWCHARTS_H__93AFC4D1_201A_4623_BD61_C82FCF7EB85E__INCLUDED_)
#define AFX_VIEWCHARTS_H__93AFC4D1_201A_4623_BD61_C82FCF7EB85E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewCharts.h : header file
//

#include "DataManager.h"

/////////////////////////////////////////////////////////////////////////////
// CViewCharts form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CData;
class CViewGraph;
class CWndGrid;

////////////////////////////////////////////////////////////////////////////////
class CViewCharts : public CFormView
{
protected: 
	CViewCharts();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewCharts)

// Form Data
public:
	//{{AFX_DATA(CViewCharts)
	enum { IDD = IDD_DIALOG_CHARTS };
	CEdit	m_editBase;
	CEdit	m_editSymbols;
	//}}AFX_DATA
	COleDateTime m_From;
	COleDateTime m_To;

	CFont m_font;

// Attributes
public:
	CViewGraph*		m_pGraph;
	CWndGrid*		m_pGrid;
	CArray<COLORREF, COLORREF> m_ColorTableArray;

	CDataCollection m_DataCollect;

// Operations
public:
	void Resize();

	void SetCharts(const char* szTokens, const char* szBase, BOOL bSetTextToEdit = false);
	void AddChart(CData* pData, int nIndex);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewCharts)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewCharts();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewCharts)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnChartsFrom();
	afx_msg void OnChartsEnd();
	afx_msg void OnChartsTable();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWCHARTS_H__93AFC4D1_201A_4623_BD61_C82FCF7EB85E__INCLUDED_)
