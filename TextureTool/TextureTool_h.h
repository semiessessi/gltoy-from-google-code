

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 7.00.0555 */
/* at Thu Oct 21 04:13:28 2010
 */
/* Compiler settings for Win32\Core\TextureTool.idl:
    Oicf, W1, Zp8, env=Win32 (32b run), target_arch=X86 7.00.0555 
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


#ifndef __TextureTool_h_h__
#define __TextureTool_h_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

/* Forward Declarations */ 

#ifndef __ITextureTool_FWD_DEFINED__
#define __ITextureTool_FWD_DEFINED__
typedef interface ITextureTool ITextureTool;
#endif 	/* __ITextureTool_FWD_DEFINED__ */


#ifndef __CTextureToolDoc_FWD_DEFINED__
#define __CTextureToolDoc_FWD_DEFINED__

#ifdef __cplusplus
typedef class CTextureToolDoc CTextureToolDoc;
#else
typedef struct CTextureToolDoc CTextureToolDoc;
#endif /* __cplusplus */

#endif 	/* __CTextureToolDoc_FWD_DEFINED__ */


#ifdef __cplusplus
extern "C"{
#endif 



#ifndef __TextureTool_LIBRARY_DEFINED__
#define __TextureTool_LIBRARY_DEFINED__

/* library TextureTool */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_TextureTool;

#ifndef __ITextureTool_DISPINTERFACE_DEFINED__
#define __ITextureTool_DISPINTERFACE_DEFINED__

/* dispinterface ITextureTool */
/* [uuid] */ 


EXTERN_C const IID DIID_ITextureTool;

#if defined(__cplusplus) && !defined(CINTERFACE)

    MIDL_INTERFACE("495A0F44-19FD-494D-A03F-0E28529B4ED1")
    ITextureTool : public IDispatch
    {
    };
    
#else 	/* C style interface */

    typedef struct ITextureToolVtbl
    {
        BEGIN_INTERFACE
        
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            ITextureTool * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            __RPC__deref_out  void **ppvObject);
        
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            ITextureTool * This);
        
        ULONG ( STDMETHODCALLTYPE *Release )( 
            ITextureTool * This);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            ITextureTool * This,
            /* [out] */ UINT *pctinfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            ITextureTool * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            ITextureTool * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            ITextureTool * This,
            /* [in] */ DISPID dispIdMember,
            /* [in] */ REFIID riid,
            /* [in] */ LCID lcid,
            /* [in] */ WORD wFlags,
            /* [out][in] */ DISPPARAMS *pDispParams,
            /* [out] */ VARIANT *pVarResult,
            /* [out] */ EXCEPINFO *pExcepInfo,
            /* [out] */ UINT *puArgErr);
        
        END_INTERFACE
    } ITextureToolVtbl;

    interface ITextureTool
    {
        CONST_VTBL struct ITextureToolVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define ITextureTool_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define ITextureTool_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define ITextureTool_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define ITextureTool_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define ITextureTool_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define ITextureTool_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define ITextureTool_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 

#endif /* COBJMACROS */


#endif 	/* C style interface */


#endif 	/* __ITextureTool_DISPINTERFACE_DEFINED__ */


EXTERN_C const CLSID CLSID_CTextureToolDoc;

#ifdef __cplusplus

class DECLSPEC_UUID("52F7D770-431D-4BB8-8491-A40B52FBC3BB")
CTextureToolDoc;
#endif
#endif /* __TextureTool_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


