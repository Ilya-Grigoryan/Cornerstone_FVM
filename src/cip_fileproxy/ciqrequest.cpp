// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "ciqrequest.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// IDataRequest properties

/////////////////////////////////////////////////////////////////////////////
// IDataRequest operations

CString IDataRequest::ToString()
{
	CString result;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long IDataRequest::GetItemCount()
{
	long result;
	InvokeHelper(0x60020001, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IDataRequest::Add(LPUNKNOWN Fn)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Fn);
}

void IDataRequest::SetIdBase(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030001, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// IRequest properties

/////////////////////////////////////////////////////////////////////////////
// IRequest operations

CString IRequest::ToString()
{
	CString result;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long IRequest::GetItemCount()
{
	long result;
	InvokeHelper(0x60020001, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IRequestorList properties

/////////////////////////////////////////////////////////////////////////////
// IRequestorList operations

void IRequestorList::Add(LPUNKNOWN Fn, long Id)
{
	static BYTE parms[] =
		VTS_UNKNOWN VTS_I4;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Fn, Id);
}

CString IRequestorList::Remove()
{
	CString result;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

LPDISPATCH IRequestorList::RemoveRequestor()
{
	LPDISPATCH result;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL IRequestorList::HasItems()
{
	BOOL result;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

long IRequestorList::GetItemCount()
{
	long result;
	InvokeHelper(0x60020004, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// IRequestor properties

/////////////////////////////////////////////////////////////////////////////
// IRequestor operations

CString IRequestor::GetAddress()
{
	CString result;
	InvokeHelper(0x60020000, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void IRequestor::SetAddress(LPCTSTR lpszNewValue)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60020000, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 lpszNewValue);
}

long IRequestor::GetId()
{
	long result;
	InvokeHelper(0x60020002, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void IRequestor::SetId(long nNewValue)
{
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60020002, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 nNewValue);
}


/////////////////////////////////////////////////////////////////////////////
// IBatchRequest properties

/////////////////////////////////////////////////////////////////////////////
// IBatchRequest operations

void IBatchRequest::AddFunction(LPUNKNOWN Fn, BOOL isArray)
{
	static BYTE parms[] =
		VTS_UNKNOWN VTS_BOOL;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Fn, isArray);
}

LPDISPATCH IBatchRequest::Remove()
{
	LPDISPATCH result;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL IBatchRequest::HasItems()
{
	BOOL result;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

BOOL IBatchRequest::Pending()
{
	BOOL result;
	InvokeHelper(0x60020003, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBatchRequest::GetRequestors()
{
	LPDISPATCH result;
	InvokeHelper(0x60020004, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

LPDISPATCH IBatchRequest::GetArrays()
{
	LPDISPATCH result;
	InvokeHelper(0x60020005, DISPATCH_PROPERTYGET, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

BOOL IBatchRequest::GetUpdateInProgress()
{
	BOOL result;
	InvokeHelper(0x60020006, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

void IBatchRequest::SetUpdateInProgress(BOOL bNewValue)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60020006, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 bNewValue);
}

long IBatchRequest::GetItemCount()
{
	long result;
	InvokeHelper(0x60020008, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// ILogPostRequest properties

/////////////////////////////////////////////////////////////////////////////
// ILogPostRequest operations

CString ILogPostRequest::ToString()
{
	CString result;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long ILogPostRequest::GetItemCount()
{
	long result;
	InvokeHelper(0x60020001, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void ILogPostRequest::SetLog(LPCTSTR data)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 data);
}


/////////////////////////////////////////////////////////////////////////////
// ILoginRequest properties

/////////////////////////////////////////////////////////////////////////////
// ILoginRequest operations

CString ILoginRequest::ToString()
{
	CString result;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long ILoginRequest::GetItemCount()
{
	long result;
	InvokeHelper(0x60020001, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void ILoginRequest::SetVersion(BOOL TestFlag)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 TestFlag);
}


/////////////////////////////////////////////////////////////////////////////
// ICompanySearchRequest properties

/////////////////////////////////////////////////////////////////////////////
// ICompanySearchRequest operations

CString ICompanySearchRequest::ToString()
{
	CString result;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

long ICompanySearchRequest::GetItemCount()
{
	long result;
	InvokeHelper(0x60020001, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

void ICompanySearchRequest::FillCriteria(LPCTSTR Filter, LPCTSTR SearchString, BOOL Inactive)
{
	static BYTE parms[] =
		VTS_BSTR VTS_BSTR VTS_BOOL;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Filter, SearchString, Inactive);
}


/////////////////////////////////////////////////////////////////////////////
// IClientFunctionList properties

/////////////////////////////////////////////////////////////////////////////
// IClientFunctionList operations

void IClientFunctionList::Add(LPUNKNOWN newFn)
{
	static BYTE parms[] =
		VTS_UNKNOWN;
	InvokeHelper(0x60020000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 newFn);
}

LPUNKNOWN IClientFunctionList::Remove()
{
	LPUNKNOWN result;
	InvokeHelper(0x60020001, DISPATCH_METHOD, VT_UNKNOWN, (void*)&result, NULL);
	return result;
}

BOOL IClientFunctionList::HasItems()
{
	BOOL result;
	InvokeHelper(0x60020002, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}
