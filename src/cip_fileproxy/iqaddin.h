// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// ICiqAddin wrapper class

class ICiqAddin : public COleDispatchDriver
{
public:
	ICiqAddin() {}		// Calls COleDispatchDriver default constructor
	ICiqAddin(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ICiqAddin(const ICiqAddin& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void InitializeSelf(LPDISPATCH pHost);
	void Enable();
};
/////////////////////////////////////////////////////////////////////////////
// ICiqAddinHost wrapper class

class ICiqAddinHost : public COleDispatchDriver
{
public:
	ICiqAddinHost() {}		// Calls COleDispatchDriver default constructor
	ICiqAddinHost(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	ICiqAddinHost(const ICiqAddinHost& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	LPDISPATCH GetApplication();
	void ReportError(LPCTSTR errorText);
	void RegisterXll(LPCTSTR fileName);
	void RegisterAutoAddin(LPCTSTR progId);
	void RegisterExtensibilityAddin(LPUNKNOWN pConnect);
	void LoadAll(long* ridError);
};
