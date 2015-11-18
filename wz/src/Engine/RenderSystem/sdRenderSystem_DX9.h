//*************************************************************************************************
// ����:	��Ⱦϵͳ��GB-D3D9ʵ��
//---------------------------------------------------------
// ����:		
// ����:		2012-06-30
// ����޸�:	2014-04-04
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
// ��Ⱦϵͳ��D3D9ʵ��
//-------------------------------------------------------------------------------------------------
class sdRenderSystem_DX9 : public IRenderSystem
{
public:
	sdRenderSystem_DX9();
	virtual ~sdRenderSystem_DX9();

	// �麯���̳�
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
	// ����
	// @{
	void DrawOpaqueUI();		///< ���Ʋ�͸��UI
	void DrawTranslucentUI();	///< ����͸��UI
	void DrawScene();			///< ���Ƴ���
	// @}

	// ����ȱʡMesh
	// @{
	NiMesh*	CreateScreenQuadMesh();
	NiMesh* CreateUnitCubeMesh();
	NiMesh* CreateUnitSphereMesh();
	NiMesh* CreateUnitConeMesh(uint uiNumSegment);
	NiMesh* CreatePyramidMesh();
	// @}

protected:
	bool m_bInitialized;	///< ����Ƿ��ʼ��

	// ����ͼ
	sdMap*			m_pkMap;

	// �����
	NiCameraPtr		m_spCamera;

	// ��Ⱦ��
	NiDX9RendererPtr m_spDX9Renderer;

	// ������ȾĿ��,Ĭ����ȱʡ��ȾĿ��
	NiRenderTargetGroupPtr	m_spRenderTargetGroup;

	// ��Ⱦ�豸
	IRenderDevice*	m_pkRenderDevice;

	// ��Ⱦ·��
	sdRenderPath*	m_pkRenderPath;


	// ����ֱ����Ⱦ�ĸ�������(��Ҫ���ڱ༭��)
	// @{
	NiVisibleArray			m_kDirectVisibleArray;			///< ֱ�ӻ��Ʋü���Ŀɼ�����
	NiCameraPtr				m_spDirectCamera;				///< ֱ�ӻ������
	NiCullingProcessPtr		m_spDirectCullingProcess;		///< ֱ�ӻ��Ʋü���
	NiAccumulatorPtr		m_spDirectAccumulator;			///< ֱ�ӻ���������
//	NiAlphaSortProcessorPtr	m_spDirectAlphaSortProcessor;	///< ֱ�ӻ���������
	// @{


	// һЩ��Ⱦ��ȱʡ������
	//@{
	// ����ȱʡ��ȾMesh
	NiMeshPtr	m_spScreenQuadMesh;
	NiMeshPtr	m_spUnitCubeMesh;
	NiMeshPtr	m_spUnitSphereMesh;
	NiMeshPtr	m_spUnitConeMesh;
	NiMeshPtr	m_spUnitPyramidMesh;
	NiMeshPtr	m_spErrorMesh;

	// ����ȱʡ����
	NiMaterialPtr		m_spDefaultMaterials[NUM_DEFAULT_MATERIALS];

	// ����ȱʡ����
	NiSourceTexturePtr	m_spDefaultTextures[NUM_DEFAULT_TEXTURES];
	//@}
};
//-------------------------------------------------------------------------------------------------
#include "sdRenderSystem_DX9.inl"
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif