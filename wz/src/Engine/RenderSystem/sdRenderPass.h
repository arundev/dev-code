//*************************************************************************************************
// ����:	��Ⱦ��������ӿ�
//---------------------------------------------------------
// ����:		
// ����:		2012-07-04
// ����޸�:	2014-05-07
//*************************************************************************************************
#pragma once
#ifndef _SD_RENDER_SYSTEM_RENDER_PASS_H__
#define _SD_RENDER_SYSTEM_RENDER_PASS_H__
#include "sdRenderObject.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
class sdLightSceneObject;
class sdProjectorSceneObject;
ENGINE_NAMESPACE_END_ENGINE

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
class sdRenderParams;
class sdRenderObject;
class sdRenderObjectAlloc;
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
// ��Ⱦ����
// RenderPass�����Լ���RenderObject����ͨ��RenderDevice��Ⱦ���ض�RenderTarget
//
// \class sdRenderPass
// 
// sdRenderPass is the base class of all special purpose render passes
//
// What is render pass?
// Render Passs properly render its render queue to the current render target
//
// For making the render pass can be reused in different render paths, they can
// access sdRenderDevice and sdRenderSystem, but cannot access sdRenderPath
//-------------------------------------------------------------------------------------------------
class sdRenderPass : public NiRefObject
{
public:
	sdRenderPass(sdRenderObjectAlloc* pkAlloc);
	virtual ~sdRenderPass();

	// �Ƿ��ʼ��
	inline bool IsInitialized(){ return m_bInitialized;};

	// �Ƿ񼤻�
	inline bool GetActive() { return m_bActived;};
	inline bool SetActive(bool bActive){ m_bActived = bActive;};

	// ������Ⱦ����
	virtual void SetRenderParams(const sdRenderParams* pkRenderParams);

	// ���ƽӿں���
	// @{
	virtual void Begin();
	virtual void Draw();
	virtual void End();
	// @}

protected:
	// �����Ӷ�����Ⱦ����
	// @{
	sdRenderObject::ShaderParamItem* CloneShaderParams(sdRenderObject::ShaderParamItem* pkParams);
	
	void InsertRenderObject(sdRenderObject* pkObject);
	void ClearRenderObjects();

	void InsertStaticSceneObject(NiMesh* spMesh, 
		NiMaterial* spMaterial,
		NiPropertyList* spRenderProps, 
		sdRenderObject::ShaderParamItem* pkShaderParams);

	void InsertGhostMeshSceneObject(NiMesh* spMesh, 
		NiMaterial* spMaterial,
		NiPropertyList* spRenderProps, 
		sdRenderObject::ShaderParamItem* pkShaderParams,
		uint uiGhostIndex);

	void InsertLightSceneObject(sdLightSceneObject* pkLight, 
		NiMaterial* spMaterial,
		NiPropertyList* spRenderProps, 
		sdRenderObject::ShaderParamItem* pkShaderParams);

	void InsertProjectorSceneObject(sdProjectorSceneObject* pkProjector, 
		NiMaterial* spMaterial,
		NiPropertyList* spRenderProps, 
		sdRenderObject::ShaderParamItem* pkShaderParams);
	// @}

	///< ��������
	///< @{
	///< Ĭ�ϵ�������
	typedef bool (*RenderObjectComparator)(sdRenderObject* lhs, sdRenderObject* rhs);
	static bool	Comparator(sdRenderObject* lhs, sdRenderObject* rhs);

	///< ��ȡ��ǰ������
	virtual RenderObjectComparator GetComparator();

	///< �����������
	virtual void Sort();
	///< @}

protected:
	bool	m_bInitialized;
	bool	m_bActived;

	typedef std::vector<sdRenderObject*>	RenderObjectVec;
	typedef std::vector<sdRenderObject*>::iterator	RenderObjectVecItr;
	RenderObjectVec			m_kRenderObjectVec;
	sdRenderObjectAlloc*	m_pkAlloc;
	sdRenderParams*			m_pkRenderParams;
};
NiSmartPointer(sdRenderPass);
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif