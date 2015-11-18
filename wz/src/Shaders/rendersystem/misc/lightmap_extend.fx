//*************************************************************************************************
// 内容:	烘培LightMap公共Pass
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-21
// 最后修改:	
//*************************************************************************************************
#include "../common.h"

//---------------------------------------------------------------------------------------
// 全局变量
//---------------------------------------------------------------------------------------
float2	g_vRecipLightMapSize	: GLOBAL = { 1.f / 256.f, 1.f / 256.f};

float	g_akSampleWeights[13]	: GLOBAL;
float2	g_akSampleOffsets[13]	: GLOBAL;

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
// LightMap
texture sdLightBuf
<
	string NTM = "base";
	int NTMIndex = 0;
>;

sampler sdLightSampler = sampler_state
{
	Texture = (sdLightBuf);
	AddressU = CLAMP;
	AddressV = CLAMP;	
	AddressW = CLAMP;	
	MipFilter = NONE;
	MinFilter = POINT;
	MagFilter = POINT;
	SRGBTexture = false;
};

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
VS_OUTPUT VS_Main(float2 vPos : POSITION0, float2 vUVSet : TEXCOORD0)
{
	VS_OUTPUT kOutput;
	kOutput.vProjPos = float4(vPos, 0.f, 1.f);
	kOutput.vUVSet = vUVSet;
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 PS_Main_Copy(VS_OUTPUT kInput) : COLOR0
{
	float2 vHalfPixel = 0.5f * g_vRecipLightMapSize;
	float2 vUVSet = kInput.vUVSet + vHalfPixel;
	return tex2D(sdLightSampler, vUVSet);
}
//---------------------------------------------------------------------------------------
float4 PS_Main_GaussBlur(VS_OUTPUT kInput) : COLOR0
{
	// 采样3x3个像素
	// x x x
	// x x x
	// x x x 
	// @{
	float2 vPixelSize = g_vRecipLightMapSize;
	float2 vUVSet = 0.f;
	
	// row0 - 00 01 02
	vUVSet.y = kInput.vUVSet.y - vPixelSize.y;
	
	vUVSet.x = kInput.vUVSet.x - vPixelSize.x;
	float4 vColor00 = tex2D(sdLightSampler, vUVSet);
	
	vUVSet.x = kInput.vUVSet.x;
	float4 vColor01 = tex2D(sdLightSampler, vUVSet);
	
	vUVSet.x = kInput.vUVSet.x + vPixelSize.x;
	float4 vColor02 = tex2D(sdLightSampler, vUVSet);
	
	// row1 - 10 11 12
	vUVSet.y = kInput.vUVSet.y;
	
	vUVSet.x = kInput.vUVSet.x - vPixelSize.x;
	float4 vColor10 = tex2D(sdLightSampler, vUVSet);
	
	vUVSet.x = kInput.vUVSet.x;
	float4 vColor11 = tex2D(sdLightSampler, vUVSet);
	
	vUVSet.x = kInput.vUVSet.x + vPixelSize.x;
	float4 vColor12 = tex2D(sdLightSampler, vUVSet);
	
	// row2 - 20 21 22
	vUVSet.y = kInput.vUVSet.y + vPixelSize.y;
	
	vUVSet.x = kInput.vUVSet.x - vPixelSize.x;
	float4 vColor20 = tex2D(sdLightSampler, vUVSet);
	
	vUVSet.x = kInput.vUVSet.x;
	float4 vColor21 = tex2D(sdLightSampler, vUVSet);
	
	vUVSet.x = kInput.vUVSet.x + vPixelSize.x;
	float4 vColor22 = tex2D(sdLightSampler, vUVSet);
	// @}
	
	
	float4 vColor = vColor11;
	float4 vOne = float4(1.f, 1.f, 1.f, 1.f);
	
	if (vColor.x == 0.f)
	{
		float4 a0 = float4(vColor00.x, vColor01.x, vColor02.x, vColor10.x);
		float4 a1 = float4(vColor12.x, vColor20.x, vColor21.x, vColor22.x);	
		float4 n0 = a0 > 0.f;
		float4 n1 = a1 > 0.f;
		float a = dot(a0, vOne) + dot(a1, vOne);
		float n = dot(n0, vOne) + dot(n1, vOne);
		vColor.x = a / n;
	}
	
	if (vColor.y == 0.f)
	{
		float4 a0 = float4(vColor00.y, vColor01.y, vColor02.y, vColor10.y);
		float4 a1 = float4(vColor12.y, vColor20.y, vColor21.y, vColor22.y);	
		float4 n0 = a0 > 0.f;
		float4 n1 = a1 > 0.f;
		float a = dot(a0, vOne) + dot(a1, vOne);
		float n = dot(n0, vOne) + dot(n1, vOne);
		vColor.y = a / n;
	}
	
	if (vColor.z == 0.f)
	{
		float4 a0 = float4(vColor00.z, vColor01.z, vColor02.z, vColor10.z);
		float4 a1 = float4(vColor12.z, vColor20.z, vColor21.z, vColor22.z);	
		float4 n0 = a0 > 0.f;
		float4 n1 = a1 > 0.f;
		float a = dot(a0, vOne) + dot(a1, vOne);
		float n = dot(n0, vOne) + dot(n1, vOne);
		vColor.z = a / n;
	}
	
	return vColor;
}
//---------------------------------------------------------------------------------------
float4 PS_Main_GaussBlur5x5(VS_OUTPUT kInput) : COLOR0
{
	float3 vSample = 0.f;
	for (int i = 0; i <= 12; ++i)
	{
		vSample = g_akSampleWeights[i] * tex2D(sdLightSampler, kInput.vUVSet + g_akSampleOffsets[i]).xyz;
	}
	
	return float4(vSample, 1.f);
}
//---------------------------------------------------------------------------------------
float4 PS_Main_ReverseColor(VS_OUTPUT kInput) : COLOR0
{
	return 1.f - tex2D(sdLightSampler, kInput.vUVSet);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique LightMap_Extend
<
	string Description = "LightMap_Extend";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main ();
		PixelShader 	= compile ps_2_a PS_Main_GaussBlur ();
	}
}
//---------------------------------------------------------------------------------------
technique LightMap_GaussBlur5x5
<
	string Description = "LightMap_Extend";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main_GaussBlur5x5 ();
	}
}
//---------------------------------------------------------------------------------------
technique LightMap_ReverseColor
<
	string Description = "LightMap_ReverseColor";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main_ReverseColor ();
	}
}
//---------------------------------------------------------------------------------------