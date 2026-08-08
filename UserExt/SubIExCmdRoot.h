// SubIExCmdRoot.h: CSubIExCmdRoot 的声明

#pragma once
#include <string>
#include "constinfo.h"
#include "resource.h"       // 主符号



#include "UserExt_i.h"



#if defined(_WIN32_WCE) && !defined(_CE_DCOM) && !defined(_CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA)
#error "Windows CE 平台(如不提供完全 DCOM 支持的 Windows Mobile 平台)上无法正确支持单线程 COM 对象。定义 _CE_ALLOW_SINGLE_THREADED_OBJECTS_IN_MTA 可强制 ATL 支持创建单线程 COM 对象实现并允许使用其单线程 COM 对象实现。rgs 文件中的线程模型已被设置为“Free”，原因是该模型是非 DCOM Windows CE 平台支持的唯一线程模型。"
#endif

using namespace ATL;


// CSubIExCmdRoot

class ATL_NO_VTABLE CSubIExCmdRoot :
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CSubIExCmdRoot, &CLSID_SubIExCmdRoot>,
	public IDispatchImpl<ISubIExCmdRoot, &IID_ISubIExCmdRoot, &LIBID_UserExtLib, /*wMajor =*/ 1, /*wMinor =*/ 0>,
	public IExplorerCommand
{
public:
	CSubIExCmdRoot()
	{
	}

DECLARE_REGISTRY_RESOURCEID(113)


BEGIN_COM_MAP(CSubIExCmdRoot)
	COM_INTERFACE_ENTRY(ISubIExCmdRoot)
	COM_INTERFACE_ENTRY(IDispatch)
	COM_INTERFACE_ENTRY(IExplorerCommand)
END_COM_MAP()



	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}

	void FinalRelease()
	{
	}


private:
	size_t m_size;

public:
	void set_m_size(size_t m_size)
	{

		this->m_size = m_size;
	}

	STDMETHOD(EnumSubCommands)(IEnumExplorerCommand** ppEnum) override { *ppEnum = nullptr; return E_NOTIMPL; }
	STDMETHOD(GetCanonicalName)(GUID* pguidCommandName) { *pguidCommandName = subIExCmdRegistry[m_size].guid; return S_OK; }
	STDMETHOD(GetFlags)(EXPCMDFLAGS* pFlags) { *pFlags = subIExCmdRegistry[m_size].flags; return S_OK; }
	STDMETHOD(GetIcon)(IShellItemArray* psiItemArray, LPWSTR* ppszIcon) { std::wstring Iconpath = subIExCmdRegistry[m_size].icon; return SHStrDup(Iconpath.c_str(), ppszIcon); }
	STDMETHOD(GetState)(IShellItemArray* psiItemArray, BOOL fOkToBeSlow, EXPCMDSTATE* pCmdState) { *pCmdState = subIExCmdRegistry[m_size].state; return S_OK; }
	STDMETHOD(GetTitle)(IShellItemArray* psiItemArray, LPWSTR* ppszName) { std::wstring titlename = subIExCmdRegistry[m_size].title; return SHStrDup(titlename.c_str(), ppszName); }
	STDMETHOD(GetToolTip)(IShellItemArray* psiItemArray, LPWSTR* ppszInfotip) { std::wstring tooltip = subIExCmdRegistry[m_size].tooltip; return SHStrDup(tooltip.c_str(), ppszInfotip); }
	STDMETHOD(Invoke)(IShellItemArray* psiItemArray, IBindCtx* pbc) override { return E_NOTIMPL; }


};

OBJECT_ENTRY_AUTO(__uuidof(SubIExCmdRoot), CSubIExCmdRoot)
