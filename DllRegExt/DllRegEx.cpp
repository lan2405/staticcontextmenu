// DllRegEx.cpp: CDllRegEx 的实现

#include "pch.h"
#include "DllRegEx.h"


// CDllRegEx

STDMETHODIMP_(HRESULT __stdcall) CDllRegEx::Initialize(PCIDLIST_ABSOLUTE pidlFolder, IDataObject* pdtobj, HKEY hkeyProgID)
{
   
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

STDMETHODIMP_(HRESULT __stdcall) CDllRegEx::QueryContextMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
{
    
    for (const auto& path : m_selectedFiles)
    {
        const TCHAR* ext = PathFindExtension(path.c_str());
        if (ext && _wcsicmp(ext, _T(".dll")) != 0)
        {
            bAllDll = false;
            break;
        }
    }
    if (!bAllDll)
    {
        return E_FAIL;
    }
    
    initRegMenu(hmenu, indexMenu, idCmdFirst);
    return MAKE_HRESULT(SEVERITY_SUCCESS, 0, CMD_END + 1);;
}

STDMETHODIMP_(HRESULT __stdcall) CDllRegEx::InvokeCommand(LPCMINVOKECOMMANDINFO lpici)
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
            if (wcscmp((LPCWSTR)lpici->lpVerb, L"注册") == 0)
            {
                RegCommand(CMD_REG);
                return S_OK;
            }
            else if (wcscmp((LPCWSTR)lpici->lpVerb, L"注销") == 0)
            {
                RegCommand(CMD_UREG);
                return S_OK;
            }
        }
        else
        {
            if (lstrcmpA((LPCSTR)lpici->lpVerb, "注册") == 0)
            {
                RegCommand(CMD_REG);
                return S_OK;
            }
            else if (lstrcmpA((LPCSTR)lpici->lpVerb, "注销") == 0)
            {
                RegCommand(CMD_UREG);
                return S_OK;
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
        case CMD_REG:
            RegCommand(CMD_REG);
            return S_OK;
        case CMD_UREG:
            RegCommand(CMD_UREG);
            return S_OK;
        default:
            return E_INVALIDARG;
        }
    }
}

STDMETHODIMP_(HRESULT __stdcall) CDllRegEx::EnumSubCommands(IEnumExplorerCommand** ppEnum)
{
    if (!ppEnum)
        return E_POINTER;

    m_currentIndex = 0;   // 重置枚举位置
    this->AddRef();        // 返回前增加引用计数
    *ppEnum = static_cast<IEnumExplorerCommand*>(this);
    return S_OK;
}


STDMETHODIMP_(HRESULT __stdcall) CDllRegEx::Clone(IEnumExplorerCommand** ppenum)
{
    // 简化实现：直接复用 Reset 后的当前对象（实际需深拷贝状态）
    *ppenum = NULL;
    CComObject<CDllRegEx>* pNew;
    HRESULT hr = CComObject<CDllRegEx>::CreateInstance(&pNew);
    if (SUCCEEDED(hr)) {
        pNew->AddRef();
        pNew->m_currentIndex = m_currentIndex;
        pNew->m_subCommands = m_subCommands;  // 共享子命令
        *ppenum = static_cast<IEnumExplorerCommand*>(pNew);
    }
    return hr;
}

STDMETHODIMP_(HRESULT __stdcall) CDllRegEx::Next(ULONG celt, IExplorerCommand** pUICommand, ULONG* pceltFetched)
{
    if (!pUICommand)
        return E_POINTER;
    ULONG fetched = 0;
    while (fetched < celt && m_currentIndex < m_subCommands.size()) {
        pUICommand[fetched] = m_subCommands[m_currentIndex];
        pUICommand[fetched]->AddRef(); // 必须增加引用计数
        m_currentIndex++;
        fetched++;
    }
    if (pceltFetched) *pceltFetched = fetched;
    return (fetched == celt) ? S_OK : S_FALSE;
}




void CDllRegEx::initRegMenu(HMENU hmenu, UINT indexMenu, UINT idCmdFirst)
{
    HMENU uhmenu = CreatePopupMenu();
    MENUITEMINFO hmenu_lpmi{ sizeof(MENUITEMINFO),MIIM_TYPE | MIIM_STATE | MIIM_SUBMENU,MFT_STRING,MFS_ENABLED };
    hmenu_lpmi.cch = idCmdFirst;
    hmenu_lpmi.hSubMenu = uhmenu;
    hmenu_lpmi.dwTypeData = _T("注册dll");

    MENUITEMINFO uhmenu_lpmi0{ sizeof(MENUITEMINFO),MIIM_TYPE | MIIM_STATE | MIIM_ID ,MFT_STRING,MFS_ENABLED ,idCmdFirst + CMD_REG };
    MENUITEMINFO uhmenu_lpmi1{ sizeof(MENUITEMINFO),MIIM_TYPE | MIIM_STATE | MIIM_ID ,MFT_STRING,MFS_ENABLED, idCmdFirst + CMD_UREG };
    uhmenu_lpmi0.dwTypeData = _T("注册");
    uhmenu_lpmi1.dwTypeData = _T("注销");
    InsertMenuItem(uhmenu, idCmdFirst + CMD_REG, TRUE, (LPCMENUITEMINFO)&uhmenu_lpmi0);
    InsertMenuItem(uhmenu, idCmdFirst + CMD_UREG, TRUE, (LPCMENUITEMINFO)&uhmenu_lpmi1);

    InsertMenuItem(hmenu, indexMenu, TRUE, (LPCMENUITEMINFO)&hmenu_lpmi);
}

void CDllRegEx::RegCommand( int com)
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



