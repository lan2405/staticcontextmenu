// ============================================================================
// ExampleShellExtension.cpp (无WIL版本)
// ============================================================================
// 示例扩展的实现文件
//
// 展示：
// 1. COM 对象实现（DllGetClassObject, DllRegisterServer, DllUnregisterServer）
// 2. 注册表操作
// 3. WRL 模块初始化
// ============================================================================

#include "ExampleShellExtension.h"
#include <shlwapi.h>
#include <wrl/client.h>
#include <wrl/module.h>

// 链接 shlwapi.lib
#pragma comment(lib, "shlwapi.lib")

// ============================================================================
// DllGetClassObject - COM 类工厂入口
// ============================================================================

STDAPI DllGetClassObject(
    REFCLSID rclsid, REFIID riid, void** ppvObject)
{
    if (rclsid == __uuidof(COpenTerminalHere))
    {
        return ComPtr<IClassFactory>(
            Callback<COpenTerminalHereFactory>::CreateInstance())
            .CopyTo(riid, ppvObject);
    }
    return CLASS_E_CLASSNOTAVAILABLE;
}

// ============================================================================
// DllCanUnloadNow - COM 卸载检查
// ============================================================================

STDAPI DllCanUnloadNow(void)
{
    // 返回 S_OK 表示可以卸载，S_FALSE 表示仍在用
    // 实际实现应检查活跃对象计数
    return S_OK;
}

// ============================================================================
// 辅助函数：将CLSID转为字符串
// ============================================================================

static HRESULT CLSIDToString(REFCLSID clsid, PWSTR* pszString)
{
    if (!pszString) return E_POINTER;
    *pszString = nullptr;

    OLECHAR szCLSID[64];
    if (StringFromGUID2(clsid, szCLSID, ARRAYSIZE(szCLSID)) == 0)
    {
        return E_FAIL;
    }

    *pszString = static_cast<PWSTR>(CoTaskMemAlloc((wcslen(szCLSID) + 1) * sizeof(wchar_t)));
    if (*pszString)
    {
        wcscpy_s(*pszString, wcslen(szCLSID) + 1, szCLSID);
    }
    return *pszString ? S_OK : E_OUTOFMEMORY;
}

// ============================================================================
// DllRegisterServer - 注册 COM 对象和 Shell 扩展
// ============================================================================

STDAPI DllRegisterServer(void)
{
    // 1. 注册 COM 类到 HKEY_CLASSES_ROOT\CLSID
    HRESULT hr = S_OK;
    wchar_t clsidStr[MAX_PATH];
    StringFromGUID2(__uuidof(COpenTerminalHere), clsidStr, ARRAYSIZE(clsidStr));

    // 创建 CLSID 项
    wchar_t szKey[MAX_PATH];
    wcscpy_s(szKey, L"CLSID\\");
    wcscat_s(szKey, clsidStr);

    HKEY hKey;
    if (RegCreateKeyExW(HKEY_CLASSES_ROOT, szKey, 0, nullptr, 0,
                        KEY_WRITE, nullptr, &hKey, nullptr) != ERROR_SUCCESS)
    {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    // 设置 InprocServer32
    const wchar_t* szValueName = L"InprocServer32";
    wchar_t szFilePath[MAX_PATH];
    GetModuleFileNameW(nullptr, szFilePath, ARRAYSIZE(szFilePath));

    if (RegSetValueExW(hKey, szValueName, 0, REG_SZ,
                       reinterpret_cast<const BYTE*>(szFilePath),
                       static_cast<DWORD>((wcslen(szFilePath) + 1) * sizeof(wchar_t))) != ERROR_SUCCESS)
    {
        hr = HRESULT_FROM_WIN32(GetLastError());
    }

    // 设置 ThreadingModel
    if (SUCCEEDED(hr))
    {
        const wchar_t* szThreadModel = L"Both";
        if (RegSetValueExW(hKey, L"ThreadingModel", 0, REG_SZ,
                           reinterpret_cast<const BYTE*>(szThreadModel),
                           static_cast<DWORD>((wcslen(szThreadModel) + 1) * sizeof(wchar_t))) != ERROR_SUCCESS)
        {
            hr = HRESULT_FROM_WIN32(GetLastError());
        }
    }

    RegCloseKey(hKey);

    // 2. 注册 Shell 扩展到 ContextMenuHandlers
    if (SUCCEEDED(hr))
    {
        // 注册表项路径数组
        const wchar_t* regPaths[] = {
            L"*\\ShellEx\\ContextMenuHandlers\\OpenTerminalHere",
            L"Directory\\Background\\ShellEx\\ContextMenuHandlers\\OpenTerminalHere",
            L"Directory\\ShellEx\\ContextMenuHandlers\\OpenTerminalHere",
            L"AllFilesystemObjects\\ShellEx\\ContextMenuHandlers\\OpenTerminalHere"
        };
        const int regPathsCount = ARRAYSIZE(regPaths);

        for (int i = 0; i < regPathsCount && SUCCEEDED(hr); ++i)
        {
            wcscpy_s(szKey, L"");
            wcscat_s(szKey, regPaths[i]);

            HKEY hRegKey;
            if (RegCreateKeyExW(HKEY_CLASSES_ROOT, szKey, 0, nullptr, 0,
                                KEY_WRITE, nullptr, &hRegKey, nullptr) == ERROR_SUCCESS)
            {
                // 默认值设为CLSID
                DWORD cbSize = static_cast<DWORD>((wcslen(clsidStr) + 1) * sizeof(wchar_t));
                if (RegSetValueExW(hRegKey, nullptr, 0, REG_SZ,
                                   reinterpret_cast<const BYTE*>(clsidStr), cbSize) != ERROR_SUCCESS)
                {
                    hr = HRESULT_FROM_WIN32(GetLastError());
                }
                RegCloseKey(hRegKey);
            }
            else
            {
                hr = HRESULT_FROM_WIN32(GetLastError());
            }
        }
    }

    return hr;
}

// ============================================================================
// DllUnregisterServer - 卸载 COM 对象和 Shell 扩展
// ============================================================================

STDAPI DllUnregisterServer(void)
{
    // 1. 删除 Shell 扩展注册表项
    const wchar_t* regPaths[] = {
        L"*\\ShellEx\\ContextMenuHandlers\\OpenTerminalHere",
        L"Directory\\Background\\ShellEx\\ContextMenuHandlers\\OpenTerminalHere",
        L"Directory\\ShellEx\\ContextMenuHandlers\\OpenTerminalHere",
        L"AllFilesystemObjects\\ShellEx\\ContextMenuHandlers\\OpenTerminalHere"
    };
    const int regPathsCount = ARRAYSIZE(regPaths);

    for (int i = 0; i < regPathsCount; ++i)
    {
        RegDeleteKeyW(HKEY_CLASSES_ROOT, regPaths[i]);
    }

    // 2. 删除 CLSID 项
    wchar_t szKey[MAX_PATH];
    wchar_t clsidStr[MAX_PATH];
    StringFromGUID2(__uuidof(COpenTerminalHere), clsidStr, ARRAYSIZE(clsidStr));

    wcscpy_s(szKey, L"CLSID\\");
    wcscat_s(szKey, clsidStr);
    RegDeleteKeyW(HKEY_CLASSES_ROOT, szKey);

    return S_OK;
}
