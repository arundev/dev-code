//*************************************************************************************************
// 内容:	Tree Leaf 前向着色器(V2版,已废弃)
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

//
//static float3 g_akCornerOffset[4] = {
//				float3( 0.5f,  0.5f, 0),
//				float3(-0.5f,  0.5f, 0),
//				float3( 0.5f, -0.5f, 0),
//				float3(-0.5f, -0.5f, 0)
//				};

//static float3 g_akCornerNormal[4] = {
//				float3( 4.f,  4.f, -1.f),
//				float3(-4.f,  4.f, -1.f),
//				float3( 4.f, -4.f, -1.f),
//				float3(-4.f, -4.f, -1.f)
//				};

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
	AddressU = WRAP;
	AddressV = CLAMP;	
	AddressW = CLAMP;	
	MipFilter = LINEAR;
	MinFilter = LINEAR;
	MagFilter = LINEAR;
	SRGB_ENABLE;
};

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
	float4	vProjPos			: POSITION0;
	float2 	vUVSet0				: TEXCOORD0;
	float3 	vUVSetWorldNormal	: TEXCOORD1;
	float3 	vUVSetWorldPos		: TEXCOORD2;
	float	fFogParam			: FOG0;
//	float4	vDiffuseAmbient		: COLOR0;
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
	float3 vCardWorldPos = mul(float4(kInput.vPosId.xyz, 1.0f), g_mWorld).xyz;
	
	// view space rotation
	float fRotAngle = dot(float4(g_fTime, kInput.vPosId.xyz), float4(g_fTreeSwaySpeed, 1.f, 1.f, 1.f));
	float3 vRotVec = float3(cos(fRotAngle), sin(fRotAngle), 0.f) * g_fTreeSwayRadius;	

	// view space offset
	float3 vCornerOffset = g_akCornerOffset[kInput.vPosId.w] * kInput.vUVSetSize.z * a_fTreeLeafScale;

	vCardWorldPos += vRotVec;
	vCardWorldPos += (vCornerOffset.x * g_mInvView[0] + vCornerOffset.y * g_mInvView[1]).xyz;

	kOutput.vUVSetWorldPos = vCardWorldPos;
	kOutput.vProjPos = mul(float4(vCardWorldPos, 1.f), g_mViewProj);
	// @}
	
	
	// 计算法线
	// @{
	float3 vCornelNormal = g_akCornerNormal[kInput.vPosId.w];
	vCornelNormal.xy *= g_fTreeLeafShading;
	
	float3 vNormal = mul(float4(vCornelNormal, 0.f), g_mInvView).xyz;
	kOutput.vUVSetWorldNormal = normalize(mul(float4(vNormal, 0.f), g_mWorld).xyz);
	// @}
	
	kOutput.vUVSet0 = kInput.vUVSetSize.xy;
	
	float4 vViewPos = mul(float4(vCardWorldPos, 1.f), g_mView);
	kOutput.fFogParam = CalcFogFactors(vViewPos.xyz);

	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	clip(tex2D(sdNormalSampler, kInput.vUVSet0).a - 0.5f);

	float3 vDiffuseAccum = 0.f;
	float3 vSpecularAccum = 0.f;
	float3 vDiffuse = 0.f;
	float3 vSpecular = 0.f;
	
	CaculateDirectionalLightContribution(kInput.vUVSetWorldPos, kInput.vUVSetWorldNormal, g_vCameraPostion.xyz, 30.f, 1.f,
		g_vMaterialAmbient, g_vMaterialDiffuse, g_vMaterialSpecular,
		g_vDirWorldDirection0, g_vDirAmbient0.rgb, g_vDirDiffuse0.rgb, g_vDirSpecular0.rgb, 1.f,
		vDiffuse, vSpecular);
		
	vDiffuseAccum += vDiffuse;
	vSpecularAccum += vSpecular;
	
	CaculateDirectionalLightContribution(kInput.vUVSetWorldPos, kInput.vUVSetWorldNormal, g_vCameraPostion.xyz, 30.f, 1.f,
		g_vMaterialAmbient, g_vMaterialDiffuse, g_vMaterialSpecular,
		g_vDirWorldDirection1, g_vDirAmbient1.rgb, g_vDirDiffuse1.rgb, g_vDirSpecular1.rgb, 1.f,
		vDiffuse, vSpecular);
		
	vDiffuseAccum += vDiffuse;
	vSpecularAccum += vSpecular;
	
	float4 vBaseColor = tex2D(sdLeafSampler, kInput.vUVSet0);
	
	float3 vColor = ((g_vAmbientAmbient0 * g_vMaterialDiffuse.rgb + vDiffuseAccum) * vBaseColor.rgb).rgb + vSpecularAccum;
	vColor = lerp(vColor, g_vFogColor, kInput.fFogParam);
	
	return float4(vColor, 0.f);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique Tree_LeafCard_Shading_V2
<
	string	Description 		= "Tree_LeafCard_Shading";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_3_0	VS_Main();
		PixelShader		= compile	ps_3_0 	PS_Main();
		
		//CullMode 			= NONE;
		//FillMode			= SOLID;
		//ZEnable 			= TRUE;
		//ZWriteEnable		= TRUE;
		//AlphaTestEnable	= FALSE;
		//AlphaBlendEnable	= FALSE;
	}
}
//---------------------------------------------------------------------------------------