#pragma once
// 已空置 转移至 staticinfo_storage.h


#include <ShObjIdl_core.h>

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


struct IExCmdParm {
    // --- GUID 双形态 ---
    GUID            guid;           ///< 结构体形式，供 COM 接口使用
    const TCHAR* guidStr;        ///< 字符串形式，供注册表/日志使用

    // --- IExplorerCommand 接口字段 ---
    EXPCMDFLAGS     flags; ///< GetFlags
    const TCHAR* icon;         ///< GetIcon
    EXPCMDSTATE     state; ///< GetState
    const TCHAR* title;          ///< GetTitle
    const TCHAR* tooltip;     ///< GetToolTip
};
extern IExCmdParm IExCmdInfoRegistry[CMD_END]={
    // ── 占位（CMD_BEGIN） ──
    {
        { 0, 0, 0, {0,0,0,0,0,0,0,0} },   // guid
        nullptr,                            // guidStr
        ECF_DEFAULT,                        // flags
        nullptr,                            // icon
        ECS_ENABLED,                        // state
        nullptr,                            // title
        nullptr                            // tooltip
    },

    // ── Visual Studio ──
    {
        // {9B38C168-3954-474C-B09B-91B3795CB981}
        { 0x9B38C168, 0x3954, 0x474C, {0xB0, 0x9B, 0x91, 0xB3, 0x79, 0x5C, 0xB9, 0x81} },
        _T("9B38C168-3954-474C-B09B-91B3795CB981"),
        ECF_DEFAULT,
        _T("Resource.dll,201"),
        ECS_ENABLED,
        _T("VisualStudio"),
        _T("使用 Visual Studio 打开")
    },

    // ── VS Code ──
    {
        // {5919EB4A-8582-471F-94F2-78E4FCFC1F3F}
        { 0x5919EB4A, 0x8582, 0x471F, {0x94, 0xF2, 0x78, 0xE4, 0xFC, 0xFC, 0x1F, 0x3F} },
        _T("5919EB4A-8582-471F-94F2-78E4FCFC1F3F"),
        ECF_DEFAULT,
        _T(""),
        ECS_ENABLED,
        _T("VSCode"),
        _T("使用 VS Code 打开")
    },

    // ── Toolbox ──
    {
        // {3A3CFC81-F941-416E-8FEA-8D59AB955881}
        { 0x3A3CFC81, 0xF941, 0x416E, {0x8F, 0xEA, 0x8D, 0x59, 0xAB, 0x95, 0x58, 0x81} },
        _T("3A3CFC81-F941-416E-8FEA-8D59AB955881"),
        ECF_DEFAULT,
        _T(""),
        ECS_ENABLED,
        _T("Toolbox"),
        _T("打开 Toolbox")
    },

    // ── WSL ──
    {
        // {E9BB5749-B06B-45A9-8C6A-E4676B6B2998}
        { 0xE9BB5749, 0xB06B, 0x45A9, {0x8C, 0x6A, 0xE4, 0x67, 0x6B, 0x6B, 0x29, 0x98} },
        _T("E9BB5749-B06B-45A9-8C6A-E4676B6B2998"),
        ECF_DEFAULT,
        _T(""),
        ECS_ENABLED,
        _T("WSL"),
        _T("打开 WSL")
    },

    // ── PowerShell ──
    {
        // {F586A024-9494-49D7-89A6-653DE4389F53}
        { 0xF586A024, 0x9494, 0x49D7, {0x89, 0xA6, 0x65, 0x3D, 0xE4, 0x38, 0x9F, 0x53} },
        _T("F586A024-9494-49D7-89A6-653DE4389F53"),
        ECF_DEFAULT,
        _T(""),
        ECS_ENABLED,
        _T("PowerShell"),
        _T("打开 PowerShell")
    },

    // ── PowerShell (管理员) ──
    {
        // {39007F76-0A3D-42F0-A0E6-8A769185C092}
        { 0x39007F76, 0x0A3D, 0x42F0, {0xA0, 0xE6, 0x8A, 0x76, 0x91, 0x85, 0xC0, 0x92} },
        _T("39007F76-0A3D-42F0-A0E6-8A769185C092"),
        ECF_DEFAULT,
        _T(""),
        ECS_ENABLED,
        _T("PowerShell (Admin)"),
        _T("以管理员权限打开 PowerShell")
    },

    // ── PowerShell (系统) ──
    {
        // {C8B3B605-E338-4D14-A764-3B65EE779CA2}
        { 0xC8B3B605, 0xE338, 0x4D14, {0xA7, 0x64, 0x3B, 0x65, 0xEE, 0x77, 0x9C, 0xA2} },
        _T("C8B3B605-E338-4D14-A764-3B65EE779CA2"),
        ECF_DEFAULT,
        _T(""),
        ECS_ENABLED,
        _T("PowerShell (System)"),
        _T("以系统权限打开 PowerShell")
    },

    // ── Windows Terminal ──
    {
        // {CA81CCCF-DA8A-4AC6-8194-2F92AC1048CA}
        { 0xCA81CCCF, 0xDA8A, 0x4AC6, {0x81, 0x94, 0x2F, 0x92, 0xAC, 0x10, 0x48, 0xCA} },
        _T("CA81CCCF-DA8A-4AC6-8194-2F92AC1048CA"),
        ECF_DEFAULT,
        _T(""),
        ECS_ENABLED,
        _T("Windows Terminal"),
        _T("打开 Windows Terminal")
    }
};
extern SHELLEXECUTEINFO RegCmdInfo[CMD_END]={
	{sizeof(SHELLEXECUTEINFO),
            SEE_MASK_DEFAULT,
            nullptr,
            _T("runas"),
            _T("regsvr32.exe"),
         _T(""),
            NULL,
            SW_SHOW,
            nullptr,
            nullptr,
            nullptr,
            nullptr},
    {sizeof(SHELLEXECUTEINFO),
            SEE_MASK_DEFAULT,
            nullptr,
            _T("runas"),
            _T("regsvr32.exe"),
            _T(""),
            NULL,
            SW_SHOW,
            nullptr,
            nullptr,
            nullptr,
            nullptr},
    {sizeof(SHELLEXECUTEINFO),
            SEE_MASK_DEFAULT,
            nullptr,
            _T("runas"),
            _T("regsvr32.exe"),
         _T(""),
            NULL,
            SW_SHOW,
            nullptr,
            nullptr,
            nullptr,
            nullptr},
    {sizeof(SHELLEXECUTEINFO),
            SEE_MASK_DEFAULT,
            nullptr,
            _T("runas"),
            _T("regsvr32.exe"),
         _T(""),
            NULL,
            SW_SHOW,
            nullptr,
            nullptr,
            nullptr,
            nullptr},
    {sizeof(SHELLEXECUTEINFO),
            SEE_MASK_DEFAULT,
            nullptr,
            _T("runas"),
            _T("regsvr32.exe"),
         _T(""),
            NULL,
            SW_SHOW,
            nullptr,
            nullptr,
            nullptr,
            nullptr},
    {sizeof(SHELLEXECUTEINFO),
            SEE_MASK_DEFAULT,
            nullptr,
            _T("runas"),
            _T("regsvr32.exe"),
         _T(""),
            NULL,
            SW_SHOW,
            nullptr,
            nullptr,
            nullptr,
            nullptr},
    {sizeof(SHELLEXECUTEINFO),
            SEE_MASK_DEFAULT,
            nullptr,
            _T("runas"),
            _T("regsvr32.exe"),
         _T(""),
            NULL,
            SW_SHOW,
            nullptr,
            nullptr,
            nullptr,
            nullptr},
    {sizeof(SHELLEXECUTEINFO),
            SEE_MASK_DEFAULT,
            nullptr,
            _T("runas"),
            _T("regsvr32.exe"),
         _T(""),
            NULL,
            SW_SHOW,
            nullptr,
            nullptr,
            nullptr,
            nullptr},
            {sizeof(SHELLEXECUTEINFO),
            SEE_MASK_DEFAULT,
            nullptr,
            _T("runas"),
            _T("regsvr32.exe"),
         _T(""),
            NULL,
            SW_SHOW,
            nullptr,
            nullptr,
            nullptr,
            nullptr}
};

