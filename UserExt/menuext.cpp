// menuext.cpp: Cmenuext 的实现

#include "pch.h"
#include "menuext.h"


// Cmenuext

HRESULT Cmenuext::Initialize(PCIDLIST_ABSOLUTE pidlFolder, IDataObject* pdtobj, HKEY hkeyProgID)
{
    return E_NOTIMPL;
}

HRESULT Cmenuext::QueryContextMenu( HMENU hmenu, UINT indexMenu, UINT idCmdFirst, UINT idCmdLast, UINT uFlags)
{
    return E_NOTIMPL;
}

HRESULT Cmenuext::InvokeCommand(LPCMINVOKECOMMANDINFO pici)
{
    return E_NOTIMPL;
}

HRESULT Cmenuext::GetCommandString(UINT_PTR idcmd, UINT uflags, UINT* pwReserved, LPSTR pszName, UINT cchMax)
{
    return E_NOTIMPL;
}
