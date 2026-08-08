// SubIExCmdMenu.h: CSubIExCmdMenu 的声明

#pragma once
#include <string>
#include <vector>

#include "constinfo.h"
#include "resource.h"       // 主符号



#include "DllRegExt_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CSubIExCmdMenu

class ATL_NO_VTABLE CSubIExCmdMenu :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSubIExCmdMenu, &CLSID_SubIExCmdMenu>,
	public IDispatchImpl<ISubIExCmdMenu, &IID_ISubIExCmdMenu, &LIBID_DllRegExtLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
public IExplorerCommand
{
private:
	size_t m_size_;
	std::vector<std::wstring> m_selectedFiles;
	std::wstring m_folderPath;

	
public:
	void set_m_size(size_t m_size)
	{
		m_size_ = m_size;
	}
	void set_m_selected_files(const std::vector<std::wstring>& m_selected_files)
	{
		m_selectedFiles = m_selected_files;
	}
	void set_m_folder_path(const std::wstring& m_folder_path)
	{
		m_folderPath = m_folder_path;
	}

DECLARE_REGISTRY_RESOURCEID(107)


BEGIN_COM_MAP(CSubIExCmdMenu)
	COM_INTERFACE_ENTRY(ISubIExCmdMenu)
	COM_INTERFACE_ENTRY(IDispatch)
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
	// 模板
	STDMETHOD(EnumSubCommands)(IEnumExplorerCommand** ppEnum) override { *ppEnum = nullptr; return E_NOTIMPL; }
	STDMETHOD(GetCanonicalName)(GUID* pguidCommandName) { *pguidCommandName = IExCmdInfoRegistry[m_size_].guid; return S_OK; }
	STDMETHOD(GetFlags)(EXPCMDFLAGS* pFlags) { *pFlags = IExCmdInfoRegistry[m_size_].flags; return S_OK; }
	STDMETHOD(GetIcon)(IShellItemArray* psiItemArray, LPWSTR* ppszIcon) { std::wstring iconpath = IExCmdInfoRegistry[m_size_].icon; return SHStrDup(iconpath.c_str(), ppszIcon); }
	STDMETHOD(GetState)(IShellItemArray* psiItemArray, BOOL fOkToBeSlow, EXPCMDSTATE* pCmdState) { *pCmdState = IExCmdInfoRegistry[m_size_].state; return S_OK; }
	STDMETHOD(GetTitle)(IShellItemArray* psiItemArray, LPWSTR* ppszName) { std::wstring titlename = IExCmdInfoRegistry[m_size_].title; return SHStrDup(titlename.c_str(), ppszName); }
	STDMETHOD(GetToolTip)(IShellItemArray* psiItemArray, LPWSTR* ppszInfotip) { std::wstring tooltip = IExCmdInfoRegistry[m_size_].tooltip; return SHStrDup(tooltip.c_str(), ppszInfotip); }
	STDMETHOD(Invoke)(IShellItemArray* psiItemArray, IBindCtx* pbc) override;
	void RegCommand(int com);
};



OBJECT_ENTRY_AUTO(__uuidof(SubIExCmdMenu), CSubIExCmdMenu)
