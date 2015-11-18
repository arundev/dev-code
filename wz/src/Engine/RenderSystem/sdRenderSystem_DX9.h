//*************************************************************************************************
// 内容:	渲染系统的GB-D3D9实现
//---------------------------------------------------------
// 作者:		
// 创建:		2012-06-30
// 最后修改:	2014-04-04
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_RENDER_SYSTEM_DX9_H__
#define _SD_ENGINE_RENDER_SYSTEM_RENDER_SYSTEM_DX9_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
class sdRenderPath;
class IRenderDevice;
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
// 渲染系统的D3D9实现
//-------------------------------------------------------------------------------------------------
class sdRenderSystem_DX9 : public IRenderSystem
{
public:
	sdRenderSystem_DX9();
	virtual ~sdRenderSystem_DX9();

	// 虚函数继承
	virtual bool Initialize();
	virtual void Destroy();
	virtual bool IsInitialized();

	virtual inline void SetMap(sdMap* pkMap);
	virtual inline sdMap* GetMap();

	virtual inline void SetCamera(NiCamera* pkCamera);
	virtual inline NiCamera* GetCamera();

	virtual void UpdateFrame(float fAccumTime, float fFrameTime);

	virtual void RenderFrame();
	virtual void RenderFrame_Begin();
	virtual void RenderFrame_Internal();
	virtual void RenderFrame_End();

	virtual void DisplayFrame();

	virtual void BeginDirectDraw(NiCamera* pkCamera);
	virtual void DirectDraw(NiAVObject* pkObject);
	virtual void DirectDrawImmediate(NiAVObject* pkObject);
	virtual void EndDirectDraw();

	virtual NiMesh*	GetDefaultMesh(eDefaultMesh eMesh);
	virtual NiMaterial*	GetDefaultMaterial(eDefaultMaterial eMaterial);
	virtual NiTexture*	GetDefaultTexture(eDefaultTexture eTexture);

protected:
	// 绘制
	// @{
	void DrawOpaqueUI();		///< 绘制不透明UI
	void DrawTranslucentUI();	///< 绘制透明UI
	void DrawScene();			///< 绘制场景
	// @}

	// 构建缺省Mesh
	// @{
	NiMesh*	CreateScreenQuadMesh();
	NiMesh* CreateUnitCubeMesh();
	NiMesh* CreateUnitSphereMesh();
	NiMesh* CreateUnitConeMesh(uint uiNumSegment);
	NiMesh* CreatePyramidMesh();
	// @}

protected:
	bool m_bInitialized;	///< 标记是否初始化

	// 主地图
	sdMap*			m_pkMap;

	// 主相机
	NiCameraPtr		m_spCamera;

	// 渲染器
	NiDX9RendererPtr m_spDX9Renderer;

	// 最终渲染目标,默认是缺省渲染目标
	NiRenderTargetGroupPtr	m_spRenderTargetGroup;

	// 渲染设备
	IRenderDevice*	m_pkRenderDevice;

	// 渲染路径
	sdRenderPath*	m_pkRenderPath;


	// 用于直接渲染的辅助对象(主要用于编辑器)
	// @{
	NiVisibleArray			m_kDirectVisibleArray;			///< 直接绘制裁剪后的可见队列
	NiCameraPtr				m_spDirectCamera;				///< 直接绘制相机
	NiCullingProcessPtr		m_spDirectCullingProcess;		///< 直接绘制裁剪器
	NiAccumulatorPtr		m_spDirectAccumulator;			///< 直接绘制排序器
//	NiAlphaSortProcessorPtr	m_spDirectAlphaSortProcessor;	///< 直接绘制排序器
	// @{


	// 一些渲染器缺省的数据
	//@{
	// 常用缺省渲染Mesh
	NiMeshPtr	m_spScreenQuadMesh;
	NiMeshPtr	m_spUnitCubeMesh;
	NiMeshPtr	m_spUnitSphereMesh;
	NiMeshPtr	m_spUnitConeMesh;
	NiMeshPtr	m_spUnitPyramidMesh;
	NiMeshPtr	m_spErrorMesh;

	// 常用缺省材质
	NiMaterialPtr		m_spDefaultMaterials[NUM_DEFAULT_MATERIALS];

	// 常用缺省纹理
	NiSourceTexturePtr	m_spDefaultTextures[NUM_DEFAULT_TEXTURES];
	//@}
};
//-------------------------------------------------------------------------------------------------
#include "sdRenderSystem_DX9.inl"
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif