//*************************************************************************************************
// 内容:	Translucent SkinnedMesh的ShadingPass着色器具体实现(已废弃)
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

#define LOCALLIGHTING 0

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
	float3	vBlendWeights	: BLENDWEIGHT0;
	int4	vBlendIndices	: BLENDINDICES0;
	
#if VS_OUTPUT_VERTEX_NORMAL
	float3  vNormal			: NORMAL;
#else
	float3	vNormal			: NORMAL;
	float3 	vBinormal		: BINORMAL;
	float3	vTangent		: TANGENT;
#endif

#if VS_OUTPUT_BASE_UV
	float2 	vUVSetBase			: TEXCOORD0;
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
	float3 	vUVSetViewNormal 	: TEXCOORD3;
#else
	float3	vUVSetViewNormal	: TEXCOORD3;
	float3 	vUVSetViewBinormal	: TEXCOORD4;
	float3	vUVSetViewTangent	: TEXCOORD5;
#endif
	
#if VS_OUTPUT_BASE_UV
	float2 	vUVSetBase		: TEXCOORD6;
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

	mSkinnedBoneTransform = mul(mSkinnedBoneTransform, g_mView);
#if VS_OUTPUT_VERTEX_NORMAL
	kOutput.vUVSetViewNormal = mul(float4(kInput.vNormal, 0.f), mSkinnedBoneTransform).xyz;
#else
	TransformNBT(kInput.vNormal, kInput.vBinormal, kInput.vTangent, mSkinnedBoneTransform,
		kOutput.vUVSetViewNormal, kOutput.vUVSetViewBinormal, kOutput.vUVSetViewTangent);
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
	float3 vViewNormal 		= kInput.vUVSetViewNormal;
#else
	float3 vNormalTex 	= tex2D(sdNormalSampler, kInput.vUVSetBase).xyz;
	vNormalTex 			= (vNormalTex - 0.5f) * 2.0f;
	vNormalTex.xy		*= g_fStaticMeshNormalScale;
	vNormalTex			= normalize(vNormalTex);
	kInput.vUVSetViewNormal 	= normalize(kInput.vUVSetViewNormal);
	kInput.vUVSetViewBinormal = normalize(kInput.vUVSetViewBinormal);
	kInput.vUVSetViewTangent = normalize(kInput.vUVSetViewTangent);	
	float3 vViewNormal = mul(vNormalTex, float3x3(kInput.vUVSetViewTangent, kInput.vUVSetViewBinormal, kInput.vUVSetViewNormal));
#endif

	float4 vLocalLight = 0.f;

	// 光照计算
	float3	vDiffuseLight;
	float3	vSpecularLight;
	float3	vViewVector = -normalize(kInput.vUVSetViewPos);
	AccumLighting(vViewVector, vViewNormal, g_fShiness, a_fMainLightOcclusion, vLocalLight, vDiffuseLight, vSpecularLight);

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
#define TECHNIQUE HEADER_KEY(TranslucentVNSkinnedShading, CHANNELS_FLAG)
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