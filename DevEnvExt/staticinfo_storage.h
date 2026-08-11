#pragma once



#include <ShObjIdl_core.h>
#include <shellapi.h>

// ── 枚举 ──
enum EnvCmd {
    CMD_BEGIN = 0,
    CMD_VisualStudio,
    CMD_VSCode,
    CMD_Toolbox,
    CMD_WSL,
    CMD_PowerShell,
    CMD_PowerShell_Admin,
    CMD_PowerShell_System,
    CMD_WindowsTerminal,
    CMD_END
};

// ── 结构体 ──
struct IExCmdParm {
    GUID            guid;
    const TCHAR* guidStr;
    EXPCMDFLAGS     flags;
    const TCHAR* icon;
    EXPCMDSTATE     state;
    const TCHAR* title;
    const TCHAR* tooltip;
};

// ── 仅声明，不定义 ──
extern IExCmdParm       IExCmdInfoRegistry[CMD_END];
extern SHELLEXECUTEINFO CmdInfo[CMD_END];
