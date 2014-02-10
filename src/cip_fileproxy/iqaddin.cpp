// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "iqaddin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// ICiqAddin properties

/////////////////////////////////////////////////////////////////////////////
// ICiqAddin operations

void ICiqAddin::InitializeSelf(LPDISPATCH pHost)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pHost);
}

void ICiqAddin::Enable()
{
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// ICiqAddinHost properties

/////////////////////////////////////////////////////////////////////////////
// ICiqAddinHost operations

LPDISPATCH ICiqAddinHost::GetApplication()
{
	LPDISPATCH result;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

void ICiqAddinHost::ReportError(LPCTSTR errorText)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 errorText);
}

void ICiqAddinHost::RegisterXll(LPCTSTR fileName)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 fileName);
}

void ICiqAddinHost::RegisterAutoAddin(LPCTSTR progId)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 progId);
}

void ICiqAddinHost::RegisterExtensibilityAddin(LPUNKNOWN pConnect)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x60020004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 pConnect);
}

void ICiqAddinHost::LoadAll(long* ridError)
{
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60020005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 ridError);
}
