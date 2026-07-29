// EnvExt.cpp: CEnvExt 的实现

#include "pch.h"
#include "EnvExt.h"


// CEnvExt

STDMETHODIMP_(HRESULT __stdcall) CEnvExt::Initialize(PCIDLIST_ABSOLUTE pidlFolder, IDataObject* pdtobj, HKEY hkeyProgID)
{

    // 获取文件夹folderPath
    TCHAR szFolderPath[MAX_PATH] = { 0 };
    if (pidlFolder != NULL)
    {
        if (SHGetPathFromIDListW(pidlFolder, szFolderPath))
        {
            // szFolderPath 现在包含文件夹的完整路径
            // 例如：C:\Users\Username\Documents
            m_folderPath = szFolderPath;
        }

    }


    // 获取选中的文件
    if (pdtobj)
    {
        FORMATETC fmt = { CF_HDROP, NULL, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
        STGMEDIUM stg = { 0 };

        if (SUCCEEDED(pdtobj->GetData(&fmt, &stg)))
        {
            HDROP hDrop = (HDROP)GlobalLock(stg.hGlobal);
            if (hDrop)
            {
                UINT nFiles = DragQueryFileW(hDrop, 0xFFFFFFFF, NULL, 0);
                for (UINT i = 0; i < nFiles; i++)
                {
                    wchar_t szFile[MAX_PATH];
                    if (DragQueryFileW(hDrop, i, szFile, MAX_PATH))
                    {
                        m_selectedFiles.push_back(szFile);
                    }
                }
                GlobalUnlock(stg.hGlobal);
            }
            ReleaseStgMedium(&stg);
        }
    }

    return S_OK; // 或其他适当的状态
}

STDMETHODIMP_(HRESULT __stdcall) CEnvExt::GetCommandString(UINT_PTR idcmd, UINT uType, UINT* pReserved, LPSTR pszName, UINT cchOut)
{
    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CEnvExt::InvokeCommand(LPCMINVOKECOMMANDINFO lpici)
{
    // 1. 基本参数验证
    if (!lpici)
        return E_INVALIDARG;

    // 2. 检查是否通过谓词字符串调用
    if (HIWORD(lpici->lpVerb) != 0)
    {
        // 根据fMask判断使用ANSI还是Unicode
        if (lpici->fMask & CMIC_MASK_UNICODE)
        {
            int i = 0;
            for (int i = CMD_VisioStudio; i < CMD_END; ++i)
            {
                if (wcscmp((LPCWSTR)lpici->lpVerb, g_CommandRegistry[i].menuText) == 0)
                {
                    RunCommand(i);
                    ++i;
                    return S_OK;
                }
            }
            if (i < 0)return E_INVALIDARG;
        }
        else
        {
            int i = 0;
            for (int i = CMD_VisioStudio; i < CMD_END; ++i)
            {
                if (wcscmp((LPCWSTR)lpici->lpVerb, g_CommandRegistry[i].menuText) == 0)
                {
                    RunCommand(i);
                    ++i;
                    return S_OK;
                }
            }
        }

        // 字符串不匹配时明确返回错误
        return E_INVALIDARG;
    }
    // 3. 检查是否通过命令ID调用
    else
    {
        switch (LOWORD(lpici->lpVerb))
        {
        case CMD_VisioStudio:
        case CMD_VSCode:
        case CMD_Git:
        case CMD_Toolbox:
        case CMD_WSL:
        case CMD_PowerShell:
        case CMD_PowerShell_Admin:
        case CMD_PowerShell_System:
        case CMD_WindowsTerminal:
            // 执行对应的环境命令
            RunCommand(static_cast<EnvCmd>(LOWORD(lpici->lpVerb)));
            return S_OK;
        default:
            return E_INVALIDARG;
        }
    }



    return S_OK;
}

STDMETHODIMP_(HRESULT __stdcall) CEnvExt::QueryContextMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
{
    // 处理特殊标志
    if (uFlags & CMF_DEFAULTONLY)
        return MAKE_HRESULT(SEVERITY_SUCCESS, 0, 1); // 默认命令索引


    initMenu(hmenu, indexMenu, idCmdFirst);
    return MAKE_HRESULT(SEVERITY_SUCCESS, 0, CMD_END);
}

void CEnvExt::initMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst)
{

    // 1. 创建子菜单句柄 (对应图片中的二级菜单)
    HMENU hSubMenu = CreatePopupMenu();


    MENUITEMINFO mii = { sizeof(MENUITEMINFO) };
    mii.fMask = MIIM_TYPE | MIIM_STATE | MIIM_ID;
    mii.fType = MFT_STRING;
    mii.fState = MFS_ENABLED;

    // 3. 循环将菜单项插入到子菜单中
    // 循环次数 = CMD_END - CMD_BEGIN - 1 (减去CMD_BEGIN本身占用的一个位)
    for (int i = CMD_VisioStudio; i < CMD_END; ++i)
    {
        if (!g_CommandRegistry[i].menuText) continue;

        // 关键点：命令ID必须是 idCmdFirst + 枚举值
        mii.wID = idCmdFirst + i;
        mii.dwTypeData = (LPTSTR)g_CommandRegistry[i].menuText;
        mii.cch = _tcslen(g_CommandRegistry[i].menuText);

        // 将项目插入子菜单，按顺序排列
        InsertMenuItem(hSubMenu, idCmdFirst + i, TRUE, (LPCMENUITEMINFO)&mii);
    }

    // 4. 配置主菜单项（即右键直接看到的那个入口）
    MENUITEMINFO mainItem = { sizeof(MENUITEMINFO) };
    mainItem.fMask = MIIM_TYPE | MIIM_STATE | MIIM_SUBMENU  ;
    mainItem.fType = MFT_STRING ;
    mainItem.fState = MFS_ENABLED;

   

    // 主菜单不需要 wID，因为它展开的是子菜单，不直接触发 InvokeCommand
    // 但为了规范，有些开发者会设 idCmdFirst，这里保持默认即可
    mainItem.hSubMenu = hSubMenu;
    mainItem.dwTypeData = _T("打开编程环境目录"); // 对应图片中的父级菜单名
    mainItem.cch = _tcslen(mainItem.dwTypeData);

    // 5. 将带有子菜单的主项插入到系统右键菜单 (hmenu) 中
    InsertMenuItem(hmenu, indexMenu, TRUE, (LPCMENUITEMINFO)&mainItem);
}

void CEnvExt::RunCommand(int cmd)
{


}




