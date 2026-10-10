#pragma once

// ============================================================================
// ShellExtensionBase.h (无WIL版本)
// ============================================================================
// IExplorerCommand + IObjectWithSite 框架基础头文件
//
// 设计目标：
// 1. 仅实现 IExplorerCommand + IObjectWithSite，不实现 IShellExtInit
// 2. 通过 IObjectWithSite::SetSite 获取站点指针，兼容新旧菜单
// 3. GetBestLocationFromSelectionOrSite 双通道取路径（psiItemArray + Site回退）
// 4. 派生类只需实现少量纯虚函数即可完成扩展
// 5. 不依赖 WIL，仅依赖 Windows SDK + WRL
//
// 使用方法：
//   1. 包含此头文件
//   2. 派生自 CShellExtensionBase
//   3. 实现纯虚函数
//   4. 通过 WRL RuntimeClass 包装为 COM 对象
//   5. 注册 CLSID 到注册表或 MSIX 清单
//
// 依赖：Windows SDK + WRL (Microsoft::WRL::ComPtr)
// ============================================================================

#include <windows.h>
#include <shellapi.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <wrl/client.h>
#include <wrl/module.h>
#include <string>

// ============================================================================
// 辅助宏（替代 WIL 的 RETURN_IF_FAILED / RETURN_HR_IF）
// ============================================================================

#define CHECK_HR(expr) \
    do { \
        HRESULT _hr = (expr); \
        if (FAILED(_hr)) return _hr; \
    } while (0)

#define CHECK_HR_IF(cond, hr) \
    do { \
        if (cond) return (hr); \
    } while (0)

// ============================================================================
// CShellExtensionBase - IExplorerCommand + IObjectWithSite 实现基类
// ============================================================================
//
// 此类直接继承 IExplorerCommand 和 IObjectWithSite 接口，
// 实现了所有接口方法的公共逻辑，并将核心行为抽取为纯虚函数。
//
// 架构说明：
// - IObjectWithSite::SetSite 由壳在扩展实例化时调用，传入站点指针
// - 站点指针可用于查询 IServiceProvider -> SID_SFolderView -> IFolderView
// - 进而通过 IFolderView::GetFolder 获取当前文件夹的 IShellItem
// - 这覆盖了 IShellExtInit::Initialize 提供的"父文件夹PIDL"语义
// - 因此无需实现 IShellExtInit 即可在旧版菜单中获取上下文
//
// 使用示例：
//   class CMyExtension : public CShellExtensionBase
//   {
//   public:
//       STDMETHODIMP DoInvoke(IShellItem* psi) override { ... }
//       STDMETHODIMP GetCommandState(IShellItem* psi) override { ... }
//       STDMETHODIMP GetTitleImpl(IShellItemArray*, PWSTR*) override { ... }
//       STDMETHODIMP GetIconImpl(IShellItemArray*, PWSTR*) override { ... }
//   };
//
//   然后使用 WRL RuntimeClass 包装：
//   class CMyExtensionCom : public RuntimeClass<
//       RuntimeClassFlags<ClassicCom>, CMyExtension>
//   {
//       // RuntimeClass 自动提供 IUnknown (AddRef/Release/QueryInterface)
//   };
// ============================================================================

class CShellExtensionBase : public IExplorerCommand, public IObjectWithSite
{
public:
    // ========================================================================
    // IObjectWithSite 实现
    // ========================================================================

    /// <summary>
    /// 设置站点指针。由壳在创建扩展对象后调用。
    /// 缓存站点指针供后续 GetLocationFromSite 使用。
    /// </summary>
    STDMETHODIMP SetSite(LPUNKNOWN pUnkSite) override 
    {
        m_site = pUnkSite;
        return S_OK;
    }

    /// <summary>
    /// 获取站点指针。
    /// </summary>
    STDMETHODIMP GetSite(REFIID riid, void** ppvSite) override 
    {
        return m_site->QueryInterface(riid, ppvSite);
    }

    // ========================================================================
    // IExplorerCommand 实现
    // ========================================================================

    /// <summary>
    /// 获取菜单标题文本。转发到派生类的 GetTitleImpl 实现。
    /// </summary>
    STDMETHODIMP GetTitle(IShellItemArray* psiItemArray, PWSTR* pszName) override
    {
        return GetTitleImpl(psiItemArray, pszName);
    }

    /// <summary>
    /// 获取菜单图标。转发到派生类的 GetIconImpl 实现。
    /// </summary>
    STDMETHODIMP GetIcon(IShellItemArray* psiItemArray, PWSTR* pszIcon) override
    {
        return GetIconImpl(psiItemArray, pszIcon);
    }

    /// <summary>
    /// 获取工具提示（默认不提供）。
    /// </summary>
    STDMETHODIMP GetToolTip(IShellItemArray*, PWSTR* pszInfotip) override
    {
        *pszInfotip = nullptr;
        return S_OK;
    }

    /// <summary>
    /// 获取命令规范名称（默认返回GUID_NULL，使用GetTitle文本）。
    /// </summary>
    STDMETHODIMP GetCanonicalName(GUID* guidCommandName) override
    {
        *guidCommandName = GUID_NULL;
        return S_OK;
    }

    /// <summary>
    /// 获取命令状态（可见性/可用性）。
    /// 内部使用 GetBestLocationFromSelectionOrSite 获取目标项，
    /// 然后调用派生类的 GetCommandState 实现。
    /// </summary>
    STDMETHODIMP GetState(IShellItemArray* psiItemArray, BOOL /*fOkToBeSlow*/, EXPCMDSTATE* pCmdState) override
    {
        Microsoft::WRL::ComPtr<IShellItem> psi;
        HRESULT hr = GetBestLocationFromSelectionOrSite(psiItemArray, psi.GetAddressOf());
        if (FAILED(hr) || !psi)
        {
            *pCmdState = ECS_HIDDEN;
            return S_OK;
        }

        *pCmdState = GetCommandState(psi.Get());
        return S_OK;
    }

    /// <summary>
    /// 执行命令。
    /// 内部使用 GetBestLocationFromSelectionOrSite 获取目标项，
    /// 然后调用派生类的 DoInvoke 实现。
    /// </summary>
    STDMETHODIMP Invoke(IShellItemArray* psiItemArray, IBindCtx* /*pBindContext*/) override
    {
        Microsoft::WRL::ComPtr<IShellItem> psi;
        HRESULT hr = GetBestLocationFromSelectionOrSite(psiItemArray, psi.GetAddressOf());
        if (FAILED(hr) || !psi)
            return S_FALSE;

        return DoInvoke(psi.Get());
    }


    // ========================================================================
    // 纯虚方法 - 派生类必须实现
    // ========================================================================

    /// <summary>
    /// 执行命令操作（派生类必须实现）。
    /// </summary>
    /// <param name="psi">
    /// 目标Shell项。文件/文件夹右键时为选中项；
    /// 空白处右键时为父文件夹。保证非空。
    /// </param>
    /// <returns>HRESULT</returns>
    virtual STDMETHODIMP DoInvoke(IShellItem* psi) = 0;

    /// <summary>
    /// 获取命令状态（派生类必须实现）。
    /// </summary>
    /// <param name="psi">目标Shell项</param>
    /// <returns>
    /// ECS_ENABLED - 菜单可用
    /// ECS_DISABLED - 菜单灰色
    /// ECS_HIDDEN - 菜单隐藏
    /// </returns>
    virtual STDMETHODIMP_(EXPCMDSTATE) GetCommandState(IShellItem* psi) = 0;

    /// <summary>
    /// 获取菜单标题（派生类必须实现）。
    /// </summary>
    /// <param name="psiItemArray">选中项数组（可能为nullptr）</param>
    /// <param name="pszName">接收标题字符串（调用方CoTaskMemFree）</param>
    /// <returns>HRESULT</returns>
    virtual STDMETHODIMP GetTitleImpl(IShellItemArray* psiItemArray, PWSTR* pszName) = 0;

    /// <summary>
    /// 获取菜单图标（派生类必须实现）。
    /// </summary>
    /// <param name="psiItemArray">选中项数组（可能为nullptr）</param>
    /// <param name="pszIcon">接收图标路径（调用方CoTaskMemFree）</param>
    /// <returns>HRESULT</returns>
    virtual STDMETHODIMP GetIconImpl(IShellItemArray* psiItemArray, PWSTR* pszIcon) = 0;

    // ========================================================================
    // 受保护辅助方法（派生类可使用）
    // ========================================================================

protected:
    virtual ~CShellExtensionBase() = default;

    /// <summary>
    /// 通过Site链获取当前文件夹的IShellItem。
    /// </summary>
    /// <param name="location">接收文件夹IShellItem</param>
    /// <returns>S_OK成功, S_FALSE站点不可用, HRESULT错误</returns>
    /// <remarks>
    /// 实现路径：site_ -> IServiceProvider -> QueryService(SID_SFolderView)
    /// -> IFolderView -> GetFolder -> IShellItem
    /// </remarks>
    HRESULT GetLocationFromSite(IShellItem** location) const noexcept
    {
        if (!m_site)
            return S_FALSE;

        Microsoft::WRL::ComPtr<IServiceProvider> serviceProvider;
        HRESULT hr = m_site->QueryInterface(IID_PPV_ARGS(serviceProvider.GetAddressOf()));
        if (FAILED(hr))
            return hr;

        Microsoft::WRL::ComPtr<IFolderView> folderView;
        hr = serviceProvider->QueryService(
            SID_SFolderView, IID_PPV_ARGS(folderView.GetAddressOf()));
        if (FAILED(hr))
            return hr;

        return folderView->GetFolder(IID_PPV_ARGS(location));
    }

    /// <summary>
    /// 双通道获取目标路径：优先psiItemArray，回退到Site。
    /// </summary>
    /// <param name="psiArray">IExplorerCommand传入的选中项数组</param>
    /// <param name="location">接收目标IShellItem</param>
    /// <returns>S_OK成功, S_FALSE无有效项, HRESULT错误</returns>
    /// <remarks>
    /// 这是解决Background右键场景的核心方法：
    /// 通道1：从psiItemArray取选中项（正常文件/文件夹右键）
    /// 通道2：psiItemArray为空时，通过SetSite缓存的站点查询当前文件夹
    ///        （Background右键场景）
    /// </remarks>
    HRESULT GetBestLocationFromSelectionOrSite(
        IShellItemArray* psiArray, IShellItem** location) const noexcept
    {
        Microsoft::WRL::ComPtr<IShellItem> psi;

        // 通道1：从psiItemArray取选中项
        if (psiArray)
        {
            DWORD count{};
            if (SUCCEEDED(psiArray->GetCount(&count)) && count > 0)
            {
                psiArray->GetItemAt(0, psi.GetAddressOf());
            }
        }

        // 通道2：psiItemArray为空或无效时，回退到Site
        if (!psi)
        {
            if (FAILED(GetLocationFromSite(psi.GetAddressOf())))
            {
                return S_FALSE;
            }
        }

        if (!psi)
            return S_FALSE;

        return psi.CopyTo(location);
    }

    /// <summary>
    /// 从IShellItem获取文件系统路径（含回退策略）。
    /// </summary>
    /// <param name="psi">Shell项</param>
    /// <param name="pszPath">接收路径字符串（调用方CoTaskMemFree）</param>
    /// <returns>HRESULT</returns>
    /// <remarks>
    /// 回退策略：
    /// 1. SIGDN_FILESYSPATH - 文件系统路径（如 C:\Users\...）
    /// 2. SIGDN_DESKTOPABSOLUTEPARSING - 桌面绝对路径
    /// 3. SIGDN_NORMALDISPLAY - 常规显示名
    /// 注意：虚拟路径（OneDrive占位符、网络路径等）可能无法转换
    /// </remarks>
    HRESULT GetFilePathFromShellItem(IShellItem* psi, PWSTR* pszPath) const noexcept
    {
        if (!psi) return E_INVALIDARG;
        if (!pszPath) return E_POINTER;

        *pszPath = nullptr;

        PWSTR pszName = nullptr;

        // 优先获取文件系统路径
        HRESULT hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &pszName);
        if (SUCCEEDED(hr))
        {
            *pszPath = pszName;
            return S_OK;
        }

        // 回退：桌面绝对路径
        hr = psi->GetDisplayName(SIGDN_DESKTOPABSOLUTEPARSING, &pszName);
        if (SUCCEEDED(hr))
        {
            *pszPath = pszName;
            return S_OK;
        }

        // 最后回退：常规显示名
        hr = psi->GetDisplayName(SIGDN_NORMALDISPLAY, &pszName);
        if (SUCCEEDED(hr))
        {
            *pszPath = pszName;
            return S_OK;
        }

        return E_FAIL;
    }

    /// <summary>
    /// 检查Shell项是否为有效的文件系统对象。
    /// </summary>
    bool IsFileSystemItem(IShellItem* psi) const noexcept
    {
        if (!psi) return false;

        SFGAOF attrs = 0;
        if (FAILED(psi->GetAttributes(SFGAO_FILESYSTEM, &attrs)))
            return false;

        return (attrs & SFGAO_FILESYSTEM) != 0;
    }

    /// <summary>
    /// 检查Shell项是否为文件夹。
    /// </summary>
    bool IsFolder(IShellItem* psi) const noexcept
    {
        if (!psi) return false;

        SFGAOF attrs = 0;
        if (FAILED(psi->GetAttributes(SFGAO_FOLDER, &attrs)))
            return false;

        return (attrs & SFGAO_FOLDER) != 0;
    }

    /// <summary>
    /// 检查Shell项是否为压缩文件夹（.zip/.7z等）。
    /// </summary>
    bool IsCompressedFolder(IShellItem* psi) const noexcept
    {
        if (!psi) return false;

        SFGAOF attrs = 0;
        if (FAILED(psi->GetAttributes(SFGAO_FOLDER | SFGAO_STREAM, &attrs)))
            return false;

        // SFGAO_STREAM 为真表示是流对象（压缩文件夹内部条目）
        return (attrs & SFGAO_STREAM) != 0;
    }

private:
    Microsoft::WRL::ComPtr<IUnknown>  m_site;
};
