//*************************************************************************************************
// 内容:	RenderSystem识别的的场景渲染对象接口, 
//			 1.SceneGraph在裁剪后向RenderPath提交SceneObject,
//			 2.RenderPath根据SceneObject类型来分发到相应RenderPass
//			 3.RenderPass根据SceneObject来产生RenderObject
//---------------------------------------------------------
// 作者:		
// 创建:		2012-12-07
// 最后修改:	2014-04-24
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_OBJECT_H__
#define _SD_ENGINE_SCENE_OBJECT_H__

ENGINE_NAMESPACE_BEGIN_ENGINE
//-------------------------------------------------------------------------------------------------
// 场景渲染对象
//	0.sdSceneObject提供对象的基本属性(目前主要是渲染属性)
//	1.sdSceneObject不提供关系和层级关系，sdSceneObject之间相互独立
//	2.sdSceneObject提供给Scene使用，在sdEntity内部提供数据
//	3.sdSceneObject提供给RenderSystem使用,向sdRenderObject提供渲染数据
//	4.在对sdMap进行裁剪之后，得到SceneObject被添加到RenderSystem，生成sdRenderObject，
//	  由RenderPath/RenderPass处理对sdRenderObject的渲染
//-------------------------------------------------------------------------------------------------
// At render system layer. a render system instance should hold a scene graph
// which is composed by various sorts of scene objects.
//
// The render path's responsiable to submit every scene objects to correct render pass
//
// At render pass layer, a render pass instance should hold a render list which
// is composed by various sorts of render objects.
//
// Render objects is composed by a scene object with some additional render data
// that will be used when rendering scene objects to render targets.
// (The gamebryo's extra data can not handle this situation clearly, beacuse one 
// scene object may corresponds to more than one render objects.)
//
// Render pass responsible to define object type(or use default render object type), 
// build render object instance from scene object instance, then draw these objects 
// correctly.
//-------------------------------------------------------------------------------------------------
class sdSceneObject : public NiRefObject
{
public:
	// 场景对象类型(Render System scene graph object types)
	enum ESceneObjectType
	{
		E_SOT_STATIC_MESH,	///< 静态几何体
		E_SOT_SKINNED_MESH,	///< 蒙皮几何体
		E_SOT_TERRAIN,		///< 地形
		E_SOT_WATER,		///< 水
//		E_SOT_DECAL,		///< 贴花	
		E_SOT_PROJECTOR,	///< 投射体
		E_SOT_SKY,			///< 
		E_SOT_LIGHT,		///< 灯光
		E_SOT_CAMERA,		///<
		E_SOT_ENV,			///<
		
		E_NUM_SOT,
		E_SOT_UNKNOWN = -1,
	};

public:
	sdSceneObject(ESceneObjectType eType) : m_eType(eType) {};
	virtual ~sdSceneObject(){};

protected:
	ESceneObjectType m_eType;	///< 场景对象类型
};
NiSmartPointer(sdSceneObject);
//-------------------------------------------------------------------------------------------------
// 灯光场景对象
//-------------------------------------------------------------------------------------------------
class sdLightSceneObject : public sdSceneObject
{
public:
	// 灯光类型
	enum ELightType
	{
		E_LT_DEFAULT,
		E_LT_AMBIENT,	///< 环境光
		E_LT_POINT,		///< 点光源
		E_LT_DIR,		///< 方向光
		E_LT_SPOT,		///< 聚光灯
		E_LT_RECT_SPOT,	///< 聚光灯
		E_LT_DUALDIR,	///< 双向方向光
		E_LT_BOX,		///< 盒式光照

		E_NUM_LT,
		E_LT_UNKNOWN = -1,
	};

public:
	sdLightSceneObject();

	// 属性
	// @{
	void SetPosition(const NiPoint3& kPosition);
	const NiPoint3& GetPosition() const;

	void SetRotate(const NiMatrix3& kRotate);
	const NiMatrix3& GetRotate() const;

	void SetLightType(ELightType eType);
	ELightType GetLightType() const;

	void SetAmbientColor(const NiColor& kColor);
	NiColor GetAmbientColor() const;

	void SetDiffuseColor(const NiColor& kColor);
	NiColor GetDiffuseColor() const;

	void SetSpecularColor(const NiColor& kColor);
	NiColor GetSpecularColor() const;

	void SetDimmer(const float& fDimmer);
	float GetDimmer() const;

	void SetAttenuRange(float fAttenuRange);
	float GetAttenuRange() const;

	void SetAttenuSoftness(float fAttenuSoftness);
	float GetAttenuSoftness() const;

	void SetSpotFovX(float fFovX);
	float GetSpotFovX() const;

	void SetSpotFovY(float fFovY);
	float GetSpotFovY() const;

	void SetSpotSoftness(float fSoftness);
	float GetSpotSoftness() const;

	void SetFilterMap(NiTexture* spTexture);
	NiTexture* GetFilterMap() const;
	// @}

protected:
	ELightType	m_eLightType;	///< 灯光类型

	NiPoint3 m_kPosition;		///< 位置
	NiMatrix3 m_kRotate;		///< 旋转

	NiColor m_kAmbientColor;	///< 环境光颜色
	NiColor m_kDiffuseColor;	///< 漫反射颜色
	NiColor m_kSpecularColor;	///< 高光颜色
	float m_fDimmer;			///< 光强

	float m_fAttenuRange;		///< 衰减，取值范围[0,infinity](用于点光源和聚光灯,控制径向衰减)
	float m_fAttenuSoftness;	///< 衰减强度，取值范围[0,1](用于点光源和聚光灯,控制径向衰减)

	float m_fSpotFovX;			///< 取值范围[0,180)(用于聚光灯，控制切向衰减)
	float m_fSpotFovY;			///< 取值范围[0,180)(用于聚光灯，控制切向衰减)
	float m_fSpotSoftness;		///< 取值范围[0,1](用于聚光灯，控制切向衰减)

	NiTexturePtr m_spFilterMap;	///< 灯光过滤贴图
	float m_fAlphaRef;			///< 灯光过滤贴图的Alpha参考值
};
NiSmartPointer(sdLightSceneObject);
//-------------------------------------------------------------------------------------------------
// 投射场景对象
// Projector
//	- Only project normal/diffuse/flatcolor to scene objects that marked as receive decals
//-------------------------------------------------------------------------------------------------
class sdProjectorSceneObject : public sdSceneObject
{
public:
	// 投射方式
	enum EProjectType
	{
		E_PT_ORTHOGONAL,	///< 正交投射
		E_PT_PERSPECTIVE,	///< 透视透射
		E_PT_OMNI,			///< 点透视

		NUM_PT,
	};

	// 混合方式
	enum EBlendType
	{
		E_BT_NONE,
		E_BT_ALPHABLEND,
		E_BT_ADD,
		E_BT_REPLACE,

		NUM_BT,
	};

public:
	sdProjectorSceneObject();

	// 属性
	// @{
	void SetBlendType(EBlendType eBlendType);
	EBlendType GetBlendType() const;

	void SetReceiveLighting(bool bRecLighting);
	bool GetReceiveLighting() const;

	void SetVisible(bool bVisible);
	bool GetVisible() const;

	void SetLayer(uint uiLayer);
	uint GetLaye() const;

	void SetPosition(const NiPoint3& kPosition);
	const NiPoint3& GetPosition() const;

	void SetDirection(const NiPoint3& kDirection);
	const NiPoint3& GetDirection() const;

	void SetUpDirection(const NiPoint3& kUp);
	const NiPoint3& GetUpDirection() const;

	void SetSize(const NiPoint3& kSize);
	const NiPoint3& GetSize() const;

	void SetLength(float fLength);
	float GetLength() const;

	void SetWidth(float fWidth);
	float GetWidth() const;

	void SetHeight(float fHeight);
	float GetHeight() const;

	void SetUVScale(const NiPoint2& kUVScale);
	const NiPoint2& GetUVScale() const;

	void SetUVOffset(const NiPoint2& kUVOffset);
	const NiPoint2& GetUVOffset() const;

	void SetUVSpeed(const NiPoint2& kUVSpeed);
	const NiPoint2& GetUVSpeed() const;

	void SetAlpha(float fAlpha);
	float GetAlpha() const;

	void SetAlphaTest(float fAlphaTest);
	float GetAlphaTest() const;

	 void SetDiffuseMaterial(const NiColor& kColor);
	 const NiColor& GetDiffuseMaterial() const;

	 void SetSpecularMaterial(const NiColor& kColor);
	 const NiColor& GetSpecularMaterial() const;

	 void SetShiness(float fShiness);
	 float GetShiness() const;

	 void SetDiffuseMap(NiTexture* spTexture);
	 NiTexture* GetDiffuseMap() const;

	 void SetNormalMap(NiTexture* spTexture);
	 NiTexture* GetNormalMap() const;

	 void SetFilterMap(NiTexture* spTexture);
	 NiTexture* GetFilterMap() const;

	 void SetMaterial(NiMaterial* spMaterial);
	 NiMaterial* GetMaterial() const;
	// @}

protected:
	EProjectType	m_eProjectType;	///< 投射类型
	EBlendType		m_eBlendType;	///< 贴花混合类型
	
	bool m_bReceiveLighting;		///< 是否接受光照
	bool m_bVisible;				///< 是否可见(???)

	// Decals will be covered by higher level decals which belong to same type
	uint m_uiLayer;					

	// 贴花几何信息
	NiPoint3	m_kPosition;		///< 贴花位置和姿态
	NiPoint3	m_kDirection;		///<
	NiPoint3	m_kUp;				///<
	NiPoint3	m_kSize;			///< 贴花尺寸

	// 贴花纹理信息
	NiPoint2	m_kUVScale;			///< 
	NiPoint2	m_kUVOffset;		///<
	NiPoint2	m_kUVSpeed;			///<

	float		m_fAlpha;
	float		m_fAlphaTest;

	// 贴花材质信息
	NiColor		m_kDiffuseMaterial;
	NiColor		m_kSpecularMaterial;
	float		m_fShiness;

	// Use 2D textures for othogonal or pespective project type decal.
	// Use cubemap textures for omni project type decal.
	// We can special one of them and leave another null, if we only want change 
	// scene surface's diffuse or normal
	NiTexturePtr	m_spDiffuseMap;
	NiTexturePtr	m_spNormalMap;
	NiTexturePtr	m_spFilterMap;

	// 贴花特殊材质
	NiMaterialPtr	m_spMaterial;
};
NiSmartPointer(sdProjectorSceneObject);
//-------------------------------------------------------------------------------------------------
#include "sdSceneObject.inl"
ENGINE_NAMESPACE_END_ENGINE
#endif