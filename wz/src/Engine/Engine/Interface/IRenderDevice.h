//*************************************************************************************************
// 内容:	渲染设备接口
//---------------------------------------------------------
// 作者:		
// 创建:		2012-06-29
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_RENDER_DEVICE_INTERFACE_H__
#define _SD_ENGINE_RENDER_SYSTEM_RENDER_DEVICE_INTERFACE_H__
#include <sdSingleton.h>

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
// 设备丢失与重设事件回调函数指针
//-------------------------------------------------------------------------------------------------
typedef NiDX9Renderer::LOSTDEVICENOTIFYFUNC LostEventDelegate;
typedef NiDX9Renderer::RESETNOTIFYFUNC		ResetEventDelegate;
//-------------------------------------------------------------------------------------------------
// 渲染设备接口,封装所有对渲染设备的操作
// (封装了渲染状态管理部分,即NiDX9RenderState)
// (封装了部分资源管理部分,即NiDX9Renderer)
//-------------------------------------------------------------------------------------------------
class IRenderDevice : public Base::sdTSingleton<IRenderDevice>
{
public:
	struct TextureSpec
	{
		uint	uiWidth;
		uint	uiHeight;
		NiTexture::FormatPrefs kFormat;
	};

public:
	virtual ~IRenderDevice() {};

	// 初始化与销毁
	virtual bool Initialize() = 0;
	virtual void Destroy() = 0;
	virtual bool IsInitialized() = 0;

	// 渲染器状态设置
	//@{
	// 渲染状态
	virtual void SetRenderState(uint uiState, uint uiValue) = 0;
	virtual uint GetRenderState(uint uiState) = 0;

	virtual void SaveAllRenderStates() = 0;
	virtual void RestoreAllRenderStates() = 0;

	// 纹理采样器状态
	virtual void SetSampleState(uint uiSlot, uint uiState, uint uiValue) = 0;
	virtual uint GetSampleState(uint uiSlot, uint uiState) = 0;

	virtual void SaveAllSampleStates() = 0;
	virtual void RestoreAllSampleStates() = 0;

	// 清除D3D设备状态
	virtual	void Clear(uint* puiColor, float* pfDepth, uint* puiStencil) = 0;
	virtual void ClearVertexBinding() = 0;
	virtual void ClearTextureBinding() = 0;

	// 相机
	virtual void SetCameraData(NiCamera* pkCamera) = 0;
	virtual const Base::Math::sdMatrix4& GetViewMatrix() const = 0;
	virtual const Base::Math::sdMatrix4& GetProjMatrix() const = 0;
	virtual const Base::Math::sdMatrix4& GetInvViewMatrix() const = 0;

	// 渲染目标
	virtual void SetRenderTargetGroup(NiRenderTargetGroup* pkRenderTarget) = 0;
	virtual NiRenderTargetGroup* GetRenderTargetGroup() = 0;
	virtual NiRenderTargetGroup* GetDefaultRenderTargetGroup() = 0;
	// @}

	// 着色器
	// @{
	// 初始化着色器系统(主要是设置着色器目录)
	virtual bool SetupShaderSystem(
		const char* apcProgramDirectoies[], uint uiNumProgramDirectoies,
		const char* apcShaderDirectoies[], uint uiNumShaderDirectoies) = 0;

	virtual void SetGlobalShaderConstant(const NiFixedString& szName, uint uiSize, const void* pData) = 0;
	virtual void GetGlobalShaderConstant(const NiFixedString& szName, uint& uiSize, const void*& pData) = 0;
	// @}

	// 材质
	// @{
	// 材质创建
	virtual NiMaterial* CreateMaterial(const NiFixedString& szName) = 0;
	// @}

	// 渲染器纹理资源创建
	//@{
	virtual NiRenderedTexturePtr	CreateRenderTexture(TextureSpec& kSpec) = 0;
	virtual	NiRenderTargetGroupPtr	CreateRenderTarget(uint uiNum) = 0;
	//virtual NiDX9Direct3DTexturePtr	CreateRawTexture() = 0;

	//virtual NiSourceTexturePtr		LoadTexture(const char* szTexName) = 0;
	//virtual	bool					SaveTexture(const char* szFileName, NiTexture* pTexture) = 0;
	//@}

	// 渲染设备丢失与重设事件
	//@{
	virtual uint AddLostEventDelegate(LostEventDelegate pfFunc, void* pData) = 0;
	virtual uint AddResetEventDelegate(ResetEventDelegate pfFunc, void* pData) = 0;

	virtual bool RemoveLostEventDelegate(uint uiIndex) = 0;
	virtual bool RemoveResetEventDelegate(uint uiIndex) = 0;

	virtual void RemoveAllEventDelegate() = 0;
	//@}

	// 获取渲染设备接口
	static IRenderDevice* GetRenderDevice(){ return InstancePtr();}
};
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif