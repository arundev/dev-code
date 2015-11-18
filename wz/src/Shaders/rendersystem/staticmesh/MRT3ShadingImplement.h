//*************************************************************************************************
// 内容:	StaticMesh的MRT3ShadingPass着色器具体实现(V3版)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-19
// 最后修改:	2013-05-27
//*************************************************************************************************
// 1.这里不需要定义避免重复的宏,因为本文件会被重复包含
// 2.这里根据Flag不同动态生成名字,避免出现命名重复
#include "static_mesh_common.h"

//---------------------------------------------------------------------------------------
// 取消一些内部会用到的预定义宏
//---------------------------------------------------------------------------------------
#undef VS_INPUT
#undef VS_OUTPUT
#undef VS_SHADER
#undef PS_SHADER
#undef TECHNIQUE

#undef SPECLIGHT
#undef RIMLIGHT
#undef GLOWLIGHT

//---------------------------------------------------------------------------------------
// 根据CHANNELS_FLAG的值来提取当前启用的通道情况
//---------------------------------------------------------------------------------------
#if TEST_CHANNEL(CHANNELS_FLAG, PIXEL_SPECULAR_LIGHTTING_CHANNEL)
	#define SPECLIGHT 1
#else
	#define SPECLIGHT 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, PIXEL_RIM_LIGHTTING_CHANNEL)
	#define RIMLIGHT 1
#else
	#define RIMLIGHT 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, PIXEL_GLOW_LIGHTTING_CHANNEL)
	#define GLOWLIGHT 1
#else
	#define GLOWLIGHT 0
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
void PS_SHADER(VS_OUTPUT kInput,
#if GLOWLIGHT
			out float4 finalColor	: COLOR0,	\
			out float4 glowColor	: COLOR1
#else
			out float4 finalColor	: COLOR0
#endif
)
{
	// 深度与法线纹理
	// @{
	float4 vGeomData 	= tex2D(sdGeomBufferSampler, kInput.vProjPos);
	float3 vViewNormal 	= UnpackNormal(vGeomData.zw);
	//float  fDepth		= UnpackDepth(vGeomData.xy);
	// @}
	
	
	// 光照贴图纹理
	// @{
	// g_fLocalLightRadio是为了修改点光源下,摄像机在某个位置的时候,建筑物会有一个从暗道亮的过程
	float4 vLocalLight	= tex2D(sdLightBufferSampler, kInput.vProjPos) * g_fLocalLightRadio;
	// }@
	
	
	// 材质纹理
	// @{
	float4 vMatData0	= tex2D(sdMatBuffer0Sampler, kInput.vProjPos);
	
	// 解压Diffuse
	float3 vDiffuseTex	= vMatData0.rgb;
	
	// decode lightmap and edge sign
	// 解压LightMap和Edge标记
	float fLMAndEdge 	= vMatData0.a * 255.0f;
	float fLightMap		= 0.0f;
	float fRimLightMask	= modf(fLMAndEdge / 2.0f, fLightMap);
	fRimLightMask		= fRimLightMask * 2.0f;
	fLightMap			/= 127.0f;
	
#if SPECLIGHT
	float4 vMatData1	= tex2D(sdMatBuffer1Sampler, kInput.vProjPos);
	
	// 解压Specular/Shiness
	float3 vSpecularTex	= vMatData1.rgb;
	float fShiness		= vMatData1.a * 255.0f;
#endif

#if GLOWLIGHT
	float4 vMatData2	= tex2D(sdMatBuffer2Sampler, kInput.vProjPos);
	
	// 解压Glow
	float3 vGlowTex 	= vMatData2.rgb;
#endif
	// @}
	
	
	// 光照计算
	// @{
	float3	vDiffuseLight;
	float3	vSpecularLight;
	float3	vViewVector = -normalize(kInput.vViewPos);
#if SPECLIGHT
	AccumLighting(vViewVector, vViewNormal, fShiness, fLightMap, vLocalLight, vDiffuseLight, vSpecularLight);
#else
	AccumLightingOnlyDiffuse(vViewNormal, fLightMap,  vLocalLight, vDiffuseLight);
#endif
	// @}
	
	
	// RimLight计算
	// @{
#if	RIMLIGHT
	float rimlightattenuation = 1.0f;
	float eta = 0.99f;
	float power = 2.5f;
	float factor = ((1.0f - eta) * (1.0f - eta)) /  ((1.0f + eta) * (1.0f + eta));
	float fresnel = factor + (1.0f - factor) * pow((1.0f - saturate(dot(vViewVector, vViewNormal))), power);
	float anglecos = (dot(vViewVector, g_vMainLightDir) + 1.0f)/ 2.0f;
	float rimdiffuse = clamp(anglecos, 0.35f, 0.7f);
	float brightness = dot(g_vMainLightColor, float3(0.2125f, 0.7154f, 0.0721f));
	float3 rimcolor = g_vMainLightColor / brightness;
	float3 vRimLight = fRimLightMask * fresnel * clamp(brightness * 1.8f, 0.2f, 0.35f) * rimcolor * rimdiffuse * rimlightattenuation;
#else
	float3 vRimLight = 0.0f;
#endif	
	// @}
	
	
	// 合成光照
	// @{
#if GLOWLIGHT
	#if SPECLIGHT
		float3 vColor = vDiffuseTex * (vDiffuseLight + vGlowTex) + vSpecularTex * vSpecularLight + vRimLight;
	#else
		float3 vColor = vDiffuseTex * (vDiffuseLight + vGlowTex) + vRimLight;
	#endif
#else
	#if SPECLIGHT
		float3 vColor = vDiffuseTex * vDiffuseLight + vSpecularTex * vSpecularLight + vRimLight;
	#else
		float3 vColor = vDiffuseTex * vDiffuseLight + vRimLight;
	#endif
#endif
	
	finalColor = float4(vColor, 0.f);
	
#if GLOWLIGHT
	glowColor = float4(lerp(float3(0.f, 0.f, 0.f), vColor, vMatData2.a), 0.f);
#endif
	// @}
};

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
#define TECHNIQUE HEADER_KEY(MRT3Shading, CHANNELS_FLAG)
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