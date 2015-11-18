//*************************************************************************************************
// 内容:	建筑物烘焙LightMap
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-22
// 最后修改:	
//*************************************************************************************************
#include "static_mesh_common.h"

//---------------------------------------------------------------------------------------
// 全局变量
//---------------------------------------------------------------------------------------
float4x4	g_mWorldToShadowSpaceNear	: GLOBAL;
float		g_fShadowNearCameraFarPlane	: GLOBAL;

//float4x4	g_mWorldToShadowSpaceFar	: GLOBAL;
//float		g_fShadowFarCameraFarPlane	: GLOBAL;

float2	g_vRecipLightMapSize		: GLOBAL = { 1.f / 256.f, 1.f / 256.f};
float2	g_vRecipShadowMapSize		: GLOBAL = { 1.f / 256.f, 1.f / 256.f};

float	g_fZBias					: GLOBAL;

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
// ShadowMap
texture sdShadowBuf
<
	string NTM = "shader";
	int NTMIndex = 3;
>;

sampler sdShadowSampler = sampler_state
{
	Texture = (sdShadowBuf);
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
	float4	vProjPos		: POSITION;
	float3	vWorldPos		: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(float3 vPos : POSITION0, float2 vUVSetLightMap : TEXCOORD1)
{
	VS_OUTPUT kOutput;

	float2 vUVSet = vUVSetLightMap * float2(2.f, -2.f) + float2(-1.f, 1.f);
	vUVSet = vUVSet + 0.5f * g_vRecipLightMapSize * float2(-1.f, 1.f);
	
	kOutput.vProjPos = float4(vUVSet, 0.f, 1.f);
	kOutput.vWorldPos = mul(float4(vPos, 1.f), g_mWorld);
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
float PCF(sampler sdShadowSampler, float fShadowPixelDepth, float fRecipShadowMapSize, float2 vUVSetShadow, float fShadowBias)
{
	// x x x
	// x . x
	// x x x

	fShadowPixelDepth -=  fShadowBias;	// 面上z-fighting
	float3 vDepth = 0.f;
	float3 vOcclusion = 0.f;
	float fOcclusionAddition = 0.f;
	float2 vUVSet = 0.f;
	float2 vEncodedDepth = 0.f;
	
	// row0 - 00 01 02
	vUVSet.y = vUVSetShadow.y - fRecipShadowMapSize;
	
	vUVSet.x = vUVSetShadow.x - fRecipShadowMapSize;
	vEncodedDepth = tex2D(sdShadowSampler, vUVSet).xy;
	vDepth.x = UnpackDepth(vEncodedDepth) * g_fShadowNearCameraFarPlane;
	vDepth.x = lerp(9999.f, vDepth.x, sign(vDepth.x));
	
	vUVSet.x = vUVSetShadow.x;
	vEncodedDepth = tex2D(sdShadowSampler, vUVSet).xy;
	vDepth.y = UnpackDepth(vEncodedDepth) * g_fShadowNearCameraFarPlane;
	vDepth.y = lerp(9999.f, vDepth.y, sign(vDepth.y));	
	
	vUVSet.x = vUVSetShadow.x + fRecipShadowMapSize;
	vEncodedDepth = tex2D(sdShadowSampler, vUVSet).xy;
	vDepth.z = UnpackDepth(vEncodedDepth) * g_fShadowNearCameraFarPlane;
	vDepth.z = lerp(9999.f, vDepth.z, sign(vDepth.z));	
	
	vOcclusion = fShadowPixelDepth <= vDepth;
	fOcclusionAddition += dot(vOcclusion, float3(1.f, 1.f, 1.f));
	
	// row1 - 10 11 12
	vUVSet.y = vUVSetShadow.y;
	vUVSet.x = vUVSetShadow.x - fRecipShadowMapSize;

	vEncodedDepth = tex2D(sdShadowSampler, vUVSet).xy;
	vDepth.x = UnpackDepth(vEncodedDepth) * g_fShadowNearCameraFarPlane;
	vDepth.x = lerp(9999.f, vDepth.x, sign(vDepth.x));
	
	vUVSet.x = vUVSetShadow.x;
	vEncodedDepth = tex2D(sdShadowSampler, vUVSet).xy;
	vDepth.y = UnpackDepth(vEncodedDepth) * g_fShadowNearCameraFarPlane;
	vDepth.y = lerp(9999.f, vDepth.y, sign(vDepth.y));	
	
	vUVSet.x = vUVSetShadow.x + fRecipShadowMapSize;
	vEncodedDepth = tex2D(sdShadowSampler, vUVSet).xy;
	vDepth.z = UnpackDepth(vEncodedDepth) * g_fShadowNearCameraFarPlane;
	vDepth.z = lerp(9999.f, vDepth.z, sign(vDepth.z));	
	
	vOcclusion = fShadowPixelDepth <= vDepth;
	fOcclusionAddition += dot(vOcclusion, float3(1.f, 1.f, 1.f));
	
	// row2 - 20 21 22
	vUVSet.y = vUVSetShadow.y + fRecipShadowMapSize;
	vUVSet.x = vUVSetShadow.x - fRecipShadowMapSize;

	vEncodedDepth = tex2D(sdShadowSampler, vUVSet).xy;
	vDepth.x = UnpackDepth(vEncodedDepth) * g_fShadowNearCameraFarPlane;
	vDepth.x = lerp(9999.f, vDepth.x, sign(vDepth.x));
	
	vUVSet.x = vUVSetShadow.x;
	vEncodedDepth = tex2D(sdShadowSampler, vUVSet).xy;
	vDepth.y = UnpackDepth(vEncodedDepth) * g_fShadowNearCameraFarPlane;
	vDepth.y = lerp(9999.f, vDepth.y, sign(vDepth.y));	
	
	vUVSet.x = vUVSetShadow.x + fRecipShadowMapSize;
	vEncodedDepth = tex2D(sdShadowSampler, vUVSet).xy;
	vDepth.z = UnpackDepth(vEncodedDepth) * g_fShadowNearCameraFarPlane;
	vDepth.z = lerp(9999.f, vDepth.z, sign(vDepth.z));	
	
	vOcclusion = fShadowPixelDepth <= vDepth;
	fOcclusionAddition += dot(vOcclusion, float3(1.f, 1.f, 1.f));
	
	return fOcclusionAddition / 9.f;
}
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	// 计算当前像素投影坐标系位置
	float4 vShadowSpacePos = mul(float4(kInput.vWorldPos, 1.f), g_mWorldToShadowSpaceNear);
	
	// 计算当前像素PCF阴影
	float 	fPixelDepth = vShadowSpacePos.w;
	float2	vUVSetShadow = vShadowSpacePos.xy;
	float fShadow = PCF(sdShadowSampler, fPixelDepth, g_vRecipShadowMapSize, vUVSetShadow, g_fZBias);
	
	return fShadow;
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique StaticMesh_Bake
<
	string Description = "StaticMesh_Bake";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main ();
	}
}
//---------------------------------------------------------------------------------------