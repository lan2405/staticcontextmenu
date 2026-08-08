// SubIExCmdMenu.cpp: CSubIExCmdMenu 的实现

#include "pch.h"
#include "SubIExCmdMenu.h"


// CSubIExCmdMenu

STDMETHODIMP_(HRESULT __stdcall)  CSubIExCmdMenu::Invoke(IShellItemArray* psiItemArray, IBindCtx* pbc)
{
	switch (m_size_)
	{
		CMD_REG:
		CMD_UREG:
        RegCommand(m_size_);
		break;
		default:
            return E_FAIL;
            break;

	}
	return S_OK;
}

void CSubIExCmdMenu::RegCommand(int com)
{

    for (const auto& path : m_selectedFiles)
    {
        std::wstring cmd = (com == CMD_REG)
            ? L"\"" + path + L"\""       // 注册：仅传递DLL路径
            : L"/u \"" + path + L"\"";   // 反注册：添加 /u 参数

        HINSTANCE result = ShellExecute(
            NULL,           // 父窗口句柄（NULL 表示无父窗口）
            _T("runas"),     // 操作类型（"open" 表示执行程序）
            _T("regsvr32.exe"), // 目标程序
            cmd.c_str(),         // 命令行参数（DLL 路径）
            NULL,           // 默认工作目录
            SW_SHOW         // 显示窗口
        );
    }
}