//*************************************************************************************************
// 内容:	StaticMesh的MRT3GeometryPass着色器具体实现(V3版)
//			使用压缩MRT渲染,使用三个RGBA8 RenderTarget
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-14
// 最后修改:	2013-05-28
//*************************************************************************************************
// 1.这里不需要定义避免重复的宏,因为本文件会被重复包含
// 2.这里根据Flag不同动态生成名字,避免出现命名重复
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
#undef GLOWMAP
#undef DETAILNORMALMAP
#undef ALPHATEST
#undef COMPACTVERTEX
#undef LEAFANIM

// 用于定义着色器输入输出结构
#undef VS_OUTPUT_VERTEX_NORMAL
#undef VS_OUTPUT_BASE_UV
#undef VS_OUTPUT_UNIQUE_UV

//---------------------------------------------------------------------------------------
// 根据CHANNELS_FLAG的值来提取当前启用的通道情况
//---------------------------------------------------------------------------------------
#if TEST_CHANNEL(CHANNELS_FLAG, VERTEX_DIFFUSEMAP_CHANNEL)
	#define DIFFUSEMAP 1
#else
	#define DIFFUSEMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, VERTEX_NORMALMAP_CHANNEL)
	#define NORMALMAP 1
#else
	#define NORMALMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, VERTEX_GLOSSMAP_CHANNEL)
	#define GLOSSMAP 1
#else
	#define GLOSSMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, VERTEX_LIGHTMAP_CHANNEL)
	#define LIGHTMAP 1
#else
	#define LIGHTMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, VERTEX_GLOWMAP_CHANNEL)
	#define GLOWMAP 1
#else
	#define GLOWMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, VERTEX_DETAIL_NORMAL_CHANNEL)
	#define DETAILNORMALMAP 1
#else
	#define DETAILNORMALMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, VERTEX_ALPHATEST_FLAG)
	#define ALPHATEST 1
#else
	#define ALPHATEST 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, VERTEX_COMPACTVERTEX_FLAG)
	#define COMPACTVERTEX 1
#else
	#define COMPACTVERTEX 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, VERTEX_LEAFANIM_FLAG)
	#define LEAFANIM 1
#else
	#define LEAFANIM 0
#endif

//---------------------------------------------------------------------------------------
// 根据当前启用的通道情况来决定具体的组合情况
//---------------------------------------------------------------------------------------
// 如果没有NormalMap则要输出顶点法线
#if (NORMALMAP)
	#define VS_OUTPUT_VERTEX_NORMAL	0
#else
	#define VS_OUTPUT_VERTEX_NORMAL 1
#endif

// DiffuseMap/NormalMap/GlossMap共用纹理坐标
#if (DIFFUSEMAP || NORMALMAP || GLOSSMAP || DETAILNORMALMAP || GLOWMAP)
	#define VS_OUTPUT_BASE_UV	1
#else
	#define VS_OUTPUT_BASE_UV 0
#endif

// LightMap使用单独的纹理坐标
#if (LIGHTMAP)
	#define VS_OUTPUT_UNIQUE_UV	1
#else
	#define VS_OUTPUT_UNIQUE_UV 0
#endif

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
#define VS_INPUT HEADER_KEY(VS_Input, CHANNELS_FLAG)
#if COMPACTVERTEX
	struct VS_INPUT
	{
		float3 	vPos			: POSITION;
		float4 	vNormal			: NORMAL;
		float4	vTangent		: TANGENT;
		
	#if VS_OUTPUT_UNIQUE_UV
		float2 	vUVSet1			: TEXCOORD1;
	#endif
	};
#else
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
		float2 	vUVSet0			: TEXCOORD0;
	#endif

	#if VS_OUTPUT_UNIQUE_UV
		float2 	vUVSet1			: TEXCOORD1;
	#endif
	};
#endif
//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
#define VS_OUTPUT HEADER_KEY(VS_Output, CHANNELS_FLAG)
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;
	float	fDepth			: TEXCOORD0;
	
#if VS_OUTPUT_BASE_UV
	float2 	vUVSet0			: TEXCOORD1;
#endif

#if VS_OUTPUT_UNIQUE_UV
	float2 	vUVSet1			: TEXCOORD2;
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
#if COMPACTVERTEX
	VS_OUTPUT VS_SHADER(VS_INPUT kInput)
	{
		VS_OUTPUT kOutput;
		
	#if LEAFANIM
		float3 vWorldPos = mul(float4(kInput.vPos.xyz, 1.f), g_mWorld).xyz;
		float3 vWorldNormal = normalize(mul(kInput.vNormal, g_mWorld).xyz);
		vWorldPos += CalcDetailWave(vWorldPos, vWorldNormal, 0.15f);
		kOutput.vProjPos = mul(float4(kInput.vPos, 1.0f), g_mWorldViewProj);	///< 投影坐标
	#else
		kOutput.vProjPos = mul(float4(kInput.vPos, 1.0f), g_mWorldViewProj);	///< 投影坐标
	#endif
	
		// 观察坐标深度
		kOutput.fDepth = dot(float4(kInput.vPos, 1.0f), g_mWorldView._m02_m12_m22_m32);
		
		// 基础纹理坐标
	#if VS_OUTPUT_BASE_UV
		kOutput.vUVSet0 =float2(kInput.vNormal.w, kInput.vTangent.w);
	#endif

		// 展开纹理坐标
	#if VS_OUTPUT_UNIQUE_UV
		kOutput.vUVSet1 = kInput.vUVSet1 * a_vLightMapTransform.zw + a_vLightMapTransform.xy;
	#endif

		// 顶点法线
	#if VS_OUTPUT_VERTEX_NORMAL
		kOutput.vNormal 		= kInput.vNormal.xyz;
	#else
		float fBinnormalSign = 0.f;
		if (kInput.vPos.w > 1.00098f)
			fBinnormalSign = 1.f;
		else
			fBinnormalSign = -1.f;
	
		float3 vBinnormal = cross(kInput.vNormal.xyz, kInput.vTangent.xyz) * fBinnormalSign;
		kOutput.vViewNormal 	= mul(float4(kInput.vNormal, 0.0f), g_mWorldView).xyz;
		kOutput.vViewBinormal 	= mul(float4(vBinnormal, 0.0f), g_mWorldView).xyz;
		kOutput.vViewTangent 	= mul(float4(kInput.vTangent, 0.0f), g_mWorldView).xyz;
	#endif

		return kOutput;
	}
#else
	VS_OUTPUT VS_SHADER(VS_INPUT kInput)
	{
		VS_OUTPUT kOutput;
		
	#if LEAFANIM
		float3 vWorldPos = mul(float4(kInput.vPos.xyz, 1.f), g_mWorld).xyz;
		float3 vWorldNormal = normalize(mul(kInput.vNormal, g_mWorld));
		vWorldPos += CalcDetailWave(vWorldPos, vWorldNormal, 0.15f);
		kOutput.vProjPos = mul(float4(kInput.vPos, 1.0f), g_mWorldViewProj);	///< 投影坐标
	#else
		kOutput.vProjPos = mul(float4(kInput.vPos, 1.0f), g_mWorldViewProj);	///< 投影坐标
	#endif
	
		// 观察坐标深度
		kOutput.fDepth = dot(float4(kInput.vPos, 1.0f), g_mWorldView._m02_m12_m22_m32);
		
		// 基础纹理坐标
	#if VS_OUTPUT_BASE_UV
		kOutput.vUVSet0 = kInput.vUVSet0;
	#endif

		// 展开纹理坐标
	#if VS_OUTPUT_UNIQUE_UV
		kOutput.vUVSet1 = kInput.vUVSet1 * a_vLightMapTransform.zw + a_vLightMapTransform.xy;
	#endif

		// 顶点法线
	#if VS_OUTPUT_VERTEX_NORMAL
		kOutput.vNormal 		= kInput.vNormal;
	#else
		kOutput.vViewNormal 	= mul(float4(kInput.vNormal, 0.0f), g_mWorldView).xyz;
		kOutput.vViewBinormal 	= mul(float4(kInput.vBinormal, 0.0f), g_mWorldView).xyz;
		kOutput.vViewTangent 	= mul(float4(kInput.vTangent, 0.0f), g_mWorldView).xyz;
	#endif

		return kOutput;
	}
#endif
//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
#define PS_SHADER HEADER_KEY(PS_Main, CHANNELS_FLAG)
void PS_SHADER(VS_OUTPUT kInput, 				\
			   out float4 geoData	: COLOR0,	\
			   out float4 matData0	: COLOR1,	\
			   out float4 matData1	: COLOR2,	\
			   out float4 matData2	: COLOR3
			   )
{
	// 计算Diffuse_Tex * Diffuse_Mat
	// 漫反射贴图的Alpha通道做AlphaTest
	// @{
#if DIFFUSEMAP
	float4 vDiffuseTex = tex2D(sdBaseSampler, kInput.vUVSet0);
	float3 vCompactedDiffuse = vDiffuseTex.rgb * g_vDiffuseMaterialColor;
	
	#ifdef ALPHATEST
		clip(vDiffuseTex.a - g_fAlphaTestRef);
	#endif
#else
	float3 vCompactedDiffuse = g_vDiffuseMaterialColor;

	// 基纹理还没有被加载则不显示
	clip(-1);
#endif	
	// @}
	
	
	// 计算Glow
	// @{
#if GLOWMAP
	float4 vGlowTex = tex2D(sdGlowSampler, kInput.vUVSet0);
	matData2.rgb 	= g_vEmissiveMaterial.rgb;
	matData2.a		= vGlowTex.r * a_fGlowPower;
#else
	matData2 = 0.f;
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
		float3 vDetailNormal = tex2D(sdDetailNormalSampler, kInput.vUVSet0 * a_vDetailNormalUVTiling).xyz;
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
	float3 vNormalTex 	= tex2D(sdNormalSampler, kInput.vUVSet0).xyz;
	vNormalTex 			= (vNormalTex - 0.5f) * 2.0f;
	vNormalTex.xy		*= g_fStaticMeshNormalScale;

	// 合成DetailNormal到Normal
	#if DETAILNORMALMAP
		float3 vDetailNormal = tex2D(sdDetailNormalSampler, kInput.vUVSet0 * a_vDetailNormalUVTiling).xyz;
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
	
	
	// 计算光照贴图
	//	1.LightMap信息放置在Alpha通道的高7bit
	//  2.RimLight信息放置在Alpha通道的最低1bit
	// @{
#if LIGHTMAP
	float fLightTex = dot(tex2D(sdDarkSampler, kInput.vUVSet1), a_vLightMapChannel);
#else
	float fLightTex = a_fMainLightOcclusion;	// 武器也受到LightDepth遮挡
#endif

	// 勾边特效,edge enhancement
	int iDarkness = fLightTex * 127.0f + 0.5f;	// 变换到7bit内,即[0,127],四舍五入
	int iDarknessShiftLeft = iDarkness * 2;		// 向左平移1bit
	int iShouldRimLight = 0;
	matData0.a		= (iDarknessShiftLeft + iShouldRimLight) / 255.0f;	// 合并后变换回浮点数
	matData0.rgb 	= vCompactedDiffuse;	
	// @}
	
	
	// 计算高光(不知道为什么不预乘以高光材质颜色,后面也没有用到高光材质颜色)
	// @{
#if GLOSSMAP
	float3 	vSpeculatTex = tex2D(sdGlossSampler, kInput.vUVSet0).rgb;
#else
	float3 	vSpeculatTex = 0.0f;	///< g_vSpecularMaterialColor
#endif

	matData1.rgb	= vSpeculatTex;
	matData1.a		= g_fShiness / 255.0f;
	// @}	
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
#define TECHNIQUE HEADER_KEY(MRT3Geometry, CHANNELS_FLAG)
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