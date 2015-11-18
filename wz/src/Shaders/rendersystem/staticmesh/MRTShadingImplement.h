//*************************************************************************************************
// 内容:	StaticMesh的MRTShadingPass着色器具体实现(V2版,已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-19
// 最后修改:	2013-05-28
//*************************************************************************************************
#include "static_mesh_common.h"

//---------------------------------------------------------------------------------------
// 取消一些内部会用到的预定义宏
//---------------------------------------------------------------------------------------
#undef VS_INPUT
#undef VS_OUTPUT
#undef VS_SHADER
#undef PS_SHADER
#undef TECHNIQUE

#undef RIMLIGHT

//---------------------------------------------------------------------------------------
// 根据CHANNELS_FLAG的值来提取当前启用的通道情况
//---------------------------------------------------------------------------------------
#if TEST_CHANNEL(CHANNELS_FLAG, RIM_LIGHTTING_CHANNEL)
	#define RIMLIGHT 1
#else
	#define RIMLIGHT 0
#endif

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
#define VS_OUTPUT HEADER_KEY(VS_Output, CHANNELS_FLAG)
struct VS_OUTPUT
{
	float4	vPos			: POSITION;
	float2 	vProjPos		: TEXCOORD0;
	float3	vViewPos		: TEXCOORD1;
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
#define VS_SHADER HEADER_KEY(VS_Main, CHANNELS_FLAG)
VS_OUTPUT VS_SHADER(float3 vPos : POSITION, float2 vUVSet0 : TEXCOORD0)
{
	VS_OUTPUT kOutput;
	
	kOutput.vPos 		= float4(vPos, 1.0f);
	kOutput.vProjPos 	= vUVSet0 + g_vHalfPixelOffset;
	kOutput.vViewPos	= float3(vPos.xy + g_vCameraFrustum.xy, g_vCameraFrustum.z);
	
	return kOutput;
};

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
#define PS_SHADER HEADER_KEY(PS_Main, CHANNELS_FLAG)
float4 PS_SHADER(VS_OUTPUT kInput) : COLOR0
{
	// 深度与法线纹理
	// @{
	float4 vGeomData 	= tex2D(sdGeomBufferSampler, kInput.vProjPos);
	float3 vViewNormal 	= UnpackNormal(vGeomData.zw);
	float  fDepth		= UnpackDepth(vGeomData.xy);
	// @}
	
	
	// 光照贴图纹理
	// @{
	// g_fLocalLightRadio是为了修改点光源下,摄像机在某个位置的时候,建筑物会有一个从暗道亮的过程
	float4 vLocalLight	= tex2D(sdLightBufferSampler, kInput.vProjPos) * g_fLocalLightRadio;
	// }@
	
	
	// 材质纹理
	// @{
	float4 vMatData	= tex2D(sdMatBufferSampler, kInput.vProjPos);
	
	// 解出shiness
	float3 bits = frac(vMatData.rgb * 255.f / 2.f) * 2.f;
	
#if RIMLIGHT
	// 只有角色拥有rimlight效果,在角色上shiness即是rimlight
	float fRimlightAttenuration = dot(bits.rgb, float3(4.f, 2.f, 1.f)) / 7.f;
	float fShiness = 20.f;
#else
	float fShiness = dot(bits.rgb, float3(4.f, 2.f, 1.f)) / 7.f * 70.f;
#endif

	// 解出gloss和lightmap
	float inter = vMatData.a * 255.f;
	float fGlossSpec = 0.f;
	float fLightMap = modf(inter / 8.f, fGlossSpec);
	fLightMap  = fLightMap * 8.f / 7.f;
	fGlossSpec /= 31.f;
	// @}
	
	
	// 光照计算
	// @{
	float3	vDiffuseLight;
	float3	vSpecularLight;
	float3	vViewVector = -normalize(kInput.vViewPos);
	AccumLighting(vViewVector, vViewNormal, fShiness, fLightMap, vLocalLight, vDiffuseLight, vSpecularLight);
	// @}
	
	
	// RimLight计算
	// @{
#ifdef	RIMLIGHT
	float rimlightattenuation = 1.0f;
	float eta = 0.99f;
	float power = 2.5f;
	float factor = ((1.0f - eta) * (1.0f - eta)) /  ((1.0f + eta) * (1.0f + eta));
	float fresnel = factor + (1.0f - factor) * pow((1.0f - saturate(dot(vViewVector, vViewNormal))), power);
	float anglecos = (dot(vViewVector, g_vMainLightDir) + 1.0f)/ 2.0f;
	float rimdiffuse = clamp(anglecos, 0.35f, 0.7f);
	float brightness = dot(g_vMainLightColor, float3(0.2125f, 0.7154f, 0.0721f));
	float3 rimcolor = g_vMainLightColor / brightness;
	float3 vRimLight = fresnel * clamp(brightness * 1.8f, 0.6f, 1.f) * rimcolor * rimdiffuse * rimlightattenuation;

	//float3 vRimLight = 0.0f;
#else
	float3 vRimLight = 0.0f;
#endif	
	// @}
	
	
	// 合成光照
	float3 vColor = vMatData.rgb * vDiffuseLight + fGlossSpec * vSpecularLight + vRimLight;

	return float4(vColor, 0.f);
};

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
#define TECHNIQUE HEADER_KEY(MRTShading, CHANNELS_FLAG)
technique TECHNIQUE
<
	string Description = MAKE_STRING(TECHNIQUE);
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_SHADER ();
		PixelShader 	= compile ps_2_a PS_SHADER ();
	}
};
//---------------------------------------------------------------------------------------