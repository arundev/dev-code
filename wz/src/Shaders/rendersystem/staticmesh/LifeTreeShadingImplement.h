//*************************************************************************************************
// 内容:	StaticMesh的生命之树特殊处理,带DetailMap动画
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-19
// 最后修改:	2013-05-30
//*************************************************************************************************
#include "static_mesh_common.h"

//---------------------------------------------------------------------------------------
// 取消一些内部会用到的预定义宏
//---------------------------------------------------------------------------------------
#undef VS_INPUT
#undef VS_OUTPUT
#undef PS_OUTPUT
#undef VS_SHADER
#undef PS_SHADER
#undef TECHNIQUE

// 用于定义通道启用情况
#undef DIFFUSEMAP
#undef NORMALMAP
#undef GLOSSMAP
#undef LIGHTMAP
#undef LOCALLIGHTING
#undef DETAILMAP

// 用于定义着色器输入输出结构
#undef VS_OUTPUT_VERTEX_NORMAL
#undef VS_OUTPUT_BASE_UV
#undef VS_OUTPUT_UNIQUE_UV

//---------------------------------------------------------------------------------------
// 根据CHANNELS_FLAG的值来提取当前启用的通道情况
//---------------------------------------------------------------------------------------
#if TEST_CHANNEL(CHANNELS_FLAG, DIFFUSEMAP_CHANNEL)
	#define DIFFUSEMAP 1
#else
	#define DIFFUSEMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, NORMALMAP_CHANNEL)
	#define NORMALMAP 1
#else
	#define NORMALMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, GLOSSMAP_CHANNEL)
	#define GLOSSMAP 1
#else
	#define GLOSSMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, LIGHTMAP_CHANNEL)
	#define LIGHTMAP 1
#else
	#define LIGHTMAP 0
#endif

#define LOCALLIGHTING 	0
#define DETAILMAP 		1
	
//---------------------------------------------------------------------------------------
// 根据当前启用的通道情况来决定具体的组合情况
//---------------------------------------------------------------------------------------
// 如果没有NormalMap则要输出顶点法线
#if (NORMALMAP)
	#define VS_OUTPUT_VERTEX_NORMAL	0
#else
	#define VS_OUTPUT_VERTEX_NORMAL 1
#endif

// DiffuseMap/NormalMap/GlossMap共用纹理坐标
#if (DIFFUSEMAP || NORMALMAP || GLOSSMAP || DETAILMAP)
	#define VS_OUTPUT_BASE_UV 1
#else
	#define VS_OUTPUT_BASE_UV 0
#endif

// LightMap使用单独的纹理坐标
#if (LIGHTMAP)
	#define VS_OUTPUT_UNIQUE_UV 1
#else
	#define VS_OUTPUT_UNIQUE_UV 0
#endif

#if (NORMALMAP || LOCALLIGHTING)
	#define VS_OUTPUT_PROJ_POS 1
#else
	#define VS_OUTPUT_PROJ_POS 0
#endif

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
#define VS_INPUT HEADER_KEY(VS_Input, CHANNELS_FLAG)
struct VS_INPUT
{
	float3 	vPos			: POSITION;
	
#if VS_OUTPUT_VERTEX_NORMAL
	float3  vNormal			: NORMAL;
#endif

#if VS_OUTPUT_BASE_UV
	float2 	vUVSetBase		: TEXCOORD0;
#endif

#if VS_OUTPUT_UNIQUE_UV
	float2 	vUVSetLightMap	: TEXCOORD1;
#endif

//	float2	vUVSetDetailMap	: TEXCOORD2;
};
	
//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
#define VS_OUTPUT HEADER_KEY(VS_Output, CHANNELS_FLAG)
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;
	float3	vUVSetViewPos	: TEXCOORD0;
	
#if VS_OUTPUT_PROJ_POS
	float4	vUVSetProjPos	: TEXCOORD1;
#endif

#if VS_OUTPUT_VERTEX_NORMAL
	float3 	vUVSetViewNormal : TEXCOORD2;
#endif
	
#if VS_OUTPUT_BASE_UV
	float2 	vUVSetBase		: TEXCOORD3;
#endif

#if VS_OUTPUT_UNIQUE_UV
	float2 	vUVSetLightMap	: TEXCOORD4;
#endif

	float2	vUVSetDetailMap	: TEXCOORD5;
};
	
//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
#define VS_SHADER HEADER_KEY(VS_Main, CHANNELS_FLAG)
VS_OUTPUT VS_SHADER(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	
	// 坐标
	float4 vViewPos = mul(float4(kInput.vPos, 1.0f), g_mWorldView);
	float4 vProjPos = mul(vViewPos, g_mProj);
	kOutput.vUVSetViewPos = vViewPos.xyz;
	kOutput.vProjPos = vProjPos;
	
#if VS_OUTPUT_PROJ_POS
	kOutput.vUVSetProjPos = vProjPos;
#endif

#if VS_OUTPUT_VERTEX_NORMAL
	kOutput.vUVSetViewNormal = mul(float4(kInput.vNormal, 0.f), g_mWorldView).xyz;
#endif

#if VS_OUTPUT_BASE_UV
	kOutput.vUVSetBase = kInput.vUVSetBase;
#endif

#if VS_OUTPUT_UNIQUE_UV
	kOutput.vUVSetLightMap = kInput.vUVSetLightMap * a_vLightMapTransform.zw + a_vLightMapTransform.xy;
#endif

	kOutput.vUVSetDetailMap = mul(float4(kInput.vUVSetBase, 0.f, 1.f), g_mDetailMapTextureTransform).xy;

	return kOutput;
}	
	
//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
#define PS_SHADER HEADER_KEY(PS_Main, CHANNELS_FLAG)
float4 PS_SHADER(VS_OUTPUT kInput) : COLOR0
{
#if DIFFUSEMAP
	float3 vDiffuseColor = tex2D(sdBaseSampler, kInput.vUVSetBase).rgb * g_vDiffuseMaterialColor;
#else
	float3 vDiffuseColor = g_vDiffuseMaterialColor;
#endif	
	
#if GLOSSMAP
	float3 vSpecularColor = tex2D(sdGlossSampler,  kInput.vUVSetBase).rgb * g_vSpecularMaterialColor;
#else
	float3 vSpecularColor = g_vSpecularMaterialColor;
#endif

#if VS_OUTPUT_PROJ_POS
	float2 vUVSetScreen = GetTexCoordForPixel(kInput.vUVSetProjPos.xy / kInput.vUVSetProjPos.w);
#endif

#if NORMALMAP
	float4 vGeomData 	= tex2D(sdGeomBufferSampler, vUVSetScreen);
	float3 vViewNormal 	= UnpackNormal(vGeomData.zw);
#else
	float3 vViewNormal 	= normalize(kInput.vUVSetViewNormal);
#endif

	float fDarkMap = 1.f;
	
#if LIGHTMAP
	float fLightMap = dot(tex2D(sdDetailSampler, kInput.vUVSetLightMap), a_vLightMapChannel);
#else
	float fLightMap = 1.f;
#endif

	float3 vDetailColor = tex2D(sdDetailSampler, kInput.vUVSetDetailMap).rgb;
	vDiffuseColor *= (2.f * vDetailColor);

#if LOCALLIGHTING
	float4 vLocalLight = tex2D(sdLightBufferSampler, vUVSetScreen) * g_fLocalLightRadio;
#else
	float4 vLocalLight = 0.f;
#endif

	// 光照计算
	float3	vDiffuseLight;
	float3	vSpecularLight;
	float3	vViewVector = -normalize(kInput.vUVSetViewPos);
	AccumLighting(vViewVector, vViewNormal, g_fShiness, fDarkMap, fLightMap, vLocalLight, vDiffuseLight, vSpecularLight);

	// 合成光照
	float3 vColor = vDiffuseColor * (vDiffuseLight + g_vEmissiveMaterialColor) + vSpecularColor * vSpecularLight;

	return float4(vColor, 0.f);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
#define TECHNIQUE HEADER_KEY(LifeTreeShading, CHANNELS_FLAG)
technique TECHNIQUE
<
	string Description = MAKE_STRING(TECHNIQUE);
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_SHADER ();
		PixelShader 	= compile ps_2_a PS_SHADER ();
	}
}
//---------------------------------------------------------------------------------------