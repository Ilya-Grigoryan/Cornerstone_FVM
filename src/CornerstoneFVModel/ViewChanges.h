#if !defined(AFX_VIEWCHANGES_H__6C631245_BA8C_43B8_A4FF_F7357651ECCC__INCLUDED_)
#define AFX_VIEWCHANGES_H__6C631245_BA8C_43B8_A4FF_F7357651ECCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewTable.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewChanges form view

#include "Company.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CWndGrid;
class CViewChanges : public CFormView
{
protected:
	CViewChanges();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewChanges)

// Form Data
public:
	//{{AFX_DATA(CViewChanges)
	enum { IDD = IDD_DIALOG_CHANGES };
	CComboBox	m_comboSort;
	//}}AFX_DATA

// Attributes
public:
	CArray<CCompany*,CCompany*>					m_arCompanies;
	CArray<SChangeCriteria,SChangeCriteria>		m_arChanges;

	CWndGrid*						m_pGrid;
	CFont							m_Font;

// Operations
public:
	void SetData(const CArray<CCompany*,CCompany*>& arCompanies, const CArray<SChangeCriteria,SChangeCriteria>& arChanges);

	void SetupGrid();
	void Resize();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewChanges)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewChanges();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewChanges)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnChangesCopy();
	afx_msg void OnChangesExcel();
	afx_msg void OnChangesOpen();
	afx_msg void OnChangesPrint();
	afx_msg void OnSelchangeComboSort();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWCHANGES_H__6C631245_BA8C_43B8_A4FF_F7357651ECCC__INCLUDED_)
