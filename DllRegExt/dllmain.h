// dllmain.h: 模块类的声明。

class CDllRegExtModule : public ATL::CAtlDllModuleT< CDllRegExtModule >
{
public :
	DECLARE_LIBID(LIBID_DllRegExtLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DLLREGEXT, "{3ba066fc-e493-451b-a26f-86b9841f1fe1}")
};

extern class CDllRegExtModule _AtlModule;
