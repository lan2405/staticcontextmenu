// dllmain.h: 模块类的声明。

class CUserExtModule : public ATL::CAtlDllModuleT< CUserExtModule >
{
public :
	DECLARE_LIBID(LIBID_UserExtLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_USEREXT, "{cba63cca-446c-4ded-a68c-3be1e7fdec41}")
};

extern class CUserExtModule _AtlModule;
