//*************************************************************************************************
// 内容:	渲染系统基础接口
//---------------------------------------------------------
// 作者:		
// 创建:		2012-06-29
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RENDER_SYSTEM_RENDER_SYSTEM_H__
#define _SD_ENGINE_RENDER_SYSTEM_RENDER_SYSTEM_H__
#include <sdVector3.h>
#include <sdVector4.h>
#include <sdSingleton.h>

class sdMap;

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RENDERSYSTEM
//-------------------------------------------------------------------------------------------------
// 渲染参数,控制渲染策略与渲染质量
//-------------------------------------------------------------------------------------------------
class sdRenderParams : public NiRefObject
{
public:
	// 着色器渲染通道,与shader的common_define.h相对应
	enum eShaderChannel
	{
		E_SHADER_DIFFUSEMAP			= 0,	///< 0x00000001
		E_SHADER_NORMALMAP			= 1,	///< 0x00000002
		E_SHADER_GLOSSMAP			= 2,	///< 0x00000004
		E_SHADER_LIGHTMAP			= 3,	///< 0x00000008
		E_SHADER_AOMAP				= E_SHADER_LIGHTMAP,
		E_SHADER_FILTERMAP			= E_SHADER_LIGHTMAP,
		E_SHADER_GLOWMAP			= 4,	///< 0x00000010
		E_SHADER_DETAILMAP			= 5,	///< 0x00000020
		E_SHADER_DETAILNORMALMAP	= 6,	///< 0x00000040
		E_SHADER_ENVIRONMENTMAP		= 7,	///< 0x00000080

		E_SHADER_SEPCULAT_LIGHTTING	= E_SHADER_GLOSSMAP,		
		E_SHADER_RIM_LIGHTTIN		= E_SHADER_LIGHTMAP,
		E_SHADER_GLOW_LIGHTTING		= E_SHADER_GLOWMAP,
		E_SHADER_ALPHA_TEST			= E_SHADER_DETAILMAP,
		E_SHADER_VERTEX_COLOR		= E_SHADER_DETAILNORMALMAP,
	};

	// 渲染通道(贴图/光照/光源/其他)
	enum eShadingChannel
	{
		E_DIFFUSEMAP			= 0,	///< 0x00000001
		E_NORMALMAP				= 1,	///< 0x00000002
		E_GLOSSMAP				= 2,	///< 0x00000004
		E_LIGHTMAP				= 3,	///< 0x00000008
		E_AOMAP					= E_LIGHTMAP,
		E_FILTERMAP				= E_LIGHTMAP,	///< for light and projector use
		E_GLOWMAP				= 4,	///< 0x00000010
		E_DETAILMAP				= 5,	///< 0x00000020
		E_DETAILNORMALMAP		= 6,	///< 0x00000040
		E_ENVIRONMENTMAP		= 7,	///< 0x00000080
		NUM_MAP_CHANNELS		= E_ENVIRONMENTMAP - E_DIFFUSEMAP + 1,

		E_EMISSIVELIGHTTING		= 8,
		E_AMBIENTLIGHTTING		= 9,
		E_DIFFUSELIGHTING		= 10,
		E_SPECULARLIGHTING		= 11,
		NUM_LIGHTING_CHINNELS	= E_SPECULARLIGHTING - E_EMISSIVELIGHTTING + 1,

		E_AMBIENTLIGHT			= 12,
		E_MAINLIGHT				= 13,
		E_ASSISTLIGHT			= 14,
		E_LOCALLIGHT			= 15,
		E_RIMLIGHT				= 16,	///< 轮廓光
		NUM_LIGHT_CHINNELS		= E_LOCALLIGHT - E_AMBIENTLIGHT + 1,

		NUM_SHADING_CHANNELS	= NUM_MAP_CHANNELS + NUM_LIGHTING_CHINNELS + NUM_LIGHT_CHINNELS,
	};

	// 物件类型
	enum eGameObjectType
	{
		E_BUILDING				= 0,	// 普通静态物件,例如建筑
		E_ACTOR					= 1,	// 动态物件,带骨骼动画
		E_TREELEAF				= 2,	// 地表灌木
		E_EFFECT				= 3,
		E_TERRAIN				= 4,	// 地形
		E_WEAPON				= 5,
		E_WATER					= 6,	// 水
		E_GODRAY				= 7,
		E_FOG					= 8,
		NUM_GAMEOBJECT_TYPE		= 9
	};

	sdRenderParams() { Reset();}
	inline void Reset()
	{
		diffuseAnisotropicOffset = 0;
		normalAnisotropicOffset = 0;

		diffuseLodBiasOffset = 0.0f;
		normalLodBiasOffset = 0.0f;

		normalScaleOffset = 0.0f;

		buildingChannelMask = 0xffffffff;
		actorChannelMask = 0xffffffff;
		terrainChannelMask = 0xffffffff;

		densityMode = false;
		densityMaxThreshold = 40000.0f;
		densityMinThreshold = 10000.0f;
	}

	// 设置与访问渲染通道掩码
	inline void EnableChannel(eGameObjectType eObjectType, eShadingChannel eChannel, bool bEnable)
	{
		NIASSERT(eObjectType >= 0 && eObjectType < NUM_GAMEOBJECT_TYPE);
		NIASSERT(eChannel >= 0 && eChannel < NUM_SHADING_CHANNELS);

		if (E_BUILDING == eObjectType)
		{
			if(bEnable)	buildingChannelMask |= (1 << eChannel);
			else		buildingChannelMask &= ~(1 << eChannel);
		}
		else if (E_ACTOR == eObjectType)
		{
			if(bEnable)	actorChannelMask |= (1 << eChannel);
			else		actorChannelMask &= ~(1 << eChannel);
		}
		else if (E_TERRAIN == eObjectType)
		{
			if(bEnable)	terrainChannelMask |= (1 << eChannel);
			else		terrainChannelMask &= ~(1 << eChannel);
		}
	}

	inline bool IsEnableChannel(eGameObjectType eObjectType, eShadingChannel eChannel)
	{
		NIASSERT(eObjectType >= 0 && eObjectType < NUM_GAMEOBJECT_TYPE);
		NIASSERT(eChannel >= 0 && eChannel < NUM_SHADING_CHANNELS);

		if (E_BUILDING == eObjectType)
		{
			return 0 != (buildingChannelMask & (1 << eChannel));
		}
		else if (E_ACTOR == eObjectType)
		{
			return 0 != (actorChannelMask & (1 << eChannel));
		}
		else if (E_TERRAIN == eObjectType)
		{
			return 0 !=	(terrainChannelMask & (1 << eChannel));
		}

		return false;
	}

public:
	// 整体各向异性过滤
	int				diffuseAnisotropicOffset;
	int				normalAnisotropicOffset;

	// 整体LOD偏移
	float			diffuseLodBiasOffset;
	float			normalLodBiasOffset;

	// 整体NormalMap强度缩放
	float			normalScaleOffset;

	// 渲染通道掩码
	unsigned int	buildingChannelMask;
	unsigned int	actorChannelMask;
	unsigned int	terrainChannelMask;

	//
	bool			densityMode;
	float			densityMinThreshold;
	float			densityMaxThreshold;
};
NiSmartPointer(sdRenderParams);
//-------------------------------------------------------------------------------------------------
// 环境参数
//-------------------------------------------------------------------------------------------------
class sdEnvironmentParams : public NiRefObject
{
public:
	sdEnvironmentParams() { Reset();}
	inline void Reset()
	{
		fogColor = NiColor::BLACK;
		fogStart = 0.0f;
		fogEnd	= 1.0f;
		fogDensity = 0.001f;
		fogHeightStart = -100.0f;
		fogHeightEnd   = 500.0f;
		fogColorDensity = 1.0f;

		skyFogColor = NiColor::BLACK;
		skyFogTop = 1.0f;
		skyFogBottom = 0.0f;
		skyFogDensity = 0.0f;
	}

public:
	NiColor			fogColor;
	float			fogStart;
	float			fogEnd;
	float			fogHeightStart;
	float			fogHeightEnd;
	float			fogColorDensity;
	float			fogSkyDensity;
	float			fogDensity;

	NiColor			skyFogColor;
	float			skyFogTop;
	float			skyFogBottom;
	float			skyFogDensity;
};
NiSmartPointer(sdEnvironmentParams);
//-------------------------------------------------------------------------------------------------
// 后期特效参数
//-------------------------------------------------------------------------------------------------
class sdPostProcessParams : public NiRefObject
{
public:
	sdPostProcessParams() { Reset();}
	inline void Reset()
	{
		enableMLAA = false;
		edgeThreshold = 0.1f;

		enableColorGrading = false;
		colorForegroundDepth = 0.f;
		colorBackgroundDepth = 500.f;
		colorSkyDepth = 0.65f;

		enableDof = false;
		dofNoBlurDepth = 0.f;
		dofFullBlurDepth = 291.793f;
		dofSkyDepth = 1.f;
		dofBlurWidth = 0.941f;

		enableGodRay = false;
		godrayDirection = NiPoint3(0.705f, 0.109f, 0.268f);
		godrayDirection.Unitize();
		godrayColor = NiColor(0.84f, 0.56f, 0.32f);
		godraySize = NiPoint2(0.4f, 0.4f);
		godrayIntensity = 0.7f;

		enableHDR = false;
		hdrAdaptationRate = 0.5f;
		hdrMaxLuminance = 2.f;
		hdrMinAverageLuminance = 0.35f;
		hdrToneMapKey = 0.8f;
		hdrBloomThreshold = 0.85f;
		hdrBloomIntensity = 1.f;
		hdrBloomBlurWidth = 2.f;

		enableRedAlert = false;
		enableDeadEffect = false;
	}

public:
	// AA参数
	bool	enableMLAA;
	float	edgeThreshold;	// MLAA边缘提取阈值

	// ColorGrading参数
	bool	enableColorGrading;
	float	colorForegroundDepth;
	float	colorBackgroundDepth;
	float	colorSkyDepth;

	// Dof参数
	bool	enableDof;
	float	dofNoBlurDepth;
	float	dofFullBlurDepth;
	float	dofSkyDepth;
	float	dofBlurWidth;

	// GodRay参数
	bool		enableGodRay;
	NiPoint3	godrayDirection;
	NiColor		godrayColor;
	NiPoint2	godraySize;
	float		godrayIntensity;
	
	// Bloom参数
	bool	enableBloom;

	// HDR参数
	bool	enableHDR;
	float	hdrAdaptationRate;	// HDR亮度变化速率
	float	hdrMaxLuminance;
	float	hdrMinAverageLuminance;
	float	hdrToneMapKey;
	float	hdrBloomThreshold;
	float	hdrBloomIntensity;
	float	hdrBloomBlurWidth;

	// SSAO参数
	bool	enableSSAO;

	// 特殊效果
	bool	enableRedAlert;
	bool	enableDeadEffect;
};
NiSmartPointer(sdPostProcessParams);
//-------------------------------------------------------------------------------------------------
// 地形参数
//-------------------------------------------------------------------------------------------------
class sdTerrainParams : public NiRefObject
{
public:
	sdTerrainParams() { Reset();}
	inline void Reset()
	{
		terrainSize = 1024;
		blendMapSize = 1024;
		tileMapSize = 512;

		ambientMaterial = Base::Math::sdVector3::UNIT_SCALE;
		diffuseMaterial = Base::Math::sdVector3::UNIT_SCALE;
		specularMaterial = Base::Math::sdVector3::UNIT_SCALE;
		shiness = 32.f;

		baseNormalMap = NULL;
		blendMap = NULL;
		tileMap = NULL;

		diffuseAtlasMap = NULL;
		diffuseAtlasTableMap = NULL;

		normalAtlasMap = NULL;
		normalAtlasTableMap = NULL;

		minHeight = 0.f;
		maxHeight = 0.f;

		terrainFarStart = 64.f;
		normalScale = 1.f;

		diffuseAnisotropic = 0;
		normalAnisotropic = 0;

		diffuseLodBias = 0.f;
		normalLodBias = 0.f;

		useTriplanar = false;
		useSpecular = true;

		showInvisiableLayers = false;
		showCurrentLayer = false;
		currentLauerIndex = 0;
		showTileGrid = false;
		showWireframe = false;
	}

public:
	// 尺寸
	uint terrainSize;	// HeightMap Size,NormalMap Size
	uint blendMapSize;	// BlendMap Size
	uint tileMapSize;	// TileMap Size

	// 地形材质信息,Material info
	// @{
	Base::Math::sdVector3	ambientMaterial;
	Base::Math::sdVector3	diffuseMaterial;
	Base::Math::sdVector3	specularMaterial;
	float					shiness;
	// @}

	// 地形纹理信息,Texture info
	// @{
	NiTexture*	baseNormalMap;			// 整个地表的法线贴图
	NiTexture*	blendMap;				// 整个地表的纹理混合贴图
	NiTexture*	tileMap;				// 整个地表的纹理混合贴图查找表

	NiTexture*	diffuseAtlasMap;		// 整个地形漫反射贴图集
	NiTexture*	diffuseAtlasTableMap;	// 整个地形漫反射贴图查找表
	Base::Math::sdVector4 diffuseAtlasTableParam;
	
	NiTexture*	normalAtlasMap;			// 整个地形漫反射贴图集
	NiTexture*	normalAtlasTableMap;	// 整个地形漫反射贴图查找表
	Base::Math::sdVector4 normalAtlasTableParam;
	// @}

	// 
	float	minHeight;		///< 地形高度图最低高度
	float	maxHeight;		///< 地形高度图最高高度

	// 
	float	terrainFarStart;		// 地形的"近处"与"远处"部分的分界距离
	float	normalScale;			// 地形NormalMap的法线缩放

	// 各向异性过滤, Anisotropic filter setting
	//	0:		means linear filter
	//	>=1:	means max anisotropic
	uint	diffuseAnisotropic;
	uint	normalAnisotropic;

	// LOD偏置, Mipmap lod bias setting
	// \remark Using it with anisotropic filter won't help visual quality 
	//		but hurt terrain rendering performance.)
	float	diffuseLodBias;
	float	normalLodBias;

	// 材质LOD信息,Material LOD info
	bool	useTriplanar;	// 是否使用立方体纹理
	bool	useSpecular;	// 是否使用高光

	// 调试信息,Debug params
	bool	showInvisiableLayers;	///<
	bool	showCurrentLayer;		///< 只显示当前图层
	uint	currentLauerIndex;		///< 当前图层索引
	bool	showTileGrid;			///< 显示TexTile边界
	bool	showWireframe;			///< 显示线框模式
};
//-------------------------------------------------------------------------------------------------
// 渲染系统基础接口
// 这里合并了原WZ的IRenderSystem/sdRenderMan/sdRenderSystem三个类
//	1.一个Engine只可以有一个RenderSystem
//  2.一个RenderSystem可以有多个RenderPath实例
//	3.每个RenderPath管理多个RenderPass实例,掌握RenderTarget资源集,负责裁剪Map
//	  每个RenderPass根据渲染RenderObject到RenderTarget
//
// RenderSystem相对独立,对于World部分,只知道sdMap,sdEntity
//-------------------------------------------------------------------------------------------------
class IRenderSystem : public Base::sdTSingleton<IRenderSystem>
{
public:
	// 缺省Mesh
	enum eDefaultMesh
	{
		E_DM_SCREEN_QUAD,	///< LeftTop: coord(-1,1), uv(0,0); RightBottom:  coord(1,-1), uv(1,1)
		E_DM_UNIT_CUBE,		///< Center: coord(0,0,0); Size Length 1.f
		E_DM_UNIT_SPHERE,	///< Center: coord(0,0,0); Radius: 0.5f
		E_DM_UNIT_CONE,		///< Center: top point(0,0,0), bottom center(1,0,0), bottom radius: 1.f
		E_DM_UNIT_PYRAMID,	///< Center: top point(0,0,0), bottom center(1,0,0), bottom side length: 2.f
		
		NUM_DEFAULT_MESHES,
	};

	// 缺省材质
	enum eDefaultMaterial
	{
		E_DM_STATIC_MESH_FLAT_SHADING,		///< 静态物件平面着色
		E_DM_STATIC_MESH_WIREFRAME_SHADING,	///< 静态物件线框着色
		E_DM_SKINNED_MESH_FLAT_SHAING,		///< 骨骼对象平面着色
		E_DM_SKINNED_MESH_WIREFRAME_SHAING,	///< 骨骼对象平面着色	

		NUM_DEFAULT_MATERIALS,
	};

	// 缺省纹理
	enum eDefaultTexture
	{
		E_DT_ERROE_TEXTURE,
		E_DT_BLACK_TEXTURE,
		E_DT_WHITE_TEXTURE,
		E_DT_GRAY75_TEXTURE,
		E_DT_CHESSBOARD_TEXTURE,
		E_DT_UP_NORMALMAP,
		E_DT_RANDOM_TEXTURE,

		NUM_DEFAULT_TEXTURES,
	};

	virtual ~IRenderSystem() {};

	// 初始化与销毁
	virtual bool Initialize() = 0;
	virtual void Destroy() = 0;
	virtual bool IsInitialized() = 0;

	// 进行绘制的地图
	virtual void SetMap(sdMap* pkMap) = 0;
	virtual sdMap* GetMap() = 0;

	// 渲染相机
	virtual void SetCamera(NiCamera* pkCamera) = 0;
	virtual NiCamera* GetCamera() = 0;

	// 渲染目标
	//virtual bool SetRenderTargetGroup(NiRenderTargetGroup* spRenderTargetGroup) = 0;

	// @{
	// @breif 更新渲染帧
	// @param[in] fAccumTime
	// @param[in] fFrameTime
	virtual void UpdateFrame(float fAccumTime, float fFrameTime) = 0;

	// @breif 渲染一帧(拆分的目的是为了允许内部加入)
	virtual void RenderFrame() = 0;

	// @breif 渲染一帧(拆分形式)
	virtual void RenderFrame_Begin() = 0;
	virtual void RenderFrame_Internal() = 0;
	virtual void RenderFrame_End() = 0;

	// @breif 显示当前渲染帧到屏幕
	virtual void DisplayFrame() = 0;

	// 单独的渲染UI,以确保登录等界面的流畅
	//virtual void RenderUIFrame() = 0;

	// 单独渲染其他物件,主要用于编辑器等
	virtual void BeginDirectDraw(NiCamera* pkCamera) = 0;
	virtual void DirectDraw(NiAVObject* pkObject) = 0;
	virtual void DirectDrawImmediate(NiAVObject* pkObject) = 0;
	virtual void EndDirectDraw() = 0;
	//@}

	//@{
	// 渲染属性设置,调整渲染效果
	//@}

	//@{
	// 缺省渲染资源
	virtual NiMesh*		GetDefaultMesh(eDefaultMesh eMesh) = 0;
	virtual NiMaterial*	GetDefaultMaterial(eDefaultMaterial eMaterial) = 0;
	virtual NiTexture*	GetDefaultTexture(eDefaultTexture eTexture) = 0;
	//@}

	//@{
	// 渲染设备
	//typedef bool (*RESETNOTIFICATIONFUNC)(void* pData);
	//virtual uint AddResetNotificationFunc(RESETNOTIFICATIONFUNC pfnNotify, void* pData) = 0;
	//virtual bool RemoveResetNotification(RESETNOTIFICATIONFUNC pfnNotify) = 0;
	//virtual void CallResetNotificationFunc() = 0;
	//@}

	// 获取渲染系统接口
	static IRenderSystem* GetRenderSystem(){ return InstancePtr();}
};
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif