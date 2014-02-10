#if !defined(AFX_VIEWHISTORY_H__A0DC902D_A3F6_40A3_9334_24BE244E4F50__INCLUDED_)
#define AFX_VIEWHISTORY_H__A0DC902D_A3F6_40A3_9334_24BE244E4F50__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewHistory.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewHistory form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CCompany;
class CWndGrid;
class CViewHistory : public CFormView
{
protected:
	CViewHistory();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewHistory)

// Form Data
public:
	//{{AFX_DATA(CViewHistory)
	enum { IDD = IDD_DIALOG_HISTORY };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CCompany*					m_pCompany;

	CWndGrid*					m_pGrid;
	CFont						m_Font;

// Operations
public:
	void SetCompany(CCompany* pCompany);
	void PopulateGrid();
	void Resize();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewHistory)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewHistory();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewHistory)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnHistoryDelete();
	afx_msg void OnHistoryExcel();
	afx_msg void OnHistoryGoto();
	afx_msg void OnHistoryGraph();
	afx_msg void OnHistorySave();
	afx_msg void OnHistoryAddToHistory();
	afx_msg void OnHistoryCopy();
	afx_msg void OnHistoryPaste();
	afx_msg void OnHistoryPrint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWHISTORY_H__A0DC902D_A3F6_40A3_9334_24BE244E4F50__INCLUDED_)
