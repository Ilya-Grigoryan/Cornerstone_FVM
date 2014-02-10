// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// ICIQFunction wrapper class

class ICIQFunction : public COleDispatchDriver
{
public:
	ICIQFunction() {}		// Calls COleDispatchDriver default constructor
	ICIQFunction(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ICIQFunction(const ICIQFunction& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void AddParameter(LPCTSTR Value);
	void SkipParameter();
	CString GetResult();
	void SetResult(LPCTSTR lpszNewValue);
	CString GetExchange();
	void SetExchange(LPCTSTR lpszNewValue);
	CString GetTicker();
	void SetTicker(LPCTSTR lpszNewValue);
	long GetCompanyId();
	void SetCompanyId(long nNewValue);
	long GetRealTimeCompanyId();
	void SetRealTimeCompanyId(long nNewValue);
	long GetDataTypeIdAsInt();
	void SetDataTypeIdAsInt(long nNewValue);
	long GetClickThroughParameters(short Index);
	void SetClickThroughParameters(short Index, long nNewValue);
	CString GetParameters(short Index);
	void SetParameters(short Index, LPCTSTR lpszNewValue);
	short GetParameterCount();
	short GetClickThroughParameterCount();
	BOOL GetAuditable();
	void SetAuditable(BOOL bNewValue);
	BOOL GetFoundInCache();
	void SetFoundInCache(BOOL bNewValue);
	BOOL GetArrayFlag();
	void SetArrayFlag(BOOL bNewValue);
	CString GetName();
	void SetName(LPCTSTR lpszNewValue);
	BOOL GetResultExpired();
	void SetResultExpired(BOOL bNewValue);
	CString ToElement(long Id);
	CString GetAddress();
	void SetAddress(LPCTSTR lpszNewValue);
	BOOL GetIdentifierInvalid();
	void SetIdentifierInvalid(BOOL bNewValue);
	CString DebugString();
	short GetLastFilledParam();
};
