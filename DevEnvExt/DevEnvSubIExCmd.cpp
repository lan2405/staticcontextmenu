// DevEnvSubIExCmd.cpp: CDevEnvSubIExCmd 的实现

#include "pch.h"
#include "DevEnvSubIExCmd.h"


HRESULT CDevEnvSubIExCmd::GetBestLocationFromSelectionOrSite(IShellItemArray* psiArray,
	IShellItem** location) const noexcept
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

// CDevEnvSubIExCmd
HRESULT CDevEnvSubIExCmd::GetLocationFromSite(IShellItem** location) const noexcept
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

HRESULT CDevEnvSubIExCmd::GetFilePathFromShellItem(IShellItem* psi, PWSTR* pszPath) const noexcept
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

IFACEMETHODIMP CDevEnvSubIExCmd::DoInvoke(IShellItem* psi)
{
	// 获取文件系统路径
	PWSTR pszPath = nullptr;
	HRESULT hr = GetFilePathFromShellItem(psi, &pszPath);
}


IFACEMETHODIMP CDevEnvSubIExCmd::Invoke(IShellItemArray* psiItemArray, IBindCtx* pbc)
{
	// ========================================================================
	// 一、从 psiItemArray 获取路径（兼容旧版菜单 NULL/空的情况）
	// ========================================================================
	std::vector<std::wstring> filePaths;

	if (psiItemArray)
	{
		DWORD dwCount = 0;
		HRESULT hr = psiItemArray->GetCount(&dwCount);
		if (SUCCEEDED(hr) && dwCount > 0)
		{
			filePaths.reserve(dwCount);
			for (DWORD i = 0; i < dwCount; ++i)
			{
				CComPtr<IShellItem> psi;
				hr = psiItemArray->GetItemAt(i, &psi);
				if (FAILED(hr) || !psi)
					continue;

				PWSTR pszPath = nullptr;
				hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &pszPath);
				if (SUCCEEDED(hr) && pszPath)
				{
					filePaths.emplace_back(pszPath);
					CoTaskMemFree(pszPath);
				}
				else
				{
					PWSTR pszName = nullptr;
					hr = psi->GetDisplayName(SIGDN_NORMALDISPLAY, &pszName);
					if (SUCCEEDED(hr) && pszName)
					{
						filePaths.emplace_back(pszName);
						CoTaskMemFree(pszName);
					}
				}
			}
		}
	}

	// 回退：旧版菜单在某些场景下 psiItemArray 为 NULL 或空（如目录背景右键）
	// 此时使用当前工作目录作为操作路径
	if (filePaths.empty())
	{
		wchar_t szDir[MAX_PATH] = {};
		if (GetCurrentDirectoryW(MAX_PATH, szDir) > 0)
		{
			filePaths.emplace_back(szDir);
		}
		else
		{
			return E_FAIL;
		}
	}




	SHELLEXECUTEINFO Commandinfo = CmdInfo[m_size_];
	std::wstring parm;
	for (const auto& path : filePaths)
	{
		switch (m_size_)
		{
		case CMD_VisualStudio:
			parm = L"\""+path + L"\" source:ExplorerBackground";
			Commandinfo.lpParameters = parm.c_str();
			break;
		case CMD_VSCode:
			Commandinfo.lpParameters = path.c_str();
			break;
		case CMD_WindowsTerminal:
			Commandinfo.lpParameters = path.c_str();
			break;
		case CMD_PowerShell_Admin:
			parm = L"-NoExit -WorkingDirectory \"" + path + L"\"";
			Commandinfo.lpParameters = parm.c_str();
			break;
		case CMD_PowerShell_System:
			parm = L"-w \"" + path + L"\" -s powershell -w Normal ";
			Commandinfo.lpParameters = parm.c_str();
			break;
		default:
			break;
		}
		Commandinfo.lpDirectory = path.c_str();
		if (!ShellExecuteEx(&Commandinfo))
		{
			return E_FAIL;
		}
	}
	return S_OK;
}

IFACEMETHODIMP CDevEnvSubIExCmd::GetSite(REFIID riid, void** ppvSite)
{
	return m_site->QueryInterface(riid, ppvSite);
}

IFACEMETHODIMP CDevEnvSubIExCmd::SetSite(IUnknown* pUnkSite)
{
	m_site = pUnkSite;
	return S_OK;
}


