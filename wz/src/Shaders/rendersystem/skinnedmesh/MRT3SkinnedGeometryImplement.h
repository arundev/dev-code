//*************************************************************************************************
// 内容:	StaticMesh的MRT3GeometryPass着色器具体实现(V3版)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-21
// 最后修改:
//*************************************************************************************************
#include "skinned_mesh_common.h"

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
#undef ALPHATEST
#undef VERTEXCOLOR

// 用于定义着色器输入输出结构
#undef VS_OUTPUT_VERTEX_NORMAL
#undef VS_OUTPUT_VERTEX_COLOR
#undef VS_OUTPUT_BASE_UV

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

#if TEST_CHANNEL(CHANNELS_FLAG, GLOWMAP_CHANNEL)
	#define GLOWMAP 1
#else
	#define GLOWMAP 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, ALPHA_TEST_FLAG)
	#define ALPHATEST 1
#else
	#define ALPHATEST 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, VERTEX_COLOR_CHANNEL)
	#define VERTEXCOLOR 1
#else
	#define VERTEXCOLOR 0
#endif

//---------------------------------------------------------------------------------------
// 根据当前启用的通道情况来决定具体的组合情况
//---------------------------------------------------------------------------------------
#if (!NORMALMAP)
	#define VS_OUTPUT_VERTEX_NORMAL	1
#else
	#define VS_OUTPUT_VERTEX_NORMAL 0
#endif

#if (VERTEXCOLOR)
	#define VS_OUTPUT_VERTEX_COLOR 1
#else
	#define VS_OUTPUT_VERTEX_COLOR 0
#endif

#if (DIFFUSEMAP || NORMALMAP || GLOSSMAP || GLOWMAP)
	#define VS_OUTPUT_BASE_UV 1
#else
	#define VS_OUTPUT_BASE_UV 0
#endif

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
#define VS_INPUT HEADER_KEY(VS_Input, CHANNELS_FLAG)
struct VS_INPUT
{
	float3 	vPos			: POSITION;
	float3	vBlendWeights	: BLENDWEIGHT0;
	int4	vBlendIndices	: BLENDINDICES0;
	
#if VS_OUTPUT_VERTEX_COLOR
	float4	vColor			: COLOR0;
#endif
	
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
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
#define VS_OUTPUT HEADER_KEY(VS_Output, CHANNELS_FLAG)
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;
	
#if VS_OUTPUT_VERTEX_COLOR
	float	fColor			: COLOR0;
#endif

	float	fDepth			: TEXCOORD0;
	
#if VS_OUTPUT_BASE_UV
	float2 	vUVSetBase		: TEXCOORD1;
#endif

#if VS_OUTPUT_VERTEX_NORMAL
	float3 	vViewNormal		: TEXCOORD2;
#else
	float3	vViewNormal		: TEXCOORD2;
	float3 	vViewBinormal	: TEXCOORD3;
	float3	vViewTangent	: TEXCOORD4;
#endif
};


//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
#define VS_SHADER HEADER_KEY(VS_Main, CHANNELS_FLAG)
VS_OUTPUT VS_SHADER(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	
	float4 vWorldPos;
	float4x4 mSkinnedBoneTransform;
	TransformSkinnedPosition(kInput.vPos, kInput.vBlendIndices, kInput.vBlendWeights,
		g_mSkinnedBoneMatrix3, vWorldPos, mSkinnedBoneTransform);
	kOutput.vProjPos = mul(vWorldPos, g_mViewProj);
	
	kOutput.fDepth = dot(vWorldPos, g_mView._m02_m12_m22_m32);
	
#if VS_OUTPUT_BASE_UV
	kOutput.vUVSetBase = kInput.vUVSetBase;
#endif

#if VS_OUTPUT_VERTEX_COLOR
	kOutput.fColor = kInput.vColor.r;;
#endif

	mSkinnedBoneTransform = mul(mSkinnedBoneTransform, g_mView);
#if VS_OUTPUT_VERTEX_NORMAL
	kOutput.vViewNormal = mul(float4(kInput.vNormal, 0.f), mSkinnedBoneTransform).xyz;
#else
	TransformNBT(kInput.vNormal, kInput.vBinormal, kInput.vTangent, mSkinnedBoneTransform,
		kOutput.vViewNormal, kOutput.vViewBinormal, kOutput.vViewTangent);
#endif

	return kOutput;
}

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
	

	// 计算Glow
	// @{
#if GLOWMAP
	float4 vGlowTex = tex2D(sdGlowSampler, kInput.vUVSetBase);
	matData2.rgb 	= g_vEmissiveMaterial.rgb;
	matData2.a		= vGlowTex.r;
#else
	matData2 = 0.f;
#endif
	// @}
	
	
	// 计算Normal与Depth
	// @{
	// 观察空间深度归一化到[0,1]
	float fDepth = kInput.fDepth / g_fCameraFarClip;
	
#if VS_OUTPUT_VERTEX_NORMAL
	float3 vViewNormal 	= kInput.vViewNormal;
#else
	// 采样NormalMap
	float3 vNormalTex 	= tex2D(sdNormalSampler, kInput.vUVSetBase).xyz;
	vNormalTex 			= (vNormalTex - 0.5f) * 2.0f;
	vNormalTex.xy		*= g_fStaticMeshNormalScale;

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
	float fLightTex = a_fMainLightOcclusion;
#else
	float fLightTex = a_fMainLightOcclusion;	// 武器也受到LightDepth遮挡
#endif

	// 勾边特效,edge enhancement
	// lightmap 7 bit, rim light 1 bit
	int iDarkness = fLightTex * 127.0f + 0.5f;	// 变换到7bit内,即[0,127],四舍五入
	int iDarknessShiftLeft = iDarkness * 2;		// 向左平移1bit
	
#if VS_OUTPUT_VERTEX_COLOR
	int iShouldRimLight = ceil(kInput.fColor);	
#else
	int iShouldRimLight = 1;	
#endif
	matData0.a		= (iDarknessShiftLeft + iShouldRimLight) / 255.0f;	// 合并后变换回浮点数
	matData0.rgb 	= vCompactedDiffuse;	
	// @}
	
	
	// 计算高光(不知道为什么不预乘以高光材质颜色,后面也没有用到高光材质颜色)
	// @{
#if GLOSSMAP
	float3 	vSpeculatTex = tex2D(sdGlossSampler, kInput.vUVSetBase).rgb;
#else
	float3 	vSpeculatTex = 0.0f;	// g_vSpecularMaterialColor
#endif

	matData1.rgb	= vSpeculatTex;
	matData1.a		= g_fShiness / 255.0f;
	// @}	
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
#define TECHNIQUE HEADER_KEY(MRT3SkinnedGeometry, CHANNELS_FLAG)
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