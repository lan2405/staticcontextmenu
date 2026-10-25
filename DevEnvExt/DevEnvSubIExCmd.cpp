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
			for (DWORD i = 0; i < count; ++i)
			{
				Microsoft::WRL::ComPtr<IShellItem> item;
				if (SUCCEEDED(psiArray->GetItemAt(i, item.GetAddressOf())))
				{
					DoInvoke(item.Get());
				}
			}

			
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

	SHELLEXECUTEINFO Commandinfo = CmdInfo[m_size_];
	std::wstring parm;
	std::wstring path = pszPath;
	switch (m_size_)
	{
	case CMD_VisualStudio:
		parm = L"\"" + path + L"\" source:ExplorerBackground";
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
	return S_OK;
}


IFACEMETHODIMP CDevEnvSubIExCmd::Invoke(IShellItemArray* psiItemArray, IBindCtx* pbc)
{
	Microsoft::WRL::ComPtr<IShellItem> psi;
	HRESULT hr = GetBestLocationFromSelectionOrSite(psiItemArray, psi.GetAddressOf());
	if (FAILED(hr) || !psi)
		return S_FALSE;
	// return DoInvoke(psi.Get());
	return S_OK;
}



