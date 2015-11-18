//*************************************************************************************************
// ����:	��Ⱦϵͳ�����ӿ�
//---------------------------------------------------------
// ����:		
// ����:		2012-06-29
// ����޸�:
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
// ��Ⱦ����,������Ⱦ��������Ⱦ����
//-------------------------------------------------------------------------------------------------
class sdRenderParams : public NiRefObject
{
public:
	// ��ɫ����Ⱦͨ��,��shader��common_define.h���Ӧ
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

	// ��Ⱦͨ��(��ͼ/����/��Դ/����)
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
		E_RIMLIGHT				= 16,	///< ������
		NUM_LIGHT_CHINNELS		= E_LOCALLIGHT - E_AMBIENTLIGHT + 1,

		NUM_SHADING_CHANNELS	= NUM_MAP_CHANNELS + NUM_LIGHTING_CHINNELS + NUM_LIGHT_CHINNELS,
	};

	// �������
	enum eGameObjectType
	{
		E_BUILDING				= 0,	// ��ͨ��̬���,���罨��
		E_ACTOR					= 1,	// ��̬���,����������
		E_TREELEAF				= 2,	// �ر��ľ
		E_EFFECT				= 3,
		E_TERRAIN				= 4,	// ����
		E_WEAPON				= 5,
		E_WATER					= 6,	// ˮ
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

	// �����������Ⱦͨ������
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
	// ����������Թ���
	int				diffuseAnisotropicOffset;
	int				normalAnisotropicOffset;

	// ����LODƫ��
	float			diffuseLodBiasOffset;
	float			normalLodBiasOffset;

	// ����NormalMapǿ������
	float			normalScaleOffset;

	// ��Ⱦͨ������
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
// ��������
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
// ������Ч����
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
	// AA����
	bool	enableMLAA;
	float	edgeThreshold;	// MLAA��Ե��ȡ��ֵ

	// ColorGrading����
	bool	enableColorGrading;
	float	colorForegroundDepth;
	float	colorBackgroundDepth;
	float	colorSkyDepth;

	// Dof����
	bool	enableDof;
	float	dofNoBlurDepth;
	float	dofFullBlurDepth;
	float	dofSkyDepth;
	float	dofBlurWidth;

	// GodRay����
	bool		enableGodRay;
	NiPoint3	godrayDirection;
	NiColor		godrayColor;
	NiPoint2	godraySize;
	float		godrayIntensity;
	
	// Bloom����
	bool	enableBloom;

	// HDR����
	bool	enableHDR;
	float	hdrAdaptationRate;	// HDR���ȱ仯����
	float	hdrMaxLuminance;
	float	hdrMinAverageLuminance;
	float	hdrToneMapKey;
	float	hdrBloomThreshold;
	float	hdrBloomIntensity;
	float	hdrBloomBlurWidth;

	// SSAO����
	bool	enableSSAO;

	// ����Ч��
	bool	enableRedAlert;
	bool	enableDeadEffect;
};
NiSmartPointer(sdPostProcessParams);
//-------------------------------------------------------------------------------------------------
// ���β���
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
	// �ߴ�
	uint terrainSize;	// HeightMap Size,NormalMap Size
	uint blendMapSize;	// BlendMap Size
	uint tileMapSize;	// TileMap Size

	// ���β�����Ϣ,Material info
	// @{
	Base::Math::sdVector3	ambientMaterial;
	Base::Math::sdVector3	diffuseMaterial;
	Base::Math::sdVector3	specularMaterial;
	float					shiness;
	// @}

	// ����������Ϣ,Texture info
	// @{
	NiTexture*	baseNormalMap;			// �����ر�ķ�����ͼ
	NiTexture*	blendMap;				// �����ر����������ͼ
	NiTexture*	tileMap;				// �����ر����������ͼ���ұ�

	NiTexture*	diffuseAtlasMap;		// ����������������ͼ��
	NiTexture*	diffuseAtlasTableMap;	// ����������������ͼ���ұ�
	Base::Math::sdVector4 diffuseAtlasTableParam;
	
	NiTexture*	normalAtlasMap;			// ����������������ͼ��
	NiTexture*	normalAtlasTableMap;	// ����������������ͼ���ұ�
	Base::Math::sdVector4 normalAtlasTableParam;
	// @}

	// 
	float	minHeight;		///< ���θ߶�ͼ��͸߶�
	float	maxHeight;		///< ���θ߶�ͼ��߸߶�

	// 
	float	terrainFarStart;		// ���ε�"����"��"Զ��"���ֵķֽ����
	float	normalScale;			// ����NormalMap�ķ�������

	// �������Թ���, Anisotropic filter setting
	//	0:		means linear filter
	//	>=1:	means max anisotropic
	uint	diffuseAnisotropic;
	uint	normalAnisotropic;

	// LODƫ��, Mipmap lod bias setting
	// \remark Using it with anisotropic filter won't help visual quality 
	//		but hurt terrain rendering performance.)
	float	diffuseLodBias;
	float	normalLodBias;

	// ����LOD��Ϣ,Material LOD info
	bool	useTriplanar;	// �Ƿ�ʹ������������
	bool	useSpecular;	// �Ƿ�ʹ�ø߹�

	// ������Ϣ,Debug params
	bool	showInvisiableLayers;	///<
	bool	showCurrentLayer;		///< ֻ��ʾ��ǰͼ��
	uint	currentLauerIndex;		///< ��ǰͼ������
	bool	showTileGrid;			///< ��ʾTexTile�߽�
	bool	showWireframe;			///< ��ʾ�߿�ģʽ
};
//-------------------------------------------------------------------------------------------------
// ��Ⱦϵͳ�����ӿ�
// ����ϲ���ԭWZ��IRenderSystem/sdRenderMan/sdRenderSystem������
//	1.һ��Engineֻ������һ��RenderSystem
//  2.һ��RenderSystem�����ж��RenderPathʵ��
//	3.ÿ��RenderPath������RenderPassʵ��,����RenderTarget��Դ��,����ü�Map
//	  ÿ��RenderPass������ȾRenderObject��RenderTarget
//
// RenderSystem��Զ���,����World����,ֻ֪��sdMap,sdEntity
//-------------------------------------------------------------------------------------------------
class IRenderSystem : public Base::sdTSingleton<IRenderSystem>
{
public:
	// ȱʡMesh
	enum eDefaultMesh
	{
		E_DM_SCREEN_QUAD,	///< LeftTop: coord(-1,1), uv(0,0); RightBottom:  coord(1,-1), uv(1,1)
		E_DM_UNIT_CUBE,		///< Center: coord(0,0,0); Size Length 1.f
		E_DM_UNIT_SPHERE,	///< Center: coord(0,0,0); Radius: 0.5f
		E_DM_UNIT_CONE,		///< Center: top point(0,0,0), bottom center(1,0,0), bottom radius: 1.f
		E_DM_UNIT_PYRAMID,	///< Center: top point(0,0,0), bottom center(1,0,0), bottom side length: 2.f
		
		NUM_DEFAULT_MESHES,
	};

	// ȱʡ����
	enum eDefaultMaterial
	{
		E_DM_STATIC_MESH_FLAT_SHADING,		///< ��̬���ƽ����ɫ
		E_DM_STATIC_MESH_WIREFRAME_SHADING,	///< ��̬����߿���ɫ
		E_DM_SKINNED_MESH_FLAT_SHAING,		///< ��������ƽ����ɫ
		E_DM_SKINNED_MESH_WIREFRAME_SHAING,	///< ��������ƽ����ɫ	

		NUM_DEFAULT_MATERIALS,
	};

	// ȱʡ����
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

	// ��ʼ��������
	virtual bool Initialize() = 0;
	virtual void Destroy() = 0;
	virtual bool IsInitialized() = 0;

	// ���л��Ƶĵ�ͼ
	virtual void SetMap(sdMap* pkMap) = 0;
	virtual sdMap* GetMap() = 0;

	// ��Ⱦ���
	virtual void SetCamera(NiCamera* pkCamera) = 0;
	virtual NiCamera* GetCamera() = 0;

	// ��ȾĿ��
	//virtual bool SetRenderTargetGroup(NiRenderTargetGroup* spRenderTargetGroup) = 0;

	// @{
	// @breif ������Ⱦ֡
	// @param[in] fAccumTime
	// @param[in] fFrameTime
	virtual void UpdateFrame(float fAccumTime, float fFrameTime) = 0;

	// @breif ��Ⱦһ֡(��ֵ�Ŀ����Ϊ�������ڲ�����)
	virtual void RenderFrame() = 0;

	// @breif ��Ⱦһ֡(�����ʽ)
	virtual void RenderFrame_Begin() = 0;
	virtual void RenderFrame_Internal() = 0;
	virtual void RenderFrame_End() = 0;

	// @breif ��ʾ��ǰ��Ⱦ֡����Ļ
	virtual void DisplayFrame() = 0;

	// ��������ȾUI,��ȷ����¼�Ƚ��������
	//virtual void RenderUIFrame() = 0;

	// ������Ⱦ�������,��Ҫ���ڱ༭����
	virtual void BeginDirectDraw(NiCamera* pkCamera) = 0;
	virtual void DirectDraw(NiAVObject* pkObject) = 0;
	virtual void DirectDrawImmediate(NiAVObject* pkObject) = 0;
	virtual void EndDirectDraw() = 0;
	//@}

	//@{
	// ��Ⱦ��������,������ȾЧ��
	//@}

	//@{
	// ȱʡ��Ⱦ��Դ
	virtual NiMesh*		GetDefaultMesh(eDefaultMesh eMesh) = 0;
	virtual NiMaterial*	GetDefaultMaterial(eDefaultMaterial eMaterial) = 0;
	virtual NiTexture*	GetDefaultTexture(eDefaultTexture eTexture) = 0;
	//@}

	//@{
	// ��Ⱦ�豸
	//typedef bool (*RESETNOTIFICATIONFUNC)(void* pData);
	//virtual uint AddResetNotificationFunc(RESETNOTIFICATIONFUNC pfnNotify, void* pData) = 0;
	//virtual bool RemoveResetNotification(RESETNOTIFICATIONFUNC pfnNotify) = 0;
	//virtual void CallResetNotificationFunc() = 0;
	//@}

	// ��ȡ��Ⱦϵͳ�ӿ�
	static IRenderSystem* GetRenderSystem(){ return InstancePtr();}
};
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RENDERSYSTEM
ENGINE_NAMESPACE_END_ENGINE
#endif