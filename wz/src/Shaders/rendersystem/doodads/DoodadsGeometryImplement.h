//*************************************************************************************************
// 内容:	Doodads Geometry着色器具体实现
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-24
// 最后修改:
//*************************************************************************************************
#include "DoodadsCommon.h"

//---------------------------------------------------------------------------------------
// 取消一些内部会用到的预定义宏
//---------------------------------------------------------------------------------------
#undef VS_INPUT
#undef VS_OUTPUT
#undef PS_INPUT
#undef PS_OUTPUT_MRT

#undef VS_SHADER
#undef PS_SHADER_MRT

#undef TECHNIQUE
#undef TECHNIQUE_MRT

#undef DLOD
#undef DANIM
#undef DFADE
#undef DNORMAL

//---------------------------------------------------------------------------------------
// 根据当前启用的通道情况来决定具体的组合情况
//---------------------------------------------------------------------------------------
#if TEST_CHANNEL(CHANNELS_FLAG, DOODADS_LOD)
	#define DLOD 1
#else
	#define DLOD 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, DOODADS_ANIM)
	#define DANIM 1
#else
	#define DANIM 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, DOODADS_FADE)
	#define DFADE 1
#else
	#define DFADE 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, DOODADS_NORMAL)
	#define DNORMAL 1
#else
	#define DNORMAL 0
#endif

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
#define VS_INPUT HEADER_KEY(VS_Input, CHANNELS_FLAG)
struct VS_INPUT
{
	float3	vPos			: POSITION0;
	float2 	vUVSet			: TEXCOORD0;
	float3 	vNormal			: TEXCOORD1;
	
	// 实例化数据
	float4	vInstMatRow0	: TEXCOORD2;	// Rotation X
	float4	vInstMatRow1	: TEXCOORD3;	// Rotation Y
	float4	vInstMatRow2	: TEXCOORD4;	// Rotation Z
	float4	vInstMatRow3	: TEXCOORD5;	// Translation Position
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
#define VS_OUTPUT HEADER_KEY(VS_Output, CHANNELS_FLAG)
struct VS_OUTPUT
{
	float4	vProjPos			: POSITION;
	float3	vUVSetUVFadingRatio	: TEXCOORD0;
	float4 	vUVSetNormalDepth	: TEXCOORD1;
};

//---------------------------------------------------------------------------------------
// 像素着色器输入数据流
//---------------------------------------------------------------------------------------
#define PS_INPUT HEADER_KEY(PS_Input, CHANNELS_FLAG)
struct PS_INPUT
{
	float4	vProjPos			: POSITION;
	float3	vUVSetUVFadingRatio	: TEXCOORD0;
	float4 	vUVSetNormalDepth	: TEXCOORD1;
	float	fFace				: VFACE;		// 标记面片朝向
};

//---------------------------------------------------------------------------------------
// 像素着色器输出数据流
//---------------------------------------------------------------------------------------
#define PS_OUTPUT_MRT HEADER_KEY(PS_Output_Mrt, CHANNELS_FLAG)
struct PS_OUTPUT_MRT
{
	float4	vNormalDepth	: COLOR0;
	float4	vDiffuse		: COLOR1;	
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
#define VS_SHADER HEADER_KEY(VS_Main, CHANNELS_FLAG)
VS_OUTPUT VS_SHADER(VS_INPUT kInput)
{
	VS_OUTPUT kOutput = (VS_OUTPUT)0;
	
	float3 vLocalPos = mul(float4(kInput.vPos, 1.f), g_mWorld).xyz;
	float3 vWorldPos;
	vWorldPos.x = dot(vLocalPos.xyz, kInput.vInstMatRow0.xyz);
	vWorldPos.y = dot(vLocalPos.xyz, kInput.vInstMatRow1.xyz);
	vWorldPos.z = dot(vLocalPos.xyz, kInput.vInstMatRow2.xyz);
	vWorldPos.xyz += kInput.vInstMatRow3.xyz;
	
#if DFADE
	float fViewDistance = distance(vWorldPos, g_vCameraPostion.xyz);
	#if DLOD
		float fF1 = g_fGrassFadeInDistance - kInput.vInstMatRow1.w;
		float fF2 = g_fGrassFadeOutDistance - kInput.vInstMatRow2.w;
		float fFadingRatio = saturate((fViewDistance - fF1) / (fF2 - fF1));
	#else
		float fFadingRatio = saturate((fViewDistance - g_fGrassFadeInDistance) / (g_fGrassFadeOutDistance - g_fGrassFadeInDistance));
	#endif
	
	kOutput.vUVSetUVFadingRatio.z = fFadingRatio;
#else
	kOutput.vUVSetUVFadingRatio.z = 0.f;
#endif

#if DANIM
	#if DFADE
		vWorldPos= WindAnimation(kInput.vInstMatRow3.xyz, vWorldPos, max(0.f, 1.f - fFadingRatio * 10.f));
	#else
		vWorldPos = WindAnimation(kInput.vInstMatRow3.xyz, vWorldPos, 1.f);
	#endif
#endif
	
	kOutput.vProjPos = mul(float4(vWorldPos, 1.f), g_mViewProj);

#if DNORMAL
	//float3 vLocalNormal = kInput.vNormal;
	float3 vLocalNormal = normalize(mul(float4(kInput.vNormal, 0.f), g_mWorld).xyz);
	float3 vWorldNormal;
	vWorldNormal.x = dot(vLocalNormal, kInput.vInstMatRow0.xyz);
	vWorldNormal.y = dot(vLocalNormal, kInput.vInstMatRow1.xyz);
	vWorldNormal.z = dot(vLocalNormal, kInput.vInstMatRow2.xyz);	
	vWorldNormal = normalize(vWorldNormal);
	kOutput.vUVSetNormalDepth.xyz = normalize(mul(float4(vWorldNormal, 0.f), g_mView).xyz);
#else
	kOutput.vUVSetNormalDepth.xyz = normalize(mul(float4(0.f, 0.f, 1.f, 0.f), g_mView).xyz);
#endif

	kOutput.vUVSetNormalDepth.w = dot(float4(vWorldPos, 1.f), g_mView._m02_m12_m22_m32);
	
	kOutput.vUVSetUVFadingRatio.xy = kInput.vUVSet;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
#define PS_SHADER HEADER_KEY(PS_Main, CHANNELS_FLAG)
float4 PS_SHADER(PS_INPUT kInput) : COLOR0
{
	PS_OUTPUT_MRT kOutput = (PS_OUTPUT_MRT)0;

	float fAlpha = tex2D(sdBaseSampler, kInput.vUVSetUVFadingRatio.xy).a;
	clip(fAlpha - 0.5f);

	float fNoise = dot(tex2D(sdNoiseSampler, kInput.vUVSetUVFadingRatio.xy).rgb, 0.3333f);
#if DFADE
	clip(fNoise - kInput.vUVSetUVFadingRatio.z);
#endif

	float fDepth = kInput.vUVSetNormalDepth.w / g_fCameraFarClip;
	float3 vNormal = kInput.vUVSetNormalDepth.xyz * sign(-kInput.fFace);

	return float4(PackDepth(fDepth), PackNormal(vNormal));
}
//---------------------------------------------------------------------------------------
#define PS_SHADER_MRT HEADER_KEY(PS_Main_Mrt, CHANNELS_FLAG)
PS_OUTPUT_MRT PS_SHADER_MRT(PS_INPUT kInput)
{
	PS_OUTPUT_MRT kOutput = (PS_OUTPUT_MRT)0;
	
	float4 vDiffuseTex = tex2D(sdBaseSampler, kInput.vUVSetUVFadingRatio.xy);
	clip(vDiffuseTex.a - 0.5f);
	
	float fNoise = dot(tex2D(sdNoiseSampler, kInput.vUVSetUVFadingRatio.xy).rgb, 0.3333f);
#if DFADE
	clip(fNoise - kInput.vUVSetUVFadingRatio.z);
#endif

	float fDepth = kInput.vUVSetNormalDepth.w / g_fCameraFarClip;
	float3 vNormal = kInput.vUVSetNormalDepth.xyz * sign(-kInput.fFace);
	kOutput.vNormalDepth = float4(PackDepth(fDepth), PackNormal(vNormal));

	kOutput.vDiffuse = float4(vDiffuseTex.rgb, 0.f);
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
#define TECHNIQUE_FORWARD HEADER_KEY(Doodads_Geometry, CHANNELS_FLAG)
technique TECHNIQUE_FORWARD
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

		//CullMode 			= NONE;
		//FillMode			= SOLID;
		//ZEnable 			= TRUE;
		//ZWriteEnable		= TRUE;
		//AlphaTestEnable	= FALSE;
		//AlphaBlendEnable	= FALSE;
	}
}
//---------------------------------------------------------------------------------------
#define TECHNIQUE HEADER_KEY(Doodads_MRTGeometry, CHANNELS_FLAG)
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
		PixelShader 	= compile ps_3_0 PS_SHADER_MRT ();
		
		//CullMode 			= NONE;
		//FillMode			= SOLID;
		//ZEnable 			= TRUE;
		//ZWriteEnable		= TRUE;
		//AlphaTestEnable	= FALSE;
		//AlphaBlendEnable	= FALSE;
	}
}
//---------------------------------------------------------------------------------------