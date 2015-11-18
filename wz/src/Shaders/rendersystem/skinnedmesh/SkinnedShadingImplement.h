//*************************************************************************************************
// 内容:	SkinnedMesh的MRTGeometryPass着色器具体实现(V1版,已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-21
// 最后修改:
//*************************************************************************************************
#include "skinned_mesh_common.h"

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
#undef RIMLIGHT

// 用于定义着色器输入输出结构
#undef VS_OUTPUT_VERTEX_NORMAL
#undef VS_OUTPUT_BASE_UV
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

#if TEST_CHANNEL(CHANNELS_FLAG, RIM_LIGHTTING_CHANNEL)
	#define RIMLIGHT 1
#else
	#define RIMLIGHT 0
#endif

#define LOCALLIGHTING 1

//---------------------------------------------------------------------------------------
// 根据当前启用的通道情况来决定具体的组合情况
//---------------------------------------------------------------------------------------
#if (!NORMALMAP)
	#define VS_OUTPUT_VERTEX_NORMAL	1
#else
	#define VS_OUTPUT_VERTEX_NORMAL 0
#endif

#if (DIFFUSEMAP || NORMALMAP || GLOSSMAP)
	#define VS_OUTPUT_BASE_UV	1
#else
	#define VS_OUTPUT_BASE_UV 0
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
	float3	vBlendWeights	: BLENDWEIGHT0;
	int4	vBlendIndices	: BLENDINDICES0;
	
#if VS_OUTPUT_VERTEX_NORMAL
	float3  vNormal			: NORMAL;
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
#define VS_OUTPUT_VERTEX_NORMAL_REG_OFFSET 	(VS_OUTPUT_PROJ_POS + 1)
#define VS_OUTPUT_UV_REG_OFFSET				(VS_OUTPUT_PROJ_POS + VS_OUTPUT_VERTEX_NORMAL_REG_OFFSET + 1)
#define NUM_VS_OUTPUTUV						VS_OUTPUT_BASEUV

#define VS_OUTPUT HEADER_KEY(VS_Output, CHANNELS_FLAG)
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;
	float3	vUVSetViewPos	: TEXCOORD0;
	float3	vUVSetWorldPos	: TEXCOORD1;
	
#if VS_OUTPUT_PROJ_POS
	float4	vUVSetProjPos	: TEXCOORD2;
  #if VS_OUTPUT_VERTEX_NORMAL
	float3 	vUVSetViewNormal : TEXCOORD3;
  #endif
#else
  #if VS_OUTPUT_VERTEX_NORMAL
	float3 	vUVSetViewNormal : TEXCOORD2;
  #endif	
#endif

#undef VS_OUTPUT_UV0_OFFSET
#if (VS_OUTPUT_PROJ_POS + VS_OUTPUT_VERTEX_NORMAL) == 0
	#define VS_OUTPUT_UV0_OFFSET 2
#elif (VS_OUTPUT_PROJ_POS + VS_OUTPUT_VERTEX_NORMAL) == 1
	#define VS_OUTPUT_UV0_OFFSET 3
#elif (VS_OUTPUT_PROJ_POS + VS_OUTPUT_VERTEX_NORMAL) == 2
	#define VS_OUTPUT_UV0_OFFSET 4
#endif
	
#if VS_OUTPUT_BASE_UV
	float2 	vUVSetBase		: LINK_MACRO(TEXCOORD, VS_OUTPUT_UV0_OFFSET);
#endif
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
#define VS_SHADER HEADER_KEY(VS_Main, CHANNELS_FLAG)
VS_OUTPUT VS_SHADER(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	
	float4 vWorldPos;
	float4x4 mSkinnedBoneTransform;
	TransformSkinnedPosition(kInput.vPos, kInput.vBlendIndices, kInput.vBlendWeights,
		g_mSkinnedBoneMatrix3, vWorldPos, mSkinnedBoneTransform);
	kOutput.vProjPos = mul(vWorldPos, g_mViewProj);
	kOutput.vUVSetWorldPos = vWorldPos.xyz;
	kOutput.vUVSetViewPos = mul(vWorldPos, g_mView).xyz;
	
#if VS_OUTPUT_PROJ_POS
	kOutput.vUVSetProjPos = kOutput.vProjPos;
#endif

#if VS_OUTPUT_VERTEX_NORMAL
	mSkinnedBoneTransform = mul(mSkinnedBoneTransform, g_mView);
	kOutput.vUVSetViewNormal = mul(float4(kInput.vNormal, 0.f), mSkinnedBoneTransform).xyz;
#endif	

#if VS_OUTPUT_BASE_UV
	kOutput.vUVSetBase = kInput.vUVSetBase;
#endif

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
	float3 vSpecularColor = 0.f;
#endif
		
#if GLOWMAP
	float3 vEmissiveColor = tex2D(sdGlowSampler, kInput.vUVSetBase).rgb * g_vEmissiveMaterialColor;
#else
	float3 vEmissiveColor = g_vEmissiveMaterialColor;
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

#if LOCALLIGHTING
	//float4 vLocalLight = tex2D(sdLightBufferSampler, vUVSetScreen) * g_fLocalLightRadio;
	float4 vLocalLight = tex2D(sdLightBufferSampler, vUVSetScreen);
#else
	float4 vLocalLight = 0.f;
#endif

	// 光照计算
	float3	vDiffuseLight;
	float3	vSpecularLight;
	float3	vViewVector = -normalize(kInput.vUVSetViewPos);
	AccumLighting(vViewVector, vViewNormal, g_fShiness, a_fMainLightOcclusion, vLocalLight, vDiffuseLight, vSpecularLight);
	
#ifdef RIMLIGHT
	float factor = ((1.0f - a_fRimLightEta) * (1.0f - a_fRimLightEta)) /  ((1.0f + a_fRimLightEta) * (1.0f + a_fRimLightEta));
	float fresnel = factor + (1.0f - factor) * pow((1.0f - saturate(dot(vViewVector, vViewNormal))), a_fRimLightPower);
	float anglecos = (dot(vViewVector, g_vMainLightDir) + 1.0f)/ 2.0f;
	float rimdiffuse = clamp(anglecos, 0.35f, 0.7f);
	float brightness = dot(g_vMainLightColor, float3(0.2125f, 0.7154f, 0.0721f));
	float3 rimcolor = g_vMainLightColor / brightness;
	float3 vRimLight = fresnel * clamp(brightness * 1.8f, 0.2f, 0.35f) * rimcolor * rimdiffuse;
#else
	float3 vRimLight = 0.0f;
#endif	
	
	// 合成光照
	float3 vColor = vDiffuseColor * (vDiffuseLight + vEmissiveColor) + vSpecularColor * vSpecularLight + vRimLight;
	
	// calc fog
	float4 vFogColorAndFactor = CalcHeightFogCurveFactor(sdCurveFogBufferSampler, float4(kInput.vUVSetWorldPos, kInput.vUVSetViewPos.z), false);
	vColor.rgb = vFogColorAndFactor.rgb + vFogColorAndFactor.a * vColor;
	
	return float4(vColor, 0.f);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
#define TECHNIQUE HEADER_KEY(SkinnedMesh_DeferredShading, CHANNELS_FLAG)
technique TECHNIQUE
<
	string Description = MAKE_STRING(TECHNIQUE);
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_SHADER ();
		PixelShader 	= compile ps_3_0 PS_SHADER ();
	}
}
//---------------------------------------------------------------------------------------