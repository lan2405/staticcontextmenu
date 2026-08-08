// ExplorerCmd.cpp: CExplorerCmd 的实现

#include "pch.h"
#include "ExplorerCmd.h"


// CExplorerCmd


STDMETHODIMP_(HRESULT __stdcall) CExplorerCmd::Invoke(IShellItemArray* psiItemArray, IBindCtx* pbc)
{
    return E_NOTIMPL;
}

STDMETHODIMP_(HRESULT __stdcall) CExplorerCmd::EnumSubCommands(IEnumExplorerCommand** ppEnum)
{
    if (!ppEnum)
        return E_POINTER;

    m_currentIndex = 0;   // 重置枚举位置
    this->AddRef();        // 返回前增加引用计数
    *ppEnum = static_cast<IEnumExplorerCommand*>(this);
    return S_OK;
}


STDMETHODIMP_(HRESULT __stdcall) CExplorerCmd::Clone(IEnumExplorerCommand** ppenum)
{
    // 简化实现：直接复用 Reset 后的当前对象（实际需深拷贝状态）
    *ppenum = NULL;
    CComObject<CExplorerCmd>* pNew;
    HRESULT hr = CComObject<CExplorerCmd>::CreateInstance(&pNew);
    if (SUCCEEDED(hr)) {
        pNew->AddRef();
        pNew->m_currentIndex = m_currentIndex;
        pNew->m_subCommands = m_subCommands;  // 共享子命令
        *ppenum = static_cast<IEnumExplorerCommand*>(pNew);
    }
    return hr;
}

STDMETHODIMP_(HRESULT __stdcall) CExplorerCmd::Next(ULONG celt, IExplorerCommand** pUICommand, ULONG* pceltFetched)
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



