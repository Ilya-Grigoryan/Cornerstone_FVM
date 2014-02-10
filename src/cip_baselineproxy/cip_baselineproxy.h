// cip_baselineproxy.h : main header file for the CIP_BASELINEPROXY application
//

#if !defined(AFX_CIP_BASELINEPROXY_H__3446E25F_DF14_45FE_9CE9_B0FD086CF37D__INCLUDED_)
#define AFX_CIP_BASELINEPROXY_H__3446E25F_DF14_45FE_9CE9_B0FD086CF37D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		// main symbols

#include "EngineMSExcel.h"

/////////////////////////////////////////////////////////////////////////////
// CApp:
// See cip_baselineproxy.cpp for the implementation of this class
//

class CApp : public CWinApp
{
public:
	CApp();

public:
	CString	CreateMainDirectory();
	BOOL	CreateTemplateFromResource(unsigned int nRes, const char* szFile);

	COleDateTime CalcInterestRate(CEngineMSExcel &excel, const COleDateTime &date, double &fWeeklyAve, double &f3MAve, double &fFridayClose);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CApp)
	public:
	virtual BOOL InitInstance();
	//}}AFX_VIRTUAL

// Implementation

	//{{AFX_MSG(CApp)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CIP_BASELINEPROXY_H__3446E25F_DF14_45FE_9CE9_B0FD086CF37D__INCLUDED_)
