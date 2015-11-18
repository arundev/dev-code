//*************************************************************************************************
// ����:	��Ⱦ����
//---------------------------------------------------------
// ����:		
// ����:		2012-06-30
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_RENDER_OBJECT_H__
#define _SD_ENGINE_RENDER_SYSTEM_RENDER_OBJECT_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
class sdLightSceneObject;
class sdProjectorSceneObject;
ENGINE_NAMESPACE_END_ENGINE

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
class sdFixedString;
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
// ��Ⱦ����
//
// ����:
//	1.������Ҫ��Ⱦ��Mesh�������Ϣ
//	2.���ݵ�ǰ�������Ⱦ��Ϣ������Ⱦ
//
// (ע�����ߴ�,����Memory/Cache����)
// (ע�ⲻҪ�ڶ����ڲ������ڴ�,��Ϊ����������Զ���ᱻ����)
//-----------------------------------------------------------------------------
// Don't allocate memory inside of sdRenderObject if you want change default render objects. 
// Their destructor will never be called. You won't have any change to release allocated memory.
//-----------------------------------------------------------------------------
// Base/Normal/Dark/Gloss/Glow/Decal/Detail/Light Maps is specified by NiMesh
// All shader maps is specified by sdRenderObject
//-------------------------------------------------------------------------------------------------
class sdRenderObject: public NiMemObject
{
public:
	// ��Ⱦ��������
	enum ERenderObjectType
	{
		E_ROT_STATIC_MESH,			///< ��̬���ζ���
		E_ROT_LIGHT_VOLUME,			///< ��Դ������
		E_ROT_PROJECTOR_VOLUME,		///< ����������
		E_ROT_GHOST_MESH,			///< ��Ӱ���ζ���
//		E_ROT_EDGE_AREA,			///< ��������
//		E_ROT_SKINNED_MESH,
//		E_ROT_WATER_MESH,
//		E_ROT_DECALE_MESH,

		E_NUM_ROT,
		E_ROT_UNKNOWN = -1,
	};

	// ��ǰShader�������(���Դ���NiExtraData�Ͳ���NiProperty,����Ⱦ������ȫ��Shader����)
	struct ShaderParamItem
	{
		const sdFixedString*	szName;		///< ��������
		unsigned int			uiByteSize;	///< �������ݳߴ�
		const void*				pvData;		///< ��������
	};

	// ��Ⱦ��������ؼ���
	struct RenderSortKey
	{
		union
		{
			struct
			{
				uint key0, key1;	///< ����RenderObject��
				uint key2, key3;	///< ����RenderObject��չ��
			};
			uint keys[4];
		};

		RenderSortKey() : key0(0), key1(0), key2(0), key3(0){}
	};

public:
	sdRenderObject(
		ERenderObjectType eType, 
		NiMesh* pkMesh, 
		NiMaterial* pkMaterial, 
		NiPropertyList* pkRenderProps,
		ShaderParamItem* pkShaderParams);

	// ���Է���
	inline const ERenderObjectType GetType() const;
	inline const RenderSortKey& GetSortKey() const;
	inline const NiMesh* GetMesh()	const;

	// ���ƽӿ�
	virtual void Draw() = 0;

protected:
	// ��Ⱦǰ�����,����ʹ��ָ����Ⱦ�������滻��Ĭ������
	// �����Ϳ���ʹͬһ��Geometry���ڶ�����Ⱦ���,��ʵ���ڸĵ��˲���Render
	void ApplyRenderSetting();
	void RestoreRenderSetting();

	virtual void ApplyRenderProps();		///< Ӧ��NiPropertyList����Ⱦ��
	virtual void RestoreRenderProps();		///< �ָ�NiPropertyList

	virtual void ApplyTexturingProp();		///< Ӧ��NiPropertyList�е�NiTexturingProperty����Ⱦ��
	virtual void RestoreTexturingProp();	///< �ָ�NiPropertyList�е�NiTexturingProperty

	virtual void ApplyMaterial();
	virtual void RestoreMaterial();

	virtual void SetShaderParams();			///< ������ɫ������
	virtual void SetRenderStates();			///< ������Ⱦ״̬

	// ��NiPropertyList�б�����ض����͵�NiProperty
	NiProperty*	FindProperty(NiPropertyList* pkPropList, int iPropType);

protected:
	ERenderObjectType	m_eType;			///< ��Ⱦ��������
	NiMesh*				m_pkMesh;			///< Mesh
	NiMaterial*			m_pkMaterial;		///< ����
	NiPropertyList*		m_pkProperties;		///< ��Ⱦ�����б�
	ShaderParamItem*	m_pkShaderParams;	///< Shader����

	// ����ؼ���
	// Only care about rendering states like : shader, vertex, index, texturing, 
	// device render states, etc. If you want to sort by object type and distance, 
	// you should process them outside of here
	RenderSortKey		m_kRenderKey;

	// �м����
	NiMaterialPtr			m_spOldMaterial;
	NiTexturingPropertyPtr	m_spOldTexturingProp;
};
//-------------------------------------------------------------------------------------------------
// StaticMesh
//-------------------------------------------------------------------------------------------------
class sdStaticMeshRenderObject : public sdRenderObject
{
public:
	sdStaticMeshRenderObject(
		NiMesh* pkMesh, 
		NiMaterial* pkMaterial, 
		NiPropertyList* pkRenderProps, 
		ShaderParamItem* pkShaderParams);

	// �麯���̳�(�̳���sdRenderObject)
	virtual void Draw();
};
//-------------------------------------------------------------------------------------------------
// GhostMesh, ��Ӱ
//-------------------------------------------------------------------------------------------------
class sdGhostMeshRenderObject : public sdRenderObject
{
public:
	sdGhostMeshRenderObject(
		NiMesh* pkMesh, 
		NiMaterial* pkMaterial, 
		NiPropertyList* pkRenderProps, 
		ShaderParamItem* pkShaderParams,
		uint uiGhostIndex);

	// �麯���̳�(�̳���sdRenderObject)
	virtual void Draw();

protected:
	uint m_uiGhostIndex;
};
//-------------------------------------------------------------------------------------------------
// sdLightVolume
//-------------------------------------------------------------------------------------------------
class sdLightVolumeRenderObject : public sdRenderObject
{
public:
	sdLightVolumeRenderObject(
		sdLightSceneObject* pkLight, 
		NiMaterial* pkMaterial, 
		NiPropertyList* pkRenderProps, 
		ShaderParamItem* pkShaderParams);

	// �麯���̳�(�̳���sdRenderObject)
	virtual void Draw();

protected:
	NiMesh*	FindVolumeMeshForLight(sdLightSceneObject* pkLight);

	virtual void ApplyLightSettings();		///< ���õƹ���ز���
	virtual void RestoreLightSettings();	///< �ָ��ƹ���ز���

protected:
	sdLightSceneObject* m_pkLight;
};
//-------------------------------------------------------------------------------------------------
// sdProjectorVolume
//-------------------------------------------------------------------------------------------------
class sdProjectorVolumeRenderObject : public sdRenderObject
{
public:
	sdProjectorVolumeRenderObject(
		sdProjectorSceneObject* pkProjector, 
		NiMaterial* pkMaterial, 
		NiPropertyList* pkRenderProps, 
		ShaderParamItem* pkShaderParams);

	// �麯���̳�
	virtual void Draw();

protected:
	NiMesh*	FindVolumeMeshForProjector(sdProjectorSceneObject* pkProjector);

	virtual void ApplyTexturingProp();
	virtual void RestoreTexturingProp();

	virtual void ApplyProjectorSettings();
	virtual void RestoreProjectorSettings();

protected:
	sdProjectorSceneObject* m_pkProjector;
};
//-------------------------------------------------------------------------------------------------
inline bool operator == (const sdRenderObject::RenderSortKey& lhs, const sdRenderObject::RenderSortKey& rhs)
{
	return 0 == memcmp(lhs.keys, rhs.keys, sizeof(lhs.keys));
}
//-------------------------------------------------------------------------------------------------
inline bool operator != (const sdRenderObject::RenderSortKey& lhs, const sdRenderObject::RenderSortKey& rhs)
{
	return 0 != memcmp(lhs.keys, rhs.keys, sizeof(lhs.keys));
}
//-------------------------------------------------------------------------------------------------
inline bool operator < (const sdRenderObject::RenderSortKey& lhs, const sdRenderObject::RenderSortKey& rhs)
{
	if (lhs.key0 != rhs.key0)	return lhs.key0 < rhs.key0;
	if (lhs.key1 != rhs.key1)	return lhs.key1 < rhs.key1;
	if (lhs.key2 != rhs.key2)	return lhs.key2 < rhs.key2;
	if (lhs.key3 != rhs.key3)	return lhs.key3 < rhs.key3;

	return false;
}
//---------------------------------------------------------------------------------------------
inline bool operator > (const sdRenderObject::RenderSortKey& lhs, const sdRenderObject::RenderSortKey& rhs)
{
	if (lhs.key0 != rhs.key0)	return lhs.key0 > rhs.key0;
	if (lhs.key1 != rhs.key1)	return lhs.key1 > rhs.key1;
	if (lhs.key2 != rhs.key2)	return lhs.key2 > rhs.key2;
	if (lhs.key3 != rhs.key3)	return lhs.key3 > rhs.key3;

	return false;
}
//-------------------------------------------------------------------------------------------------
#include "sdRenderObject.inl"
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif