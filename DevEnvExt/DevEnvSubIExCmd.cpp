// DevEnvSubIExCmd.cpp: CDevEnvSubIExCmd 的实现

#include "pch.h"
#include "DevEnvSubIExCmd.h"



// CDevEnvSubIExCmd
HRESULT CDevEnvSubIExCmd::GetLocationFromSite(IShellItem** location) const noexcept
{
	wil::assign_null_to_opt_param(location);

	if (!site_)
	{
		return S_FALSE;
	}

	wil::com_ptr_nothrow<IServiceProvider> serviceProvider;
	RETURN_IF_FAILED(site_.query_to(serviceProvider.put()));
	wil::com_ptr_nothrow<IFolderView> folderView;
	RETURN_IF_FAILED(serviceProvider->QueryService(SID_SFolderView, IID_PPV_ARGS(folderView.put())));
	RETURN_IF_FAILED(folderView->GetFolder(IID_PPV_ARGS(location)));
	return S_OK;
}

HRESULT CDevEnvSubIExCmd::GetBestLocationFromSelectionOrSite(IShellItemArray* psiArray,
	IShellItem** location) const noexcept
{
	wil::com_ptr_nothrow<IShellItem> psi;
	if (psiArray)
	{
		DWORD count{};
		RETURN_IF_FAILED(psiArray->GetCount(&count));
		if (count) // Sometimes we get an array with a count of 0. Fall back to the site chain.
		{
			RETURN_IF_FAILED(psiArray->GetItemAt(0, psi.put()));
		}
	}

	if (!psi)
	{
		RETURN_IF_FAILED(GetLocationFromSite(psi.put()));
	}

	RETURN_HR_IF(S_FALSE, !psi);
	RETURN_IF_FAILED(psi.copy_to(location));
	return S_OK;
}


IFACEMETHODIMP CDevEnvSubIExCmd::Invoke (IShellItemArray* psiItemArray, IBindCtx* pbc)
{
	//const auto runElevated = IsControlAndShiftPressed();

	wil::com_ptr_nothrow<IShellItem> psi;
	GetBestLocationFromSelectionOrSite(psiItemArray, psi.put());
	if (!psi)
	{
		return S_FALSE;
	}

	wil::unique_cotaskmem_string pszName;
	RETURN_IF_FAILED(psi->GetDisplayName(SIGDN_FILESYSPATH, &pszName));

	SHELLEXECUTEINFO Commandinfo = CmdInfo[m_size_];
	std::wstring parm;
	std::wstring path = pszName.get();
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


