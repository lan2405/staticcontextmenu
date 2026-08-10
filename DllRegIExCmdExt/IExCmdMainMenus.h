// IExCmdMainMenus.h: CIExCmdMainMenus 的声明

#pragma once
#include <string>
#include <vector>
#include "resource.h"       // 主符号


#include "DllRegIExCmdExt_i.h"
#include "IExCmdSubMenus.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;

// CIExCmdMainMenus

class ATL_NO_VTABLE CIExCmdMainMenus :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CIExCmdMainMenus, &CLSID_IExCmdMainMenus>,
	public IDispatchImpl<IIExCmdMainMenus, &IID_IIExCmdMainMenus, &LIBID_DllRegIExCmdExtLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IExplorerCommand,
	public IEnumExplorerCommand
{
public:
	DECLARE_REGISTRY_RESOURCEID(106)
	BEGIN_COM_MAP(CIExCmdMainMenus)
		COM_INTERFACE_ENTRY(IIExCmdMainMenus)
		COM_INTERFACE_ENTRY(IDispatch)
		COM_INTERFACE_ENTRY(IExplorerCommand)
		COM_INTERFACE_ENTRY(IEnumExplorerCommand)
	END_COM_MAP()
	DECLARE_PROTECT_FINAL_CONSTRUCT()
	HRESULT FinalConstruct()
	{
		m_subCommands.clear();
		for (int i = 1; i < CMD_END; i++)
		{

			CComObject<CIExCmdSubMenus>* pRaw = nullptr;
			HRESULT hr = CComObject<CIExCmdSubMenus>::CreateInstance(&pRaw);
			if (FAILED(hr))
				continue;

			pRaw->set_m_size(i);
			// CComPtr 构造函数会 AddRef，refcount: 0 → 1
			CComPtr<CIExCmdSubMenus> pSubCmd(pRaw);
			m_subCommands.push_back(pSubCmd);
		}
		return S_OK;
	}

	void FinalRelease()
	{
		m_subCommands.clear();
	}

private:
	size_t m_currentIndex = 0;
	std::vector<CComPtr<CIExCmdSubMenus>> m_subCommands;
public:
	// Init


	// IExplorerCommand
	IFACEMETHOD(EnumSubCommands)(IEnumExplorerCommand** ppEnum) override
	{
		if (!ppEnum)
			return E_POINTER;

		m_currentIndex = 0;   // 重置枚举位置
		this->AddRef();        // 返回前增加引用计数
		*ppEnum = static_cast<IEnumExplorerCommand*>(this);
		return S_OK;
	}
	IFACEMETHOD(GetCanonicalName)(GUID* pguidCommandName)override { *pguidCommandName = GetObjectCLSID(); return S_OK; }
	IFACEMETHOD(GetFlags)(EXPCMDFLAGS* pFlags)override { *pFlags = ECF_HASSUBCOMMANDS; return S_OK; }
	IFACEMETHOD(GetIcon)(IShellItemArray* psiItemArray, LPWSTR* ppszIcon)override { return SHStrDup(_T("Resource.dll,201"), ppszIcon); }
	IFACEMETHOD(GetState)(IShellItemArray* psiItemArray, BOOL fOkToBeSlow, EXPCMDSTATE* pCmdState)override { *pCmdState = ECS_ENABLED; return S_OK; }
	IFACEMETHOD(GetTitle)(IShellItemArray* psiItemArray, LPWSTR* ppszName) override { std::wstring titlename = _T("IExCmd主注册"); return SHStrDup(titlename.c_str(), ppszName); }
	IFACEMETHOD(GetToolTip)(IShellItemArray* psiItemArray, LPWSTR* ppszInfotip)override { std::wstring tooltip = _T("IExplorerCommand注册主程序"); return SHStrDup(tooltip.c_str(), ppszInfotip); }
	IFACEMETHOD(Invoke)(IShellItemArray* psiItemArray, IBindCtx* pbc) override { return E_NOTIMPL; }
	// IEnumExplorerCommand
	IFACEMETHOD(Clone)(IEnumExplorerCommand** ppenum) override
	{
		*ppenum = NULL;
		CComObject<CIExCmdMainMenus>* pNew;
		HRESULT hr = CComObject<CIExCmdMainMenus>::CreateInstance(&pNew);
		if (SUCCEEDED(hr)) {
			pNew->AddRef();
			pNew->m_currentIndex = m_currentIndex;
			pNew->m_subCommands = m_subCommands;  // 共享子命令
			*ppenum = static_cast<IEnumExplorerCommand*>(pNew);
		}
		return hr;
	}
	IFACEMETHOD(Next)(ULONG celt, IExplorerCommand** pUICommand, ULONG* pceltFetched) override
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
	IFACEMETHOD(Reset)() override { m_currentIndex = 0; return S_OK; }
	IFACEMETHOD(Skip)(ULONG celt) override { return E_NOTIMPL; }
	// Private



};


OBJECT_ENTRY_AUTO(__uuidof(IExCmdMainMenus), CIExCmdMainMenus)
