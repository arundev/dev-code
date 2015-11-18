//*************************************************************************************************
// 内容:	贴花阴影
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-08
// 最后修改:	
//*************************************************************************************************
#include "DecalCommon.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
float4	g_vShadowVertexTransform	: GLOBAL = float4(1.f, 0.f, 0.f, 1.414f);
float3	g_vShadowTranslate			: GLOBAL = float3(0.f, 0.f, 0.f);
float4 	g_vShadowTextureTransform	: GLOBAL = float4(0.f, 0.f, 1.f, 1.f);	// xy sx sy
float4	g_vShadowChannel			: GLOBAL;

float4x4	g_mWorldToProjectorUV	: GLOBAL;

// 阴影实例数组
float4	g_akShadowInstance[64]		: GLOBAL;

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT_SHADOW
{
	float4	vProjPos				: POSITION;
	float4	vUVSetProjUVW			: TEXCOORD0;	
	float4	vUVSetFarClipWorldPos	: TEXCOORD1;	
	float3	vUVSetWorldPos			: TEXCOORD2;
};
//---------------------------------------------------------------------------------------
struct VS_OUTPUT_SHADOWINSTANCE
{
	float4	vProjPos				: POSITION;
	float4	vUVSetProjUVW			: TEXCOORD0;	
	float4	vUVSetFarClipWorldPos	: TEXCOORD1;	
	float3	vUVSetWorldPos			: TEXCOORD2;
	float3	vUVSetCenterPos			: TEXCOORD3;
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
VS_OUTPUT_SHADOW VS_Main_Shadow(float4 vPos : POSITION0)
{
	VS_OUTPUT_SHADOW kOutput = (VS_OUTPUT_SHADOW)0;
	
	// 计算世界坐标,投影坐标
	float3 vWorldPos = vPos.xyz * g_vShadowVertexTransform.w + g_vShadowVertexTransform.xyz * vPos.w + g_vShadowTranslate;
	float4 vProjPos = mul(float4(vWorldPos, 1.f), g_mViewProj);
	kOutput.vProjPos = vProjPos;
	kOutput.vUVSetWorldPos = vWorldPos;
	
	//
	float2 vProjTextureUV = vProjPos.xy / vProjPos.w;
	kOutput.vUVSetFarClipWorldPos = mul(float4(vProjTextureUV, 1, 1), g_mDepthToWorld) * vProjPos.w;
	
	//
	float2 vBiasedTextureUV = GetTexCoordForPixel(vProjTextureUV);
	kOutput.vUVSetProjUVW = float4(vBiasedTextureUV, 0, 1) * vProjPos.w;
	
	return kOutput;
}
//---------------------------------------------------------------------------------------
// 圆盘阴影
VS_OUTPUT_SHADOWINSTANCE VS_Main_RoundShadow(float4 vPos : POSITION0, int4 iGeoId : TEXCOORD0)
{
	VS_OUTPUT_SHADOWINSTANCE kOutput = (VS_OUTPUT_SHADOWINSTANCE)0;
	
	// 
	float3 vLocalPos = vPos.xyz * min(g_akShadowInstance[iGeoId.x].w * 0.5f, 1.5f);	// 圆盘阴影尺寸限制到一个最大值
	float3 vWorldPos = vLocalPos + float3(0, 0, -1) * g_akShadowInstance[iGeoId.x].w * vPos.w * 1.5f + g_akShadowInstance[iGeoId.x].xyz;
	float4 vProjPos  = mul(float4(vWorldPos, 1.f), g_mViewProj);
	kOutput.vProjPos = vProjPos;
	kOutput.vUVSetWorldPos = vWorldPos;

	//
	float2 vProjTextureUV = vProjPos.xy / vProjPos.w;
	kOutput.vUVSetFarClipWorldPos = mul(float4(vProjTextureUV, 1, 1), g_mDepthToWorld) * vProjPos.w;
	
	//
	float2 vBiasedTextureUV = GetTexCoordForPixel(vProjTextureUV);
	kOutput.vUVSetProjUVW = float4(vBiasedTextureUV, 0, 1) * vProjPos.w;
	kOutput.vUVSetCenterPos = float3(g_akShadowInstance[iGeoId.x].xy, abs(vLocalPos.x));
	
	return kOutput;
}
//---------------------------------------------------------------------------------------
// 使用圆盘阴影后的最终全屏Pass
float4 VS_Main_RoundQuad(float4 vPos : POSITION0) : POSITION0
{
	return vPos;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 PS_Main_Shadow(VS_OUTPUT_SHADOW kInput) : COLOR0
{
	// 获取地形深度
	float2 vPackedDepth = tex2D(sdGeomSampler, kInput.vUVSetProjUVW.xy / kInput.vUVSetProjUVW.w).xy;
	float fDepth = UnpackDepth(vPackedDepth.xy);
	
	// 反算世界坐标
	// (根据线性深度,对相机位置和远平面对应点位置进行插值)
	float3 vWorldPos = lerp(g_vViewPos, kInput.vUVSetFarClipWorldPos.xyz, fDepth);
	
	// 转换到贴花投射坐标系
	float4 vUVW = mul(float4(vWorldPos, 1.f), g_mWorldToProjectorUV);
	clip(vUVW.xyz);
	clip(1.f - vUVW.xyz);
	
	// 
	float2 vUVSetUVW = vUVW.xy * g_vShadowTextureTransform.zw + g_vShadowTextureTransform.xy;
	float4 vColor = tex2D(sdDarkSampler, vUVSetUVW);
	
	//
	float fAlpha = dot(vColor, g_vShadowChannel);
	clip(fAlpha - 0.9f);	// 避免阴影重叠时变黑
	
	return float4(0, 0, 0, fAlpha * 0.4f);
}
//---------------------------------------------------------------------------------------
float4 PS_Main_RoundShadow(VS_OUTPUT_SHADOWINSTANCE kInput) : COLOR0
{
	// 获取地形深度
	float2 vPackedDepth = tex2D(sdGeomSampler, kInput.vUVSetProjUVW.xy / kInput.vUVSetProjUVW.w).xy;
	float fDepth = UnpackDepth(vPackedDepth.xy);
	
	// 反算世界坐标
	// (根据线性深度,对相机位置和远平面对应点位置进行插值)
	float3 vWorldPos = lerp(g_vViewPos, kInput.vUVSetFarClipWorldPos.xyz, fDepth);
	
	//
	float2 vUVSetUVW = (vWorldPos.xy - kInput.vUVSetCenterPos.xy) / kInput.vUVSetCenterPos.z;
	vUVSetUVW = vUVSetUVW * 0.5f + 0.5f;
	vUVSetUVW += 0.5f / 32.f;
	clip(vUVSetUVW);
	clip(1.f - vUVSetUVW);
	
	float4 vColor = tex2D(sdDarkSampler, vUVSetUVW);
	clip(vColor.r - 0.1f);
	
	return float4(0, 0, 0, vColor.r * 0.5f);
}
//---------------------------------------------------------------------------------------
float4 PS_Main_RoundQuad() : COLOR0
{
	return float4(0, 0, 0, 0.5f);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Decal_ActorProjectionShadow
<
	string Description = "Decal_ActorProjectionShadow";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main_Shadow ();
		PixelShader 	= compile ps_2_a PS_Main_Shadow ();
	}
}
//---------------------------------------------------------------------------------------
technique Decal_ActorRoundShadow
<
	string Description = "Decal_ActorRoundShadow";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main_RoundShadow ();
		PixelShader 	= compile ps_2_a PS_Main_RoundShadow ();
	}
}
//---------------------------------------------------------------------------------------
technique Decal_ActorRoundShadowShading
<
	string Description = "Decal_ActorRoundShadowShading";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main_RoundQuad ();
		PixelShader 	= compile ps_2_a PS_Main_RoundQuad ();
	}
}
//---------------------------------------------------------------------------------------








