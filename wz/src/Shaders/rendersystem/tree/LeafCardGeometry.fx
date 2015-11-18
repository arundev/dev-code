//*************************************************************************************************
// 内容:	Tree Leaf Geometry着色器(已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-14
// 最后修改:
//*************************************************************************************************
#include "TreeCommon.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 	vPosId			: POSITION0;	///< center.x, center.y, center.z, id
	float4	vUVSetSize		: TEXCOORD0;	///< uv0.x, uv0.y, size, 1.f	
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;
	float	fUVSetDepth		: TEXCOORD0;
	float3 	vUVSetNormal	: TEXCOORD1;
	float2 	vUVSet0			: TEXCOORD2;
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	
	// 计算空间坐标
	// @{
	// 观察空间中心点坐标
	float3 vCardViewPos = mul(float4(kInput.vPosId.xyz, 1.0f), g_mWorldView).xyz;
	
	// view space rotation
	float fRotAngle = dot(float4(g_fTime, kInput.vPosId.xyz), float4(g_fTreeSwaySpeed, 1.f, 1.f, 1.f));
	float3 vRotVec = float3(cos(fRotAngle), sin(fRotAngle), 0.f) * g_fTreeSwayRadius;	

	// view space offset
	float3 vCornerOffset = g_akCornerOffset[kInput.vPosId.w] * kInput.vUVSetSize.z * a_fTreeLeafScale;
	
	// 计算观察空间坐标	
	float3 vViewPos = vCardViewPos + vRotVec + vCornerOffset;

	// 计算投影空间坐标
	kOutput.vProjPos = mul(float4(vViewPos, 1.f), g_mProj);
	kOutput.fUVSetDepth = vViewPos.z;
	// @}
	
	
	// 计算法线
	// @{
	float3 vCenterToCorner = normalize(vViewPos - g_mWorldView._m30_m31_m32);
	float3 vCardNormal = lerp(-g_vMainLightDir, vCenterToCorner, g_fTreeLightPassing);
	
	float3 vCornelNormal = g_akCornerNormal[kInput.vPosId.w];
	vCornelNormal.xy *= g_fTreeLeafShading;
	
	float3 vNormal = lerp(vCornelNormal, vCardNormal, saturate(length(vViewPos) / g_fTreeWholeLightDistance));
	
	kOutput.vUVSetNormal = vNormal;
	// @}
	
	// 基础纹理坐标
	kOutput.vUVSet0 = kInput.vUVSetSize.xy;

	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 PS_Main_DiffuseMapAlphaTest(VS_OUTPUT kInput) : COLOR0
{
	float4 vDiffuseTex = tex2D(sdBaseSampler, kInput.vUVSet0);
	clip(vDiffuseTex.a - 0.5f);
	
	float fNoise = dot(tex2Dlod(sdNoiseSampler, float4(kInput.vUVSet0, 0, 0)).rgb, 0.3333f);
	clip(g_fAlpha - fNoise);
	
	float fDepth = kInput.fUVSetDepth / g_fCameraFarClip;
	float3 vNormal = normalize(kInput.vUVSetNormal);
	return float4(PackDepth(fDepth), PackNormal(vNormal));
}
//---------------------------------------------------------------------------------------
float4 PS_Main_NormalMapAlphaTest(VS_OUTPUT kInput) : COLOR0
{
	float4 vDiffuseTex = tex2D(sdNormalSampler, kInput.vUVSet0);
	clip(vDiffuseTex.a - 0.5f);
	
	float fNoise = dot(tex2Dlod(sdNoiseSampler, float4(kInput.vUVSet0, 0, 0)).rgb, 0.3333f);
	clip(g_fAlpha - fNoise);
	
	float fDepth = kInput.fUVSetDepth / g_fCameraFarClip;
	float3 vNormal = normalize(kInput.vUVSetNormal);
	return float4(PackDepth(fDepth), PackNormal(vNormal));
}
//---------------------------------------------------------------------------------------
float4 PS_Main_Solid(VS_OUTPUT kInput) : COLOR0
{
	float fNoise = dot(tex2Dlod(sdNoiseSampler, float4(kInput.vUVSet0, 0, 0)).rgb, 0.3333f);
	clip(g_fAlpha - fNoise);
	
	float fDepth = kInput.fUVSetDepth / g_fCameraFarClip;
	float3 vNormal = normalize(kInput.vUVSetNormal);
	return float4(PackDepth(fDepth), PackNormal(vNormal));
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Tree_LeafCard_DiffuseMapAlphaTest_Geometry
<
	string	Description 		= "Tree_LeafCard_DiffuseMapAlphaTest_Geometry";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_3_0	VS_Main();
		PixelShader		= compile	ps_3_0 	PS_Main_DiffuseMapAlphaTest();
	}
}
//---------------------------------------------------------------------------------------
technique Tree_LeafCard_NormalMapAlphaTest_Geometry
<
	string	Description 		= "Tree_LeafCard_NormalMapAlphaTest_Geometry";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_3_0	VS_Main();
		PixelShader		= compile	ps_3_0 	PS_Main_NormalMapAlphaTest();
	}
}
//---------------------------------------------------------------------------------------
technique Tree_LeafCard_Solid_Geometry
<
	string	Description 		= "Tree_LeafCard_Solid_Geometry";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_3_0	VS_Main();
		PixelShader		= compile	ps_3_0 	PS_Main_Solid();
	}
}
//---------------------------------------------------------------------------------------