// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "iciqfunction.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// ICIQFunction properties

/////////////////////////////////////////////////////////////////////////////
// ICIQFunction operations

void ICIQFunction::AddParameter(LPCTSTR Value)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Value);
}

void ICIQFunction::SkipParameter()
{
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString ICIQFunction::GetResult()
{
	CString result;
	InvokeHelper(0x60020002, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void ICIQFunction::SetResult(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020002, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString ICIQFunction::GetExchange()
{
	CString result;
	InvokeHelper(0x60020004, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void ICIQFunction::SetExchange(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020004, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

CString ICIQFunction::GetTicker()
{
	CString result;
	InvokeHelper(0x60020006, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void ICIQFunction::SetTicker(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020006, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long ICIQFunction::GetCompanyId()
{
	long result;
	InvokeHelper(0x60020008, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void ICIQFunction::SetCompanyId(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020008, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long ICIQFunction::GetRealTimeCompanyId()
{
	long result;
	InvokeHelper(0x6002000a, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void ICIQFunction::SetRealTimeCompanyId(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long ICIQFunction::GetDataTypeIdAsInt()
{
	long result;
	InvokeHelper(0x6002000c, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void ICIQFunction::SetDataTypeIdAsInt(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002000c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}

long ICIQFunction::GetClickThroughParameters(short Index)
{
	long result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x6002000e, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, parms,
		Index);
	return result;
}

void ICIQFunction::SetClickThroughParameters(short Index, long nNewValue)
{
	static BYTE parms[] =
		VTS_I2 VTS_I4;
	InvokeHelper(0x6002000e, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 Index, nNewValue);
}

CString ICIQFunction::GetParameters(short Index)
{
	CString result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x60020010, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, parms,
		Index);
	return result;
}

void ICIQFunction::SetParameters(short Index, LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_I2 VTS_BSTR;
	InvokeHelper(0x60020010, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 Index, lpszNewValue);
}

short ICIQFunction::GetParameterCount()
{
	short result;
	InvokeHelper(0x60020012, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

short ICIQFunction::GetClickThroughParameterCount()
{
	short result;
	InvokeHelper(0x60020013, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

BOOL ICIQFunction::GetAuditable()
{
	BOOL result;
	InvokeHelper(0x60020014, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void ICIQFunction::SetAuditable(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60020014, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL ICIQFunction::GetFoundInCache()
{
	BOOL result;
	InvokeHelper(0x60020016, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void ICIQFunction::SetFoundInCache(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60020016, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

BOOL ICIQFunction::GetArrayFlag()
{
	BOOL result;
	InvokeHelper(0x60020018, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void ICIQFunction::SetArrayFlag(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60020018, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString ICIQFunction::GetName()
{
	CString result;
	InvokeHelper(0x6002001a, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void ICIQFunction::SetName(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6002001a, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL ICIQFunction::GetResultExpired()
{
	BOOL result;
	InvokeHelper(0x6002001c, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void ICIQFunction::SetResultExpired(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x6002001c, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString ICIQFunction::ToElement(long Id)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x6002001e, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Id);
	return result;
}

CString ICIQFunction::GetAddress()
{
	CString result;
	InvokeHelper(0x6002001f, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void ICIQFunction::SetAddress(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x6002001f, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

BOOL ICIQFunction::GetIdentifierInvalid()
{
	BOOL result;
	InvokeHelper(0x60020021, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void ICIQFunction::SetIdentifierInvalid(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60020021, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

CString ICIQFunction::DebugString()
{
	CString result;
	InvokeHelper(0x60020023, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

short ICIQFunction::GetLastFilledParam()
{
	short result;
	InvokeHelper(0x60020024, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}
