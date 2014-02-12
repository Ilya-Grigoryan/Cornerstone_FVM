

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.00.0595 */
/* at Wed Feb 12 22:13:45 2014
 */
/* Compiler settings for CornerstoneFVModel.odl:
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


#ifndef __CornerstoneFVModel_h_h__
#define __CornerstoneFVModel_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ICornerstoneFVModel_FWD_DEFINED__
#define __ICornerstoneFVModel_FWD_DEFINED__
typedef interface ICornerstoneFVModel ICornerstoneFVModel;

#endif 	/* __ICornerstoneFVModel_FWD_DEFINED__ */


#ifndef __Document_FWD_DEFINED__
#define __Document_FWD_DEFINED__

#ifdef __cplusplus
typedef class Document Document;
#else
typedef struct Document Document;
#endif /* __cplusplus */

#endif 	/* __Document_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __CornerstoneFVModel_LIBRARY_DEFINED__
#define __CornerstoneFVModel_LIBRARY_DEFINED__

/* library CornerstoneFVModel */
/* [version][uuid] */ 


DEFINE_GUID(LIBID_CornerstoneFVModel,0x865087CF,0x2992,0x400A,0xBD,0x35,0x9D,0x53,0xEE,0x76,0xAD,0x4C);

#ifndef __ICornerstoneFVModel_DISPINTERFACE_DEFINED__
#define __ICornerstoneFVModel_DISPINTERFACE_DEFINED__

/* dispinterface ICornerstoneFVModel */
/* [uuid] */ 


DEFINE_GUID(DIID_ICornerstoneFVModel,0xDBE8718D,0x7465,0x4457,0x95,0xC8,0x62,0x0C,0x7B,0xF4,0xFF,0x45);

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("DBE8718D-7465-4457-95C8-620C7BF4FF45")
    ICornerstoneFVModel : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct ICornerstoneFVModelVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ICornerstoneFVModel * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ICornerstoneFVModel * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ICornerstoneFVModel * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ICornerstoneFVModel * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ICornerstoneFVModel * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ICornerstoneFVModel * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ICornerstoneFVModel * This,
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
    } ICornerstoneFVModelVtbl;

    interface ICornerstoneFVModel
    {
        CONST_VTBL struct ICornerstoneFVModelVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ICornerstoneFVModel_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ICornerstoneFVModel_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ICornerstoneFVModel_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ICornerstoneFVModel_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ICornerstoneFVModel_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ICornerstoneFVModel_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ICornerstoneFVModel_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __ICornerstoneFVModel_DISPINTERFACE_DEFINED__ */


DEFINE_GUID(CLSID_Document,0xBDA9D066,0xEB57,0x48F8,0xB6,0x0D,0xA7,0xE0,0xE6,0x56,0xB9,0x31);

#ifdef __cplusplus

class DECLSPEC_UUID("BDA9D066-EB57-48F8-B60D-A7E0E656B931")
Document;
#endif
#endif /* __CornerstoneFVModel_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


