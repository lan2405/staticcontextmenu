// IExCmdSubMenus.cpp: CIExCmdSubMenus 的实现

#include "pch.h"
#include "IExCmdSubMenus.h"




// CIExCmdSubMenus

IFACEMETHODIMP CIExCmdSubMenus::Invoke(IShellItemArray* psiItemArray, IBindCtx* pbc)
{
	// ========================================================================
		// 一、参数校验
		// ========================================================================
	if (!psiItemArray)
		return E_INVALIDARG;

	// ========================================================================
	// 二、从 psiItemArray 获取选中项数量
	// ========================================================================
	DWORD dwCount = 0;
	HRESULT hr = psiItemArray->GetCount(&dwCount);
	if (FAILED(hr) || dwCount == 0)
		return S_OK; // 没有选中项，静默返回
	// ========================================================================
	// 三、方式一：通过 GetItemAt 按索引遍历（推荐，简单直观）
	// ========================================================================
	std::vector<std::wstring> filePaths;
	filePaths.reserve(dwCount);

	for (DWORD i = 0; i < dwCount; ++i)
	{
		CComPtr<IShellItem> psi;
		hr = psiItemArray->GetItemAt(i, &psi);
		if (FAILED(hr) || !psi)
			continue;

		// ------ 3.1 获取文件系统完整路径（最常用） ------
		PWSTR pszPath = nullptr;
		hr = psi->GetDisplayName(SIGDN_FILESYSPATH, &pszPath);
		if (SUCCEEDED(hr) && pszPath)
		{
			filePaths.emplace_back(pszPath);
			CoTaskMemFree(pszPath); // 必须释放！
		}
		else
		{
			// 非文件系统项（如虚拟文件夹），回退获取显示名称
			PWSTR pszName = nullptr;
			hr = psi->GetDisplayName(SIGDN_NORMALDISPLAY, &pszName);
			if (SUCCEEDED(hr) && pszName)
			{
				filePaths.emplace_back(pszName);
				CoTaskMemFree(pszName);
			}
		}

		// ------ 3.2 获取项的属性标志（判断是文件还是文件夹） ------
		SFGAOF sfgaoAttrs = 0;
		hr = psi->GetAttributes(
			SFGAO_FOLDER | SFGAO_FILESYSTEM | SFGAO_LINK,
			&sfgaoAttrs);
		if (SUCCEEDED(hr))
		{
			bool bIsFolder = (sfgaoAttrs & SFGAO_FOLDER) != 0;
			bool bIsFileSystem = (sfgaoAttrs & SFGAO_FILESYSTEM) != 0;
			bool bIsLink = (sfgaoAttrs & SFGAO_LINK) != 0;
			// 可根据 bIsFolder / bIsFileSystem / bIsLink 做分支处理
		}
	}
	SHELLEXECUTEINFO RegCmdInfo;
	std::wstring cmd;
	for (const auto& path : filePaths)
	{
		cmd = (m_size_ == CMD_REG)
			? L"\"" + path + L"\""       // 注册：仅传递DLL路径
			: L"/u \"" + path + L"\"";   // 反注册：添加 /u 参数
		RegCmdInfo = {
	sizeof(SHELLEXECUTEINFO),
	SEE_MASK_DEFAULT,
	nullptr,
	_T("runas"),
	_T("regsvr32.exe"),
	cmd.c_str(),
	NULL,
	SW_SHOW,
	nullptr,
	nullptr,
	nullptr,
	nullptr
		};
		BOOL hInst = ShellExecuteEx(&RegCmdInfo);
		if (!hInst)
		{
			return E_FAIL;
		}
	}

	return S_OK;

}


