// dllmain.h: 模块类的声明。

class CDllRegIExCmdExtModule : public ATL::CAtlDllModuleT< CDllRegIExCmdExtModule >
{
public :
	DECLARE_LIBID(LIBID_DllRegIExCmdExtLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DLLREGIEXCMDEXT, "{0d83b8e4-9ab5-4324-ab19-2ca77465bb46}")
};

extern class CDllRegIExCmdExtModule _AtlModule;
