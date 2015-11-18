//*************************************************************************************************
// 内容:	地形着色器(已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-22
// 最后修改:	2013-05-02
//*************************************************************************************************
#include "terrain_common.h"

//---------------------------------------------------------------------------------------
// 全局变量
//---------------------------------------------------------------------------------------
//float3 	g_vEyePos			: EYEPOS;
//float		g_fTime				: GLOBAL;
//int		g_iTerrainSize		: GLOBAL;
//float		g_fTerrainMinHeight	: GLOBAL;
//float		g_fTerrainMaxHeight	: GLOBAL;

//float4x4	g_mViewProj			: VIEWPROJECTION;

// [origin_x][origin_y][scale][morphy_factor]
//float2x4	a_mTerrainQuadParam	: ATTRIBUTE;
//#define	QUAD_ORIGIN			float3(a_mTerrainQuadParam[0][0], a_mTerrainQuadParam[0][1], 0.f)
//#define	QUAD_SCALE			a_mTerrainQuadParam[0][2]
//#define	QUAD_MORPH_FACTOR	a_mTerrainQuadParam[0][3]

float4		g_vTerrainPickColor	: GLOBAL = float4(1.f, 1.f, 1.f, 1.f);

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
texture sdTerrainBaseTex
<
	string NTM = "base";
	int NTMIndex = 0;
>;

sampler sdTerrainBaseSampler = sampler_state
{
	Texture = (sdTerrainBaseTex);
	AddressU = MIRROR;
	AddressV = MIRROR;	
	AddressW = MIRROR;	
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	SRGBTexture = true;
};

texture sdTerrainNormalTex
<
	string NTM = "normal";
	int NTMIndex = 0;
>;

sampler sdTerrainNormalSampler = sampler_state
{
	Texture = (sdTerrainNormalTex);
	AddressU = WRAP;
	AddressV = WRAP;	
	AddressW = WRAP;	
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	SRGBTexture = false;
};

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos			: POSITION;
	float2	vZDisplacement	: TEXCOORD0;
//	float2	vGradient		: TEXCOORD1;
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;
	float2	vUVSet			: TEXCOORD0;
	float2	vUVSetNormalMap	: TEXCOORD1;
//	float2	vUVSetGradient	: TEXCOORD2;
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	
	float3 vWorldPosition = 0.f;
	vWorldPosition.xy 	= kInput.vPos.xy * a_fQuadScale + a_vQuadOrigin;
	vWorldPosition.z	= lerp(kInput.vZDisplacement.x, kInput.vZDisplacement.y, a_fQuadMorphFactor);
	
	kOutput.vProjPos = mul(float4(vWorldPosition, 1.f), g_mViewProj);
	kOutput.vUVSet = vWorldPosition.xy / 10.f;
	kOutput.vUVSetNormalMap = vWorldPosition.xy / (g_fTerrainSize - 1.f);
//	kOutput.vUVSetGradient = float2(1.f, 1.f);
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	float3 vNormal = tex2D(sdTerrainNormalSampler, kInput.vUVSetNormalMap).xyz;
	vNormal = (vNormal - 0.5f) * 2.f;
	//vNormal = normalize(vNormal);
	//vNormal.z = sqrt(1.f - dot(vNormal.xy, vNormal.xy));
	
	float3 vLightVec = normalize(float3(0.5f, 0.5f, 0.5f));
	float3 vDiffuseColor = float3(0.8f, 0.8f, 0.8f) * max(dot(vNormal, vLightVec), 0.f) + float3(0.3f, 0.3f, 0.3f);
	
	float3 vBaseTex = tex2D(sdTerrainBaseSampler, kInput.vUVSet).rgb;
	
	return float4(vDiffuseColor * vBaseTex, 1.f);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique Terrain_MeshQuad
<
	string Description = "Terrain_MeshQuad";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main ();
		
		//ZEnable = true;
		//ZWriteEnable = true;
	}
}
//---------------------------------------------------------------------------------------