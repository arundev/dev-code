//*************************************************************************************************
// 内容:	渲染对象
//---------------------------------------------------------
// 作者:		
// 创建:		2012-06-30
// 最后修改:
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
// 渲染对象
//
// 功能:
//	1.保存需要渲染的Mesh的相关信息
//	2.根据当前保存的渲染信息进行渲染
//
// (注意对象尺寸,考虑Memory/Cache对齐)
// (注意不要在对象内部分配内存,因为析构函数永远不会被调用)
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
	// 渲染对象类型
	enum ERenderObjectType
	{
		E_ROT_STATIC_MESH,			///< 静态几何对象
		E_ROT_LIGHT_VOLUME,			///< 光源几何体
		E_ROT_PROJECTOR_VOLUME,		///< 贴花几何体
		E_ROT_GHOST_MESH,			///< 残影几何对象
//		E_ROT_EDGE_AREA,			///< 勾边区域
//		E_ROT_SKINNED_MESH,
//		E_ROT_WATER_MESH,
//		E_ROT_DECALE_MESH,

		E_NUM_ROT,
		E_ROT_UNKNOWN = -1,
	};

	// 当前Shader所需参数(用以代替NiExtraData和部分NiProperty,向渲染器设置全局Shader参数)
	struct ShaderParamItem
	{
		const sdFixedString*	szName;		///< 参数名称
		unsigned int			uiByteSize;	///< 参数数据尺寸
		const void*				pvData;		///< 参数数据
	};

	// 渲染对象排序关键字
	struct RenderSortKey
	{
		union
		{
			struct
			{
				uint key0, key1;	///< 用于RenderObject类
				uint key2, key3;	///< 用于RenderObject扩展类
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

	// 属性访问
	inline const ERenderObjectType GetType() const;
	inline const RenderSortKey& GetSortKey() const;
	inline const NiMesh* GetMesh()	const;

	// 绘制接口
	virtual void Draw() = 0;

protected:
	// 渲染前后调用,用于使用指定渲染属性来替换掉默认属性
	// 这样就可以使同一个Geometry用于多种渲染情况,其实等于改掉了部分Render
	void ApplyRenderSetting();
	void RestoreRenderSetting();

	virtual void ApplyRenderProps();		///< 应用NiPropertyList到渲染器
	virtual void RestoreRenderProps();		///< 恢复NiPropertyList

	virtual void ApplyTexturingProp();		///< 应用NiPropertyList中的NiTexturingProperty到渲染器
	virtual void RestoreTexturingProp();	///< 恢复NiPropertyList中的NiTexturingProperty

	virtual void ApplyMaterial();
	virtual void RestoreMaterial();

	virtual void SetShaderParams();			///< 设置着色器参数
	virtual void SetRenderStates();			///< 设置渲染状态

	// 在NiPropertyList列表查找特定类型的NiProperty
	NiProperty*	FindProperty(NiPropertyList* pkPropList, int iPropType);

protected:
	ERenderObjectType	m_eType;			///< 渲染对象类型
	NiMesh*				m_pkMesh;			///< Mesh
	NiMaterial*			m_pkMaterial;		///< 材质
	NiPropertyList*		m_pkProperties;		///< 渲染属性列表
	ShaderParamItem*	m_pkShaderParams;	///< Shader参数

	// 排序关键字
	// Only care about rendering states like : shader, vertex, index, texturing, 
	// device render states, etc. If you want to sort by object type and distance, 
	// you should process them outside of here
	RenderSortKey		m_kRenderKey;

	// 中间变量
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

	// 虚函数继承(继承自sdRenderObject)
	virtual void Draw();
};
//-------------------------------------------------------------------------------------------------
// GhostMesh, 残影
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

	// 虚函数继承(继承自sdRenderObject)
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

	// 虚函数继承(继承自sdRenderObject)
	virtual void Draw();

protected:
	NiMesh*	FindVolumeMeshForLight(sdLightSceneObject* pkLight);

	virtual void ApplyLightSettings();		///< 设置灯光相关参数
	virtual void RestoreLightSettings();	///< 恢复灯光相关参数

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

	// 虚函数继承
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