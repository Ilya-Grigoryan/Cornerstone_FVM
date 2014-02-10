

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Mon Feb 10 16:14:23 2014
 */
/* Compiler settings for cip_bloombergproxy.odl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 8.00.0595 
    protocol : dce , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */

#pragma warning( disable: 4049 )  /* more than 64k source lines */


/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 475
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif // __RPCNDR_H_VERSION__


#ifndef __cip_bloombergproxy_h_h__
#define __cip_bloombergproxy_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IBBGProxy_FWD_DEFINED__
#define __IBBGProxy_FWD_DEFINED__
typedef interface IBBGProxy IBBGProxy;

#endif 	/* __IBBGProxy_FWD_DEFINED__ */


#ifndef __Proxy_FWD_DEFINED__
#define __Proxy_FWD_DEFINED__

#ifdef __cplusplus
typedef class Proxy Proxy;
#else
typedef struct Proxy Proxy;
#endif /* __cplusplus */

#endif 	/* __Proxy_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __Cip_bloombergproxy_LIBRARY_DEFINED__
#define __Cip_bloombergproxy_LIBRARY_DEFINED__

/* library Cip_bloombergproxy */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_Cip_bloombergproxy,0x8084AA00,0x5CC8,0x400E,0xBE,0x10,0x5D,0xF4,0x01,0x76,0x89,0x7A);

#ifndef __IBBGProxy_DISPINTERFACE_DEFINED__
#define __IBBGProxy_DISPINTERFACE_DEFINED__

/* dispinterface IBBGProxy */
/* [uuid] */ 


DEFINE_GUID(DIID_IBBGProxy,0x0677A2C9,0x58F8,0x4169,0x87,0x1A,0xD7,0x6A,0x21,0x3E,0x25,0x63);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("0677A2C9-58F8-4169-871A-D76A213E2563")
    IBBGProxy : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IBBGProxyVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBBGProxy * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBBGProxy * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBBGProxy * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IBBGProxy * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IBBGProxy * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IBBGProxy * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IBBGProxy * This,
            /* [annotation][in] */ 
            _In_  DISPID dispIdMember,
            /* [annotation][in] */ 
            _In_  REFIID riid,
            /* [annotation][in] */ 
            _In_  LCID lcid,
            /* [annotation][in] */ 
            _In_  WORD wFlags,
            /* [annotation][out][in] */ 
            _In_  DISPPARAMS *pDispParams,
            /* [annotation][out] */ 
            _Out_opt_  VARIANT *pVarResult,
            /* [annotation][out] */ 
            _Out_opt_  EXCEPINFO *pExcepInfo,
            /* [annotation][out] */ 
            _Out_opt_  UINT *puArgErr);
        
        END_INTERFACE
    } IBBGProxyVtbl;

    interface IBBGProxy
    {
        CONST_VTBL struct IBBGProxyVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBBGProxy_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBBGProxy_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBBGProxy_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBBGProxy_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IBBGProxy_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IBBGProxy_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IBBGProxy_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IBBGProxy_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Proxy,0xC3890E54,0x4890,0x4832,0x8C,0x55,0x6F,0x81,0xE4,0x21,0xAD,0x7D);

#ifdef __cplusplus

class DECLSPEC_UUID("C3890E54-4890-4832-8C55-6F81E421AD7D")
Proxy;
#endif
#endif /* __Cip_bloombergproxy_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


