// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// IDataRequest wrapper class

class IDataRequest : public COleDispatchDriver
{
public:
	IDataRequest() {}		// Calls COleDispatchDriver default constructor
	IDataRequest(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IDataRequest(const IDataRequest& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString ToString();
	long GetItemCount();
	void Add(LPUNKNOWN Fn);
	void SetIdBase(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// IRequest wrapper class

class IRequest : public COleDispatchDriver
{
public:
	IRequest() {}		// Calls COleDispatchDriver default constructor
	IRequest(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IRequest(const IRequest& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString ToString();
	long GetItemCount();
};
/////////////////////////////////////////////////////////////////////////////
// IRequestorList wrapper class

class IRequestorList : public COleDispatchDriver
{
public:
	IRequestorList() {}		// Calls COleDispatchDriver default constructor
	IRequestorList(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IRequestorList(const IRequestorList& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void Add(LPUNKNOWN Fn, long Id);
	CString Remove();
	LPDISPATCH RemoveRequestor();
	BOOL HasItems();
	long GetItemCount();
};
/////////////////////////////////////////////////////////////////////////////
// IRequestor wrapper class

class IRequestor : public COleDispatchDriver
{
public:
	IRequestor() {}		// Calls COleDispatchDriver default constructor
	IRequestor(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IRequestor(const IRequestor& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString GetAddress();
	void SetAddress(LPCTSTR lpszNewValue);
	long GetId();
	void SetId(long nNewValue);
};
/////////////////////////////////////////////////////////////////////////////
// IBatchRequest wrapper class

class IBatchRequest : public COleDispatchDriver
{
public:
	IBatchRequest() {}		// Calls COleDispatchDriver default constructor
	IBatchRequest(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IBatchRequest(const IBatchRequest& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void AddFunction(LPUNKNOWN Fn, BOOL isArray);
	LPDISPATCH Remove();
	BOOL HasItems();
	BOOL Pending();
	LPDISPATCH GetRequestors();
	LPDISPATCH GetArrays();
	BOOL GetUpdateInProgress();
	void SetUpdateInProgress(BOOL bNewValue);
	long GetItemCount();
};
/////////////////////////////////////////////////////////////////////////////
// ILogPostRequest wrapper class

class ILogPostRequest : public COleDispatchDriver
{
public:
	ILogPostRequest() {}		// Calls COleDispatchDriver default constructor
	ILogPostRequest(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ILogPostRequest(const ILogPostRequest& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString ToString();
	long GetItemCount();
	void SetLog(LPCTSTR data);
};
/////////////////////////////////////////////////////////////////////////////
// ILoginRequest wrapper class

class ILoginRequest : public COleDispatchDriver
{
public:
	ILoginRequest() {}		// Calls COleDispatchDriver default constructor
	ILoginRequest(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ILoginRequest(const ILoginRequest& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString ToString();
	long GetItemCount();
	void SetVersion(BOOL TestFlag);
};
/////////////////////////////////////////////////////////////////////////////
// ICompanySearchRequest wrapper class

class ICompanySearchRequest : public COleDispatchDriver
{
public:
	ICompanySearchRequest() {}		// Calls COleDispatchDriver default constructor
	ICompanySearchRequest(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ICompanySearchRequest(const ICompanySearchRequest& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString ToString();
	long GetItemCount();
	void FillCriteria(LPCTSTR Filter, LPCTSTR SearchString, BOOL Inactive);
};
/////////////////////////////////////////////////////////////////////////////
// IClientFunctionList wrapper class

class IClientFunctionList : public COleDispatchDriver
{
public:
	IClientFunctionList() {}		// Calls COleDispatchDriver default constructor
	IClientFunctionList(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	IClientFunctionList(const IClientFunctionList& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void Add(LPUNKNOWN newFn);
	LPUNKNOWN Remove();
	BOOL HasItems();
};
