#pragma once

// ============================================================================
// ExampleShellExtension.h (无WIL版本)
// ============================================================================
// 示例：基于 CShellExtensionBase 实现一个简单的"在此处打开终端"扩展
//
// 此文件展示如何：
// 1. 派生自 CShellExtensionBase
// 2. 实现纯虚函数
// 3. 使用 WRL RuntimeClass 包装为 COM 对象
// 4. 实现 COM 类工厂
// ============================================================================

#include "ShellExtensionBase.h"
#include <wrl/client.h>
#include <wrl/module.h>

// ============================================================================
// COpenTerminalHere - 示例扩展实现
// ============================================================================
// 这是一个完整的示例，实现了"在此处打开 Windows Terminal"的功能。
// 展示了如何在 Invoke 中使用 GetFilePathFromShellItem 获取路径，
// 以及如何在 GetCommandState 中检查文件系统有效性。
// ============================================================================

class COpenTerminalHere : public CShellExtensionBase
{
public:
    // ========================================================================
    // IExplorerCommand 纯虚函数实现
    // ========================================================================

    /// <summary>
    /// 执行命令：在当前文件夹路径启动 Windows Terminal。
    /// </summary>
    STDMETHODIMP DoInvoke(IShellItem* psi) override
    {
        // 获取文件系统路径
        PWSTR pszPath = nullptr;
        HRESULT hr = GetFilePathFromShellItem(psi, &pszPath);
        if (FAILED(hr) || !pszPath)
        {
            // 无法获取路径（可能是虚拟命名空间对象）
            return E_FAIL;
        }

        // 检查是否为文件夹（背景右键时psi就是文件夹）
        if (!IsFolder(psi))
        {
            // 如果是文件，获取其父目录
            Microsoft::WRL::ComPtr<IShellItem> psiParent;
            hr = psi->GetParent(psiParent.GetAddressOf());
            if (SUCCEEDED(hr) && psiParent)
            {
                PWSTR pszParentPath = nullptr;
                hr = GetFilePathFromShellItem(psiParent.Get(), &pszParentPath);
                if (SUCCEEDED(hr) && pszParentPath)
                {
                    CoTaskMemFree(pszPath);
                    pszPath = pszParentPath;
                }
                else
                {
                    CoTaskMemFree(pszPath);
                    return E_FAIL;
                }
            }
            else
            {
                CoTaskMemFree(pszPath);
                return E_FAIL;
            }
        }

        // 启动 Windows Terminal
        // 实际项目中应通过注册表或配置获取 Terminal 路径
        const wchar_t* terminalExe = L"wt.exe";
        // 注意：CreateProcessW 的第二个参数可能被修改，建议使用副本
        std::wstring args = L"new-tab --title \"Here\" --cwd \"" + std::wstring(pszPath) + L"\"";

        STARTUPINFOW si{};
        si.cb = sizeof(si);
        PROCESS_INFORMATION pi{};

        // CreateProcessW 会修改命令行字符串，需要拷贝
        std::wstring cmdLine = args;
        BOOL result = CreateProcessW(
            nullptr,                          // 应用程序名（nullptr表示用命令行第一个词）
            const_cast<PWSTR>(cmdLine.c_str()), // 命令行
            nullptr,                          // 进程安全属性
            nullptr,                          // 线程安全属性
            FALSE,                            // 不继承句柄
            0,                                // 创建标志
            nullptr,                          // 环境变量
            nullptr,                          // 当前目录
            &si,                              // 启动信息
            &pi                               // 进程信息
        );

        CoTaskMemFree(pszPath);

        if (!result)
        {
            return HRESULT_FROM_WIN32(GetLastError());
        }

        // 关闭进程和线程句柄
        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        return S_OK;
    }

    /// <summary>
    /// 获取命令状态：仅对文件系统对象启用。
    /// </summary>
    STDMETHODIMP_(EXPCMDSTATE) GetCommandState(IShellItem* psi) override
    {
        // 检查是否为有效的文件系统对象
        if (!IsFileSystemItem(psi))
        {
            return ECS_HIDDEN;  // 非文件系统对象（如控制面板、网络等）隐藏命令
        }

        // 检查是否为压缩文件夹
        if (IsCompressedFolder(psi))
        {
            return ECS_HIDDEN;  // 压缩文件夹内不支持
        }

        return ECS_ENABLED;  // 正常启用
    }

    /// <summary>
    /// 获取菜单标题。
    /// </summary>
    STDMETHODIMP GetTitleImpl(IShellItemArray* /*psiItemArray*/, PWSTR* pszName) override
    {
        // 从字符串表加载或硬编码
        *pszName = nullptr;
        // 实际项目中应使用 LoadStringW 从资源加载
        // 这里简化为硬编码
        const wchar_t* title = L"在此处打开终端";
        *pszName = static_cast<PWSTR>(CoTaskMemAlloc((wcslen(title) + 1) * sizeof(wchar_t)));
        if (*pszName)
        {
            wcscpy_s(*pszName, wcslen(title) + 1, title);
        }
        return *pszName ? S_OK : E_OUTOFMEMORY;
    }

    /// <summary>
    /// 获取菜单图标。
    /// </summary>
    STDMETHODIMP GetIconImpl(IShellItemArray* /*psiItemArray*/, PWSTR* pszIcon) override
    {
        // 从资源文件加载图标
        *pszIcon = nullptr;
        // 实际项目中应从资源文件或exe中提取图标路径
        // 返回空表示使用默认图标
        return S_OK;
    }

    // ========================================================================
    // 额外辅助方法（非虚函数）
    // ========================================================================

    /// <summary>
    /// 检查是否按下了 Ctrl+Shift 组合键（用于提升权限）。
    /// </summary>
    bool IsControlAndShiftPressed()
    {
        short control = GetAsyncKeyState(VK_CONTROL);
        short shift = GetAsyncKeyState(VK_SHIFT);
        return (control < 0) && (shift < 0);
    }
};


// ============================================================================
// COM 类工厂
// ============================================================================

class COpenTerminalHereFactory : public RuntimeClass<
    RuntimeClassFlags<ClassicCom>, IClassFactory>
{
public:
    STDMETHOD(CreateInstance)(
        IUnknown* pUnkOuter, REFIID riid, void** ppvObject) override
    {
        // 检查是否支持聚合（本示例不支持）
        if (pUnkOuter)
        {
            return CLASS_E_NOAGGREGATION;
        }

        // 直接构造对象（而不是通过 CoCreateInstance 递归）
        COpenTerminalHere* p = new (std::nothrow) COpenTerminalHere();
        if (!p)
        {
            return E_OUTOFMEMORY;
        }

        // 通过 QueryInterface 获取请求的接口
        HRESULT hr = p->QueryInterface(riid, ppvObject);
        if (FAILED(hr))
        {
            delete p;
        }
        return hr;
    }

    STDMETHOD(LockServer)(BOOL fLock) override
    {
        // 实现服务器锁定计数（省略）
        return S_OK;
    }
};


// ============================================================================
// COM 注册/注销函数声明
// ============================================================================

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, void** ppvObject);
STDAPI DllCanUnloadNow(void);
STDAPI DllRegisterServer(void);
STDAPI DllUnregisterServer(void);
