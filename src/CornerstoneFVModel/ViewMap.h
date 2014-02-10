#if !defined(AFX_VIEWMAP_H__EAC0EFFB_8676_4966_96D1_42BD753EA93D__INCLUDED_)
#define AFX_VIEWMAP_H__EAC0EFFB_8676_4966_96D1_42BD753EA93D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewMap.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewMap form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CViewGraph;
class CViewMap : public CFormView
{
protected:
	CViewMap();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewMap)

// Form Data
public:
	//{{AFX_DATA(CViewMap)
	enum { IDD = IDD_DIALOG_MAP };
	CEdit	m_editSymbols;
	//}}AFX_DATA

// Attributes
public:
	COleDateTime m_From;
	COleDateTime m_To;

	int	m_nStep;

	CViewGraph*		m_pGraph;
	CArray<CCompany*, CCompany*>	m_arCompanies;

	CArray<COLORREF, COLORREF> m_ColorTableArray;

// Operations
public:
	void Resize();

	void SetMaps(const char* tokens);
	void SetupMap(CCompany* pCompany);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewMap)
	public:
	virtual void OnInitialUpdate();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewMap();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewMap)
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnMapFrom();
	afx_msg void OnMapEnd();
	afx_msg void OnMapHistory();
	afx_msg void OnMapGraph();
	afx_msg void OnMapStep();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWMAP_H__EAC0EFFB_8676_4966_96D1_42BD753EA93D__INCLUDED_)
