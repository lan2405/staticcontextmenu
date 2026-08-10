// DllRegEx.h: CDllRegEx 的声明

#pragma once
#include "resource.h"       // 主符号
#include <ShlObj_core.h>
#include <vector>
#include <string>
#include "DllRegExt_i.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


enum MyCmd {
	CMD_ROOT = 0,          ///< 根命令
	CMD_REG,            ///< 注册
	CMD_UREG,         ///< 反注册
	CMD_END             ///< 结束
};


// CDllRegEx

class ATL_NO_VTABLE CDllRegEx :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDllRegEx, &CLSID_DllRegEx>,
	public IDispatchImpl<IDllRegEx, &IID_IDllRegEx, &LIBID_DllRegExtLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IShellExtInit,
	public IContextMenu
{
public:
	CDllRegEx()
	{
	}

DECLARE_REGISTRY_RESOURCEID(106)
BEGIN_COM_MAP(CDllRegEx)
	COM_INTERFACE_ENTRY(IDllRegEx)
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
	IFACEMETHOD(Initialize)(
		_In_ PCIDLIST_ABSOLUTE pidlFolder,	
		_In_ IDataObject* pdtobj, 
		_In_ HKEY   hkeyProgID
		)override;

	//// IContextMenu
	IFACEMETHOD(QueryContextMenu)(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)override;
	IFACEMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO lpici)override;
	IFACEMETHOD(GetCommandString)(UINT_PTR idCmd, UINT uType, UINT* pwReserved, LPSTR pszName, UINT cchMax) override { return E_NOTIMPL; }
	
private:
	std::vector<std::wstring> m_selectedFiles;
	std::wstring m_folderPath;
	bool bAllDll = true;

	void initRegMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst);
	void RegCommand(int com);
};

OBJECT_ENTRY_AUTO(__uuidof(DllRegEx), CDllRegEx)
