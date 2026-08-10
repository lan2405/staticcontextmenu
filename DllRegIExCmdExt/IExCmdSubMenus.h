// IExCmdSubMenus.h: CIExCmdSubMenus 的声明

#pragma once
#include <string>
#include <vector>
#include "resource.h"       // 主符号
#include <ShlObj_core.h>
#include "DllRegIExCmdExt_i.h"


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
struct ExplorerCommand {
	GUID            guid;
	EXPCMDFLAGS     flags; ///< GetFlags
	const TCHAR* icon;         ///< GetIcon
	EXPCMDSTATE     state; ///< GetState
	const TCHAR* title;          ///< GetTitle
	const TCHAR* tooltip;     ///< GetToolTip
};


const ExplorerCommand IExCmdInfoRegistry[CMD_END] = {
	{
		{ 0, 0, 0, {0,0,0,0,0,0,0,0} },   // guid
		ECF_DEFAULT,                        // flags
		nullptr,                            // icon
		ECS_ENABLED,                        // state
		nullptr,                            // title
		nullptr                            // tooltip
	},
	{
		// {5087E382-421C-487A-841E-4B219BDA0DD4}
{0x5087e382, 0x421c, 0x487a, { 0x84, 0x1e, 0x4b, 0x21, 0x9b, 0xda, 0xd, 0xd4 }},
	ECF_DEFAULT,
	nullptr,
	ECS_ENABLED,
	_T("注册组件"),
	_T("注册组件")
	},
	// ── 注销组件 ──
{
	// {39FB48D9-5E73-4533-B0C3-565857AFF622}
	{ 0x39fb48d9, 0x5e73, 0x4533, { 0xb0, 0xc3, 0x56, 0x58, 0x57, 0xaf, 0xf6, 0x22 } },
	ECF_DEFAULT,
	_T(""),
	ECS_ENABLED,
	_T("注销组件"),
	_T("注销组件")
	},
};
// CIExCmdSubMenus

class ATL_NO_VTABLE CIExCmdSubMenus :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CIExCmdSubMenus, &CLSID_IExCmdSubMenus>,
	public IDispatchImpl<IIExCmdSubMenus, &IID_IIExCmdSubMenus, &LIBID_DllRegIExCmdExtLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IExplorerCommand
{
public:
	CIExCmdSubMenus()
	{
	}

DECLARE_REGISTRY_RESOURCEID(107)


BEGIN_COM_MAP(CIExCmdSubMenus)
	COM_INTERFACE_ENTRY(IIExCmdSubMenus)
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
private:
	size_t m_size_=NULL;
public:
	void set_m_size(size_t m_size) { m_size_ = m_size; }
	// IExplorerCommand
	IFACEMETHOD(EnumSubCommands)(IEnumExplorerCommand** ppEnum) override { *ppEnum = nullptr; return E_NOTIMPL; }
	IFACEMETHOD(GetCanonicalName)(GUID* pguidCommandName) { *pguidCommandName =IExCmdInfoRegistry[m_size_].guid; return S_OK; }
	IFACEMETHOD(GetFlags)(EXPCMDFLAGS* pFlags) { *pFlags = ECF_DEFAULT; return S_OK; }
	IFACEMETHOD(GetIcon)(IShellItemArray* psiItemArray, LPWSTR* ppszIcon) { return SHStrDup(IExCmdInfoRegistry[m_size_].icon, ppszIcon); }
	IFACEMETHOD(GetState)(IShellItemArray* psiItemArray, BOOL fOkToBeSlow, EXPCMDSTATE* pCmdState) { *pCmdState = ECS_ENABLED; return S_OK; }
	IFACEMETHOD(GetTitle)(IShellItemArray* psiItemArray, LPWSTR* ppszName) { std::wstring titlename = IExCmdInfoRegistry[m_size_].title; return SHStrDup(titlename.c_str(), ppszName); }
	IFACEMETHOD(GetToolTip)(IShellItemArray* psiItemArray, LPWSTR* ppszInfotip) { std::wstring tooltip = IExCmdInfoRegistry[m_size_].tooltip; return SHStrDup(tooltip.c_str(), ppszInfotip); }
	IFACEMETHOD(Invoke)(IShellItemArray* psiItemArray, IBindCtx* pbc) override;


};

OBJECT_ENTRY_AUTO(__uuidof(IExCmdSubMenus), CIExCmdSubMenus)
