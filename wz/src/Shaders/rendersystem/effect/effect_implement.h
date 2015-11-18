//*************************************************************************************************
// 内容:	实现
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-15
// 最后修改:
//*************************************************************************************************
#include "effect_common.h"

//---------------------------------------------------------------------------------------
// 取消一些内部会用到的预定义宏
//---------------------------------------------------------------------------------------
#undef VS_INPUT
#undef VS_OUTPUT
#undef VS_SHADER
#undef PS_SHADER
#undef TECHNIQUE

#undef EDARKMAP
#undef EDETAILMAP
#undef EBASEUVANI
#undef EALPHAADD
#undef EHAEDWARESKIN
#undef EVERTEXCOLOR
#undef ESOFTPD

//---------------------------------------------------------------------------------------
// 根据CHANNELS_FLAG的值来提取当前启用的通道情况
//---------------------------------------------------------------------------------------
#if TEST_CHANNEL(CHANNELS_FLAG, EFFECT_DARKMAP)
	#define EDARKMAP 1
#else
	#define EDARKMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, EFFECT_DETAILMAP)
	#define EDETAILMAP 1
#else
	#define EDETAILMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, EFFECT_BASEUVANI)
	#define EBASEUVANI 1
#else
	#define EBASEUVANI 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, EFFECT_ALPHAADD)
	#define EALPHAADD 1
#else
	#define EALPHAADD 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, EFFECT_HAEDWARESKIN)
	#define EHAEDWARESKIN 1
#else
	#define EHAEDWARESKIN 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, EFFECT_VERTEXCOLOR)
	#define EVERTEXCOLOR 1
#else
	#define EVERTEXCOLOR 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, EFFECT_SOFTPD)
	#define ESOFTPD 1
#else
	#define ESOFTPD 0
#endif

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
#define VS_INPUT HEADER_KEY(VS_Input, CHANNELS_FLAG)
struct VS_INPUT
{
	float3	vPos			: POSITION0;
	float2	vUVSet0			: TEXCOORD0;
	
#if EVERTEXCOLOR
	float4	vColor			: COLOR0;
#endif
	
#if EHARDWARESKIN
	float3	vBlendWeights	: BLENDWEIGHT0;
	int4	vBlendIndices	: BLENDINDICES0;
#endif
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
#define VS_OUTPUT HEADER_KEY(VS_Output, CHANNELS_FLAG)
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;
	float2	vUVSet0			: TEXCOORD0;
	float4	vUVSetColor		: TEXCOORD1;
	float3	vUVSetWorldPos	: TEXCOORD2;
	float3	vUVSetViewPos	: TEXCOORD3;
	
#if EDARKMAP
	float2	vUVSetDarkMap	: TEXCOORD4;
#endif

#if EDETAILMAP
	float2	vUVSetDetailMap	: TEXCOORD5;
#endif

#if ESOFTPD
	float2	vUVSetProjPos	: TEXCOORD6;
#endif
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
#define VS_SHADER HEADER_KEY(VS_Main, CHANNELS_FLAG)
VS_OUTPUT VS_SHADER(VS_INPUT kInput)
{
	VS_OUTPUT kOutput = (VS_OUTPUT)0;
	
#if EHARDWARESKIN
	float4 vWorldPos;
	float4x4 mSkinBoneTransform;
	TransformSkinnedPosition(kInput.vPos, kInput.vBlendIndices, kInput.vBlendWeights,
		g_mSkinnedBoneMatrix3, vWorldPos, mSkinBoneTransform);
#else
	float4 vWorldPos = mul(float4(kInput.vPos, 1.f), g_mWorld);
#endif
	
	kOutput.vUVSetWorldPos 	= vWorldPos.xyz;
	kOutput.vUVSetViewPos 	= mul(vWorldPos, g_mView).xyz;	
	kOutput.vProjPos = mul(vWorldPos, g_mViewProj);
	
#if EBASEUVANI
	kOutput.vUVSet0 = mul(float4(kInput.vUVSet0, 0, 1.f), g_mBaseMapTextureTransform).xy;
#else
	kOutput.vUVSet0 = kInput.vUVSet0;
#endif

#if EDARKMAP
	kOutput.vUVSetDarkMap = mul(float4(kInput.vUVSet0, 0, 1.f), g_mDarkMapTextureTransform).xy;
#endif

#if EDETAILMAP
	kOutput.vUVSetDetailMap = mul(float4(kInput.vUVSet0, 0, 1.f), g_mDetailMapTextureTransform).xy;
#endif

#if ESOFTPS
	kOutput.vUVSetProjPos = kOutput.vProjPos;
#endif

#if EVERTEXCOLOR
	kOutput.vUVSetColor = kInput.vColor;
#else
	kOutput.vUVSetColor = g_vEmissiveMaterial;
#endif

	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
#define PS_SHADER HEADER_KEY(PS_Main, CHANNELS_FLAG)
float4 PS_SHADER(VS_OUTPUT kInput) : COLOR0
{
	float4 vBaseColor = tex2D(sdBaseSampler, kInput.vUVSet0);
	//clip(vBaseColor.a - g_fAlphaTestRef);

#if EDARKMAP
	float3 vDarkColor = tex2D(sdDarkSampler, kInput.vUVSetDarkMap);
	vBaseColor.rgb *= vDarkColor;
#endif
	
#if EDETAILMAP
	float3 vDetailColor = tex2D(sdDetailSampler, kInput.vUVSetDetailMap);
	vBaseColor.rgb *= 2.f * vDetailColor;
#endif
	
	vBaseColor.rgb *= kInput.vUVSetColor.rgb;
	
	float4 vFogColorAndFactor = CalcHeightFogCurveFactor(sdCurveFogSampler, float4(kInput.vUVSetWorldPos, kInput.vUVSetViewPos.z), false);
#if EALPHAADD
	//vBaseColor.rgb *= vFogColorAndFactor.w;
#else
	vBaseColor.rgb = vFogColorAndFactor.rgb + vFogColorAndFactor.a * vBaseColor.rgb;
#endif

#if ESOFTPS
	float2 vUVSetScreen = GetTexCoordForPixel(kInput.vUVSetProjPos.xy / kInput.vUVSetProjPos.w);
	float4 vGeomData = tex2D(sdGeomSampler, vUVSetScreen);
	float fDepth = UnpackDepth(vGeomData.xy);
	float fOpacity;
	if (fDepth == 0.f)
	{
		fOpacity = 1.f;
	}
	else
	{
		float fInvParticleScale = 0.5f;	// xxx - needs to be exposed to editor;
		fOpacity = (fDepth * g_fCameraFarClip - kInput.vUVSetViewPos.z) * fInvParticleScale;	// no z-testing
		clip(fOpacity);
		fOpacity = saturate(fOpacity);
	}
#else
	float fOpacity = 1.f;
#endif
	
	return float4(vBaseColor.rgb, kInput.vUVSetColor.a * vBaseColor.a * g_vDiffuseMaterial.a * fOpacity);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
#define TECHNIQUE HEADER_KEY(Effect_Effect, CHANNELS_FLAG)
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