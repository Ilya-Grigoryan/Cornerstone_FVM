

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Mon Feb 10 16:14:28 2014
 */
/* Compiler settings for cip_fileproxy.odl:
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


#ifndef __cip_fileproxy_h_h__
#define __cip_fileproxy_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __IBloomberg_FWD_DEFINED__
#define __IBloomberg_FWD_DEFINED__
typedef interface IBloomberg IBloomberg;

#endif 	/* __IBloomberg_FWD_DEFINED__ */


#ifndef __Bloomberg_FWD_DEFINED__
#define __Bloomberg_FWD_DEFINED__

#ifdef __cplusplus
typedef class Bloomberg Bloomberg;
#else
typedef struct Bloomberg Bloomberg;
#endif /* __cplusplus */

#endif 	/* __Bloomberg_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __Cip_fileproxy_LIBRARY_DEFINED__
#define __Cip_fileproxy_LIBRARY_DEFINED__

/* library Cip_fileproxy */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_Cip_fileproxy,0x119DAA14,0xD576,0x4794,0x9D,0x2C,0x09,0xD9,0x64,0x6C,0x7A,0x2C);

#ifndef __IBloomberg_DISPINTERFACE_DEFINED__
#define __IBloomberg_DISPINTERFACE_DEFINED__

/* dispinterface IBloomberg */
/* [uuid] */ 


DEFINE_GUID(DIID_IBloomberg,0xD2D82D43,0x851F,0x4FCA,0x83,0x3B,0x86,0x2D,0xD4,0x64,0x1F,0xB7);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("D2D82D43-851F-4FCA-833B-862DD4641FB7")
    IBloomberg : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct IBloombergVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IBloomberg * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IBloomberg * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IBloomberg * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IBloomberg * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IBloomberg * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IBloomberg * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IBloomberg * This,
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
    } IBloombergVtbl;

    interface IBloomberg
    {
        CONST_VTBL struct IBloombergVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IBloomberg_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IBloomberg_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IBloomberg_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IBloomberg_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IBloomberg_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IBloomberg_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IBloomberg_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __IBloomberg_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Bloomberg,0x4FE15ED2,0x711D,0x4B42,0xAD,0x79,0x6F,0xF5,0x6C,0x2B,0x88,0x78);

#ifdef __cplusplus

class DECLSPEC_UUID("4FE15ED2-711D-4B42-AD79-6FF56C2B8878")
Bloomberg;
#endif
#endif /* __Cip_fileproxy_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


