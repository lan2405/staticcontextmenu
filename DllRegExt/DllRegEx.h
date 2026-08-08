// DllRegEx.h: CDllRegEx 的声明

#pragma once
#include "resource.h"       // 主符号
#include <ShlObj_core.h>
#include <vector>
#include <string>
#include "constinfo.h"
#include "DllRegExt_i.h"
#include "SubIExCmdMenu.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;





// CDllRegEx

class ATL_NO_VTABLE CDllRegEx :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDllRegEx, &CLSID_DllRegEx>,
	public IDispatchImpl<IDllRegEx, &IID_IDllRegEx, &LIBID_DllRegExtLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IShellExtInit,
	public IContextMenu,
	public IExplorerCommand,
	public IEnumExplorerCommand
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
	COM_INTERFACE_ENTRY(IEnumExplorerCommand)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()
	size_t m_currentIndex = 0;
	std::vector<CComPtr<CSubIExCmdMenu>> m_subCommands;
	HRESULT FinalConstruct()
	{
		m_subCommands.clear();
		for (int i = 1; i < CMD_END; i++)
		{
			CComObject<CSubIExCmdMenu>* pRaw = nullptr;
			HRESULT hr = CComObject<CSubIExCmdMenu>::CreateInstance(&pRaw);
			if (FAILED(hr))
				continue;

			pRaw->set_m_size(i);
			pRaw->set_m_folder_path(m_folderPath);
			pRaw->set_m_selected_files(m_selectedFiles);
			// CComPtr 构造函数会 AddRef，refcount: 0 → 1
			CComPtr<CSubIExCmdMenu> pSubCmd(pRaw);
			m_subCommands.push_back(pSubCmd);
		}
		return S_OK;
	}

	void FinalRelease()
	{
		m_subCommands.clear();
	}

public:
	// IShellExtInit
    STDMETHODIMP Initialize(
		_In_ PCIDLIST_ABSOLUTE pidlFolder,
		_In_ IDataObject* pdtobj,
		_In_ HKEY   hkeyProgID
	)override;

	// IContextMenu
	STDMETHOD(QueryContextMenu)(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags) override;
	STDMETHOD(InvokeCommand)(LPCMINVOKECOMMANDINFO pici) override;
	STDMETHOD(GetCommandString)(UINT_PTR idCmd, UINT uType, UINT* pwReserved, LPSTR pszName, UINT cchMax) override { return E_NOTIMPL; }

	// IExplorerCommand
// 模板
	STDMETHOD(EnumSubCommands)(IEnumExplorerCommand** ppEnum) override;
	STDMETHOD(GetCanonicalName)(GUID* pguidCommandName) override { *pguidCommandName = GetObjectCLSID(); return S_OK; }
	STDMETHOD(GetFlags)(EXPCMDFLAGS* pFlags) override { *pFlags = (!bAllDll)? ECF_HIDELABEL : ECF_HASSUBCOMMANDS; return S_OK; }
	STDMETHOD(GetIcon)(IShellItemArray* psiItemArray, LPWSTR* ppszIcon) override { return SHStrDup(_T("Resource.dll,201"), ppszIcon); }
	STDMETHOD(GetState)(IShellItemArray* psiItemArray, BOOL fOkToBeSlow, EXPCMDSTATE* pCmdState) override { *pCmdState = ECS_ENABLED; return S_OK; }
	STDMETHOD(GetTitle)(IShellItemArray* psiItemArray, LPWSTR* ppszName) override { std::wstring titlename = _T("注册IECmd"); return SHStrDup(titlename.c_str(), ppszName); }
	STDMETHOD(GetToolTip)(IShellItemArray* psiItemArray, LPWSTR* ppszInfotip) override { std::wstring tooltip = _T("注册IExCommand"); return SHStrDup(tooltip.c_str(), ppszInfotip); }
	STDMETHOD(Invoke)(IShellItemArray* psiItemArray, IBindCtx* pbc) override { return E_NOTIMPL; }
	// IEnumExplorerCommand
	STDMETHOD(Clone)(IEnumExplorerCommand** ppenum) override;
	STDMETHOD(Next)(ULONG celt, IExplorerCommand** pUICommand, ULONG* pceltFetched) override;
	STDMETHOD(Reset)() override{ m_currentIndex = 0; return S_OK; };
	STDMETHOD(Skip)(ULONG celt) override { return E_NOTIMPL; }

private:
	std::vector<std::wstring> m_selectedFiles;
	std::wstring m_folderPath;
	bool bAllDll = true;

	void initRegMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst);
	void RegCommand(int com);
};

OBJECT_ENTRY_AUTO(__uuidof(DllRegEx), CDllRegEx)
