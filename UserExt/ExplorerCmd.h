// ExplorerCmd.h: CExplorerCmd 的声明

#pragma once
#include "constinfo.h"
#include "resource.h"       // 主符号
#include "UserExt_i.h"
#include <ShlObj_core.h>
#include <string>
#include <vector>

#include "SubIExCmdRoot.h"


#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;



// CExplorerCmd

class ATL_NO_VTABLE CExplorerCmd :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CExplorerCmd, &CLSID_ExplorerCmd>,
	public IDispatchImpl<IExplorerCmd, &IID_IExplorerCmd, &LIBID_UserExtLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IExplorerCommand,
	public IEnumExplorerCommand
{
private:
	size_t m_currentIndex = 0;
	std::vector<CComPtr<CSubIExCmdRoot>> m_subCommands;
public:

	
	DECLARE_REGISTRY_RESOURCEID(110)


	BEGIN_COM_MAP(CExplorerCmd)
		COM_INTERFACE_ENTRY(IExplorerCmd)
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
			
			CComObject<CSubIExCmdRoot>* pRaw = nullptr;
			HRESULT hr = CComObject<CSubIExCmdRoot>::CreateInstance(&pRaw);
			if (FAILED(hr))
				continue;

			pRaw->set_m_size(i);
			// CComPtr 构造函数会 AddRef，refcount: 0 → 1
			CComPtr<CSubIExCmdRoot> pSubCmd(pRaw);
			m_subCommands.push_back(pSubCmd);
		}
		return S_OK;
	}

	void FinalRelease()
	{
		m_subCommands.clear();
	}

public:
	// IExplorerCommand
	STDMETHOD(GetCanonicalName)(GUID* pguidCommandName) { *pguidCommandName = GetObjectCLSID(); return S_OK; }
	STDMETHOD(GetFlags)(EXPCMDFLAGS* pFlags) { *pFlags = ECF_HASSUBCOMMANDS; return S_OK; }
	STDMETHOD(GetIcon)(IShellItemArray* psiItemArray, LPWSTR* ppszIcon) { return SHStrDup(_T("Resource.dll,-201"), ppszIcon); }
	STDMETHOD(GetState)(IShellItemArray* psiItemArray, BOOL fOkToBeSlow, EXPCMDSTATE* pCmdState) { *pCmdState = ECS_ENABLED; return S_OK; }
	STDMETHOD(GetTitle)(IShellItemArray* psiItemArray, LPWSTR* ppszName) { std::wstring titlename = _T("UserExt"); return SHStrDup(titlename.c_str(), ppszName); }
	STDMETHOD(GetToolTip)(IShellItemArray* psiItemArray, LPWSTR* ppszInfotip) { std::wstring tooltipinfo = _T("ToolTip");  return SHStrDup(tooltipinfo.c_str(), ppszInfotip); }
	STDMETHOD(Invoke)(IShellItemArray* psiItemArray, IBindCtx* pbc) override;
	STDMETHODIMP EnumSubCommands(IEnumExplorerCommand** ppEnum) override;
	// IEnumExplorerCommand
	STDMETHOD(Clone)(IEnumExplorerCommand** ppenum) override;
	STDMETHOD(Next)(ULONG celt, IExplorerCommand** pUICommand, ULONG* pceltFetched) override;
	STDMETHOD(Reset)() override { m_currentIndex = 0; return S_OK; }
	STDMETHOD(Skip)(ULONG celt) override { return E_NOTIMPL; }

};




OBJECT_ENTRY_AUTO(__uuidof(ExplorerCmd), CExplorerCmd)

