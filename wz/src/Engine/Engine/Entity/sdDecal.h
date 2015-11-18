//*************************************************************************************************
// 内容:	场景贴花对象
//---------------------------------------------------------
// 作者:		
// 创建:		2012-12-06
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_DECAL_H__
#define _SD_ENGINE_SCENE_DECAL_H__
#include "sdEntity.h"

//-------------------------------------------------------------------------------------------------
// 贴花对象(有待添加修改器，以生成动态贴花效果,诸如平移、旋转、缩放、动画等)
//-------------------------------------------------------------------------------------------------
class sdDecal : public sdEntity
{
	NiDeclareRTTI;
	NiDeclareStream;

	DECLARE_PROPFUNC_MAP()
	DECLARE_DYNAMIC(sdDecal)

public:
	sdDecal();
	virtual ~sdDecal();

	// 虚函数继承(继承自sdEntity)
	virtual void Cull(const NiCamera& kCamera, Engine::RenderSystem::IRenderPath& kRenderPath);

	// 通用贴花参数设置
	// @{
	void SetWidth(const float& fValue);
	float GetWidth() const;

	void SetHeight(const float& fValue);
	float GetHeight() const;

	void SetLength(const float& fValue);
	float GetLength() const;

	void SetDiffuseMaterial(const NiColor& kValue);
	NiColor GetDiffuseMaterial() const;

	void SetSpecularMaterial(const NiColor& kValue);
	NiColor GetSpecularMaterial() const;

	void SetUVScale(const NiPoint2& kValue);
	NiPoint2 GetUVScale() const;

	void SetUVOffset(const NiPoint2& kValue);
	NiPoint2 GetUVOffset() const;

	void SetUVSpeed(const NiPoint2& kValue);
	NiPoint2 GetUVSpeed() const;

	void sdDecal::SetDiffuseMap(const NiFixedString& kValue);
	NiFixedString GetDiffuseMap() const;

	void sdDecal::SetNormalMap(const NiFixedString& kValue);
	NiFixedString GetNormalMap() const;

	void sdDecal::SetFilterMap(const NiFixedString& kValue);
	NiFixedString GetFilterMap() const;

	void sdDecal::SetRecvLighting(const bool& bValue);
	bool GetRecvLighting() const;

	void sdDecal::SetAlpha(const float& fValue);
	float GetAlpha() const;

	void sdDecal::SetBlendType(const int& iValue);
	int GetBlendType() const;
	// @}

protected:
	// 虚函数(继承自NiNode)
	virtual void UpdateWorldBound();
	virtual void UpdateDownwardPass(NiUpdateProcess& kUpdate);

protected:
	// 贴花参数
	float m_fWidth;					///< 贴花尺寸
	float m_fHeight;				///<
	float m_fLength;				///<

	NiColor m_kDiffuseMaterial;		///< 贴花漫反射材质
	NiColor m_kSpecularMaterial;	///< 贴花高光材质

	NiPoint2 m_kUVScale;			///< 贴花纹理坐标
	NiPoint2 m_kUVOffset;			///<
	NiPoint2 m_kUVSpeed;			///< 

	bool m_bDiffuseMapDirty;		///< 标记漫反射贴图是否被修改过
	NiFixedString m_kDiffuseMap;	///< 漫反射贴图路径
	NiTexturePtr m_spDiffuseMap;	///< 漫反射贴图

	bool m_bNormalMapDirty;			///< 标记法线贴图是否被修改过
	NiFixedString m_kNormalMap;		///< 法线贴图路径
	NiTexturePtr m_spNormalMap;		///< 法线贴图

	bool m_bFilterMapDirty;			///< 标记过滤贴图是否被修改过
	NiFixedString m_kFilterMap;		///< 过滤贴图路径
	NiTexturePtr m_spFilterMap;		///< 过滤贴图

	bool m_bReceiveLighting;		///< 是否受光
	float m_fAlpha;					///< 透明度
	int m_iBlendType;				///< 混合类型

	// 贴花渲染对象
	Engine::sdProjectorSceneObjectPtr m_spPSOProjector;
};
NiSmartPointer(sdDecal);
//-------------------------------------------------------------------------------------------------
#include "sdDecal.inl"
#endif