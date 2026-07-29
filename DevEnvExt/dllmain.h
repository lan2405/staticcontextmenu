// dllmain.h: 模块类的声明。

class CDevEnvExtModule : public ATL::CAtlDllModuleT< CDevEnvExtModule >
{
public :
	DECLARE_LIBID(LIBID_DevEnvExtLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_DEVENVEXT, "{2ca39d14-34da-42e7-a60d-bddfb6546117}")
};

extern class CDevEnvExtModule _AtlModule;
