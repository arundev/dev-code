//*************************************************************************************************
// 内容:	勾边实现
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-15
// 最后修改:	2014-05-06
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

#undef COMPACTVERTEX
#undef EHARDWARESKIN
#undef LEAFANIM

//---------------------------------------------------------------------------------------
// 根据CHANNELS_FLAG的值来提取当前启用的通道情况
//---------------------------------------------------------------------------------------
#if TEST_CHANNEL(CHANNELS_FLAG, VERTEX_COMPACTVERTEX_FLAG)
	#define COMPACTVERTEX 1
#else
	#define COMPACTVERTEX 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, EFFECT_HAEDWARESKIN)
	#define EHARDWARESKIN 1
#else
	#define EHARDWARESKIN 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, VERTEX_LEAFANIM_FLAG)
	#define LEAFANIM 1
#else
	#define LEAFANIM 0
#endif

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
#define VS_INPUT HEADER_KEY(VS_Input, CHANNELS_FLAG)
#if COMPACTVERTEX
	struct VS_INPUT
	{
		float3 	vPos			: POSITION;
		float4 	vNormal			: NORMAL;
		float4	vTangent		: TANGENT;
		
	#if EHARDWARESKIN
		float3	vBlendWeights	: BLENDWEIGHT0;
		int4	vBlendIndices	: BLENDINDICES0;
	#endif
	};
#else
	struct VS_INPUT
	{
		float3	vPos			: POSITION0;
		float2	vUVSet0			: TEXCOORD0;
		
	#if EHARDWARESKIN
		float3	vBlendWeights	: BLENDWEIGHT0;
		int4	vBlendIndices	: BLENDINDICES0;
	#endif
	};
#endif

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
#define VS_OUTPUT HEADER_KEY(VS_Output, CHANNELS_FLAG)
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;
	float2	vUVSet0			: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
#define VS_SHADER HEADER_KEY(VS_Main, CHANNELS_FLAG)
VS_OUTPUT VS_SHADER(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	
#if EHARDWARESKIN
	float4 vWorldPos;
	float4x4 mSkinBoneTransform;
	
	TransformSkinnedPositionPrecision(kInput.vPos, kInput.vBlendIndices, kInput.vBlendWeights,
		g_mSkinnedBoneMatrix3, vWorldPos, mSkinBoneTransform);
	float4x4 mZeroView = float4x4(g_mView[0], g_mView[1], g_mView[2], float4(0.f, 0.f, 0.f, 1.f));	///< ???
	float4 vViewPos = mul(vWorldPos, mZeroView);
	kOutput.vProjPos = mul(vViewPos, g_mProj);	
	
//	TransformSkinnedPosition(kInput.vPos, kInput.vBlendIndices, kInput.vBlendWeights,
//		g_mSkinnedBoneMatrix3, vWorldPos, mSkinBoneTransform);
//	float4x4 mZeroView = float4x4(g_mView[0], g_mView[1], g_mView[2], g_mView[3]);
//	float4 vViewPos = mul(vWorldPos, mZeroView);
//	kOutput.vProjPos = mul(vViewPos, g_mProj);
//	kOutput.vProjPos = mul(vWorldPos, g_mViewProj);
#else
	#if LEAFANIM
		float3 vWorldPos = mul(float4(kInput.vPos.xyz, 1.f), g_mWorld).xyz;	///< 顶点转换到世界坐标系
		float3 vWorldNormal = normalize(mul(kInput.vNormal, g_mWorld)).xyz;	///< 法线转换到世界坐标系
		vWorldPos += CalcDetailWave(vWorldPos, vWorldNormal, 0.15f);		///< 计算世界坐标系树叶动画
		kOutput.vProjPos = mul(float4(vWorldPos, 1.0f), g_mViewProj);		///< 树叶从世界坐标转换到投影坐标系
	#else
		kOutput.vProjPos = mul(float4(kInput.vPos, 1.f), g_mWorldViewProj);	///< 从模型坐标系变换到投影坐标系
	#endif
#endif
	
#if COMPACTVERTEX
	kOutput.vUVSet0 = float2(kInput.vNormal.w, kInput.vTangent.w);
#else
	kOutput.vUVSet0 = kInput.vUVSet0;
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
//	clip(vBaseColor.a - g_fAlphaTestRef);
	
	return 1.f;
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
#define TECHNIQUE HEADER_KEY(Effect_Edge, CHANNELS_FLAG)
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