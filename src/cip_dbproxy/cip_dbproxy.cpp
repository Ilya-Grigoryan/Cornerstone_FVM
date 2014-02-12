// cip_dbproxy.cpp : Defines the initialization routines for the DLL.
//

#include "stdafx.h"
#include "cip_dbproxy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//
//	Note!
//
//		If this DLL is dynamically linked against the MFC
//		DLLs, any functions exported from this DLL which
//		call into MFC must have the AFX_MANAGE_STATE macro
//		added at the very beginning of the function.
//
//		For example:
//
//		extern "C" BOOL PASCAL EXPORT ExportedFunction()
//		{
//			AFX_MANAGE_STATE(AfxGetStaticModuleState());
//			// normal function body here
//		}
//
//		It is very important that this macro appear in each
//		function, prior to any calls into MFC.  This means that
//		it must appear as the first statement within the
//		function, even before any object variable declarations
//		as their constructors may generate calls into the MFC
//		DLL.
//
//		Please see MFC Technical Notes 33 and 58 for additional
//		details.
//

/////////////////////////////////////////////////////////////////////////////
// CCip_dbproxyApp

BEGIN_MESSAGE_MAP(CCip_dbproxyApp, CWinApp)
	//{{AFX_MSG_MAP(CCip_dbproxyApp)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CCip_dbproxyApp construction

CCip_dbproxyApp::CCip_dbproxyApp()
{
	m_bConnected = false;
}

/////////////////////////////////////////////////////////////////////////////
// The one and only CCip_dbproxyApp object

CCip_dbproxyApp theApp;

BOOL CCip_dbproxyApp::InitInstance()
{
	// get connection information from registry
	SetRegistryKey(_T("Cornerstone"));
	strcpy((char*)m_pszProfileName , "FV Model");

	CString sServer1 = GetProfileString("Server Connections", "server1");
	CString sServer2 = GetProfileString("Server Connections", "server2");
	CString sServer3 = GetProfileString("Server Connections", "server3");

	CString sLogin1 = GetProfileString("Server Connections", "login1");
	CString sLogin2 = GetProfileString("Server Connections", "login2");
	CString sLogin3 = GetProfileString("Server Connections", "login3");

	CString sPsw1 = GetProfileString("Server Connections", "psw1");
	CString sPsw2 = GetProfileString("Server Connections", "psw2");
	CString sPsw3 = GetProfileString("Server Connections", "psw3");

	CString sDb = GetProfileString("Server Connections", "db");

	if(sDb.IsEmpty() || sDb == "0")
	{
		m_sDatabase = "cip_v2"; //"cip_universe";
	}
	else
	{
		m_sDatabase = "cip_v2"; //"cip_universe_blp";
	}
	// first time default (compatibility)
	if(sServer1.IsEmpty())
	{
		sServer1 = "cornerstone_dev";
		sLogin1 = "cip_client";
		sPsw1 = "cipclientresearch";
	}

	// init
	if( !(m_sql = mysql_init((MYSQL*)0)) )
	{
		::MessageBox(NULL,	_T("Unable to initialize MySQL. Please reinstall the application"),
		             _T("Cornerstone Fair Value Model"), MB_ICONSTOP);
		m_bConnected = false;
		return CWinApp::InitInstance();
	}

	CString sMsg;
	m_bConnected = false;

	// try to connect to the first server
	if(mysql_real_connect( m_sql, sServer1, sLogin1, sPsw1, m_sDatabase, MYSQL_PORT, NULL, 0 ))
	{
		m_sql->reconnect= 1;
		m_bConnected = true;
		return CWinApp::InitInstance();
	}
	else
	{
		goto _server2;
	}

_server2:
	if(sServer2.IsEmpty())
	{
		sMsg.Format(_T("Unable to connect to the specified servers or a proper Data-Base is missing.\n\nPlease enter connection settings on 'Connections/Server Connections' tab and restart the application."));
		::MessageBox(NULL, sMsg, _T("Cornerstone Fair Value Model"), MB_ICONSTOP);
		return CWinApp::InitInstance();
	}

	sMsg.Format(_T("Unable to connect to the server '%s' or a proper Data-Base is missing on the server.\n\nDo you want to connect to the second preferred server '%s'? Note that the second server may not contain the latest information available."),
	            sServer1, sServer2);
	int nResult = ::MessageBox(NULL, sMsg, _T("Cornerstone Fair Value Model"), MB_ICONQUESTION|MB_YESNO);
	if(nResult != IDYES)
		return CWinApp::InitInstance();

	// try to connect to the second server
	if(mysql_real_connect( m_sql, sServer2, sLogin2, sPsw2, NULL, MYSQL_PORT, NULL, 0 ))
	{
		m_sql->reconnect= 1;
		if(mysql_select_db( m_sql, m_sDatabase ) != 0)
			goto _server3;
		m_bConnected = true;
		return CWinApp::InitInstance();
	}

_server3:
	if(sServer3.IsEmpty())
	{
		sMsg.Format(_T("Unable to connect to the specified servers or a proper Data-Base is missing.\n\nPlease enter connection settings on 'Connections/Server Connections' tab and restart the application."));
		::MessageBox(NULL, sMsg, _T("Cornerstone Fair Value Model"), MB_ICONSTOP);
		return CWinApp::InitInstance();
	}

	sMsg.Format(_T("Unable to connect to the server '%s' or a proper Data-Base is missing on the server.\n\nDo you want to connect to the third preferred server '%s'? Note that the second server may not contain the latest information available."),
	            sServer2, sServer3);
	nResult = ::MessageBox(NULL, sMsg, _T("Cornerstone Fair Value Model"), MB_ICONQUESTION|MB_YESNO);
	if(nResult != IDYES)
		return CWinApp::InitInstance();

	// try to connect to the third server
	if(mysql_real_connect( m_sql, sServer3, sLogin3, sPsw3, NULL, MYSQL_PORT, NULL, 0 ))
	{
		m_sql->reconnect= 1;
		if(mysql_select_db( m_sql, m_sDatabase ) != 0)
		{
			sMsg.Format(_T("Unable to connect to the specified servers or a proper Data-Base is missing.\n\nPlease enter connection settings on 'Connections/Server Connections' tab and restart the application."));
			::MessageBox(NULL, sMsg, _T("Cornerstone Fair Value Model"), MB_ICONSTOP);
			return CWinApp::InitInstance();
		}
		m_bConnected = true;
		return CWinApp::InitInstance();
	}

	return CWinApp::InitInstance();
}
