#if !defined(AFX_COMBO_H__64C4373E_F88D_4FF2_9FA2_7EF75883D130__INCLUDED_)
#define AFX_COMBO_H__64C4373E_F88D_4FF2_9FA2_7EF75883D130__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Combo.h : header file
//

//#include "s:\include\sortedarray.h"

/////////////////////////////////////////////////////////////////////////////
// CCombo window
#define WMU_SETTEXT		WM_USER + 121

class CCombo : public CComboBox
{
// Construction
public:
	CCombo();
	BOOL Create(CWnd* pParent, BOOL bEdit);
	BOOL Show(CWnd* pParent, CRect& rc, CStringArray* pArray, LPCTSTR szCurrent, BOOL bEdit);

// Attributes
protected:
	void CreateCorrectSize(CStringArray*);

public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCombo)
	protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCombo();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCombo)
	afx_msg void OnSelchange();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSelendcancel();
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_COMBO_H__64C4373E_F88D_4FF2_9FA2_7EF75883D130__INCLUDED_)
