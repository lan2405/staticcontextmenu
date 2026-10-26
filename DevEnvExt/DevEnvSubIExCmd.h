// DevEnvSubIExCmd.h: CDevEnvSubIExCmd 的声明
#pragma once



#include <string>
#include "staticinfo_storage.h"
#include "resource.h"       // 主符号
#include <vector>
#include <ShObjIdl_core.h>
#include <wil\com.h>
#include <wil\win32_helpers.h>
#include <wil/result.h>
#include "DevEnvExt_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CDevEnvSubIExCmd

class ATL_NO_VTABLE CDevEnvSubIExCmd :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CDevEnvSubIExCmd, &CLSID_DevEnvSubIExCmd>,
	public IDispatchImpl<IDevEnvSubIExCmd, &IID_IDevEnvSubIExCmd, &LIBID_DevEnvExtLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IExplorerCommand,
	public IObjectWithSite
{
public:
DECLARE_REGISTRY_RESOURCEID(108)
BEGIN_COM_MAP(CDevEnvSubIExCmd)
	COM_INTERFACE_ENTRY(IDevEnvSubIExCmd)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IExplorerCommand)
	COM_INTERFACE_ENTRY(IObjectWithSite)
END_COM_MAP()
DECLARE_PROTECT_FINAL_CONSTRUCT()
private:
	size_t m_size_ = 0;
	wil::com_ptr_nothrow<IUnknown> site_;
	std::wstring m_modulePath;
public:
	void set_m_size(size_t m_size) { m_size_ = m_size; }
	void SetModulePath(const std::wstring& path) { m_modulePath = path; }
	HRESULT GetLocationFromSite(IShellItem** location) const noexcept;
	HRESULT GetBestLocationFromSelectionOrSite(IShellItemArray* psiArray, IShellItem** location) const noexcept;

	// IExplorerCommand
	IFACEMETHOD(EnumSubCommands)(IEnumExplorerCommand** ppEnum) override { *ppEnum = nullptr; return E_NOTIMPL; }
	IFACEMETHOD(GetCanonicalName)(GUID* pguidCommandName) { *pguidCommandName = IExCmdInfoRegistry[m_size_].guid; return S_OK; }
	IFACEMETHOD(GetFlags)(EXPCMDFLAGS* pFlags) { *pFlags = IExCmdInfoRegistry[m_size_].flags; return S_OK; }
	IFACEMETHOD(GetIcon)(IShellItemArray* psiItemArray, LPWSTR* ppszIcon) {
		wchar_t iconPath[MAX_PATH + 32] = {};
		if (IExCmdInfoRegistry[m_size_].icon)swprintf_s(iconPath, IExCmdInfoRegistry[m_size_].icon, m_modulePath.c_str()); // 当前组件位置
		return SHStrDup(iconPath, ppszIcon);
	}
	IFACEMETHOD(GetState)(IShellItemArray* psiItemArray, BOOL fOkToBeSlow, EXPCMDSTATE* pCmdState) { *pCmdState = IExCmdInfoRegistry[m_size_].state; return S_OK; }
	IFACEMETHOD(GetTitle)(IShellItemArray* psiItemArray, LPWSTR* ppszName) { std::wstring titlename = IExCmdInfoRegistry[m_size_].title; return SHStrDup(titlename.c_str(), ppszName); }
	IFACEMETHOD(GetToolTip)(IShellItemArray* psiItemArray, LPWSTR* ppszInfotip) { std::wstring tooltip = IExCmdInfoRegistry[m_size_].tooltip; return SHStrDup(tooltip.c_str(), ppszInfotip); }
	IFACEMETHOD(Invoke)(IShellItemArray* psiItemArray, IBindCtx* pbc) override;

	// IObjectWithSite
	IFACEMETHOD(SetSite)(IUnknown* pUnkSite) override {
		site_ = pUnkSite;
		return S_OK;
	}
	IFACEMETHOD(GetSite)(REFIID riid, void** ppvSite) override {
		RETURN_IF_FAILED(site_.query_to(riid, ppvSite));
		return S_OK;
	}
	

};

OBJECT_ENTRY_AUTO(__uuidof(DevEnvSubIExCmd), CDevEnvSubIExCmd)

