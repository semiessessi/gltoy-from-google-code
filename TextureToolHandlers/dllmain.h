// dllmain.h : Declaration of module class.

class CTextureToolHandlersModule : public ATL::CAtlDllModuleT< CTextureToolHandlersModule >
{
public :
	DECLARE_LIBID(LIBID_TextureToolHandlersLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_TEXTURETOOLHANDLERS, "{F7E01B8E-18D1-444F-96E8-4234585FC1B7}")
};

extern class CTextureToolHandlersModule _AtlModule;
