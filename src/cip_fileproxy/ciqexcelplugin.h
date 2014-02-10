// Machine generated IDispatch wrapper class(es) created with ClassWizard
/////////////////////////////////////////////////////////////////////////////
// _VBASettings wrapper class

class _VBASettings : public COleDispatchDriver
{
public:
	_VBASettings() {}		// Calls COleDispatchDriver default constructor
	_VBASettings(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_VBASettings(const _VBASettings& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	VARIANT GetCIQDefault();
	VARIANT GetCIQPrecision();
	VARIANT GetCIQDateFormat();
	VARIANT GetCIQDateToggle();
	VARIANT GetCIQRestatedFlag();
	VARIANT GetCIQCurrencyType();
	VARIANT GetCIQConversionMode();
	VARIANT GetCIQEnabled();
};
/////////////////////////////////////////////////////////////////////////////
// _API wrapper class

class _API : public COleDispatchDriver
{
public:
	_API() {}		// Calls COleDispatchDriver default constructor
	_API(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_API(const _API& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void Refresh(LPDISPATCH* R);
};
/////////////////////////////////////////////////////////////////////////////
// _API_SpecialFolders wrapper class

class _API_SpecialFolders : public COleDispatchDriver
{
public:
	_API_SpecialFolders() {}		// Calls COleDispatchDriver default constructor
	_API_SpecialFolders(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_API_SpecialFolders(const _API_SpecialFolders& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString GetSpecialFolderPath(long* inFolderId);
};
/////////////////////////////////////////////////////////////////////////////
// _API_Locale wrapper class

class _API_Locale : public COleDispatchDriver
{
public:
	_API_Locale() {}		// Calls COleDispatchDriver default constructor
	_API_Locale(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_API_Locale(const _API_Locale& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	CString GetCurrentLocaleSetting(long* LCType);
	CString GetLocaleSetting(long dwLocaleID, long dwLCType);
	long GetLocaleID();
};
/////////////////////////////////////////////////////////////////////////////
// _PluginSession wrapper class

class _PluginSession : public COleDispatchDriver
{
public:
	_PluginSession() {}		// Calls COleDispatchDriver default constructor
	_PluginSession(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_PluginSession(const _PluginSession& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	// method 'GetPluginSettings' not emitted because of invalid return type or parameter type
	// method 'WritePluginSettings' not emitted because of invalid return type or parameter type
	// method 'GetDefaultSettings' not emitted because of invalid return type or parameter type
	LPDISPATCH SessionXMLDocument();
	CString SessionXML();
	CString GetRegSetting(long settingId);
};
/////////////////////////////////////////////////////////////////////////////
// _FormulaCIQAGG wrapper class

class _FormulaCIQAGG : public COleDispatchDriver
{
public:
	_FormulaCIQAGG() {}		// Calls COleDispatchDriver default constructor
	_FormulaCIQAGG(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_FormulaCIQAGG(const _FormulaCIQAGG& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL GetIsValid();
	short GetMaxParam();
	CString GetStringValue();
	CString GetFormulaType();
	CString GetFunctionName();
	void SetFunctionName(BSTR* newValue);
	void Class_Initialize();
	CString GetParameter(short Number, BOOL IgnoreString);
	void SetParameter(short Number, LPCTSTR Value, BOOL StringParam);
	void ClearParam(short Number);
	BOOL ParameterIsString(short Number);
};
/////////////////////////////////////////////////////////////////////////////
// _OnlineTemplates wrapper class

class _OnlineTemplates : public COleDispatchDriver
{
public:
	_OnlineTemplates() {}		// Calls COleDispatchDriver default constructor
	_OnlineTemplates(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_OnlineTemplates(const _OnlineTemplates& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// _Connect wrapper class

class _Connect : public COleDispatchDriver
{
public:
	_Connect() {}		// Calls COleDispatchDriver default constructor
	_Connect(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Connect(const _Connect& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
};
/////////////////////////////////////////////////////////////////////////////
// _ToolBarManager wrapper class

class _ToolBarManager : public COleDispatchDriver
{
public:
	_ToolBarManager() {}		// Calls COleDispatchDriver default constructor
	_ToolBarManager(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_ToolBarManager(const _ToolBarManager& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL EnabledState();
	void SetEnabledState(BOOL* newState);
	BOOL SyncExpenseCode();
	void ToggleToolbarVisible();
	void ToggleToolbarEnabled();
	void SetStateFromCapability();
	void EnableExpenseCode();
	void DisableExpenseCode();
};
/////////////////////////////////////////////////////////////////////////////
// _CapabilityList wrapper class

class _CapabilityList : public COleDispatchDriver
{
public:
	_CapabilityList() {}		// Calls COleDispatchDriver default constructor
	_CapabilityList(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_CapabilityList(const _CapabilityList& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	// method 'GetCapabilityArray' not emitted because of invalid return type or parameter type
};
/////////////////////////////////////////////////////////////////////////////
// _AddinEvents wrapper class

class _AddinEvents : public COleDispatchDriver
{
public:
	_AddinEvents() {}		// Calls COleDispatchDriver default constructor
	_AddinEvents(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_AddinEvents(const _AddinEvents& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void PluginEnabled();
	void PluginDisabled();
	void LoginSuccessful();
};
/////////////////////////////////////////////////////////////////////////////
// _Addin wrapper class

class _Addin : public COleDispatchDriver
{
public:
	_Addin() {}		// Calls COleDispatchDriver default constructor
	_Addin(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Addin(const _Addin& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	BOOL EnsureLogin(BOOL forceLogin, BOOL checkExpenseCode);
	BOOL IsPluginEnabled();
	CString GetExpenseCode();
	short GetReleaseId();
	void Subscribe(LPDISPATCH callback);
	void EnablePlugin(BOOL Enable);
	CString GetExcelVersion();
	void RefreshRightClickCaptions();
	BOOL InitPlugin(LPCTSTR PasswordUE, BOOL Force);
	void UnInitPlugin();
	BOOL IsPluginInitialized();
	void FinishFirstLogin();
};
/////////////////////////////////////////////////////////////////////////////
// _Stack wrapper class

class _Stack : public COleDispatchDriver
{
public:
	_Stack() {}		// Calls COleDispatchDriver default constructor
	_Stack(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_Stack(const _Stack& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	long GetCount();
	BOOL GetIsEmpty();
	VARIANT Push(const VARIANT& Value);
	VARIANT Pop();
};
/////////////////////////////////////////////////////////////////////////////
// _ExpenseCodeList wrapper class

class _ExpenseCodeList : public COleDispatchDriver
{
public:
	_ExpenseCodeList() {}		// Calls COleDispatchDriver default constructor
	_ExpenseCodeList(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_ExpenseCodeList(const _ExpenseCodeList& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	// method 'GetExpenseCodeList' not emitted because of invalid return type or parameter type
	// method 'GetExpenseCodeProperList' not emitted because of invalid return type or parameter type
	void SaveExpenseCodeList(LPCTSTR csvList);
	CString GetCurrentExpenseCode();
	void SetCurrentExpenseCode(LPCTSTR expCode);
	void AddExpenseCode(LPCTSTR expCode);
	void ClearCurrentExpenseCode();
	CString GetExpenseCodeSeparator();
};
/////////////////////////////////////////////////////////////////////////////
// _MenuFunctions wrapper class

class _MenuFunctions : public COleDispatchDriver
{
public:
	_MenuFunctions() {}		// Calls COleDispatchDriver default constructor
	_MenuFunctions(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	_MenuFunctions(const _MenuFunctions& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void ShowFormulaBuilder();
	void ShowCompSet();
	void Refresh(short refreshType);
	void Unlink(short unlinkType);
	void RefreshTemplateMenu();
	void SearchIdent(LPCTSTR searchString);
	void SearchFinancials(LPCTSTR searchString);
	void Upload(short uploadType, BOOL fromRetry);
	CString GetIdent(LPCTSTR searchString, BOOL flashForm, BOOL multiSelect);
};
/////////////////////////////////////////////////////////////////////////////
// __FormulaCIQAGG wrapper class

class __FormulaCIQAGG : public COleDispatchDriver
{
public:
	__FormulaCIQAGG() {}		// Calls COleDispatchDriver default constructor
	__FormulaCIQAGG(LPDISPATCH pDispatch) : COleDispatchDriver(pDispatch) {}
	__FormulaCIQAGG(const __FormulaCIQAGG& dispatchSrc) : COleDispatchDriver(dispatchSrc) {}

// Attributes
public:

// Operations
public:
	void Change();
};
