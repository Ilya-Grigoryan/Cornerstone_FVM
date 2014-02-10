#if !defined(AFX_VIEWMEETINGS_H__5EE37171_15F9_408B_AC30_79ABDC22EFA2__INCLUDED_)
#define AFX_VIEWMEETINGS_H__5EE37171_15F9_408B_AC30_79ABDC22EFA2__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewMeetings.h : header file
//

#include "ListMeetings.h"
#include "ListFiles.h"

#include "SQLMeetings.h"

/////////////////////////////////////////////////////////////////////////////
// CViewMeetings form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CViewMeetings : public CFormView
{
protected:
	CViewMeetings();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewMeetings)

// Form Data
public:
	//{{AFX_DATA(CViewMeetings)
	enum { IDD = IDD_DIALOG_MEETINGS };
	CListMeetings	m_ListMeetings;
	CListFiles	m_ListFiles;
	CComboBox	m_comboRec;
	CComboBox	m_comboDec;
	CEdit	m_editDesc;
	CEdit	m_editAnalyst;
	CEdit	m_editDate;
	CComboBox	m_comboCompany;
	//}}AFX_DATA
	int				m_nCompanyID;
	COleDateTime	m_From;
	COleDateTime	m_To;
	int				m_nRec;
	int				m_nDec;
	int				m_nSort;

	SMeetingArray	m_arMeetings;

// Attributes
public:

// Operations
public:
	void Load();
	void SetData();

	void FilterByCompany(const char* szTicker);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewMeetings)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewMeetings();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewMeetings)
	afx_msg void OnMeetingsDelete();
	afx_msg void OnMeetingsFilter();
	afx_msg void OnMeetingsHistory();
	afx_msg void OnMeetingsNew();
	afx_msg void OnMeetingsSave();
	afx_msg void OnButtonOpenFile();
	afx_msg void OnButtonAddFile();
	afx_msg void OnButtonDeleteFile();
	afx_msg void OnButtonDeleteAllfiles();
	afx_msg void OnSelchangeListMain();
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnMeetingsCompany();
	afx_msg void OnMeetingsChart();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWMEETINGS_H__5EE37171_15F9_408B_AC30_79ABDC22EFA2__INCLUDED_)
