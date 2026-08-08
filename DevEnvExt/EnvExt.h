// EnvExt.h: CEnvExt 的声明

#pragma once
#include "resource.h"       // 主符号
#include<ShlObj_core.h>
#include<vector>
#include <string>

#include "DevEnvExt_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;

enum EnvCmd {
	CMD_BEGIN,
	CMD_VisioStudio,
	CMD_VSCode,
	CMD_Toolbox,
	CMD_WSL,
	CMD_PowerShell,
	CMD_PowerShell_Admin,
	CMD_PowerShell_System,
	CMD_WindowsTerminal,
	CMD_END
};

// 定义命令信息结构体
typedef struct _CommandInfo {
	int CmdId;
	const TCHAR* menuText;      ///< 菜单显示文本
	const TCHAR* commandPath;    ///< 可执行文件路径 (e.g., "devenv.exe")
	const TCHAR* arguments;      ///<  启动参数 (e.g., "/path/to/project.sln")
	BOOL requiresAdmin;         ///< 是否需要管理员权限 冗余设计未使用
	BOOL isSystemLevel;        ///< 是否为系统级命令 (如 PowerShell_System) 冗余设计未使用
} CommandInfo;

// 定义菜单文本数组，顺序必须与 enum EnvCmd 严格对应
// 注意：CMD_BEGIN 和 CMD_END 是控制位，不对应具体菜单项，所以从 CMD_VisioStudio 开始
//const TCHAR* menuTexts[] = {
//	NULL,      // CMD_BEGIN (0)
//	_T("使用 Visual Studio 打开(&V)"),      // CMD_VisioStudio (1)
//	_T("通过 Code 打开(&C)"),                  // CMD_VSCode (2)
//	_T("Open Git Bash here"),              // CMD_Git (3)
//	_T("JetBrains Toolbox"),               // CMD_Toolbox (4)
//	_T("在此处打开 Linux shell(&L)"),       // CMD_WSL (5)
//	_T("PowerShell"),                      // CMD_PowerShell (6)
//	_T("PowerShell (管理员)"),             // CMD_PowerShell_Admin (7)
//	_T("PowerShell (系统)"),               // CMD_PowerShell_System (8)
//	_T("Windows Terminal"),                 // CMD_WindowsTerminal (9)
//	NULL      // CMD_END (10)
//};

// 用结构体数组统一存储所有命令信息(关键改进！)
const CommandInfo g_CommandRegistry[CMD_END] = {
	// 每行显式设置CmdId，彻底解耦枚举值与数组索引
    {
		CMD_BEGIN,    // 占位0规范数组
		NULL,
		NULL,
		NULL,
		FALSE,
		FALSE
	},
	{
		CMD_VisioStudio,
		_T("使用 Visual Studio 打开(&V)"),
		_T("C:\\Program Files (x86)\\Common Files\\Microsoft Shared\\MSEnv\\VSLauncher.exe"),
		_T("source:ExplorerBackground"),
		FALSE,
		FALSE
	},
	{
		CMD_VSCode,
		_T("通过 Code 打开(&C)"),
		_T("D:\\Program Files\\Microsoft VS Code\\Code.exe"),
		_T(""),
		FALSE,
		FALSE
	},
	{
		CMD_Toolbox,
		_T("JetBrains Toolbox"),
		_T("C:\\Users\\Dallas\\AppData\\Local\\JetBrains\\Toolbox\\bin\\jetbrains-toolbox.exe"),
		_T(""),
		FALSE,
		FALSE
	},
	{
		CMD_WSL,
		_T("在此处打开 Linux shell(&L)"),
		_T("wsl.exe "),
		_T(""),
		FALSE,
		FALSE
	},
	{
		CMD_PowerShell,
		_T("PowerShell"),
		_T("powershell.exe"),
		_T("-NoExit -Command cd "),
		FALSE,
		FALSE
	},
	{
		CMD_PowerShell_Admin,
		_T("PowerShell (管理员)"),
		_T("powershell.exe"),
		_T("-NoExit -Command cd "),
		TRUE,
		FALSE
	},
	{
		CMD_PowerShell_System,
		_T("PowerShell (系统)"),
		_T("psexec.exe"),
		_T("-s powershell -w Normal "),
		TRUE,
		TRUE
	},
	{
		CMD_WindowsTerminal,
		_T("Windows Terminal"),
		_T("wt.exe"),
		_T(""),
		FALSE,
		FALSE
	}
};


// CEnvExt

class ATL_NO_VTABLE CEnvExt :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CEnvExt, &CLSID_EnvExt>,
	public IDispatchImpl<IEnvExt, &IID_IEnvExt, &LIBID_DevEnvExtLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IShellExtInit,
	public IContextMenu,
    public IExplorerCommand
{
public:
	CEnvExt()
	{}

	DECLARE_REGISTRY_RESOURCEID(106)


	BEGIN_COM_MAP(CEnvExt)
		COM_INTERFACE_ENTRY(IEnvExt)
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
	{}

public:
	// IShellExtInit
	STDMETHODIMP Initialize(
		_In_ PCIDLIST_ABSOLUTE pidlFolder,
		_In_ IDataObject* pdtobj,
		_In_ HKEY   hkeyProgID
	) override;
	// IContextMenu
	STDMETHODIMP GetCommandString(
		_In_ UINT_PTR idcmd,
		_In_ UINT uType,
		_In_ UINT* pReserved,
		_Out_writes_bytes_opt_(cchOut) LPSTR pszName,
		_In_ UINT cchOut
	)override;
	STDMETHODIMP InvokeCommand(
		_In_ LPCMINVOKECOMMANDINFO lpici
	) override;
	STDMETHODIMP QueryContextMenu(
		_In_ HMENU hmenu,
		_In_ UINT indexMenu,
		_In_ UINT idCmdFirst,
		_In_ UINT idCmdLast,
		_In_ UINT uFlags
	) override;



private:
	std::vector<std::wstring> m_selectedFiles;
	std::wstring m_folderPath;

	void initMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst);
	void RunCommandOnce(int cmdid);
	void RunCommandPathAuth(int cmdid);
    void RunCommandPath(int cmdid);
};

OBJECT_ENTRY_AUTO(__uuidof(EnvExt), CEnvExt)
