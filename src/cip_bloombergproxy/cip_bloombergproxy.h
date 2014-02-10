// cip_bloombergproxy.h : main header file for the CIP_BLOOMBERGPROXY DLL
//

#if !defined(AFX_CIP_BLOOMBERGPROXY_H__E7C7095D_4880_4D3B_94B2_109DE33C214E__INCLUDED_)
#define AFX_CIP_BLOOMBERGPROXY_H__E7C7095D_4880_4D3B_94B2_109DE33C214E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

/////////////////////////////////////////////////////////////////////////////
// CCip_bloombergproxyApp
// See cip_bloombergproxy.cpp for the implementation of this class
//

class CCip_bloombergproxyApp : public CWinApp
{
public:
	CCip_bloombergproxyApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCip_bloombergproxyApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCip_bloombergproxyApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CIP_BLOOMBERGPROXY_H__E7C7095D_4880_4D3B_94B2_109DE33C214E__INCLUDED_)
