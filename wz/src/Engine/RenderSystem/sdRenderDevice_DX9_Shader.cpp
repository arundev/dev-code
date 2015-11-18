#include "sdRenderSystemPCH.h"
#include "sdRenderDevice_DX9.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
	bool sdRenderDevice_DX9::FXLibraryClassCreate(const char* pcLibFile, NiRenderer* spRender, int iDirectoryCount, const char* apcDirecties[],	bool bRecurseSubFolders, NiShaderLibrary** sppLibrary)
{
	(*sppLibrary) = NULL;

#ifndef _USRDLL
	return NiD3DXEffectShaderLib_LoadShaderLibrary((NiD3DRenderer*)spRender, iDirectoryCount, apcDirecties, bRecurseSubFolders);
#else
	NiD3DXEffectShaderLibrary* spLibrary = NiD3DXEffectShaderLibrary::Create((NiD3DRenderer*)spRender, iDirectoryCount, apcDirecties, bRecurseSubFolders);
	*sppLibrary = spLibrary;
	return true;
#endif
}
//-------------------------------------------------------------------------------------------------
bool sdRenderDevice_DX9::SetupShaderSystem(const char* apcProgramDirectoies[], uint uiNumProgramDirectoies,	const char* apcShaderDirectoies[], uint uiNumShaderDirectoies)
{
	NiD3DShaderProgramFactory* spShaderProgramFactory = NiD3DShaderProgramFactory::GetInstance();
	NIASSERT(spShaderProgramFactory);

	// 设置编译变量,用于Shader调试
#ifdef DEBUG_SHADER
	NiD3DShaderProgramFactory::SetUniversalShaderCreationFlags(
		D3DXSHADER_DEBUG | D3DXSHADER_FORCE_VS_SOFTWARE_NOOPT | D3DXSHADER_FORCE_PS_SOFTWARE_NOOPT);
#endif

	// 保存程序目录
	for (uint i = 0; i < uiNumProgramDirectoies; ++i)
	{
		spShaderProgramFactory->AddProgramDirectory(apcProgramDirectoies[i]);
	}

	// 初始化着色器库
	NiShaderFactory::RegisterClassCreationCallback(FXLibraryClassCreate);
	if (!NiShaderFactory::LoadAndRegisterShaderLibrary(NULL, uiNumShaderDirectoies, apcShaderDirectoies, true))
	{
		NIASSERT(0);
		NiOutputDebugString("fx shader library failed to load any shaders!");
		return false;
	}

	return true;
}
//-------------------------------------------------------------------------------------------------
void sdRenderDevice_DX9::SetGlobalShaderConstant(const NiFixedString& szName, uint uiSize, const void* pData)
{
	NiD3DShaderFactory::UpdateGlobalShaderConstant(szName, uiSize, pData);
}
//-------------------------------------------------------------------------------------------------
void sdRenderDevice_DX9::GetGlobalShaderConstant(const NiFixedString& szName, uint& uiSize, const void*& pData)
{
	NiD3DShaderFactory::RetrieveGlobalShaderConstant(szName, uiSize, pData);
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE