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
    CMD_ROOT,          ///< 根命令
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
	public IContextMenu,
	public IExplorerCommand
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
    COM_INTERFACE_ENTRY(IExplorerCommand)
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
    STDMETHODIMP Initialize(
		_In_ PCIDLIST_ABSOLUTE pidlFolder,
		_In_ IDataObject* pdtobj,
		_In_ HKEY   hkeyProgID
	)override;

	// IContextMenu
    STDMETHODIMP QueryContextMenu(
		_In_ HMENU hmenu,
		_In_ UINT indexMenu,
		_In_ UINT idCmdFirst,
		_In_ UINT idCmdLast,
		_In_ UINT uFlags
	)override;
    STDMETHODIMP InvokeCommand(
		_In_ LPCMINVOKECOMMANDINFO lpici
	)override;
    STDMETHODIMP GetCommandString(
		_In_ UINT_PTR idcmd,
		_In_ UINT uType,
		_In_ UINT* pReserved,
		_Out_writes_bytes_opt_(cchOut) LPSTR pszName,
		_In_ UINT cchOut
	)override;

	// IExplorerCommand
	STDMETHOD(EnumSubCommands)(IEnumExplorerCommand** ppEnum) override;
	STDMETHOD(GetCanonicalName)(GUID* pguidCommandName) override;
	STDMETHOD(GetFlags)(EXPCMDFLAGS* pFlags) override;
	STDMETHOD(GetIcon)(IShellItemArray* psiItemArray, LPWSTR* ppszIcon) override;
	STDMETHOD(GetState)(IShellItemArray* psiItemArray, BOOL fOkToBeSlow, EXPCMDSTATE* pCmdState) override;
	STDMETHOD(GetTitle)(IShellItemArray* psiItemArray, LPWSTR* ppszName) override;
	STDMETHOD(GetToolTip)(IShellItemArray* psiItemArray, LPWSTR* ppszInfotip) override;
	STDMETHOD(Invoke)(IShellItemArray* psiItemArray, IBindCtx* pbc) override;


private:
	std::vector<std::wstring> m_selectedFiles;
	std::wstring m_folderPath;

	void initRegMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst);
	void RegCommand(int com);
};

OBJECT_ENTRY_AUTO(__uuidof(DllRegEx), CDllRegEx)
