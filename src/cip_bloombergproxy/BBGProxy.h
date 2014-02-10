#if !defined(AFX_BBGPROXY_H__1B0E85CA_9289_4084_B2B0_D0869467369A__INCLUDED_)
#define AFX_BBGPROXY_H__1B0E85CA_9289_4084_B2B0_D0869467369A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// BBGProxy.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CBBGProxy command target

class CBBGProxy : public CCmdTarget
{
	DECLARE_DYNCREATE(CBBGProxy)

	CBBGProxy();           // protected constructor used by dynamic creation

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CBBGProxy)
	public:
	virtual void OnFinalRelease();
	//}}AFX_VIRTUAL

// Implementation
protected:
	virtual ~CBBGProxy();

	// Generated message map functions
	//{{AFX_MSG(CBBGProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
	DECLARE_OLECREATE(CBBGProxy)

	// Generated OLE dispatch map functions
	//{{AFX_DISPATCH(CBBGProxy)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_DISPATCH
	DECLARE_DISPATCH_MAP()
	DECLARE_INTERFACE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BBGPROXY_H__1B0E85CA_9289_4084_B2B0_D0869467369A__INCLUDED_)
