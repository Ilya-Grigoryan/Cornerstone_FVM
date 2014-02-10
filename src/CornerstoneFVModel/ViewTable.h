#if !defined(AFX_VIEWTABLE_H__6C631245_BA8C_43B8_A4FF_F7357651ECCC__INCLUDED_)
#define AFX_VIEWTABLE_H__6C631245_BA8C_43B8_A4FF_F7357651ECCC__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewTable.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewTable form view

#include "DataManager.h"

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CWndGrid;
class CViewTable : public CFormView
{
protected:
	CViewTable();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewTable)

// Form Data
public:
	//{{AFX_DATA(CViewTable)
	enum { IDD = IDD_DIALOG_TABLE };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CDataCollection				m_DataCollect;
	CWndGrid*					m_pGrid;
	CFont						m_Font;

// Operations
public:
	void SetData(const CDataCollection& dataCollect);

	void SetupGrid();
	void Resize();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewTable)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewTable();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewTable)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnTableCopy();
	afx_msg void OnTableExcel();
	afx_msg void OnTablePrint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWTABLE_H__6C631245_BA8C_43B8_A4FF_F7357651ECCC__INCLUDED_)
