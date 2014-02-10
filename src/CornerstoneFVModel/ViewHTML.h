#if !defined(AFX_VIEWHTML_H__93A9DDAB_CE30_4BB9_BA3B_35E12141DD8F__INCLUDED_)
#define AFX_VIEWHTML_H__93A9DDAB_CE30_4BB9_BA3B_35E12141DD8F__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewHTML.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewHTML html view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif
#include <afxhtml.h>

class CViewHTML : public CHtmlView
{
protected:
	CViewHTML();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewHTML)

// html Data
public:
	//{{AFX_DATA(CViewHTML)
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewHTML)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewHTML();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewHTML)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWHTML_H__93A9DDAB_CE30_4BB9_BA3B_35E12141DD8F__INCLUDED_)
