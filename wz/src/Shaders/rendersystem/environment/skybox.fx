//*************************************************************************************************
// 内容:	Sky Box
//				- 用于绘制6面天空盒子, 以后考虑改成一个球类似CryEngine/BigWorld
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-31
// 最后修改:
//*************************************************************************************************
#include "environment_common.h"

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_LINEAR_CLAMP_SAMPLE(0, sdSkySampler, sdSkyTex, true);

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos			: POSITION0;
	float2	vUVSet0			: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;		// 投影坐标
	float2	vUVSet0			: TEXCOORD0;
	float3	vUVSetViewPos	: TEXCOORD1;	// 观察空间坐标
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	
	kOutput.vProjPos = mul(float4(kInput.vPos, 1.f), g_mWorldViewProj).xyww;
	kOutput.vUVSet0 = kInput.vUVSet0;
	kOutput.vUVSetViewPos = kInput.vPos;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	float fDensity = CalcSkyFogDensity(normalize(kInput.vUVSetViewPos));
	float3 vFinalColor = lerp(tex2D(sdSkySampler, kInput.vUVSet0).xyz, g_vSkyFogColor, fDensity);
	
	return float4(vFinalColor, 1.f);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique Environment_SkyBox
<
	string Description = "Environment_SkyBox";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main ();
		
		//CullMode = None;
		//ZEnable = true;
		//ZWriteenable = false;
		//AlphaTestEnable = false;
		//AlphaBlendEnable = false;
	}
}
//---------------------------------------------------------------------------------------