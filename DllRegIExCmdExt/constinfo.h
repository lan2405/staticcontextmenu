

#include <shobjidl_core.h>



extern enum MyCmd {
	CMD_ROOT = 0,          ///< 根命令
	CMD_REG,            ///< 注册
	CMD_UREG,         ///< 反注册
	CMD_END             ///< 结束
};


struct _ExplorerCommand {
	GUID            guid;
	EXPCMDFLAGS     flags; ///< GetFlags
	const TCHAR* icon;         ///< GetIcon
	EXPCMDSTATE     state; ///< GetState
	const TCHAR* title;          ///< GetTitle
	const TCHAR* tooltip;     ///< GetToolTip
}ExplorerCommand;


extern ExplorerCommand IExCmdInfoRegistry[CMD_END] = {
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


extern SHELLEXECUTEINFO RegCmdInfo = {
	sizeof(SHELLEXECUTEINFO),
	SEE_MASK_DEFAULT,
	nullptr,
	_T("runas"),
	_T("regsvr32.exe"),
	_T("/u"),
	NULL,
	SW_SHOW,
	nullptr,
	nullptr,
	nullptr,
	nullptr
};




