//*************************************************************************************************
// 内容:	ForwardShadingPass着色器实现(原始实现,已废弃)
// 作者:		
// 创建:		2012-11-19
// 最后修改:	2013-05-28
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
#undef ALPHATEST
#undef NORMALMAP
#undef GLOSSMAP
#undef GLOWMAP
#undef PURECOLOR
#undef HARDWARESKIN
#undef ENVMAP

// 用于定义着色器输入输出结构
#undef VS_OUTPUT_VERTEX_NORMAL

//---------------------------------------------------------------------------------------
// 根据CHANNELS_FLAG的值来提取当前启用的通道情况
//---------------------------------------------------------------------------------------
#if TEST_CHANNEL(CHANNELS_FLAG, FR_ALPHATEST_CHANNEL)
	#define ALPHATEST 1
#else
	#define ALPHATEST 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, FR_NORMALMAP_CHANNEL)
	#define NORMALMAP 1
#else
	#define NORMALMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, FR_GLOSSMAP_CHANNEL)
	#define GLOSSMAP 1
#else
	#define GLOSSMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, FR_GLOWMAP_CHANNEL)
	#define GLOWMAP 1
#else
	#define GLOWMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, FR_PURECOLOR_CHANNEL)
	#define PURECOLOR 1
#else
	#define PURECOLOR 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, FR_HARDWARESKIN_CHANNEL)
	#define HARDWARESKIN 1
#else
	#define HARDWARESKIN 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, FR_ENVMAP_CHANNEL)
	#define ENVMAP 1
#else
	#define ENVMAP 0
#endif

//---------------------------------------------------------------------------------------
// 根据当前启用的通道情况来决定具体的组合情况
//---------------------------------------------------------------------------------------
// 如果没有NormalMap则要输出顶点法线
#if (NORMALMAP)
	#define VS_OUTPUT_VERTEX_NORMAL	0
#else
	#define VS_OUTPUT_VERTEX_NORMAL 1
#endif

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
#define VS_INPUT HEADER_KEY(VS_Input, CHANNELS_FLAG)
struct VS_INPUT
{
	float3 	vPos			: POSITION;
	
#if HARDWARESKIN
	float3	vBlendWeights	: BLENDWEIGHT0;
	int4	vBlendIndices	: BLENDINDICES0;
#endif
	
#if !PRUECOLOR
#if VS_OUTPUT_VERTEX_NORMAL
	float3  vNormal			: NORMAL;
#else
	float3	vNormal			: NORMAL;
	float3 	vBinormal		: BINORMAL;
	float3	vTangent		: TANGENT;
#endif
#endif

	float2 	vUVSetBase		: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
#define VS_OUTPUT HEADER_KEY(VS_Output, CHANNELS_FLAG)
struct VS_OUTPUT
{
	float4	vProjPos			: POSITION;
	float2 	vUVSetBase			: TEXCOORD0;

#if !PRUECOLOR
	float3	vUVSetViewPos		: TEXCOORD1;
  #if VS_OUTPUT_VERTEX_NORMAL
	float3 	vUVSetViewNormal  	: TEXCOORD2;
  #else
	float3	vUVSetViewNormal  	: TEXCOORD2;
	float3 	vUVSetViewBinormal	: TEXCOORD3;
	float3	vUVSetViewTangent 	: TEXCOORD4;
  #endif
#endif

#if ENVMAP
	float2 	vUVSetEnvMap		: TEXCOORD5;
#endif
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
#define VS_SHADER HEADER_KEY(VS_Main, CHANNELS_FLAG)
VS_OUTPUT VS_SHADER(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	
#if HARDWARESKIN
	float4 vWorldPos;
	float4x4 mSkinBoneTransform;
	TransformSkinnedPosition(kInput.vPos, kInput.vBlendIndices, kInput.vBlendWeights,
		g_mSkinnedBoneMatrix3, vWorldPos, mSkinBoneTransform);
	kOutput.vProjPos = mul(vWorldPos, g_mViewProj);
#else
	float4 vWorldPos = mul(float4(kInput.vPos, 1.f), g_mWorld);
	kOutput.vProjPos = mul(vWorldPos, g_mViewProj);
#endif
	
#if !PRUECOLOR
	kOutput.vUVSetViewPos = mul(vWorldPos, g_mView).xyz;

	#if HARDWARESKIN
		mSkinBoneTransform = mul(mSkinBoneTransform, g_mView);
		#if VS_OUTPUT_VERTEX_NORMAL
			kOutput.vUVSetViewNormal = mul(kInput.vNormal, (float3x3)mSkinBoneTransform);
		#else
			TransformNBT(kInput.vNormal, kInput.vBinormal, kInput.vTangent, mSkinBoneTransform,
				kOutput.vUVSetViewNormal, kOutput.vUVSetViewBinormal, kOutput.vUVSetViewTangent);
		#endif
	#else
		#if VS_OUTPUT_VERTEX_NORMAL
			kOutput.vUVSetViewNormal = mul(float4(kInput.vNormal, 0.f), g_mWorldView).xyz;
		#else
			kOutput.vUVSetViewNormal 	= mul(float4(kInput.vNormal, 0.f), g_mWorldView).xyz;
			kOutput.vUVSetViewBinormal 	= mul(float4(kInput.vBinormal, 0.f), g_mWorldView).xyz;
			kOutput.vUVSetViewTangent 	= mul(float4(kInput.vTangent, 0.f), g_mWorldView).xyz;
		#endif
	#endif
	
	#if ENVMAP
		float3 vEyeRay = normalize(g_vCameraPostion - vWorldPos).xyz;
		float3 vWorldNormal = normalize(mul(kOutput.vUVSetViewNormal, (float3x3)g_mInvView));
		kOutput.vUVSetEnvMap = mul(float4(reflect(-vEyeRay, vWorldNormal), 1.f), a_mWorldProjectionMat).xy;
	#endif
#endif

	kOutput.vUVSetBase = kInput.vUVSetBase;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
#define PS_SHADER HEADER_KEY(PS_Main, CHANNELS_FLAG)
float4 PS_SHADER(VS_OUTPUT kInput) : COLOR0
{
#if PRUECOLOR
	#if ALPHATEST
		#if GLOWMAP
			clip(tex2D(sdGlowSampler, kInput.vUVSetBase).a - g_fAlphaTestRef);
		#else
			clip(tex2D(sdBaseSampler, kInput.vUVSetBase).a - g_fAlphaTestRef);
		#endif
	#endif
	
	return 1.f;
#else
	#if GLOWMAP
		float fGlowFactor = tex2D(sdGlowSampler, kInput.vUVSetBase).r;
		//float3 vEmissiveColor = tex2D(sdGlowSampler, kInput.vUVSetBase).rgb * g_vEmissiveMaterialColor;
		//return float4(vEmissiveColor, 1.f);
	#else
		//float3 vEmissiveColor = g_vEmissiveMaterialColor;
	#endif
	
	float4 vOrigionalDiffuseColor = 1.f;
	vOrigionalDiffuseColor = tex2D(sdBaseSampler, kInput.vUVSetBase);
	#if ALPHATEST
		clip(vOrigionalDiffuseColor.a - g_fAlphaTestRef);
	#endif

	float3 vDiffuseColor = vOrigionalDiffuseColor.rgb * g_vDiffuseMaterialColor;
	
	#if GLOSSMAP
		float3 vSpecularColor = tex2D(sdGlossSampler, kInput.vUVSetBase).rgb * g_vSpecularMaterialColor;
	#else
		float3 vSpecularColor = g_vSpecularMaterialColor;
	#endif
	
	#if VS_OUTPUT_VERTEX_NORMAL
		float3 vViewNormal 	= normalize(kInput.vUVSetViewNormal);
	#else
		float3 vNormalTex 	= tex2D(sdNormalSampler, kInput.vUVSetBase).xyz;
		vNormalTex 			= (vNormalTex - 0.5f) * 2.0f;
		vNormalTex.xy		*= g_fStaticMeshNormalScale;
		vNormalTex			= normalize(vNormalTex);

		float3 vNorNormal 	= normalize(kInput.vUVSetViewNormal);
		float3 vNorBinormal = normalize(kInput.vUVSetViewBinormal);
		float3 vNorTangent 	= normalize(kInput.vUVSetViewTangent);
		float3 vViewNormal = mul(vNormalTex, float3x3(vNorTangent, vNorBinormal, vNorNormal));
	#endif

	#if ENVMAP
		float3 vEnvColor = tex2D(sdEnvSampler, kInput.vUVSetEnvMap).rgb;
	#else
		float3 vEnvColor = 0.f;
	#endif
	
	float3	vDiffuseLight;
	float3	vSpecularLight;
	float3	vViewVector = -normalize(kInput.vUVSetViewPos);
	AccumLighting(vViewVector, vViewNormal, g_fShiness, vDiffuseLight, vSpecularLight);
	
	float3 vColor = vDiffuseColor * (vDiffuseLight + g_vEmissiveMaterialColor) + vSpecularColor * vSpecularLight;
	vColor += vEnvColor * vSpecularColor;
	
	return float4(vColor, vOrigionalDiffuseColor.a * g_fAlpha);
#endif
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
#define TECHNIQUE HEADER_KEY(ForwardShading, CHANNELS_FLAG)
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