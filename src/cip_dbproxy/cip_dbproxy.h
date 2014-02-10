// cip_dbproxy.h : main header file for the CIP_DBPROXY DLL
//

#if !defined(AFX_CIP_DBPROXY_H__28ACAC9F_6676_41C9_A8F4_078464C58D6E__INCLUDED_)
#define AFX_CIP_DBPROXY_H__28ACAC9F_6676_41C9_A8F4_078464C58D6E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

typedef unsigned int           SOCKET;
#include <mysql.h>

/////////////////////////////////////////////////////////////////////////////
// CCip_dbproxyApp
// See cip_dbproxy.cpp for the implementation of this class
//

class CCip_dbproxyApp : public CWinApp
{
public:
	CCip_dbproxyApp();

	MYSQL*	m_sql;
	BOOL	m_bConnected;
	CString	m_sError;

	CString	m_sDatabase;

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCip_dbproxyApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCip_dbproxyApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CIP_DBPROXY_H__28ACAC9F_6676_41C9_A8F4_078464C58D6E__INCLUDED_)
