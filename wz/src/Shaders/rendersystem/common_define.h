//*************************************************************************************************
// ����:	������ɫ���Ĺ�������/�궨��/����
//			������Ԥ�����������
//---------------------------------------------------------
// ����:		
// ����:		2012-07-10
// ����޸�:
//*************************************************************************************************
#ifndef _COMMON_DEFINE_H__
#define _COMMON_DEFINE_H__

//*****************************************************************************
// ���Բ���
// float4 	g_DebugParam			: GLOBAL;

//*****************************************************************************
// GB�ڽ�����
//*****************************************************************************
//-----------------------------------------------------------------------------
// �任����
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
// Ӳ����������
#if defined (DIRECT3D)
	#define BONEMATRIX_TYPE	float4x3
#else
	#define BONEMATRIX_TYPE	float3x4
#endif

static const int MAX_BONES = 32;

BONEMATRIX_TYPE	g_mSkinnedBoneMatrix3[MAX_BONES] : SKINBONEMATRIX3;

//-----------------------------------------------------------------------------
// ���
float4 		g_vCameraPostion		: EYEPOS;	// �����������	
float4		g_vCameraUp				: GLOBAL;	// ���������̬
float4		g_vCameraRight			: GLOBAL;

float4		g_vCameraFrustum		: GLOBAL;	// �����׶��(���,���,��,Զ)
float		g_fCameraFarClip		: GLOBAL;	// ���Զ�ü���

float4x4	g_mDepthToWorld			: GLOBAL;
#define		g_vViewPos				g_mDepthToWorld._m30_m31_m32

float4x4	g_mDepthToView			: GLOBAL;

//-----------------------------------------------------------------------------
// ����
float2		g_vPixelSize			: GLOBAL;	// ���سߴ�
float2		g_vHalfPixelOffset		: GLOBAL;	// ������ƫ��
float2		g_vPixToTexOffset		: GLOBAL;	// ͶӰ���굽��Ļ���������ƫ��
float2		g_vWindowSize			: GLOBAL;	// ���ڳߴ�

//-----------------------------------------------------------------------------
// ʱ��
float		g_fTime					: GLOBAL;

//-----------------------------------------------------------------------------
// ������ɫ
float4		g_vWireframeColor		: GLOBAL = float4(1.0f, 1.0f, 1.0f, 1.0f);
float4		g_vSolidColor			: GLOBAL = float4(0.75f, 0.75f, 0.75f, 1.0f);
float4 		g_vFlatShadingColor 	: GLOBAL = float4(1.0f, 0, 0, 1.0f);

//*****************************************************************************
// ��������(����/ȫ����)
//*****************************************************************************
// ������
float3		g_vTerraimAmbientColor		: GLOBAL;
float3		g_vSkyAmbientColor			: GLOBAL;

// �������
float3		g_vMainLightDir				: GLOBAL;
float3		g_vMainLightColor			: GLOBAL;
float3 		g_vMainLightAmbientColor	: GLOBAL;

// ���������
float3		g_vAssistLightDir			: GLOBAL;
float3		g_vAssistLightColor			: GLOBAL;
float3		g_vAssistLightAmbientColor	: GLOBAL;

// �ڵ�����
float4		g_vLightFactor				: GLOBAL;
#define		g_fAmbientLightAOFactor		g_vLightFactor.x
#define		g_fMainLightAOFactor		g_vLightFactor.y
#define		g_fAssistLightAOFactor		g_vLightFactor.z
#define		g_fMainLightLightMapFactor	g_vLightFactor.w

// ��֪����ʲô
float		a_fMainLightOcclusion		: GLOBAL = 1.0f;

// rimlight���Ʋ���(��ɫ�����ƹ�)
// [fresnel-eta][fresnel-power][brightness]
// [color.r][color.g][color.b]
// [dir.x][dir.y][dir.z]
float3x3	a_mRimLightParam			: ATTRIBUTE;
#define 	a_fRimLightEta				a_mRimLightParam[0][0]
#define 	a_fRimLightPower			a_mRimLightParam[0][1]	
#define 	a_fRimBrightness			a_mRimLightParam[0][2]
#define 	a_vRimLightColor			float3(a_mRimLightParam[1][0], a_mRimLightParam[1][1], a_mRimLightParam[1][2])	
#define 	a_vRimLightDirection		float3(a_mRimLightParam[2][0], a_mRimLightParam[2][1], a_mRimLightParam[2][2])

// ��ɫת�Ҷȵ�ϵ��
// (�ο�http://blog.csdn.net/xueyong1203/article/details/2214311)
#define		g_vLuminScale				float3(0.299f, 0.587f, 0.114f)		// Digital CCIR601, ITU-CCR 601
#define		g_vLuminScale2				float3(0.2125f, 0.7154f, 0.0721f)	// ��һ��
#define		g_vLuminScale3				float3(0.2126f, 0.7152f, 0.0722f)	// Photometric/digital ITU-R Recommendation BT. 709

// ���صƹⷶΧ[0, 4.0f]
#define		g_fLocalLightRange			4.0f							
#define		g_fLocalLightScale			(1.0f / g_fLocalLightRange)	

//
#define		PI							3.1415926535

//*****************************************************************************
// ������Ч
//*****************************************************************************
// Gamma
//float		g_fGammaValue				: GLOBAL;
//float		g_fFinalGammaValue			: GLOBAL;
//bool		g_bUseToneMapping			: GLOBAL;
//bool		g_bUseColorGrading			: GLOBAL;
//bool		g_bUseDepthColoeGrading		: GLOBAL;

//-----------------------------------------------------------------------------
// ȫ����
float4		g_vFogColorDensity			: GLOBAL;
float4		g_vFogStartEnd				: GLOBAL;
float4		g_vFogDensityParams			: GLOBAL;
float		g_fFogColorFactor			: GLOBAL;
#define		g_vFogColor					g_vFogColorDensity.rgb	// ȫ������ɫ
#define		g_fFogDensity				g_vFogColorDensity.a	// ȫ����ǿ��
#define		g_fFogDistanceScale			g_vFogColorDensity.a
#define		g_fFogStartDistance			g_vFogStartEnd.x		// ȫ�����ƽ��
#define		g_fFogEndDistance			g_vFogStartEnd.y		// ȫ����Զƽ��
#define		g_fFogStartHeight			g_vFogStartEnd.z		// ȫ������ʼ�߶�
#define		g_fFogEndHeight				g_vFogStartEnd.w		// ȫ������ֹ�߶�
#define		g_fFogExtinctionDistance	5000.0f
#define		g_fFogCameraFar				2500.0f
#define		g_fFogSkyDensity			g_vFogDensityParams.x

//*****************************************************************************
// ����
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

// ����Shaderִ��AlphaTest�Ĳο�ֵ
float 		g_fAlphaTestRef				: GLOBAL;

//
float 		g_fEdgeEnhancement			: GLOBAL;

// (��Shader����)
float4 		g_vDiffuseMapSize			: GLOBAL;
float2		g_vDensityThreshold			: GLOBAL;

//*****************************************************************************
// �������
//*****************************************************************************
float4		g_vNormalScale				: GLOBAL;
#define		g_fStaticMeshNormalScale	(g_vNormalScale.x)
#define 	g_fLeafcardNormalScale		(g_vNormalScale.y)
#define 	g_fTerrainNormalScale		(g_vNormalScale.z)
#define 	g_fRoadNormalScale			(g_vNormalScale.w)

// �༭��״̬��
float3		g_vVertexColorMask			: GLOBAL;
float3		g_vDiffuseMapMask			: GLOBAL;	// �༭��״̬��,���DiffuseMap����
float		g_fLightMapMask				: GLOBAL;	// �༭��״̬��,���LightMap����
float		g_fGlossMapMask				: GLOBAL;	// �༭��״̬��,���GlossMap����

// ����任
float4x4	g_mBaseMapTextureTransform		: TexTransformBase;
float4x4	g_mDarkMapTextureTransform		: TexTransformDark;
float4x4	g_mDetailMapTextureTransform	: TexTransformDetail;
float4x4	g_mGlowMapTextureTransform		: TexTransformGlow;

//*****************************************************************************
// ��Ⱦͨ��
//*****************************************************************************
//
//-----------------------------------------------------------------------------
// ͨ����Ⱦͨ��,������ͼ/����/��Դ/����
#define DIFFUSEMAP_CHANNEL			0x00000001	// ������ͨ��,ʹ��GB��BaseMap
#define NORMALMAP_CHANNEL			0x00000002	// ������ͼͨ��,ʹ��GB��NormalMap
#define GLOSSMAP_CHANNEL			0x00000004	// �߹���ͼͨ��,ʹ��GB��NormalMap
#define LIGHTMAP_CHANNEL			0x00000008	// ������ͼͨ��
#define AOMAP_CHANNEL				0x00000008	// �ڵ���ͼ
#define FILTERMAP_CHANNEL			0x00000008	
#define GLOWMAP_CHANNEL				0x00000010	// �Թ���ͼͨ��
#define DETAILMAP_CHANNEL			0x00000020	// ϸ��������ͼͨ��
#define DETAIL_NORMAL_CHANNEL		0x00000040	// ϸ����ͼͨ��,ʹ��GB��ParallaxMap
#define ENVMAP_CHANNEL				0x00000080	// ������ͼͨ��,ʹ��GB��BumpMap

#define	SPECULAR_LIGHTTING_CHANNEL	0x00000004	// �߹�,������GlossMapͬʱ����
#define	RIM_LIGHTTING_CHANNEL		0x00000008	// ������,������LightMapͬʱ����
#define	GLOW_LIGHTTING_CHANNEL		0x00000010	// �Թ�,������GlowMapͬʱ����
#define ALPHA_TEST_FLAG				0x00000020	// ͸������,������DetailMapͬʱ����
#define VERTEX_COLOR_CHANNEL		0x00000040	// ����ɫ,������DetailNormalMapͬʱ����

//#define	EMISSIVE_LIGHTTING_CHANNEL	0x00000000	// �Է���
//#define	AMBIENT_LIGHTTING_CHANNEL	0x00000000	// ������
//#define	DIFFUSE_LIGHTTING_CHANNEL	0x00000000	// �����
//#define	SPECULAR_LIGHTTING_CHANNEL	0x00000000	// �߹�

//#define	AMBIENT_LIGHT_CHANNEL		0x00000000	// ȫ�ֻ�����Դ
//#define	MAIN_LIGHT_CHANNEL			0x00000000	// ȫ������Դ
//#define	ASSIST_LIGHT_CHANNEL		0x00000000	// ȫ�ָ�����Դ
//#define	LOCAL_LIGHT_CHANNEL			0x00000000	// �ֲ���Դ


// ǰ����ɫͨ����Ⱦͨ��
#define FR_ALPHATEST_CHANNEL	0x00000001
#define FR_NORMALMAP_CHANNEL	0x00000002
#define FR_GLOSSMAP_CHANNEL		0x00000004
#define FR_GLOWMAP_CHANNEL		0x00000008
#define FR_PURECOLOR_CHANNEL	0x00000010
#define FR_HARDWARESKIN_CHANNEL	0x00000020
#define FR_ENVMAP_CHANNEL		0x00000040

// ���ڱ�Ǻ�����Ч
//#define	FINALPROCESS_DOF			0x00000001
//#define	FINALPROCESS_CURVED_DOF		0x00000002
//#define	FINALPROCESS_GODRAY			0x00000004
//#define	FINALPROCESS_COLOR_GRADING 	0x00000008
//
//#define	COPY_RED_ALERT				0x00000001
//#define	COPY_DEAD_EFFECT			0x00000002

// ����Effect��Ⱦ
#define	EFFECT_DARKMAP			0x00000001
#define	EFFECT_DETAILMAP		0x00000002
#define	EFFECT_BASEUVANI		0x00000004
#define	EFFECT_ALPHAADD			0x00000008
#define	EFFECT_HAEDWARESKIN		0x00000010
#define	EFFECT_VERTEXCOLOR		0x00000020
#define	EFFECT_SOFTPD			0x00000040

// ���ڸ���flag��̬���ɱ�����������֮���
#define HEADER_KEY(header, flag)	header##flag

// ���������ַ���
#define LINK_MACRO(a,b)	a##b

// ���������ַ���
#define MAKE_STRING(a)	#a
 
// �����ж�flag��ָ��λ�Ƿ�����
#define TEST_CHANNEL(flag, channel)	(((flag / channel) / 2) != ((flag / channel + 1) / 2))
//*****************************************************************************
#endif