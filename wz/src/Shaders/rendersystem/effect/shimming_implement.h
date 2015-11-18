//*************************************************************************************************
// 内容:	shimming实现
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

#undef EVERTEXCOLOR

//---------------------------------------------------------------------------------------
// 根据CHANNELS_FLAG的值来提取当前启用的通道情况
//---------------------------------------------------------------------------------------
#if TEST_CHANNEL(CHANNELS_FLAG, EFFECT_VERTEXCOLOR)
	#define EVERTEXCOLOR 1
#else
	#define EVERTEXCOLOR 0
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
	float4	vUVSet0			: TEXCOORD0;
	float4	vUVSetProjPos	: TEXCOORD1;
	float 	vUVSetDepth		: TEXCOORD2;
	
#if EVERTEXCOLOR
	float2	vUVSetColor		: TEXCOORD4;
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

	float4 vProjPos = mul(vWorldPos, g_mViewProj);
	kOutput.vProjPos = vProjPos;
	kOutput.vUVSetProjPos = vProjPos;
	kOutput.vUVSetDepth = mul(vWorldPos, g_mView._m02_m12_m22_m32);
	
//#if EBASEUVANI
	kOutput.vUVSet0.xy = mul(float4(kInput.vUVSet0, 0, 1.f), g_mBaseMapTextureTransform).xy;
	kOutput.vUVSet0.zw = kInput.vUVSet0;
//#else
//	kOutput.vUVSet0 = kInput.vUVSet0;
//#endif

#if EVERTEXCOLOR
	kOutput.vUVSetColor = kInput.vColor;
#endif

	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
#define PS_SHADER HEADER_KEY(PS_Main, CHANNELS_FLAG)
float4 PS_SHADER(VS_OUTPUT kInput) : COLOR0
{
	// 计算屏幕纹理坐标
	float2 vUVSetScreen = GetTexCoordForPixel(kInput.vUVSetProjPos.xy / kInput.vUVSetProjPos.w);
	
	float4 vNoise1 = tex2D(sdGlossSampler, kInput.vUVSet0.xy);
	float4 vNoise2 = tex2D(sdGlossSampler, kInput.vUVSet0.yx);
	
	float2 vNoiseOffset = vNoise1.xy + vNoise2.xy - float2(1.f, 1.f);
	vNoiseOffset *= g_fNoiseStrength;
	
	float4 vScroll = tex2D(sdBaseSampler, kInput.vUVSet0.xy);
	float fWeight = tex2D(sdBaseSampler, kInput.vUVSet0.zw).a;
#if EVERTEXCOLOR
	fWeight *= kInput.vUVSetColor.a;
#endif
	
	// 计算扰动屏幕纹理坐标
	float2 vUVSetRealCrd = vUVSetScreen + (vScroll.xy + vNoiseOffset) * g_fScrollStrength * g_vPixelSize * fWeight;
	
	float fDepth = kInput.vUVSetDepth / g_fCameraFarClip + 0.000001f;
	float fSceneDepth = UnpackDepth(tex2D(sdGeomSampler, vUVSetRealCrd).xy);
	if (fSceneDepth <= 0.000001f)
		fSceneDepth = 1.f;
		
	float3 vOutColor;
	if (fSceneDepth > fDepth)
	{
		vOutColor = tex2D(sdShadingSampler, vUVSetRealCrd).rgb;
	}
	else
	{
		vOutColor = tex2D(sdShadingSampler, vUVSetScreen).rgb;	
	}
	
	return float4(vOutColor, 1.f);
}
	
//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
#define TECHNIQUE HEADER_KEY(Effect_Shimming, CHANNELS_FLAG)
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