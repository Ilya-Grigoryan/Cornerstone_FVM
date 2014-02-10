// Machine generated IDispatch wrapper class(es) created with ClassWizard

#include "stdafx.h"
#include "ciqexcelplugin.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif



/////////////////////////////////////////////////////////////////////////////
// _VBASettings properties

/////////////////////////////////////////////////////////////////////////////
// _VBASettings operations

VARIANT _VBASettings::GetCIQDefault()
{
	VARIANT result;
	InvokeHelper(0x68030007, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT _VBASettings::GetCIQPrecision()
{
	VARIANT result;
	InvokeHelper(0x68030006, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT _VBASettings::GetCIQDateFormat()
{
	VARIANT result;
	InvokeHelper(0x68030005, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT _VBASettings::GetCIQDateToggle()
{
	VARIANT result;
	InvokeHelper(0x68030004, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT _VBASettings::GetCIQRestatedFlag()
{
	VARIANT result;
	InvokeHelper(0x68030003, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT _VBASettings::GetCIQCurrencyType()
{
	VARIANT result;
	InvokeHelper(0x68030002, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT _VBASettings::GetCIQConversionMode()
{
	VARIANT result;
	InvokeHelper(0x68030001, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}

VARIANT _VBASettings::GetCIQEnabled()
{
	VARIANT result;
	InvokeHelper(0x68030000, DISPATCH_PROPERTYGET, VT_VARIANT, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _API properties

/////////////////////////////////////////////////////////////////////////////
// _API operations

void _API::Refresh(LPDISPATCH* R)
{
	static BYTE parms[] =
		VTS_PDISPATCH;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 R);
}


/////////////////////////////////////////////////////////////////////////////
// _API_SpecialFolders properties

/////////////////////////////////////////////////////////////////////////////
// _API_SpecialFolders operations

CString _API_SpecialFolders::GetSpecialFolderPath(long* inFolderId)
{
	CString result;
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		inFolderId);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _API_Locale properties

/////////////////////////////////////////////////////////////////////////////
// _API_Locale operations

CString _API_Locale::GetCurrentLocaleSetting(long* LCType)
{
	CString result;
	static BYTE parms[] =
		VTS_PI4;
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		LCType);
	return result;
}

CString _API_Locale::GetLocaleSetting(long dwLocaleID, long dwLCType)
{
	CString result;
	static BYTE parms[] =
		VTS_I4 VTS_I4;
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		dwLocaleID, dwLCType);
	return result;
}

long _API_Locale::GetLocaleID()
{
	long result;
	InvokeHelper(0x60030002, DISPATCH_METHOD, VT_I4, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _PluginSession properties

/////////////////////////////////////////////////////////////////////////////
// _PluginSession operations

LPDISPATCH _PluginSession::SessionXMLDocument()
{
	LPDISPATCH result;
	InvokeHelper(0x60030003, DISPATCH_METHOD, VT_DISPATCH, (void*)&result, NULL);
	return result;
}

CString _PluginSession::SessionXML()
{
	CString result;
	InvokeHelper(0x60030004, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _PluginSession::GetRegSetting(long settingId)
{
	CString result;
	static BYTE parms[] =
		VTS_I4;
	InvokeHelper(0x60030005, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		settingId);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _FormulaCIQAGG properties

/////////////////////////////////////////////////////////////////////////////
// _FormulaCIQAGG operations

BOOL _FormulaCIQAGG::GetIsValid()
{
	BOOL result;
	InvokeHelper(0x68030004, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

short _FormulaCIQAGG::GetMaxParam()
{
	short result;
	InvokeHelper(0x68030003, DISPATCH_PROPERTYGET, VT_I2, (void*)&result, NULL);
	return result;
}

CString _FormulaCIQAGG::GetStringValue()
{
	CString result;
	InvokeHelper(0x68030002, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _FormulaCIQAGG::GetFormulaType()
{
	CString result;
	InvokeHelper(0x68030001, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

CString _FormulaCIQAGG::GetFunctionName()
{
	CString result;
	InvokeHelper(0x68030000, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _FormulaCIQAGG::SetFunctionName(BSTR* newValue)
{
	static BYTE parms[] =
		VTS_PBSTR;
	InvokeHelper(0x68030000, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms,
		 newValue);
}

void _FormulaCIQAGG::Class_Initialize()
{
	InvokeHelper(0x60030005, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString _FormulaCIQAGG::GetParameter(short Number, BOOL IgnoreString)
{
	CString result;
	static BYTE parms[] =
		VTS_I2 VTS_BOOL;
	InvokeHelper(0x60030007, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		Number, IgnoreString);
	return result;
}

void _FormulaCIQAGG::SetParameter(short Number, LPCTSTR Value, BOOL StringParam)
{
	static BYTE parms[] =
		VTS_I2 VTS_BSTR VTS_BOOL;
	InvokeHelper(0x60030008, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Number, Value, StringParam);
}

void _FormulaCIQAGG::ClearParam(short Number)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x60030009, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Number);
}

BOOL _FormulaCIQAGG::ParameterIsString(short Number)
{
	BOOL result;
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x6003000a, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		Number);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _OnlineTemplates properties

/////////////////////////////////////////////////////////////////////////////
// _OnlineTemplates operations


/////////////////////////////////////////////////////////////////////////////
// _Connect properties

/////////////////////////////////////////////////////////////////////////////
// _Connect operations


/////////////////////////////////////////////////////////////////////////////
// _ToolBarManager properties

/////////////////////////////////////////////////////////////////////////////
// _ToolBarManager operations

BOOL _ToolBarManager::EnabledState()
{
	BOOL result;
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _ToolBarManager::SetEnabledState(BOOL* newState)
{
	static BYTE parms[] =
		VTS_PBOOL;
	InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 newState);
}

BOOL _ToolBarManager::SyncExpenseCode()
{
	BOOL result;
	InvokeHelper(0x60030003, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _ToolBarManager::ToggleToolbarVisible()
{
	InvokeHelper(0x60030004, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _ToolBarManager::ToggleToolbarEnabled()
{
	InvokeHelper(0x60030005, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _ToolBarManager::SetStateFromCapability()
{
	InvokeHelper(0x60030006, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _ToolBarManager::EnableExpenseCode()
{
	InvokeHelper(0x60030007, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _ToolBarManager::DisableExpenseCode()
{
	InvokeHelper(0x60030008, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// _CapabilityList properties

/////////////////////////////////////////////////////////////////////////////
// _CapabilityList operations


/////////////////////////////////////////////////////////////////////////////
// _AddinEvents properties

/////////////////////////////////////////////////////////////////////////////
// _AddinEvents operations

void _AddinEvents::PluginEnabled()
{
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _AddinEvents::PluginDisabled()
{
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _AddinEvents::LoginSuccessful()
{
	InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// _Addin properties

/////////////////////////////////////////////////////////////////////////////
// _Addin operations

BOOL _Addin::EnsureLogin(BOOL forceLogin, BOOL checkExpenseCode)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BOOL VTS_BOOL;
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		forceLogin, checkExpenseCode);
	return result;
}

BOOL _Addin::IsPluginEnabled()
{
	BOOL result;
	InvokeHelper(0x60030002, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

CString _Addin::GetExpenseCode()
{
	CString result;
	InvokeHelper(0x60030003, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

short _Addin::GetReleaseId()
{
	short result;
	InvokeHelper(0x60030004, DISPATCH_METHOD, VT_I2, (void*)&result, NULL);
	return result;
}

void _Addin::Subscribe(LPDISPATCH callback)
{
	static BYTE parms[] =
		VTS_DISPATCH;
	InvokeHelper(0x60030005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 callback);
}

void _Addin::EnablePlugin(BOOL Enable)
{
	static BYTE parms[] =
		VTS_BOOL;
	InvokeHelper(0x60030006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 Enable);
}

CString _Addin::GetExcelVersion()
{
	CString result;
	InvokeHelper(0x60030007, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _Addin::RefreshRightClickCaptions()
{
	InvokeHelper(0x60030008, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL _Addin::InitPlugin(LPCTSTR PasswordUE, BOOL Force)
{
	BOOL result;
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL;
	InvokeHelper(0x60030009, DISPATCH_METHOD, VT_BOOL, (void*)&result, parms,
		PasswordUE, Force);
	return result;
}

void _Addin::UnInitPlugin()
{
	InvokeHelper(0x6003000a, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

BOOL _Addin::IsPluginInitialized()
{
	BOOL result;
	InvokeHelper(0x6003000b, DISPATCH_METHOD, VT_BOOL, (void*)&result, NULL);
	return result;
}

void _Addin::FinishFirstLogin()
{
	InvokeHelper(0x6003000c, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}


/////////////////////////////////////////////////////////////////////////////
// _Stack properties

/////////////////////////////////////////////////////////////////////////////
// _Stack operations

long _Stack::GetCount()
{
	long result;
	InvokeHelper(0x68030001, DISPATCH_PROPERTYGET, VT_I4, (void*)&result, NULL);
	return result;
}

BOOL _Stack::GetIsEmpty()
{
	BOOL result;
	InvokeHelper(0x68030000, DISPATCH_PROPERTYGET, VT_BOOL, (void*)&result, NULL);
	return result;
}

VARIANT _Stack::Push(const VARIANT& Value)
{
	VARIANT result;
	static BYTE parms[] =
		VTS_VARIANT;
	InvokeHelper(0x60030002, DISPATCH_METHOD, VT_VARIANT, (void*)&result, parms,
		&Value);
	return result;
}

VARIANT _Stack::Pop()
{
	VARIANT result;
	InvokeHelper(0x60030003, DISPATCH_METHOD, VT_VARIANT, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _ExpenseCodeList properties

/////////////////////////////////////////////////////////////////////////////
// _ExpenseCodeList operations

void _ExpenseCodeList::SaveExpenseCodeList(LPCTSTR csvList)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 csvList);
}

CString _ExpenseCodeList::GetCurrentExpenseCode()
{
	CString result;
	InvokeHelper(0x60030003, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}

void _ExpenseCodeList::SetCurrentExpenseCode(LPCTSTR expCode)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030004, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 expCode);
}

void _ExpenseCodeList::AddExpenseCode(LPCTSTR expCode)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 expCode);
}

void _ExpenseCodeList::ClearCurrentExpenseCode()
{
	InvokeHelper(0x60030006, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

CString _ExpenseCodeList::GetExpenseCodeSeparator()
{
	CString result;
	InvokeHelper(0x60030007, DISPATCH_METHOD, VT_BSTR, (void*)&result, NULL);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// _MenuFunctions properties

/////////////////////////////////////////////////////////////////////////////
// _MenuFunctions operations

void _MenuFunctions::ShowFormulaBuilder()
{
	InvokeHelper(0x60030000, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _MenuFunctions::ShowCompSet()
{
	InvokeHelper(0x60030001, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _MenuFunctions::Refresh(short refreshType)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x60030002, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 refreshType);
}

void _MenuFunctions::Unlink(short unlinkType)
{
	static BYTE parms[] =
		VTS_I2;
	InvokeHelper(0x60030003, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 unlinkType);
}

void _MenuFunctions::RefreshTemplateMenu()
{
	InvokeHelper(0x60030004, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}

void _MenuFunctions::SearchIdent(LPCTSTR searchString)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030005, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 searchString);
}

void _MenuFunctions::SearchFinancials(LPCTSTR searchString)
{
	static BYTE parms[] =
		VTS_BSTR;
	InvokeHelper(0x60030006, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 searchString);
}

void _MenuFunctions::Upload(short uploadType, BOOL fromRetry)
{
	static BYTE parms[] =
		VTS_I2 VTS_BOOL;
	InvokeHelper(0x60030007, DISPATCH_METHOD, VT_EMPTY, NULL, parms,
		 uploadType, fromRetry);
}

CString _MenuFunctions::GetIdent(LPCTSTR searchString, BOOL flashForm, BOOL multiSelect)
{
	CString result;
	static BYTE parms[] =
		VTS_BSTR VTS_BOOL VTS_BOOL;
	InvokeHelper(0x60030008, DISPATCH_METHOD, VT_BSTR, (void*)&result, parms,
		searchString, flashForm, multiSelect);
	return result;
}


/////////////////////////////////////////////////////////////////////////////
// __FormulaCIQAGG properties

/////////////////////////////////////////////////////////////////////////////
// __FormulaCIQAGG operations

void __FormulaCIQAGG::Change()
{
	InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, NULL);
}
