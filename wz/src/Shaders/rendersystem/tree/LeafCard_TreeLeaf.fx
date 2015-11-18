//*************************************************************************************************
// 内容:	Tree Leaf 前向着色器(V1版,已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-22
// 最后修改:
//*************************************************************************************************
#include "TreeCommon.h"
#include "../_Deprecated/common_lightting.inl"
#include "../_Deprecated/common_fun.inl"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
//float		g_fTreeSwayRadius		: GLOBAL = 0.3f;
//float		g_fTreeSwaySpeed		: GLOBAL = 0.11f;
//float		g_fTreeLightPassing		: GLOBAL = 0.f;
//float		g_fTreeLeafShading		: GLOBAL = 0.f;
//float3	g_vTreeLeafMaterial		: GLOBAL = float3(1.f, 1.f, 1.f);

//float		a_fTreeLeafScale		: ATTRIBUTE = 1.f;

//float		g_fCloudAlpha	: GLOBAL;
//float3	g_vCloudColor	: GLOBAL;
//float3	g_vSunPosition	: GLOBAL;
//float		g_fSunAngle		: GLOBAL;
//float		g_fSunDimmer	: GLOBAL;
//float		g_fTime			: GLOBAL;

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
texture sdLeafTex
<
	string NTM = "base";
	int NTMIndex = 0;
>;

sampler sdLeafSampler = sampler_state
{
	Texture = (sdLeafTex);
	AddressU = CLAMP;
	AddressV = CLAMP;	
	AddressW = CLAMP;	
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	SRGB_ENABLE;
};

texture sdLeafNormalTex
<
	string NTM = "normal";
	int NTMIndex = 0;
>;

sampler sdLeafNormalSampler = sampler_state
{
	Texture = (sdLeafTex);
	AddressU = CLAMP;
	AddressV = CLAMP;	
	AddressW = CLAMP;	
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	SRGB_DISABLE;
};

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos					: POSITION0;
	float2 	vUVSet0					: TEXCOORD0;
	float3 	vUVSetClusterCenterl	: TEXCOORD1;
	float3 	vUVSetWorldPos			: TEXCOORD2;
	float3	vMisc					: TEXCOORD3;	// size, N/A, N/A
	float2	vExtendFlags			: TEXCOORD4;
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos			: POSITION0;
	float2 	vUVSet0				: TEXCOORD0;
	float3 	vUVSetWorldNormal	: TEXCOORD1;
	float3 	vUVSetWorldPos		: TEXCOORD2;
	float	fUVSetFogParam		: TEXCOORD3;
	float	fUVSetBBLight		: TEXCOORD4;
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	
	float3 U = g_mInvView[0].xyz;
	float3 V = g_mInvView[1].xyz;
	float3 N = g_mInvView[2].xyz;	
	
	float3 vSpiritWorldPos = mul(float4(kInput.vPos, 1.f), g_mWorld).xyz;
	float3 vEyeRay = normalize(g_vCameraPostion.xyz - vSpiritWorldPos);
	U = cross(vEyeRay, V);
	float3  vBillboardPos = (kInput.vMisc.x * kInput.vExtendFlags.x * U + kInput.vMisc.x * kInput.vExtendFlags.y * V) * a_fTreeLeafScale;
	
	kOutput.fUVSetBBLight = clamp(kInput.vExtendFlags.y + 0.5f, 0.35f, 1.f);
	
	// 旋转
	float3 vCenterRotatePos = U * g_fTreeSwayRadius;
	float fSita = g_fTime * g_fTreeSwaySpeed + kInput.vPos.x + kInput.vPos.y + kInput.vPos.z;
	float4 vQuad = { cos(fSita), sin(fSita) * N.x, sin(fSita) * N.y, sin(fSita) * N.z};
	float4 vTmp = QuadCross(vQuad.x, vQuad.yzw, 0.f, vCenterRotatePos);
	vCenterRotatePos = QuadCross(vTmp.x, vTmp.yzw, vQuad.x, -vQuad.yzw).yzw;
	
	vBillboardPos += vCenterRotatePos;
	float3 vLocalPos = vBillboardPos + kInput.vPos;
	
	// 取得精灵的世界坐标
	float3 vVertexWorldPos = vSpiritWorldPos + vBillboardPos;
	kOutput.vProjPos = mul(float4(vVertexWorldPos, 1.f), g_mViewProj);
	kOutput.vUVSetWorldPos = vVertexWorldPos;
	
	// 计算光照
	float3 vCloudNormal = normalize(vLocalPos);
	float3x3 mRotateMatrix;
	mRotateMatrix[0] = float3(g_mWorld[0][0], g_mWorld[0][1], g_mWorld[0][2]);
	mRotateMatrix[1] = float3(g_mWorld[1][0], g_mWorld[1][1], g_mWorld[1][2]);
	mRotateMatrix[2] = float3(g_mWorld[2][0], g_mWorld[2][1], g_mWorld[2][2]);
	kOutput.vUVSetWorldNormal = mul(vCloudNormal, mRotateMatrix);
	
	// 计算雾因子
	float4 vViewPos =  mul(float4(vVertexWorldPos, 1.f), g_mView);
	kOutput.fUVSetFogParam = CalcFogFactors(vViewPos.xyz);
	
	kOutput.vUVSet0 = kInput.vUVSet0;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	float4 vDiffuse = tex2D(sdLeafSampler, kInput.vUVSet0);
	
	// 可以优化
	//float3 vNormal = tex2D(sdLeafNormalSampler, kInput.vUVSet0).xyz;
	//vNormal = normalize((vNormal - 0.5f) * 2.f);

	float3 vSunAngle = -normalize(g_vDirWorldDirection1);
	float3 vView = normalize(g_vCameraPostion.xyz - kInput.vUVSetWorldPos);
	float fFactor = max(dot(vView, vSunAngle), 0.f);
	float3 vSunDiffuse = g_vDirDiffuse1.rgb + g_vDirDiffuse0.rgb;
	float3 vLeafLightColor = float3(0.9f, 1.f, 0.7f);
	vSunDiffuse = lerp(vSunDiffuse * vLeafLightColor, vSunDiffuse, fFactor);
	
	// 漫射
	float3 vLerpedNormal = lerp(kInput.vUVSetWorldNormal, vSunAngle, g_fTreeLeafShading);
	float fDot = max(dot(vSunAngle, vLerpedNormal), 0.f) + g_fTreeLightPassing;
	float3 vDiffuseLighting = vSunDiffuse * fDot + g_vDirAmbient1.rgb + g_vDirAmbient0.rgb;
	vDiffuseLighting *= g_vTreeLeafMaterial;
	vDiffuseLighting *= kInput.fUVSetBBLight;
	
	// 高光
	//float3 vLightHalfVec = normalize(vSunAngle, vView);
	//float fLightNDotH = max(dot(vSunAngle, vLightHalfVec), 0.f);
	//float fLightSpecIntensity = pow(fLightNDotH, 50.f);
	
	float3 vFinalColor = lerp(vDiffuseLighting * vDiffuse.rgb, g_vFogColor, 0.f);
	
	return float4(vFinalColor, vDiffuse.a);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique Tree_LeafCard_Shading_V1
<
	string	Description 		= "Tree_LeafCard_Shading";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_2_0	VS_Main();
		PixelShader		= compile	ps_2_0 	PS_Main();
		
		//CullMode 			= NONE;
		//FillMode			= SOLID;
		//ZEnable 			= TRUE;
		//ZWriteEnable		= TRUE;
		//AlphaTestEnable	= FALSE;
		//AlphaBlendEnable	= FALSE;
	}
}
//---------------------------------------------------------------------------------------