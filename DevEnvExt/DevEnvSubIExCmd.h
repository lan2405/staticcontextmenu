// DevEnvSubIExCmd.h: CDevEnvSubIExCmd 的声明
#pragma once



#include <string>
#include "staticinfo_storage.h"
#include "resource.h"       // 主符号
#include <vector>


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
	public IExplorerCommand
{
public:
DECLARE_REGISTRY_RESOURCEID(108)
BEGIN_COM_MAP(CDevEnvSubIExCmd)
	COM_INTERFACE_ENTRY(IDevEnvSubIExCmd)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IExplorerCommand)
END_COM_MAP()
DECLARE_PROTECT_FINAL_CONSTRUCT()
private:
	size_t m_size_ = NULL;
public:
	void set_m_size(size_t m_size) { m_size_ = m_size; }
	// IExplorerCommand
	IFACEMETHOD(EnumSubCommands)(IEnumExplorerCommand** ppEnum) override { *ppEnum = nullptr; return E_NOTIMPL; }
	IFACEMETHOD(GetCanonicalName)(GUID* pguidCommandName) { *pguidCommandName = IExCmdInfoRegistry[m_size_].guid; return S_OK; }
	IFACEMETHOD(GetFlags)(EXPCMDFLAGS* pFlags) { *pFlags = IExCmdInfoRegistry[m_size_].flags; return S_OK; }
	IFACEMETHOD(GetIcon)(IShellItemArray* psiItemArray, LPWSTR* ppszIcon) { return SHStrDup(IExCmdInfoRegistry[m_size_].icon, ppszIcon); }
	IFACEMETHOD(GetState)(IShellItemArray* psiItemArray, BOOL fOkToBeSlow, EXPCMDSTATE* pCmdState) { *pCmdState = IExCmdInfoRegistry[m_size_].state; return S_OK; }
	IFACEMETHOD(GetTitle)(IShellItemArray* psiItemArray, LPWSTR* ppszName) { std::wstring titlename = IExCmdInfoRegistry[m_size_].title; return SHStrDup(titlename.c_str(), ppszName); }
	IFACEMETHOD(GetToolTip)(IShellItemArray* psiItemArray, LPWSTR* ppszInfotip) { std::wstring tooltip = IExCmdInfoRegistry[m_size_].tooltip; return SHStrDup(tooltip.c_str(), ppszInfotip); }
	IFACEMETHOD(Invoke)(IShellItemArray* psiItemArray, IBindCtx* pbc) override;



};

OBJECT_ENTRY_AUTO(__uuidof(DevEnvSubIExCmd), CDevEnvSubIExCmd)
