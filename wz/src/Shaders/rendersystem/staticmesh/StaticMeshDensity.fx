//*************************************************************************************************
// 内容:	StaticMesh Density着色器(已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-21
// 最后修改:
//*************************************************************************************************
#include "static_mesh_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;
	float2 	vUVSet			: TEXCOORD0;
	float4	vUVSetWorldPos	: TEXCOORD1;

};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(float3 vPos : POSITION0, float2 vUVSet : TEXCOORD0)
{
	VS_OUTPUT kOutput;
	kOutput.vProjPos = mul(float4(vPos, 1.f), g_mWorldViewProj);
	kOutput.vUVSet = vUVSet;
	kOutput.vUVSetWorldPos = mul(float4(vPos, 1.f), g_mWorld);
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	float2 vDDXUV = ddx(kInput.vUVSet);
	float2 vDDYUV = ddy(kInput.vUVSet);
	float3 vCrossUV = cross(float3(vDDXUV, 0.f), float3(vDDYUV, 0.f));
	float fUVArea = length(vCrossUV);
	fUVArea *= (g_vDiffuseMapSize.x * g_vDiffuseMapSize.z);
	
	float3 vDDXPos = ddx(kInput.vUVSetWorldPos);
	float3 vDDYPos = ddy(kInput.vUVSetWorldPos);
	float3 vCrossPos = cross(vDDXPos, vDDYPos);
	float fFaceArea = length(vCrossPos);
	
	float fDensity = fUVArea / fFaceArea;
	//fDensity = 0.00001f * fDensity;
	
	float2 vThreshold = fDensity * g_vDensityThreshold.xy;
	//fDensity = smoothstep(0.f, 1.f, fDensity);
	//return lerp(float4(0.f, 0.f, 1.f, 1.f), float4(1.f, 0.f, 0.f, 1.f), fDensity);
	
	//if (fDensity < fRatio.x)
	//{
	//	float fRatio = saturate(fDensity * 2.f);
	//	return lerp(float4(1.f, 0.f, 0.f, 1.f), float4(0.f, 1.f, 0.f, 1.f), fRatio);
	//}
	//else if (fDensity > fRatio.y)
	//{
	//	float fRatio = saturate(fDensity * 2.f - 1.f);
	//	return lerp(float4(0.f, 1.f, 0.f, 1.f), float4(0.f, 0.f, 1.f, 1.f), fRatio);
	//}
	//else
	//{
	//	float fRatio = saturate(fDensity * 2.f - 1.f);
	//	return lerp(float4(0.f, 1.f, 0.f, 1.f), float4(0.f, 0.f, 1.f, 1.f), fRatio);
	//}
	//return float4(fDensity.xxx, 1.f);
	
	if (vThreshold.x < 1.f)
	{
		float fRatio = saturate((vThreshold.x - 0.5f) * 2.f);
		return lerp(float4(1.f, 0.f, 0.f, 1.f), float4(0.5f, 0.f, 0.f, 1.f), fRatio);
	}
	else if (vThreshold.y > 1.f)
	{
		float fRatio = saturate(vThreshold.y - 1.f);
		return lerp(float4(0.f, 0.f, 0.5f, 1.f), float4(0.f, 0.f, 1.f, 1.f), fRatio);
	}
	else
	{
		float fRatio = vThreshold.y;
		return lerp(float4(0.f, 0.5f, 0.f, 1.f), float4(0.f, 1.f, 01.f, 1.f), fRatio);
	}
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique StaticMesh_Density
<
	string	Description 		= "StaticMesh_Density";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_2_a	VS_Main();
		PixelShader		= compile	ps_2_a 	PS_Main();
	}
}
//---------------------------------------------------------------------------------------