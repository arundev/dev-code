//*************************************************************************************************
// 内容:	StaticMesh的MRTGeometryPass着色器具体实现(V2版,已废弃)
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
#undef PS_OUTPUT
#undef VS_SHADER
#undef PS_SHADER
#undef TECHNIQUE

// 用于定义通道启用情况
#undef DIFFUSEMAP
#undef NORMALMAP
#undef GLOSSMAP
#undef LIGHTMAP
#undef DETAILNORMALMAP
#undef ALPHATEST

// 用于定义着色器输入输出结构
#undef VS_OUTPUT_VERTEX_NORMAL
#undef VS_OUTPUT_BASE_UV
#undef VS_OUTPUT_UNIQUE_UV

//---------------------------------------------------------------------------------------
// 根据CHANNELS_FLAG的值来提取当前启用的通道情况
//---------------------------------------------------------------------------------------
#if TEST_CHANNEL(CHANNELS_FLAG, DIFFUSEMAP_CHANNEL)
	#define DIFFUSEMAP 1
#else
	#define DIFFUSEMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, NORMALMAP_CHANNEL)
	#define NORMALMAP 1
#else
	#define NORMALMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, GLOSSMAP_CHANNEL)
	#define GLOSSMAP 1
#else
	#define GLOSSMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, LIGHTMAP_CHANNEL)
	#define LIGHTMAP 1
#else
	#define LIGHTMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, DETAIL_NORMAL_CHANNEL)
	#define DETAILNORMALMAP 1
#else
	#define DETAILNORMALMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, ALPHA_TEST_FLAG)
	#define ALPHATEST 1
#else
	#define ALPHATEST 0
#endif

//---------------------------------------------------------------------------------------
// 根据当前启用的通道情况来决定具体的组合情况
//---------------------------------------------------------------------------------------
#if (!NORMALMAP)
	#define VS_OUTPUT_VERTEX_NORMAL	1
#else
	#define VS_OUTPUT_VERTEX_NORMAL 0
#endif

#if (DIFFUSEMAP || NORMALMAP || GLOSSMAP || DETAILNORMALMAP)
	#define VS_OUTPUT_BASE_UV 1
#else
	#define VS_OUTPUT_BASE_UV 0
#endif

#if (LIGHTMAP)
	#define VS_OUTPUT_UNIQUE_UV	1
#else
	#define VS_OUTPUT_UNIQUE_UV 0
#endif

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
#define VS_INPUT HEADER_KEY(VS_Input, CHANNELS_FLAG)
struct VS_INPUT
{
	float3 	vPos			: POSITION;
	
#if VS_OUTPUT_VERTEX_NORMAL
	float3  vNormal			: NORMAL;
#else
	float3	vNormal			: NORMAL;
	float3 	vBinormal		: BINORMAL;
	float3	vTangent		: TANGENT;
#endif

#if VS_OUTPUT_BASE_UV
	float2 	vUVSetBase		: TEXCOORD0;
#endif

#if VS_OUTPUT_UNIQUE_UV
	float2 	vUVSetLightMap	: TEXCOORD1;
#endif
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
#define VS_OUTPUT HEADER_KEY(VS_Output, CHANNELS_FLAG)
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;
	float	fDepth			: TEXCOORD0;
	
#if VS_OUTPUT_BASE_UV
	float2 	vUVSetBase		: TEXCOORD1;
#endif

#if VS_OUTPUT_UNIQUE_UV
	float2 	vUVSetLightMap	: TEXCOORD2;
#endif

#if VS_OUTPUT_VERTEX_NORMAL
	float3 	vNormal			: TEXCOORD3;
#else
	float3	vViewNormal		: TEXCOORD3;
	float3 	vViewBinormal	: TEXCOORD4;
	float3	vViewTangent	: TEXCOORD5;
#endif
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
#define VS_SHADER HEADER_KEY(VS_Main, CHANNELS_FLAG)
VS_OUTPUT VS_SHADER(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	
	kOutput.vProjPos = mul(float4(kInput.vPos, 1.0f), g_mWorldViewProj);
	
	kOutput.fDepth = dot(float4(kInput.vPos, 1.0f), g_mWorldView._m02_m12_m22_m32);
	
#if VS_OUTPUT_BASE_UV
	kOutput.vUVSetBase = kInput.vUVSetBase;
#endif

#if VS_OUTPUT_UNIQUE_UV
	kOutput.vUVSetLightMap = kInput.vUVSetLightMap * a_vLightMapTransform.zw + a_vLightMapTransform.xy;
#endif

#if VS_OUTPUT_VERTEX_NORMAL
	kOutput.vNormal 		= kInput.vNormal;
#else
	kOutput.vViewNormal 	= mul(float4(kInput.vNormal, 0.0f), g_mWorldView).xyz;
	kOutput.vViewBinormal 	= mul(float4(kInput.vBinormal, 0.0f), g_mWorldView).xyz;
	kOutput.vViewTangent 	= mul(float4(kInput.vTangent, 0.0f), g_mWorldView).xyz;
#endif

	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
#define PS_SHADER HEADER_KEY(PS_Main, CHANNELS_FLAG)
void PS_SHADER(VS_OUTPUT kInput, 				\
			   out float4 geoData	: COLOR0,	\
			   out float4 matData	: COLOR1
			   )
{
	// 计算Diffuse_Tex * Diffuse_Mat
	// 漫反射贴图的Alpha通道做AlphaTest
	// @{
#if DIFFUSEMAP
	float4 vDiffuseTex = tex2D(sdBaseSampler, kInput.vUVSetBase);
	float3 vCompactedDiffuse = vDiffuseTex.rgb * g_vDiffuseMaterialColor;
	
	#ifdef ALPHATEST
		clip(vDiffuseTex.a - g_fAlphaTestRef);
	#endif
#else
	clip(-1);	// 基纹理还没有被加载则不显示
	float3 vCompactedDiffuse = g_vDiffuseMaterialColor;
#endif	
	// @}
	

	// 计算Normal与Depth
	// @{
	// 观察空间深度归一化到[0,1]
	float fDepth = kInput.fDepth / g_fCameraFarClip;
	
#if VS_OUTPUT_VERTEX_NORMAL
	float3 vNormal = kInput.vNormal;
	
	// 合成DetailNormal到Normal
	#if DETAILNORMALMAP
		float3 vDetailNormal = tex2D(sdDetailNormalSampler, kInput.vUVSetBase * a_vDetailNormalUVTiling).xyz;
		vDetailNormal 		= (vDetailNormal - 0.5f) * 2.0f;
		vDetailNormal		*= a_vDetailNormalScale;
		
		// 合成
		vDetailNormal.xy 	*= g_fStaticMeshNormalScale * saturate(1.0f - kInput.fDepth / a_fDetailNormalFadeDistance);
		vNormal 			+= vDetailNormal;
	#endif
	
	// 转换到观察坐标系并归一化
	float3 vViewNormal = normalize(mul(float4(vNormal, 1.0f), g_mWorldView)).rgb;
#else
	// 采样NormalMap
	float3 vNormalTex 	= tex2D(sdNormalSampler, kInput.vUVSetBase).xyz;
	vNormalTex 			= (vNormalTex - 0.5f) * 2.0f;
	vNormalTex.xy		*= g_fStaticMeshNormalScale;

	// 合成DetailNormal到Normal
	#if DETAILNORMALMAP
		float3 vDetailNormal = tex2D(sdDetailNormalSampler, kInput.vUVSetBase * a_vDetailNormalUVTiling).xyz;
		vDetailNormal 		= (vDetailNormal - 0.5f) * 2.0f;
		vDetailNormal		*= a_vDetailNormalScale;
		
		// 合成
		vNormalTex			+= vDetailNormal;
		vNormalTex.xy 		*= g_fStaticMeshNormalScale * saturate(1.0f - kInput.fDepth / a_fDetailNormalFadeDistance);
	#endif

	// 归一化,并转换到观察坐标系
	vNormalTex = normalize(vNormalTex);
	
	float3 vNorNormal 	= normalize(kInput.vViewNormal);
	float3 vNorBinormal = normalize(kInput.vViewBinormal);
	float3 vNorTangent 	= normalize(kInput.vViewTangent);
	float3 vViewNormal = mul(vNormalTex, float3x3(vNorTangent, vNorBinormal, vNorNormal));
#endif
	
	geoData	= float4(PackDepth(fDepth), PackNormal(vViewNormal));
	// @}
	
	
	// 计算高光系数
	// @{
	// 这里可以直接让美术提供亮度,从而优化掉这个dot计算
	float fSpecMatLum = dot(g_vSpecularMaterial.rgb, g_vLuminScale2);
#if GLOSSMAP
	float fGlossSpec = tex2D(sdGlossSampler, kInput.vUVSetBase).r * fSpecMatLum;
#else
	//float fGlossSpec = fSpecMatLum;
	float fGlossSpec = 0.f;
#endif
	
	
	// 计算光照贴图
	//	1.LightMap信息放置在Alpha通道的高7bit
	//  2.RimLight信息放置在Alpha通道的最低1bit
	// @{
#if LIGHTMAP
	float fLightTex = dot(tex2D(sdDarkSampler, kInput.vUVSetLightMap), a_vLightMapChannel);
#else
	float fLightTex = a_fMainLightOcclusion;	// 武器也受到LightDepth遮挡
#endif

	// 整数版
	int iGlossSpec = fGlossSpec * 31.f + 0.5f;	///< gloss 5bit, lightmap 3bit
	int iGlossSpecShiftLeft3 = iGlossSpec * 8;
	int iLightMap = fLightTex * 7.f + 0.5f;
	matData.a = (iLightMap + iGlossSpecShiftLeft3) / 255.f;
	
	///< shiness 3bit
	int iShiness = int(g_fShiness);	///< 0-7中间一个值
	int3 inter = 0;
	inter.x = iShiness / 4;
	int re = iShiness - (inter.x * 4);
	inter.y = re / 2;
	inter.z = re - (inter.y * 2);
	
	int3 vDiffuseLeastBit0 = int3(vCompactedDiffuse * 255 / 2) * 2;
	matData.rgb = float3(vDiffuseLeastBit0 + inter.xyz) / 255.f;	
	// @}
	
/*
	// 压缩Alpha Shiness和gloss数据共占a通道8bit, gloss和spec_mat 5bit, lightmap 3bit
	// @{
	matData.a = (floor(fGlossSpec * 31.f + 0.5f) * 8.f + floor(fLightTex * 7.f + 0.5f)) / 255.f;
	
	// 将shiness放入diffuse_rgb通道低位, shiness最大128,分为8级 0, 18, 36, 54, 72, 90, 108, 126
	float fComapctedShiness = floor(saturate(g_fShiness / 126.f) * 7.f + 0.5f);
	
	// 应该存在更高效的
	float3 inter;
	float fracc;
	fracc = modf(fComapctedShiness / 2.f, inter.r);
	fracc = modf(fracc * 4.f / 2.f, inter.g);
	inter.b = fracc * 2.f;
	
	float3 vIntegerPart = 0.f;
	modf((vCompactedDiffuse.rgb * 255.f / 2.f), vIntegerPart.rgb);
	vIntegerPart *= 2.f;
	
	vIntegerPart = (vIntegerPart.rgb + inter.rgb) / 255.f;
	matData.rgb = vIntegerPart.rgb;
	// @}
*/
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
#define TECHNIQUE HEADER_KEY(MRTGeometry, CHANNELS_FLAG)
technique TECHNIQUE
<
	string Description = MAKE_STRING(TECHNIQUE);
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_SHADER ();
		PixelShader 	= compile ps_3_0 PS_SHADER ();
	}
}
//---------------------------------------------------------------------------------------