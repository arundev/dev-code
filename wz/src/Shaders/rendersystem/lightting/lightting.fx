//*************************************************************************************************
// 内容:	局部光照着色器
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-08
// 最后修改:	2014-04-20
//*************************************************************************************************
#include "../common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
// 光源颜色(DiffuseColor)
float3		g_vLocalLightColor			: GLOBAL;

// 光源高光强度
float		g_fLocalLightShiness		: GLOBAL = 70.f;

// 光源阴影参数(AmbientColor)
float4		g_vLocalLightShadow			: GLOBAL;
#define		g_vLocalLightShadowColor	g_vLocalLightShadow.xyz
#define		g_vLocalLightShadowTotal	g_vLocalLightShadow.w

// 光源位置(观察坐标系)
float3		g_vLocalLightViewPos		: GLOBAL;

// 光源自身额外参数
float4		g_vLocalLightAttenuParams	: GLOBAL;

// 观察空间到光照切向空间变换矩阵
float4x4	g_mViewToLightClipSpace			: GLOBAL;

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
// 地形屏幕深度缓存
SD_POINT_CLAMP_SAMPLE(0, sdGeomSampler, sdGeomBuf, false);		

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos				: POSITION0;	///< 输入坐标
};
//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos			: POSITION;		///< 投影坐标
	float4	vUVSetScreenTex		: TEXCOORD0;	///< 纠正半像素偏移的屏幕纹理坐标(考虑放入PS计算)
	float4	vUVFarClipViewPos	: TEXCOORD1;	///< 当前点对应远裁剪面上的点的观察坐标
};
//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// 模型坐标转换到投影坐标
	kOutput.vProjPos = mul(float4(kInput.vPos, 1.0f), g_mWorldViewProj);
	
	// 投影空间齐次坐标
	float2 vTProjPos = kOutput.vProjPos.xy / kOutput.vProjPos.w;
	
	// 当前点对应远裁剪面上的点的世界坐标
	float4 vUVFarClipProjPos  = float4(vTProjPos.xy, 1.f, 1.f);
	kOutput.vUVFarClipViewPos = mul(vUVFarClipProjPos, g_mDepthToView) * kOutput.vProjPos.w;
	
	// 投影坐标转换到屏幕纹理坐标(修正半像素偏移)
	//float2 vUVSetScreenTex = vTProjPos.xy * float2(0.5f, -0.5f) + float2(0.5f, 0.5f) + g_vHalfPixelOffset;
	float2 vUVSetScreenTex = GetTexCoordForPixel(vTProjPos);
	kOutput.vUVSetScreenTex = float4(vUVSetScreenTex, 0.f, 1.f) * kOutput.vProjPos.w;
	
	return kOutput;
}
//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
// 点光源
float4 PS_Main_PointLightting(VS_OUTPUT kInput) : COLOR0
{
	// DepthBuf
	// @{
	// 屏幕纹理坐标
	float2 vUVSetScreenTex = kInput.vUVSetScreenTex.xy / kInput.vUVSetScreenTex.w;
	
	// 获取地形深度
	float4 vGeoTex = tex2D(sdGeomSampler, vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);
	
	// 反算观察坐标系坐标
	float3 vViewPos = kInput.vUVFarClipViewPos.xyz / kInput.vUVFarClipViewPos.w * fDepth;
	// @}

	
	// Lightting
	// @{
	// 光线方向
	float3 vLightVec = g_vLocalLightViewPos - vViewPos;
	float fLightDis = length(vLightVec);
	
	// 计算衰减(先计算径向衰减）
	float fAttenu = g_vLocalLightAttenuParams.x * fLightDis + g_vLocalLightAttenuParams.y;
	//clip(fAttenu);
	fAttenu = saturate(fAttenu);
	
	// 计算漫反射光照
	float3 L = vLightVec / fLightDis;
	float3 N = UnpackNormal(vGeoTex.zw);
	float NL = saturate(dot(N, L));
	//clip(g_vLocalLightShadowTotal + NL);
	float3 vDiffuseLight = lerp(g_vLocalLightShadowColor, g_vLocalLightColor, NL) * fAttenu;
	
	// 计算高光
	float3 V = -normalize(vViewPos);
	float3 R = reflect(-L, N);
	float VR = saturate(dot(V, R));
	//float VR = max((dot(V, R), 0.001);
	float3 vSpecularLight = pow(VR, g_fLocalLightShiness) * vDiffuseLight;
	float fSpecularLumin = dot(vSpecularLight, g_vLuminScale);
	// @}
	
	return float4(vDiffuseLight, fSpecularLumin) * g_fLocalLightScale;
//	return float4(vDiffuseLight, 0.f);
//	return float4(lerp(g_vLocalLightShadowColor, g_vLocalLightColor, NL), 0.f);
//	return float4(fAttenu, fAttenu, fAttenu, 0.f);
//	return float4(1.f, 1.f, 1.f, 0.0f);

//	float ff1 = fLightDis/10.f;
//	return float4(ff1, ff1, ff1, 0.f);
	
//	float ff2 = length(g_vLocalLightViewPos);
//	float ff2_c = saturate(ff2/20.f);

//	float ff3 = length(vViewPos);
//	float ff3_c = saturate(ff3/20.f);

//	float3 vWorldPos = mul(float4(vViewPos, 1.f), g_mInvView).xyz;
//	float ff4 = length(vWorldPos);
//	float ff4_c = saturate(ff4/20.f);

//	float3 vWorldPos1 = lerp(g_vViewPos, kInput.vUVFarClipWorldPos, fDepth);	
//	float ff5_c = vWorldPos1.x / 20.f;
	
//	float3 vViewPos1 = mul(vWorldPos1, g_mView).xyz;
//	float ff2 = length(vViewPos);
//	float ff2_c = saturate(ff2/50.f);
//	return float4(ff2_c, ff2_c, ff2_c, 0.f);

//	float value = fAttenu;
//	if (value > 1.f)
//		return float4(1.f, 0.f, 0.f, 0.f);
//	else if (value > 0.6f)
//		return float4(0.f, 1.f, 0.f, 0.f);
//	else if (value > 0.3f)
//		return float4(0.f, 0.f, 1.f, 0.f);
//	else
//		return float4(1.f, 1.f, 1.f, 0.f);
}
//---------------------------------------------------------------------------------------
// 聚光灯
float4 PS_Main_SpotLightting(VS_OUTPUT kInput) : COLOR0
{
	// DepthBuf
	// @{
	// 屏幕纹理坐标
	float2 vUVSetScreenTex = kInput.vUVSetScreenTex.xy / kInput.vUVSetScreenTex.w;
	
	// 获取地形深度
	float4 vGeoTex = tex2D(sdGeomSampler, vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);
	
	// 反算观察坐标系坐标
	float3 vViewPos = kInput.vUVFarClipViewPos.xyz / kInput.vUVFarClipViewPos.w * fDepth;
	// @}
	
	
	// Lightting
	// @{
	// 反算光源空间坐标
	float4 vLightSpacePos = mul(float4(vViewPos, 1.0), g_mViewToLightClipSpace);
	//clip(vLightSpacePos.zw);
	vLightSpacePos.xy /= vLightSpacePos.w;
	vLightSpacePos.z = saturate(vLightSpacePos.z);
	
	// 光照参数(先计算切向衰减，再计算径向衰减)
	float fWeight = length(vLightSpacePos.xy);
	//clip(1.0 - fWeight);
	float fAttenu = saturate(g_vLocalLightAttenuParams.x * fWeight + g_vLocalLightAttenuParams.y);
	fAttenu	*= vLightSpacePos.z;
	
	// 计算漫反射光照
	float3 L = normalize(g_vLocalLightViewPos - vViewPos);
	float3 N = UnpackNormal(vGeoTex.zw);
	float NL = saturate(dot(N, L));
	//clip(g_vLocalLightShadowTotal + NL);
	float3 vDiffuseLight = lerp(g_vLocalLightShadowColor, g_vLocalLightColor, NL) * fAttenu;
	
	// 计算高光
	float3 V = -normalize(vViewPos);
	float3 R = reflect(-L, N);
	float VR = saturate(dot(V, R));
	float3 vSpecularLight = pow(VR, g_fLocalLightShiness) * vDiffuseLight;
	float fSpecularLumin = dot(vSpecularLight, g_vLuminScale);
	// @}
	
	return float4(vDiffuseLight, fSpecularLumin) * g_fLocalLightScale;
}
//---------------------------------------------------------------------------------------
// 矩形聚光灯
float4 PS_Main_RectSpotLightting(VS_OUTPUT kInput) : COLOR0
{
	// DepthBuf
	// @{
	// 屏幕纹理坐标
	float2 vUVSetScreenTex = kInput.vUVSetScreenTex.xy / kInput.vUVSetScreenTex.w;
	
	// 获取地形深度
	float4 vGeoTex = tex2D(sdGeomSampler, vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);
	
	// 反算观察坐标系坐标
	float3 vViewPos = kInput.vUVFarClipViewPos.xyz / kInput.vUVFarClipViewPos.w * fDepth;
	// @}
	
	
	// Lightting
	// @{
	// 反算光源空间坐标
	float4 vLightSpacePos = mul(float4(vViewPos, 1.0), g_mViewToLightClipSpace);
	//clip(vLightSpacePos.zw);
	vLightSpacePos.xy /= vLightSpacePos.w;
	vLightSpacePos.z = saturate(vLightSpacePos.z);
	
	// 光照参数(先计算切向衰减，再计算径向衰减)
	float2 vXYOffset = abs(vLightSpacePos.xy);
	//clip(1.0 - vXYOffset)
	float fWeight = max(vXYOffset.x, vXYOffset.y);
	//clip(1.0 - fWeight);
	
	float fAttenu = saturate(g_vLocalLightAttenuParams.x * fWeight + g_vLocalLightAttenuParams.y);
	fAttenu	*= vLightSpacePos.z;
	
	// 计算漫反射光照
	float3 L = normalize(g_vLocalLightViewPos - vViewPos);
	float3 N = UnpackNormal(vGeoTex.zw);
	float NL = saturate(dot(N, L));
	//clip(g_vLocalLightShadowTotal + NL);
	float3 vDiffuseLight = lerp(g_vLocalLightShadowColor, g_vLocalLightColor, NL) * fAttenu;
	
	// 计算高光
	float3 V = -normalize(vViewPos);
	float3 R = reflect(-L, N);
	float VR = saturate(dot(V, R));
	float3 vSpecularLight = pow(VR, g_fLocalLightShiness) * vDiffuseLight;
	float fSpecularLumin = dot(vSpecularLight, g_vLuminScale);
	// @}
	
	return float4(vDiffuseLight, fSpecularLumin) * g_fLocalLightScale;
}
//---------------------------------------------------------------------------------------
// GB点光源
float4 PS_Main_GBPointLightting(VS_OUTPUT kInput) : COLOR0
{
	// DepthBuf
	// @{
	// 屏幕纹理坐标
	float2 vUVSetScreenTex = kInput.vUVSetScreenTex.xy / kInput.vUVSetScreenTex.w;
	
	// 获取地形深度
	float4 vGeoTex = tex2D(sdGeomSampler, vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);
	
	// 反算观察坐标系坐标
	float3 vViewPos = kInput.vUVFarClipViewPos.xyz / kInput.vUVFarClipViewPos.w * fDepth;
	// @}
	
	
	// Lightting
	// @{
	// 光线方向
	float3 vLightVec = g_vLocalLightViewPos - vViewPos;
	float fLightDis = length(vLightVec);
	
	// 光照参数
	float fAttenu = 1.0 / dot(g_vLocalLightAttenuParams.xyz, float3(1.0, fLightDis, fLightDis * fLightDis));
	//clip(fAttenu);
	fAttenu = saturate(fAttenu);
	
	// 计算漫反射光照
	float3 L = vLightVec / fLightDis;
	float3 N = UnpackNormal(vGeoTex.zw);
	float NL = dot(N, L);
	//clip(NL);
	NL = saturate(NL);
	float3 vDiffuseLight = g_vLocalLightColor * NL * fAttenu;
	
	// 计算高光
	float3 V = -normalize(vViewPos);
	float3 R = reflect(-L, N);
	float VR = saturate(dot(V, R));
	//float VR = max((dot(V, R), 0.001);
	float3 vSpecularLight = pow(VR, g_fLocalLightShiness) * vDiffuseLight;
	float fSpecularLumin = dot(vSpecularLight, g_vLuminScale);
	// @}
	
	return float4(vDiffuseLight, fSpecularLumin) * g_fLocalLightScale;
}
//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Point_Lightting
<
	string Description = "Point_Lightting";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main ();
		PixelShader 	= compile ps_2_a PS_Main_PointLightting ();
	}
}
//---------------------------------------------------------------------------------------
technique Spot_Lightting
<
	string Description = "Spot_Lightting";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main ();
		PixelShader 	= compile ps_2_a PS_Main_SpotLightting ();
	}
}
//---------------------------------------------------------------------------------------
technique RectSpot_Lightting
<
	string Description = "RectSpot_Lightting";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main ();
		PixelShader 	= compile ps_2_a PS_Main_RectSpotLightting ();
	}
}
//---------------------------------------------------------------------------------------
technique GBPoint_Lightting
<
	string Description = "GBPoint_Lightting";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main ();
		PixelShader 	= compile ps_2_a PS_Main_GBPointLightting ();
	}
}
//---------------------------------------------------------------------------------------