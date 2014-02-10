#if !defined(AFX_BLOOMBERG_H__C1B8164B_3167_4847_951A_E8B524F93ADB__INCLUDED_)
#define AFX_BLOOMBERG_H__C1B8164B_3167_4847_951A_E8B524F93ADB__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// Bloomberg.h : header file
//
class _Worksheet;


/////////////////////////////////////////////////////////////////////////////
// CBloomberg command target

class CBloomberg : public CCmdTarget
{
	DECLARE_DYNCREATE(CBloomberg)

	CBloomberg();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBloomberg)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

	long LoadEPS(_Worksheet *sheet);
	long LoadBalanceSheet(_Worksheet *sheet);

	static COleDateTime GetLastFriday(const COleDateTime &date);
	static COleDateTime GetDateFromString(const CString& str);

// Implementation
protected:
	virtual ~CBloomberg();

	// Generated message map functions
	//{{AFX_MSG(CBloomberg)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CBloomberg)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CBloomberg)
	afx_msg long LoadReturns(LPDISPATCH pSheet);
	afx_msg long SetTicker(LPDISPATCH lpSheet);
	afx_msg long PrepareTickers();
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BLOOMBERG_H__C1B8164B_3167_4847_951A_E8B524F93ADB__INCLUDED_)
