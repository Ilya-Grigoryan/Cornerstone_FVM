// cip_fileproxy.h : main header file for the CIP_FILEPROXY DLL
//

#if !defined(AFX_CIP_FILEPROXY_H__BAE117CA_00A3_4567_A7FB_65B9B4584EAF__INCLUDED_)
#define AFX_CIP_FILEPROXY_H__BAE117CA_00A3_4567_A7FB_65B9B4584EAF__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include <afxtempl.h>

#define _INF_	((double)(0xffffffff-1))

/////////////////////////////////////////////////////////////////////////////
// CCip_fileproxyApp
// See cip_fileproxy.cpp for the implementation of this class
//

class CCip_fileproxyApp : public CWinApp
{
public:
	CCip_fileproxyApp();

	BOOL	IsInf(double val)		{ return val > _INF_;	}

	// this is 
	int				m_nCurrent;
	CArray<int,int> m_arIds;
	CStringArray	m_arTickers;

	int				m_nAttempt;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCip_fileproxyApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCip_fileproxyApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CIP_FILEPROXY_H__BAE117CA_00A3_4567_A7FB_65B9B4584EAF__INCLUDED_)
