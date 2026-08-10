

/* this ALWAYS GENERATED file contains the definitions for the interfaces */


 /* File created by MIDL compiler version 8.01.0628 */
/* at Tue Jan 19 11:14:07 2038
 */
/* Compiler settings for DllRegIExCmdExt.idl:
    Oicf, W1, Zp8, env=Win64 (32b run), target_arch=AMD64 8.01.0628 
    protocol : all , ms_ext, c_ext, robust
    error checks: allocation ref bounds_check enum stub_data 
    VC __declspec() decoration level: 
         __declspec(uuid()), __declspec(selectany), __declspec(novtable)
         DECLSPEC_UUID(), MIDL_INTERFACE()
*/
/* @@MIDL_FILE_HEADING(  ) */



/* verify that the <rpcndr.h> version is high enough to compile this file*/
#ifndef __REQUIRED_RPCNDR_H_VERSION__
#define __REQUIRED_RPCNDR_H_VERSION__ 500
#endif

#include "rpc.h"
#include "rpcndr.h"

#ifndef __RPCNDR_H_VERSION__
#error this stub requires an updated version of <rpcndr.h>
#endif /* __RPCNDR_H_VERSION__ */

#ifndef COM_NO_WINDOWS_H
#include "windows.h"
#include "ole2.h"
#endif /*COM_NO_WINDOWS_H*/

#ifndef __DllRegIExCmdExt_i_h__
#define __DllRegIExCmdExt_i_h__

#if defined(_MSC_VER) && (_MSC_VER >= 1020)
#pragma once
#endif

#ifndef DECLSPEC_XFGVIRT
#if defined(_CONTROL_FLOW_GUARD_XFG)
#define DECLSPEC_XFGVIRT(base, func) __declspec(xfg_virtual(base, func))
#else
#define DECLSPEC_XFGVIRT(base, func)
#endif
#endif

/* Forward Declarations */ 

#ifndef __IIExCmdMainMenus_FWD_DEFINED__
#define __IIExCmdMainMenus_FWD_DEFINED__
typedef interface IIExCmdMainMenus IIExCmdMainMenus;

#endif 	/* __IIExCmdMainMenus_FWD_DEFINED__ */


#ifndef __IIExCmdSubMenus_FWD_DEFINED__
#define __IIExCmdSubMenus_FWD_DEFINED__
typedef interface IIExCmdSubMenus IIExCmdSubMenus;

#endif 	/* __IIExCmdSubMenus_FWD_DEFINED__ */


#ifndef __IExCmdMainMenus_FWD_DEFINED__
#define __IExCmdMainMenus_FWD_DEFINED__

#ifdef __cplusplus
typedef class IExCmdMainMenus IExCmdMainMenus;
#else
typedef struct IExCmdMainMenus IExCmdMainMenus;
#endif /* __cplusplus */

#endif 	/* __IExCmdMainMenus_FWD_DEFINED__ */


#ifndef __IExCmdSubMenus_FWD_DEFINED__
#define __IExCmdSubMenus_FWD_DEFINED__

#ifdef __cplusplus
typedef class IExCmdSubMenus IExCmdSubMenus;
#else
typedef struct IExCmdSubMenus IExCmdSubMenus;
#endif /* __cplusplus */

#endif 	/* __IExCmdSubMenus_FWD_DEFINED__ */


/* header files for imported files */
#include "oaidl.h"
#include "ocidl.h"
#include "shobjidl.h"

#ifdef __cplusplus
extern "C"{
#endif 


#ifndef __IIExCmdMainMenus_INTERFACE_DEFINED__
#define __IIExCmdMainMenus_INTERFACE_DEFINED__

/* interface IIExCmdMainMenus */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IIExCmdMainMenus;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("52507bcb-3938-436b-9427-64e39d4515d8")
    IIExCmdMainMenus : public IDispatch
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct IIExCmdMainMenusVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IIExCmdMainMenus * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IIExCmdMainMenus * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IIExCmdMainMenus * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IIExCmdMainMenus * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IIExCmdMainMenus * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IIExCmdMainMenus * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IIExCmdMainMenus * This,
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
    } IIExCmdMainMenusVtbl;

    interface IIExCmdMainMenus
    {
        CONST_VTBL struct IIExCmdMainMenusVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IIExCmdMainMenus_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IIExCmdMainMenus_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IIExCmdMainMenus_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IIExCmdMainMenus_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IIExCmdMainMenus_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IIExCmdMainMenus_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IIExCmdMainMenus_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IIExCmdMainMenus_INTERFACE_DEFINED__ */


#ifndef __IIExCmdSubMenus_INTERFACE_DEFINED__
#define __IIExCmdSubMenus_INTERFACE_DEFINED__

/* interface IIExCmdSubMenus */
/* [unique][nonextensible][dual][uuid][object] */ 


EXTERN_C const IID IID_IIExCmdSubMenus;

#if defined(__cplusplus) && !defined(CINTERFACE)
    
    MIDL_INTERFACE("d811f3f1-91ce-46ca-ba51-a6838a480087")
    IIExCmdSubMenus : public IDispatch
    {
    public:
    };
    
    
#else 	/* C style interface */

    typedef struct IIExCmdSubMenusVtbl
    {
        BEGIN_INTERFACE
        
        DECLSPEC_XFGVIRT(IUnknown, QueryInterface)
        HRESULT ( STDMETHODCALLTYPE *QueryInterface )( 
            IIExCmdSubMenus * This,
            /* [in] */ REFIID riid,
            /* [annotation][iid_is][out] */ 
            _COM_Outptr_  void **ppvObject);
        
        DECLSPEC_XFGVIRT(IUnknown, AddRef)
        ULONG ( STDMETHODCALLTYPE *AddRef )( 
            IIExCmdSubMenus * This);
        
        DECLSPEC_XFGVIRT(IUnknown, Release)
        ULONG ( STDMETHODCALLTYPE *Release )( 
            IIExCmdSubMenus * This);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfoCount)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfoCount )( 
            IIExCmdSubMenus * This,
            /* [out] */ UINT *pctinfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetTypeInfo)
        HRESULT ( STDMETHODCALLTYPE *GetTypeInfo )( 
            IIExCmdSubMenus * This,
            /* [in] */ UINT iTInfo,
            /* [in] */ LCID lcid,
            /* [out] */ ITypeInfo **ppTInfo);
        
        DECLSPEC_XFGVIRT(IDispatch, GetIDsOfNames)
        HRESULT ( STDMETHODCALLTYPE *GetIDsOfNames )( 
            IIExCmdSubMenus * This,
            /* [in] */ REFIID riid,
            /* [size_is][in] */ LPOLESTR *rgszNames,
            /* [range][in] */ UINT cNames,
            /* [in] */ LCID lcid,
            /* [size_is][out] */ DISPID *rgDispId);
        
        DECLSPEC_XFGVIRT(IDispatch, Invoke)
        /* [local] */ HRESULT ( STDMETHODCALLTYPE *Invoke )( 
            IIExCmdSubMenus * This,
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
    } IIExCmdSubMenusVtbl;

    interface IIExCmdSubMenus
    {
        CONST_VTBL struct IIExCmdSubMenusVtbl *lpVtbl;
    };

    

#ifdef COBJMACROS


#define IIExCmdSubMenus_QueryInterface(This,riid,ppvObject)	\
    ( (This)->lpVtbl -> QueryInterface(This,riid,ppvObject) ) 

#define IIExCmdSubMenus_AddRef(This)	\
    ( (This)->lpVtbl -> AddRef(This) ) 

#define IIExCmdSubMenus_Release(This)	\
    ( (This)->lpVtbl -> Release(This) ) 


#define IIExCmdSubMenus_GetTypeInfoCount(This,pctinfo)	\
    ( (This)->lpVtbl -> GetTypeInfoCount(This,pctinfo) ) 

#define IIExCmdSubMenus_GetTypeInfo(This,iTInfo,lcid,ppTInfo)	\
    ( (This)->lpVtbl -> GetTypeInfo(This,iTInfo,lcid,ppTInfo) ) 

#define IIExCmdSubMenus_GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId)	\
    ( (This)->lpVtbl -> GetIDsOfNames(This,riid,rgszNames,cNames,lcid,rgDispId) ) 

#define IIExCmdSubMenus_Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr)	\
    ( (This)->lpVtbl -> Invoke(This,dispIdMember,riid,lcid,wFlags,pDispParams,pVarResult,pExcepInfo,puArgErr) ) 


#endif /* COBJMACROS */


#endif 	/* C style interface */




#endif 	/* __IIExCmdSubMenus_INTERFACE_DEFINED__ */



#ifndef __DllRegIExCmdExtLib_LIBRARY_DEFINED__
#define __DllRegIExCmdExtLib_LIBRARY_DEFINED__

/* library DllRegIExCmdExtLib */
/* [version][uuid] */ 


EXTERN_C const IID LIBID_DllRegIExCmdExtLib;

EXTERN_C const CLSID CLSID_IExCmdMainMenus;

#ifdef __cplusplus

class DECLSPEC_UUID("fdd83540-b3a7-4c67-8805-f1442273303f")
IExCmdMainMenus;
#endif

EXTERN_C const CLSID CLSID_IExCmdSubMenus;

#ifdef __cplusplus

class DECLSPEC_UUID("3d8cc906-2942-44d0-8487-b86e473498f1")
IExCmdSubMenus;
#endif
#endif /* __DllRegIExCmdExtLib_LIBRARY_DEFINED__ */

/* Additional Prototypes for ALL interfaces */

/* end of Additional Prototypes */

#ifdef __cplusplus
}
#endif

#endif


