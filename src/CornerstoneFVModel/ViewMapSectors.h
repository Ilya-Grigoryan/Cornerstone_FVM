#if !defined(AFX_VIEWMAPSECTORS_H__8778838B_17E7_4F13_AD92_A0593CF58A21__INCLUDED_)
#define AFX_VIEWMAPSECTORS_H__8778838B_17E7_4F13_AD92_A0593CF58A21__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ViewMapSectors.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CViewMapSectors form view

#ifndef __AFXEXT_H__
#include <afxext.h>
#endif

class CViewGraph;
class CViewMapSectors : public CFormView
{
protected:
	CViewMapSectors();           // protected constructor used by dynamic creation
	DECLARE_DYNCREATE(CViewMapSectors)

// Form Data
public:
	//{{AFX_DATA(CViewMapSectors)
	enum { IDD = IDD_DIALOG_MAP_SECTORS };
		// NOTE: the ClassWizard will add data members here
	//}}AFX_DATA

// Attributes
public:
	CViewGraph*		m_pGraph;

// Operations
public:
	void Resize();
	void SetupMap();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CViewMapSectors)
	public:
	virtual void OnInitialUpdate();
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CViewMapSectors();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

	// Generated message map functions
	//{{AFX_MSG(CViewMapSectors)
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_VIEWMAPSECTORS_H__8778838B_17E7_4F13_AD92_A0593CF58A21__INCLUDED_)
