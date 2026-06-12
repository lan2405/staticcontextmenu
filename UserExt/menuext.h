// menuext.h: Cmenuext 的声明

#pragma once
#include "resource.h"       // 主符号
#include <ShObjIdl.h>


#include "UserExt_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// Cmenuext

class ATL_NO_VTABLE Cmenuext :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<Cmenuext, &CLSID_menuext>,
	public IDispatchImpl<Imenuext, &IID_Imenuext, &LIBID_UserExtLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IShellExtInit,
    public IContextMenu
{
public:
	Cmenuext()
	{
	}

DECLARE_REGISTRY_RESOURCEID(106)


BEGIN_COM_MAP(Cmenuext)
	COM_INTERFACE_ENTRY(Imenuext)
	COM_INTERFACE_ENTRY(IDispatch)
    COM_INTERFACE_ENTRY(IShellExtInit)
    COM_INTERFACE_ENTRY(IContextMenu)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}

public:
// IShellExtInit
	HRESULT Initialize(
		PCIDLIST_ABSOLUTE pidlFolder,
		IDataObject* pdtobj,
		HKEY   hkeyProgID
	)override;
	// IContextMenu
    HRESULT QueryContextMenu(
        HMENU hmenu,
        UINT indexMenu,
        UINT idCmdFirst,
        UINT idCmdLast,
        UINT uFlags
    )override;
    HRESULT InvokeCommand(
        LPCMINVOKECOMMANDINFO pici
    )override;
    HRESULT GetCommandString(
        UINT_PTR idcmd,
        UINT uflags,
        UINT* pwReserved,
        LPSTR pszName,
        UINT cchMax
    )override;


};

OBJECT_ENTRY_AUTO(__uuidof(menuext), Cmenuext)
