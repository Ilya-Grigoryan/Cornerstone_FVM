// cip_core.h : main header file for the CIP_CORE DLL
//

#if !defined(AFX_CIP_CORE_H__26D28ED2_BE98_40FB_8611_4120A8E6C213__INCLUDED_)
#define AFX_CIP_CORE_H__26D28ED2_BE98_40FB_8611_4120A8E6C213__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols
#include "cipexception.h"
#include "macroparameters.h"

/////////////////////////////////////////////////////////////////////////////
// CCip_coreApp
// See cip_core.cpp for the implementation of this class
//

class CCip_coreApp : public CWinApp
{
public:
	CCip_coreApp();

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCip_coreApp)
	//}}AFX_VIRTUAL

	//{{AFX_MSG(CCip_coreApp)
		// NOTE - the ClassWizard will add and remove member functions here.
		//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	CCIPException		m_Exception;
	CMacroParameters	m_Macro;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CIP_CORE_H__26D28ED2_BE98_40FB_8611_4120A8E6C213__INCLUDED_)
