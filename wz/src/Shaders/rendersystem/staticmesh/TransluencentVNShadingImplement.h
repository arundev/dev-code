//*************************************************************************************************
// 内容:	Translucent StaticMesh的ShadingPass着色器具体实现(已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-19
// 最后修改:
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
#undef GLOWMAP
#undef LOCALLIGHTING

// 用于定义着色器输入输出结构
#undef VS_OUTPUT_VERTEX_NORMAL
#undef VS_OUTPUT_BASE_UV
#undef VS_OUTPUT_UNIQUE_UV
#undef VS_OUTPUT_PROJ_POS

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

#if TEST_CHANNEL(CHANNELS_FLAG, GLOWMAP_CHANNEL)
	#define GLOWMAP 1
#else
	#define GLOWMAP 0
#endif

#define LOCALLIGHTING 0

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
#if (DIFFUSEMAP || NORMALMAP || GLOSSMAP || GLOWMAP)
	#define VS_OUTPUT_BASE_UV	1
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
#else
	float3	vNormal			: NORMAL;
	float3 	vBinormal		: BINORMAL;
	float3	vTangent		: TANGENT;
#endif

#if VS_OUTPUT_BASE_UV
	float2 	vUVSetBase		: TEXCOORD0;
#endif

#if VS_OUTPUT_UNIQUE_UV
	float2 	vUVSetLightMap	: TEXCOORD1;
#endif
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
#define VS_OUTPUT HEADER_KEY(VS_Output, CHANNELS_FLAG)
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;
	float3	vUVSetWorldPos	: TEXCOORD0;
	float3	vUVSetViewPos	: TEXCOORD1;
	
#if VS_OUTPUT_PROJ_POS
	float4	vUVSetProjPos	: TEXCOORD2;
#endif

#if VS_OUTPUT_VERTEX_NORMAL
	float3 	vUVSetViewNormal	: TEXCOORD3;
#else
	float3	vUVSetViewNormal	: TEXCOORD3;
	float3 	vUVSetViewBinormal	: TEXCOORD4;
	float3	vUVSetViewTangent	: TEXCOORD5;
#endif
	
#if VS_OUTPUT_BASE_UV
	float2 	vUVSetBase		: TEXCOORD6;
#endif

#if VS_OUTPUT_UNIQUE_UV
	float2 	vUVSetLightMap	: TEXCOORD7;
#endif
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
#define VS_SHADER HEADER_KEY(VS_Main, CHANNELS_FLAG)
VS_OUTPUT VS_SHADER(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	
	// 投影坐标
	float4 vWorldPos = mul(float4(kInput.vPos, 1.0f), g_mWorld);
	float4 vViewPos = mul(vWorldPos, g_mView);
	float4 vProjPos = mul(vViewPos, g_mProj);
	kOutput.vUVSetWorldPos = vWorldPos.xyz;
	kOutput.vUVSetViewPos = vViewPos.xyz;
	kOutput.vProjPos = vProjPos;
	
#if VS_OUTPUT_PROJ_POS
	kOutput.vUVSetProjPos = vProjPos;
#endif

#if VS_OUTPUT_VERTEX_NORMAL
	kOutput.vUVSetViewNormal = mul(float4(kInput.vNormal, 0.f), g_mWorldView).xyz;
#else
	kOutput.vUVSetViewNormal 	= mul(float4(kInput.vNormal, 0.0f), g_mWorldView).xyz;
	kOutput.vUVSetViewBinormal 	= mul(float4(kInput.vBinormal, 0.0f), g_mWorldView).xyz;
	kOutput.vUVSetViewTangent 	= mul(float4(kInput.vTangent, 0.0f), g_mWorldView).xyz;
#endif

#if VS_OUTPUT_BASE_UV
	kOutput.vUVSetBase = kInput.vUVSetBase;
#endif

#if VS_OUTPUT_UNIQUE_UV
	kOutput.vUVSetLightMap = kInput.vUVSetLightMap * a_vLightMapTransform.zw + a_vLightMapTransform.xy;
#endif

	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
#define PS_SHADER HEADER_KEY(PS_Main, CHANNELS_FLAG)
float4 PS_SHADER(VS_OUTPUT kInput) : COLOR0
{
	float4 vOriginalDiffuseColor = 1.f;
#if DIFFUSEMAP
	vOriginalDiffuseColor = tex2D(sdBaseSampler, kInput.vUVSetBase);
	float3 vDiffuseColor = vOriginalDiffuseColor.rgb * g_vDiffuseMaterialColor;
#else
	float3 vDiffuseColor = g_vDiffuseMaterialColor;
	clip(-1);	// 基纹理还没有被加载则不显示
#endif	
	
#if GLOSSMAP
	float3 vSpecularColor = tex2D(sdGlossSampler,  kInput.vUVSetBase).rgb * g_vSpecularMaterialColor;
#else
	//float3 vSpecularColor = g_vSpecularMaterialColor;
	float3 vSpecularColor = 0.f;
#endif
		
#if GLOWMAP
	float fGlowFactor = tex2D(sdGlowSampler, kInput.vUVSetBase).r;
#endif

#if VS_OUTPUT_PROJ_POS
	float2 vUVSetScreen = GetTexCoordForPixel(kInput.vUVSetProjPos.xy / kInput.vUVSetProjPos.w);
#endif

#if VS_OUTPUT_VERTEX_NORMAL
	//float4 vGeomData 		= tex2D(sdGeomBufferSampler, vUVSetScreen);
	//float3 vViewNormal 	= UnpackNormal(vGeomData.zw);
	//float3 vViewNormal 	= normalize(kInput.vUVSetViewNormal);
	float3 vViewNormal 		= kInput.vUVSetViewNormal;
#else
	float3 vNormalTex 	= tex2D(sdNormalSampler, kInput.vUVSetBase).xyz;
	vNormalTex 			= (vNormalTex - 0.5f) * 2.0f;
	vNormalTex.xy		*= g_fStaticMeshNormalScale;
	vNormalTex			= normalize(vNormalTex);
	//kInput.vUVSetViewNormal 	= normalize(kInput.vUVSetViewNormal);
	//kInput.vUVSetViewBinormal = normalize(kInput.vUVSetViewBinormal);
	//kInput.vUVSetViewTangent = normalize(kInput.vUVSetViewTangent);	
	float3 vViewNormal = mul(vNormalTex, float3x3(kInput.vUVSetViewTangent, kInput.vUVSetViewBinormal, kInput.vUVSetViewNormal));
#endif

	float fDarkMap = 1.f;
	
#if LIGHTMAP
	float fLightMap = dot(tex2D(sdDetailSampler, kInput.vUVSetLightMap), a_vLightMapChannel);
#else
	float fLightMap = 1.f;
#endif

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
	
	// calc fog
	float4 vFogColorAndFactor = CalcHeightFogCurveFactor(sdCurveFogBufferSampler, float4(kInput.vUVSetWorldPos, kInput.vUVSetViewPos.z), false);
	vColor.rgb = vFogColorAndFactor.rgb + vFogColorAndFactor.a * vColor;
	
	return float4(vColor, vOriginalDiffuseColor.a * g_fAlpha);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
#define TECHNIQUE HEADER_KEY(TranslucentVNShading, CHANNELS_FLAG)
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