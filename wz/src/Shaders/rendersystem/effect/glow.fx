//*************************************************************************************************
// 内容:	辉光
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-21
// 最后修改:
//*************************************************************************************************
#include "effect_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
float	g_fGlowTCDownScale		: GLOBAL = 1.5f;
float	g_fGlowContrastScale	: GLOBAL = 1.f;

float4	g_vGlowBlurParam		: GLOBAL;

float 	g_fGlowIntensity		: GLOBAL = 1.5f;

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos	: POSITION;
	float2	vUVSet		: TEXCOORD0;	
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(float3 vPos : POSITION0, float2 vUVSet : TEXCOORD0)
{
	VS_OUTPUT kOutput = (VS_OUTPUT)0;
	kOutput.vProjPos = float4(vPos, 1.f);
	kOutput.vUVSet = vUVSet + g_vHalfPixelOffset;
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 PS_Main_Glow_DownFilter(VS_OUTPUT kInput) : COLOR0
{
	float fAverage = 1.f / 16.f;
	float2 vPixelSize = 1.f / g_vWindowSize;
	float4 vAccumColor = 0.f;
	float2 vUVSet = 0.f;
	for (int i = 0, row = 0, col = 0; i < 16; ++i)
	{
		row = i / 4;
		col = i - row * 4;
		vUVSet = kInput.vUVSet + float2((float)col - 1.5f, (float)row - 1.5f) * g_vPixelSize * g_fGlowTCDownScale;
		vAccumColor += tex2D(sdBaseSampler, vUVSet);
	}
	
	return vAccumColor * fAverage * g_fGlowContrastScale;
}
//---------------------------------------------------------------------------------------
float4 PS_Main_Glow_Blur(VS_OUTPUT kInput) : COLOR0
{
	float akWeight[13] = {
				0.00221600011f, 0.00876399968f, 0.0269949995f, 	0.0647590011f, 	0.120985001f, 	0.176033005f,
				0.199470997f,
				0.176033005f,	0.120985001f,	0.0647590011f,	0.0269949995f,	0.00876399968f,	0.00221600011f};
				
	float2 vBias = 0.f;
	float fIndex = 0.f;
	float2 vPixelSize = 4.f / g_vWindowSize;
	float4 vAccumColor = 0.f;	
	float2 vUVSet = 0.f;
	for (int i = 0; i < 13; ++i)
	{
		fIndex = (float)i - 6.f;
		vBias = fIndex * g_vGlowBlurParam.xy * vPixelSize + vPixelSize * 0.5f * sign(fIndex);
		vUVSet = kInput.vUVSet + vBias * g_vGlowBlurParam.z;
		vAccumColor += tex2D(sdBaseSampler, vUVSet) * akWeight[i];
	}
	
	return vAccumColor * 1.5f;
}
//---------------------------------------------------------------------------------------
float4 PS_Main_Glow_Composite(VS_OUTPUT kInput) : COLOR0
{
	float3 vGlowColor = tex2D(sdBaseSampler, kInput.vUVSet).rgb;
	return float4(vGlowColor * g_fGlowIntensity, 1.f);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Effect_Glow_DownFilter
<
	string Description = "Effect_Glow_DownFilter";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_Glow_DownFilter ();
	}
}
//---------------------------------------------------------------------------------------
technique Effect_Glow_Blur
<
	string Description = "Effect_Glow_Blur";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_Glow_Blur ();
	}
}
//---------------------------------------------------------------------------------------
technique Effect_Glow_Composite
<
	string Description = "Effect_Glow_Composite";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_Glow_Composite ();
	}
}
//---------------------------------------------------------------------------------------