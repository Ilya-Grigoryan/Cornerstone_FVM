// BBGProxy.cpp : implementation file
//

#include "stdafx.h"
#include "cip_bloombergproxy.h"
#include "BBGProxy.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CBBGProxy

IMPLEMENT_DYNCREATE(CBBGProxy, CCmdTarget)

CBBGProxy::CBBGProxy()
{
	EnableAutomation();
	
	// To keep the application running as long as an OLE automation 
	//	object is active, the constructor calls AfxOleLockApp.
	
	AfxOleLockApp();
}

CBBGProxy::~CBBGProxy()
{
	// To terminate the application when all objects created with
	// 	with OLE automation, the destructor calls AfxOleUnlockApp.
	
	AfxOleUnlockApp();
}


void CBBGProxy::OnFinalRelease()
{
	// When the last reference for an automation object is released
	// OnFinalRelease is called.  The base class will automatically
	// deletes the object.  Add additional cleanup required for your
	// object before calling the base class.

	CCmdTarget::OnFinalRelease();
}


BEGIN_MESSAGE_MAP(CBBGProxy, CCmdTarget)
	//{{AFX_MSG_MAP(CBBGProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BEGIN_DISPATCH_MAP(CBBGProxy, CCmdTarget)
	//{{AFX_DISPATCH_MAP(CBBGProxy)
		// NOTE - the ClassWizard will add and remove mapping macros here.
	//}}AFX_DISPATCH_MAP
END_DISPATCH_MAP()

// Note: we add support for IID_IBBGProxy to support typesafe binding
//  from VBA.  This IID must match the GUID that is attached to the 
//  dispinterface in the .ODL file.

// {0677A2C9-58F8-4169-871A-D76A213E2563}
static const IID IID_IBBGProxy =
{ 0x677a2c9, 0x58f8, 0x4169, { 0x87, 0x1a, 0xd7, 0x6a, 0x21, 0x3e, 0x25, 0x63 } };

BEGIN_INTERFACE_MAP(CBBGProxy, CCmdTarget)
	INTERFACE_PART(CBBGProxy, IID_IBBGProxy, Dispatch)
END_INTERFACE_MAP()

// {C3890E54-4890-4832-8C55-6F81E421AD7D}
IMPLEMENT_OLECREATE(CBBGProxy, "CIPBloomberg.Proxy", 0xc3890e54, 0x4890, 0x4832, 0x8c, 0x55, 0x6f, 0x81, 0xe4, 0x21, 0xad, 0x7d)

/////////////////////////////////////////////////////////////////////////////
// CBBGProxy message handlers
