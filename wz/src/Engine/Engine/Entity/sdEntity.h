//*************************************************************************************************
// 内容:	场景对象节点抽象类,用于构建场景图
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-31
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_SCENE_ENTITY_H__
#define _SD_ENGINE_SCENE_ENTITY_H__
#include "sdGameObject.h"
#include "IRenderPath.h"
#include <sdMath.h>

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
class sdRenderPath;
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE

class sdMap;
class sdLight;
//--------------------------------------------------------------------------------------
enum EEntityType
{
	E_ET_ENTITY = 0,
	E_ET_BUILDING,		///< 建筑
	E_ET_LIGHT,			///< 光源
	E_ET_DECAL,			///< 贴花
};
//--------------------------------------------------------------------------------------
enum ELoadingState
{
	E_LS_WAIT = 0,
	E_LS_LOADING,
	E_LS_VALID,
};
//--------------------------------------------------------------------------------------
enum ECullLevel
{
	E_CULLLEVEL_NEVER	= 0,
	E_CULLLEVEL_HIGH	= 1,
	E_CULLLEVEL_MIDDLE	= 2,
	E_CULLLEVEL_LOW		= 3,
};
//--------------------------------------------------------------------------------------
enum EEntityTexMask
{
	E_ETL_DIFFUSEMAP		= 0x00000001,
	E_ETL_GLOSSMAP			= 0x00000002,
	E_ETL_NORMALMAP			= 0x00000004,
	E_ETL_LIGHTMAP			= 0x00000008,
	E_ETL_DETAILNORMALMAP	= 0x00000010,
	E_ETL_ENVIRONMENTMAP	= 0x00000020,
};
//-------------------------------------------------------------------------------------------------
// 一个场景对象,支持以下效果控制：
//
//	网格
//	动画
//	物理
//	寻路
//	材质
//	纹理
//
//	灯光
//	投射阴影
//	接收阴影
//	接收贴花
//	使用光探头(Use Light Probes)	为该网格启用基于探头的照明
//	锚点覆盖(Anchor Override)		用于使用光探头系统时确定插值位置
//	勾边效果
//	残影效果
//-------------------------------------------------------------------------------------------------
class sdEntity : public sdGameObject
{
	///< 只能暂时共存
	NiDeclareRTTI;
	NiDeclareStream;
	
	///< 新动态系统
	DECLARE_PROPFUNC_MAP()
	DECLARE_DYNAMIC(sdEntity)

public:
	sdEntity();
	virtual ~sdEntity();

	// @breif 虚函数,从sdEntity中提取需要被渲染系统处理的sdSceneObject对象
	// @param[in] kCamera		裁剪相机
	// @param[in] kMeshVec		渲染对象
	virtual void Cull(const NiCamera& kCamera, std::vector<NiMesh*>& kMeshVec);

	// @breif 虚函数,从sdEntity中提取需要被渲染系统处理的SceneObject对象加入到RenderPath
	// @param[in] kCamera		裁剪相机
	// @param[in] kRenderPath	渲染路径
	virtual void Cull(const NiCamera& kCamera, Engine::RenderSystem::IRenderPath& kRenderPath);

	// 虚函数,更新
	virtual void UpdateGame();

	// 虚函数继承,重写基类Update函数
	// 假定父节点bound无穷大,所以这里避免父节点被多次调用BoundMerge,降低CPU压力
	virtual void Update(float fTime, bool bUpdateControllers = true);
	virtual void UpdateSelected(float fTime);

	// 属性访问
	//	1.Translate(TR)
	//	2.Rotation(RT)
	//	3.Scale(SC)
	//	4.ViewDistance(VD)
	// @{
	DEFINE_GAMEOBJECT_PROP(NiPoint3, TR, TranslateProp)
	SD_INLINE NiPoint3 GetTranslateProp() const;
	SD_INLINE void SetTranslateProp(const NiPoint3&);
	
	DEFINE_GAMEOBJECT_PROP(NiPoint3, RT, RotateProp)
	SD_INLINE NiPoint3 GetRotateProp() const;
	SD_INLINE void SetRotateProp(const NiPoint3&);
	
	DEFINE_GAMEOBJECT_PROP(float, SC, ScaleProp)
	SD_INLINE float	GetScaleProp() const;
	SD_INLINE void SetScaleProp(const float&);

	DEFINE_GAMEOBJECT_PROP(float, VD, ViewDistance)
	SD_INLINE float	GetViewDistance() const;
	SD_INLINE void SetViewDistance(const float&);
	// @}

	// 地图
	SD_INLINE void SetMap(sdMap* pkMap);
	SD_INLINE sdMap* GetMap();

	// 类型
	SD_INLINE void SetType(EEntityType eType);
	SD_INLINE EEntityType GetType();

	// 加载状况
	SD_INLINE void SetLoadingStatus(ELoadingState eStatus);
	SD_INLINE ELoadingState GetLoadingStatus();
	
	// 勾边
	SD_INLINE void SetShowEdge(bool bShow);
	SD_INLINE bool GetShowEdge() const;

	SD_INLINE void SetEdgeColor(const NiColor& kColor);
	SD_INLINE const NiColor& GetEdgeColor() const;

	// 灯光
	SD_INLINE void AddAffectLight(sdLight* pkLight);
	SD_INLINE void ClearAffectLight();
	SD_INLINE uint GetAffecttLightNum();
	SD_INLINE std::vector<sdLight*> GetAffectLights();

	// 渲染帧计数
	// @{
	SD_INLINE uint GetRenderedFrameId();
	SD_INLINE void TickRenderedFrameId();

	SD_INLINE static uint GetSystemRenderedFrameId();
	SD_INLINE static void TickSystemRenderedFrameId();

	SD_INLINE static uint GetSystemVisiableFrameId();
	SD_INLINE static void TickSystemVisiableFrameId();
	// @}

protected:

	// 处理勾边
	virtual void ProcessEdge(NiAVObject* pkAVObject);
	virtual void ProcessEdgeRecursion(NiAVObject* pkAVObject, bool bEdge);


protected:
	sdMap*		m_pkMap;			///< 所属地图
	EEntityType	m_eType;			///< 对象类型
	ELoadingState m_eLoadingStatus;	///< 资源加载状况
	
	//NiPoint3	m_kBoundCenter;
	//float		m_fBoundRadius;

	float		m_fViewDistance;	///< 可视见距离

	//uint		m_uiLightMask;		///< 光照通道掩码
	//uint		m_uiPickMask;		///< 对象拾取掩码
	//uint		m_uiTypeMask;		///< 对象类型掩码

	bool		m_bVisible;
	bool		m_bLocked;
	bool		m_bDynamic;			///<
	bool		m_bRenderable;

	// 全局光照
	bool m_bLightProbe;		///< 是否打开全局光照
							///< 全局光照效果

	// 阴影效果
	bool m_bCastShadow;		///< 是否投射阴影
	bool m_bReceiveShadow;	///< 是否接收阴影

	// 勾边效果
	bool m_bEdge;
	NiColor m_kEdgeColor;

	// 残影效果
	bool m_bEnableGhost;
	bool m_bRenderGhost;

	// 局部灯光列表
	std::vector<sdLight*> m_kAffectLight;	

	uint		m_uiRenderedFrameId;		///< 渲染帧计数		
	static uint	ms_uiSystemVisiableFrameId;	///< 当前系统可视帧计数
	static uint ms_uiSystemRenderedFrameId;	///< 当前系统渲染帧计数
};
NiSmartPointer(sdEntity);
//-------------------------------------------------------------------------------------------------
typedef std::vector<sdEntityPtr> EntityVec;
typedef std::vector<sdEntityPtr>::iterator EntityVecItr;
typedef std::vector<sdEntityPtr>::reverse_iterator EntityVecRItr;
typedef std::vector<sdEntityPtr>::const_iterator EntityVecConItr;
//-------------------------------------------------------------------------------------------------
#include "sdEntity.inl"
#endif