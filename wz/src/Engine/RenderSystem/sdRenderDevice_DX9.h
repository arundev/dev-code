//*************************************************************************************************
// 内容:	渲染设备的GB-D3D9实现
//---------------------------------------------------------
// 作者:		
// 创建:		2012-06-30
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_RENDER_DEVICE_DX9_H__
#define _SD_ENGINE_RENDER_SYSTEM_RENDER_DEVICE_DX9_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
class sdRenderDevice_DX9: public IRenderDevice
{
public:
	sdRenderDevice_DX9();
	virtual ~sdRenderDevice_DX9();

	// 虚函数继承
	virtual bool Initialize();
	virtual void Destroy();
	virtual bool IsInitialized();

	virtual void SetRenderState(uint uiState, uint uiValue);
	virtual uint GetRenderState(uint uiState);

	virtual void SaveAllRenderStates();
	virtual void RestoreAllRenderStates();

	virtual void SetSampleState(uint uiSlot, uint uiState, uint uiValue);
	virtual uint GetSampleState(uint uiSlot, uint uiState);

	virtual void SaveAllSampleStates();
	virtual void RestoreAllSampleStates();

	virtual	void Clear(uint* puiColor, float* pfDepth, uint* puiStencil);
	virtual void ClearVertexBinding();
	virtual void ClearTextureBinding();

	virtual bool SetupShaderSystem(const char* apcProgramDirectoies[], uint uiNumProgramDirectoies,	const char* apcShaderDirectoies[], uint uiNumShaderDirectoies);
	virtual void SetGlobalShaderConstant(const NiFixedString& szName, uint uiSize, const void* pData);
	virtual void GetGlobalShaderConstant(const NiFixedString& szName, uint& uiSize, const void*& pData);

	virtual void SetCameraData(NiCamera* pkCamera);
	virtual const Base::Math::sdMatrix4& GetViewMatrix() const;
	virtual const Base::Math::sdMatrix4& GetProjMatrix() const;
	virtual const Base::Math::sdMatrix4& GetInvViewMatrix() const;

	virtual void SetRenderTargetGroup(NiRenderTargetGroup* pkRenderTarget);
	virtual NiRenderTargetGroup* GetRenderTargetGroup();
	virtual NiRenderTargetGroup* GetDefaultRenderTargetGroup();

	virtual NiMaterial* CreateMaterial(const NiFixedString& szName);

	virtual NiRenderedTexturePtr	CreateRenderTexture(TextureSpec& kSpec);
	virtual	NiRenderTargetGroupPtr	CreateRenderTarget(uint uiNum);
	//virtual NiDX9Direct3DTexturePtr	CreateRawTexture();

	//virtual NiSourceTexturePtr		LoadTexture(const char* szTexName);
	//virtual	bool					SaveTexture(const char* szFileName, NiTexture* pTexture);

	virtual uint AddLostEventDelegate(LostEventDelegate pfFunc, void* pData);
	virtual uint AddResetEventDelegate(ResetEventDelegate pfFunc, void* pData);
	virtual bool RemoveLostEventDelegate(uint uiIndex);
	virtual bool RemoveResetEventDelegate(uint uiIndex);
	virtual void RemoveAllEventDelegate();

protected:
	// fx着色器相关的回调函数
	// @{
	static bool FXLibraryClassCreate(const char* pcLibFile, NiRenderer* spRender,
		int iDirectoryCount, const char* apcDirecties[],
		bool bRecurseSubFolders, NiShaderLibrary** sppLibrary);
	//static bool FXLiteLibraryClassCreate(const char* pcLibFile, NiRenderer* spRender,
	//	int iDirectoryCount const char* apcDirecties[],
	//	bool bRecurseSubFolders, NiShaderLibrary** sppLibrary);
	// }@

protected:
	bool m_bInitialized;	///< 标记是否初始化

	NiDX9RendererPtr		m_spDX9Render;
	NiDX9RenderStatePtr		m_spDX9RenderState;
	NiRenderTargetGroupPtr	m_spRenderTarget;

	Base::Math::sdMatrix4	m_kViewMatrix;
	Base::Math::sdMatrix4	m_kProjMatrix;
	Base::Math::sdMatrix4	m_kInvViewMatrix;
};
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif