// dllmain.h: 模块类的声明。

class CResourceModule : public ATL::CAtlDllModuleT< CResourceModule >
{
public :
	DECLARE_LIBID(LIBID_ResourceLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_RESOURCE, "{8688c9b3-8485-4610-b0d6-2aeb690c94ff}")
};

extern class CResourceModule _AtlModule;
