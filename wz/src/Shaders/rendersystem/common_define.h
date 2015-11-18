//*************************************************************************************************
// 内容:	所有着色器的公共变量/宏定义/函数
//			这里是预定义变量部分
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-10
// 最后修改:
//*************************************************************************************************
#ifndef _COMMON_DEFINE_H__
#define _COMMON_DEFINE_H__

//*****************************************************************************
// 调试参数
// float4 	g_DebugParam			: GLOBAL;

//*****************************************************************************
// GB内建参数
//*****************************************************************************
//-----------------------------------------------------------------------------
// 变换矩阵
float4x4	g_mWorld				: WORLD;
float4x4	g_mInvWorld				: INVWORLD;

float4x4	g_mView					: VIEW;
float4x4	g_mInvView				: INVVIEW;
float4x4	g_mInvViewT				: VIEWIT;

float4x4	g_mProj					: PROJ;
float4x4	g_mInvProj				: INVPROJ;

float4x4	g_mViewProj				: VIEWPROJECTION;
float4x4	g_mInvViewProj			: INVVIEWPROJ;

float4x4	g_mWorldView			: WORLDVIEW;
float4x4	g_mInvWorldView			: INVWORLDVIEW;

float4x4	g_mWorldViewProj		: WORLDVIEWPROJ;
float4x4	g_mInvWorldViewProj		: INVWORLDVIEWPROJ;

//-----------------------------------------------------------------------------
// 硬件骨骼矩阵
#if defined (DIRECT3D)
	#define BONEMATRIX_TYPE	float4x3
#else
	#define BONEMATRIX_TYPE	float3x4
#endif

static const int MAX_BONES = 32;

BONEMATRIX_TYPE	g_mSkinnedBoneMatrix3[MAX_BONES] : SKINBONEMATRIX3;

//-----------------------------------------------------------------------------
// 相机
float4 		g_vCameraPostion		: EYEPOS;	// 相机世界坐标	
float4		g_vCameraUp				: GLOBAL;	// 相机世界姿态
float4		g_vCameraRight			: GLOBAL;

float4		g_vCameraFrustum		: GLOBAL;	// 相机视锥体(半宽,半高,近,远)
float		g_fCameraFarClip		: GLOBAL;	// 相机远裁剪面

float4x4	g_mDepthToWorld			: GLOBAL;
#define		g_vViewPos				g_mDepthToWorld._m30_m31_m32

float4x4	g_mDepthToView			: GLOBAL;

//-----------------------------------------------------------------------------
// 窗口
float2		g_vPixelSize			: GLOBAL;	// 像素尺寸
float2		g_vHalfPixelOffset		: GLOBAL;	// 半像素偏移
float2		g_vPixToTexOffset		: GLOBAL;	// 投影坐标到屏幕纹理坐标的偏移
float2		g_vWindowSize			: GLOBAL;	// 窗口尺寸

//-----------------------------------------------------------------------------
// 时间
float		g_fTime					: GLOBAL;

//-----------------------------------------------------------------------------
// 调试着色
float4		g_vWireframeColor		: GLOBAL = float4(1.0f, 1.0f, 1.0f, 1.0f);
float4		g_vSolidColor			: GLOBAL = float4(0.75f, 0.75f, 0.75f, 1.0f);
float4 		g_vFlatShadingColor 	: GLOBAL = float4(1.0f, 0, 0, 1.0f);

//*****************************************************************************
// 环境变量(光照/全局雾)
//*****************************************************************************
// 环境光
float3		g_vTerraimAmbientColor		: GLOBAL;
float3		g_vSkyAmbientColor			: GLOBAL;

// 主方向光
float3		g_vMainLightDir				: GLOBAL;
float3		g_vMainLightColor			: GLOBAL;
float3 		g_vMainLightAmbientColor	: GLOBAL;

// 辅助方向光
float3		g_vAssistLightDir			: GLOBAL;
float3		g_vAssistLightColor			: GLOBAL;
float3		g_vAssistLightAmbientColor	: GLOBAL;

// 遮挡因子
float4		g_vLightFactor				: GLOBAL;
#define		g_fAmbientLightAOFactor		g_vLightFactor.x
#define		g_fMainLightAOFactor		g_vLightFactor.y
#define		g_fAssistLightAOFactor		g_vLightFactor.z
#define		g_fMainLightLightMapFactor	g_vLightFactor.w

// 不知道是什么
float		a_fMainLightOcclusion		: GLOBAL = 1.0f;

// rimlight控制参数(角色轮廓灯光)
// [fresnel-eta][fresnel-power][brightness]
// [color.r][color.g][color.b]
// [dir.x][dir.y][dir.z]
float3x3	a_mRimLightParam			: ATTRIBUTE;
#define 	a_fRimLightEta				a_mRimLightParam[0][0]
#define 	a_fRimLightPower			a_mRimLightParam[0][1]	
#define 	a_fRimBrightness			a_mRimLightParam[0][2]
#define 	a_vRimLightColor			float3(a_mRimLightParam[1][0], a_mRimLightParam[1][1], a_mRimLightParam[1][2])	
#define 	a_vRimLightDirection		float3(a_mRimLightParam[2][0], a_mRimLightParam[2][1], a_mRimLightParam[2][2])

// 彩色转灰度的系数
// (参考http://blog.csdn.net/xueyong1203/article/details/2214311)
#define		g_vLuminScale				float3(0.299f, 0.587f, 0.114f)		// Digital CCIR601, ITU-CCR 601
#define		g_vLuminScale2				float3(0.2125f, 0.7154f, 0.0721f)	// 另一组
#define		g_vLuminScale3				float3(0.2126f, 0.7152f, 0.0722f)	// Photometric/digital ITU-R Recommendation BT. 709

// 本地灯光范围[0, 4.0f]
#define		g_fLocalLightRange			4.0f							
#define		g_fLocalLightScale			(1.0f / g_fLocalLightRange)	

//
#define		PI							3.1415926535

//*****************************************************************************
// 后期特效
//*****************************************************************************
// Gamma
//float		g_fGammaValue				: GLOBAL;
//float		g_fFinalGammaValue			: GLOBAL;
//bool		g_bUseToneMapping			: GLOBAL;
//bool		g_bUseColorGrading			: GLOBAL;
//bool		g_bUseDepthColoeGrading		: GLOBAL;

//-----------------------------------------------------------------------------
// 全局雾
float4		g_vFogColorDensity			: GLOBAL;
float4		g_vFogStartEnd				: GLOBAL;
float4		g_vFogDensityParams			: GLOBAL;
float		g_fFogColorFactor			: GLOBAL;
#define		g_vFogColor					g_vFogColorDensity.rgb	// 全局雾颜色
#define		g_fFogDensity				g_vFogColorDensity.a	// 全局雾强度
#define		g_fFogDistanceScale			g_vFogColorDensity.a
#define		g_fFogStartDistance			g_vFogStartEnd.x		// 全局雾近平面
#define		g_fFogEndDistance			g_vFogStartEnd.y		// 全局雾远平面
#define		g_fFogStartHeight			g_vFogStartEnd.z		// 全局雾起始高度
#define		g_fFogEndHeight				g_vFogStartEnd.w		// 全局雾终止高度
#define		g_fFogExtinctionDistance	5000.0f
#define		g_fFogCameraFar				2500.0f
#define		g_fFogSkyDensity			g_vFogDensityParams.x

//*****************************************************************************
// 材质
//*****************************************************************************
float4 		g_vAmbientMaterial			: GLOBAL = float4(1.0f, 1.0f, 1.0f, 1.0f);

float4 		g_vDiffuseMaterial			: GLOBAL = float4(1.0f, 1.0f, 1.0f, 1.0f);
#define		g_vDiffuseMaterialColor		g_vDiffuseMaterial.rgb
#define		g_fLocalLightRadio			g_vDiffuseMaterial.a
#define		g_fAlpha					g_vDiffuseMaterial.a

float4 		g_vSpecularMaterial			: GLOBAL = float4(1.0f, 1.0f, 1.0f, 1.0f);
#define		g_vSpecularMaterialColor	g_vSpecularMaterial.rgb
#define 	g_fShiness					g_vSpecularMaterial.a

float4 		g_vEmissiveMaterial			: GLOBAL = float4(0.0f, 0.0f, 0.0f, 0.0f);
#define		g_vEmissiveMaterialColor	g_vEmissiveMaterial.rgb

// 用于Shader执行AlphaTest的参考值
float 		g_fAlphaTestRef				: GLOBAL;

//
float 		g_fEdgeEnhancement			: GLOBAL;

// (旧Shader参数)
float4 		g_vDiffuseMapSize			: GLOBAL;
float2		g_vDensityThreshold			: GLOBAL;

//*****************************************************************************
// 纹理参数
//*****************************************************************************
float4		g_vNormalScale				: GLOBAL;
#define		g_fStaticMeshNormalScale	(g_vNormalScale.x)
#define 	g_fLeafcardNormalScale		(g_vNormalScale.y)
#define 	g_fTerrainNormalScale		(g_vNormalScale.z)
#define 	g_fRoadNormalScale			(g_vNormalScale.w)

// 编辑器状态下
float3		g_vVertexColorMask			: GLOBAL;
float3		g_vDiffuseMapMask			: GLOBAL;	// 编辑器状态下,最低DiffuseMap亮度
float		g_fLightMapMask				: GLOBAL;	// 编辑器状态下,最低LightMap亮度
float		g_fGlossMapMask				: GLOBAL;	// 编辑器状态下,最低GlossMap亮度

// 纹理变换
float4x4	g_mBaseMapTextureTransform		: TexTransformBase;
float4x4	g_mDarkMapTextureTransform		: TexTransformDark;
float4x4	g_mDetailMapTextureTransform	: TexTransformDetail;
float4x4	g_mGlowMapTextureTransform		: TexTransformGlow;

//*****************************************************************************
// 渲染通道
//*****************************************************************************
//
//-----------------------------------------------------------------------------
// 通用渲染通道,包括贴图/光照/光源/其他
#define DIFFUSEMAP_CHANNEL			0x00000001	// 漫反射通道,使用GB的BaseMap
#define NORMALMAP_CHANNEL			0x00000002	// 法线贴图通道,使用GB的NormalMap
#define GLOSSMAP_CHANNEL			0x00000004	// 高光贴图通道,使用GB的NormalMap
#define LIGHTMAP_CHANNEL			0x00000008	// 光照贴图通道
#define AOMAP_CHANNEL				0x00000008	// 遮挡贴图
#define FILTERMAP_CHANNEL			0x00000008	
#define GLOWMAP_CHANNEL				0x00000010	// 辉光贴图通道
#define DETAILMAP_CHANNEL			0x00000020	// 细节纹理贴图通道
#define DETAIL_NORMAL_CHANNEL		0x00000040	// 细节贴图通道,使用GB的ParallaxMap
#define ENVMAP_CHANNEL				0x00000080	// 环境贴图通道,使用GB的BumpMap

#define	SPECULAR_LIGHTTING_CHANNEL	0x00000004	// 高光,不会与GlossMap同时存在
#define	RIM_LIGHTTING_CHANNEL		0x00000008	// 轮廓光,不会与LightMap同时存在
#define	GLOW_LIGHTTING_CHANNEL		0x00000010	// 辉光,不会与GlowMap同时存在
#define ALPHA_TEST_FLAG				0x00000020	// 透明测试,不会与DetailMap同时存在
#define VERTEX_COLOR_CHANNEL		0x00000040	// 顶点色,不会与DetailNormalMap同时存在

//#define	EMISSIVE_LIGHTTING_CHANNEL	0x00000000	// 自发光
//#define	AMBIENT_LIGHTTING_CHANNEL	0x00000000	// 环境光
//#define	DIFFUSE_LIGHTTING_CHANNEL	0x00000000	// 漫射光
//#define	SPECULAR_LIGHTTING_CHANNEL	0x00000000	// 高光

//#define	AMBIENT_LIGHT_CHANNEL		0x00000000	// 全局环境光源
//#define	MAIN_LIGHT_CHANNEL			0x00000000	// 全局主光源
//#define	ASSIST_LIGHT_CHANNEL		0x00000000	// 全局辅助光源
//#define	LOCAL_LIGHT_CHANNEL			0x00000000	// 局部光源


// 前向着色通用渲染通道
#define FR_ALPHATEST_CHANNEL	0x00000001
#define FR_NORMALMAP_CHANNEL	0x00000002
#define FR_GLOSSMAP_CHANNEL		0x00000004
#define FR_GLOWMAP_CHANNEL		0x00000008
#define FR_PURECOLOR_CHANNEL	0x00000010
#define FR_HARDWARESKIN_CHANNEL	0x00000020
#define FR_ENVMAP_CHANNEL		0x00000040

// 用于标记后期特效
//#define	FINALPROCESS_DOF			0x00000001
//#define	FINALPROCESS_CURVED_DOF		0x00000002
//#define	FINALPROCESS_GODRAY			0x00000004
//#define	FINALPROCESS_COLOR_GRADING 	0x00000008
//
//#define	COPY_RED_ALERT				0x00000001
//#define	COPY_DEAD_EFFECT			0x00000002

// 用于Effect渲染
#define	EFFECT_DARKMAP			0x00000001
#define	EFFECT_DETAILMAP		0x00000002
#define	EFFECT_BASEUVANI		0x00000004
#define	EFFECT_ALPHAADD			0x00000008
#define	EFFECT_HAEDWARESKIN		0x00000010
#define	EFFECT_VERTEXCOLOR		0x00000020
#define	EFFECT_SOFTPD			0x00000040

// 用于根据flag动态生成变量名函数名之类的
#define HEADER_KEY(header, flag)	header##flag

// 用于连接字符串
#define LINK_MACRO(a,b)	a##b

// 用于生成字符串
#define MAKE_STRING(a)	#a
 
// 用于判定flag的指定位是否启用
#define TEST_CHANNEL(flag, channel)	(((flag / channel) / 2) != ((flag / channel + 1) / 2))
//*****************************************************************************
#endif