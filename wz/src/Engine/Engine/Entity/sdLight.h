//*************************************************************************************************
// 内容:	场景光源对象
//---------------------------------------------------------
// 作者:		
// 创建:		2012-09-27
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_LIGHT_H__
#define _SD_ENGINE_SCENE_LIGHT_H__
#include "sdEntity.h"

ENGINE_NAMESPACE_BEGIN_ENGINE
class sdLightSceneObject;
NiSmartPointer(sdLightSceneObject);
ENGINE_NAMESPACE_END_ENGINE

class sdLightModifier;
//-------------------------------------------------------------------------------------------------
// 灯光对象(有待添加修改器，以生成动态灯光效果)
//-------------------------------------------------------------------------------------------------
class sdLight : public sdEntity
{
	NiDeclareRTTI;
	NiDeclareStream;

	DECLARE_PROPFUNC_MAP()
	DECLARE_DYNAMIC(sdLight)

public:
	// 灯光类型
	enum ELightType
	{
		E_LT_UNKNOWN,
		E_LT_AMBIENT,	///< 环境光
		E_LT_POINT,		///< 点光源
		E_LT_DIR,		///< 方向光
		E_LT_SPOT,		///< 聚光灯
		E_LT_RECTSPOT,	///< 聚光灯
		E_LT_DUALDIR,	///< 双向方向光
		E_LT_BOX,		///< 盒式光照

		NUM_LIGHTTYPES = E_LT_BOX,
	};

public:
	sdLight();
	virtual ~sdLight();

	// 虚函数继承(继承自sdEntity)
	virtual void Cull(const NiCamera& kCamera, Engine::RenderSystem::IRenderPath& kRenderPath);

	// 通用光照参数设置
	// @{
	void SetLightType(const uint& uiLightType);
	uint GetLightType() const;

	void SetAmbientColor(const NiColor& kColor);
	NiColor GetAmbientColor() const;

	void SetDiffuseColor(const NiColor& kColor);
	NiColor GetDiffuseColor() const;

	void SetSpecularColor(const NiColor& kColor);
	NiColor GetSpecularColor() const;

	void SetDimmer(const float& fDimmer);
	float GetDimmer() const;

	void SetAttenuRange(const float& fAttenuRange);
	float GetAttenuRange() const;

	void SetAttenuSoftness(const float& fAttenuSoftness);
	float GetAttenuSoftness() const;

	void SetSpotFovX(const float& fFovX);
	float GetSpotFovX() const;

	void SetSpotFovY(const float& fFovY);
	float GetSpotFovY() const;

	void SetSpotSoftness(const float& fSoftness);
	float GetSpotSoftness() const;
	// @}

protected:
	// 虚函数(继承自NiNode)
	virtual void UpdateWorldBound();
	virtual void UpdateDownwardPass(NiUpdateProcess& kUpdate);

//	// 更新渲染对象
//	void UpdateLightSceneObject();

protected:
	// 灯光类型
	ELightType	m_eLightType;

	// 灯光参数
	NiColor m_kAmbientColor;	///< 环境光颜色
	NiColor m_kDiffuseColor;	///< 漫反射颜色
	NiColor m_kSpecularColor;	///< 高光颜色
	float m_fDimmer;			///< 光强

	float m_fAttenuRange;		///< 衰减，取值范围[0,infinity](用于点光源和聚光灯,控制径向衰减)
	float m_fAttenuSoftness;	///< 衰减强度，取值范围[0,1](用于点光源和聚光灯,控制径向衰减)

	float m_fSpotFovX;			///< 取值范围[0,180)(用于聚光灯，控制切向衰减)
	float m_fSpotFovY;			///< 取值范围[0,180)(用于聚光灯，控制切向衰减)
	float m_fSpotSoftness;		///< 取值范围[0,1](用于聚光灯，控制切向衰减)

	// 灯光渲染对象
	Engine::sdLightSceneObjectPtr m_spLSOLight;

//	// 延迟光照的绘制Mesh(用于局部光源)
//	NiMeshPtr	m_spDummyMesh;
};
NiSmartPointer(sdLight);
//-------------------------------------------------------------------------------------------------
#include "sdLight.inl"
#endif