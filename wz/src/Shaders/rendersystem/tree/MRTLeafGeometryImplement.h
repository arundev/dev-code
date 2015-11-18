//*************************************************************************************************
// 内容:	Tree Leaf Geometry着色器具体实现
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-14
// 最后修改:
//*************************************************************************************************
#include "TreeCommon.h"

//---------------------------------------------------------------------------------------
// 取消一些内部会用到的预定义宏
//---------------------------------------------------------------------------------------
#undef VS_INPUT
#undef VS_OUTPUT

#undef VS_SHADER
#undef PS_SHADER

#undef TECHNIQUE

#undef DIFFUSEMAP
#undef NORMALMAP
#undef NOISEFADING
#undef LEAFANIM

#undef VS_OUTPUT_VERTEX_NORMAL
#undef VS_OUTPUT_BASE_UV

//---------------------------------------------------------------------------------------
// 根据当前启用的通道情况来决定具体的组合情况
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

#if TEST_CHANNEL(CHANNELS_FLAG, NOISEFADING_FLAG)
	#define NOISEFADING 1
#else
	#define NOISEFADING 0
#endif

#if TEST_CHANNEL(CHANNELS_FLAG, LEAFANIM_FLAG)
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
#if (DIFFUSEMAP || NORMALMAP || NOISEFADING)
	#define VS_OUTPUT_BASE_UV	1
#else
	#define VS_OUTPUT_BASE_UV 	0
#endif

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
#define VS_INPUT HEADER_KEY(VS_Input, CHANNELS_FLAG)
struct VS_INPUT
{
	float4 	vPosId			: POSITION0;	///< center.x, center.y, center.z, id
	float4	vUVSetSize		: TEXCOORD0;	///< uv0.x, uv0.y, size, 1.f	
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
#define VS_OUTPUT HEADER_KEY(VS_Output, CHANNELS_FLAG)
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;
	float	fUVSetDepth		: TEXCOORD0;
	float3 	vUVSetNormal	: TEXCOORD1;
	
#if VS_OUTPUT_BASE_UV
	float2 	vUVSet0			: TEXCOORD2;
#endif
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
#define VS_SHADER HEADER_KEY(VS_Main, CHANNELS_FLAG)
VS_OUTPUT VS_SHADER(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	
	// 计算空间坐标
	// @{
	// 观察空间中心点坐标
	float3 vCardViewPos = mul(float4(kInput.vPosId.xyz, 1.0f), g_mWorldView).xyz;
	
#if LEAFANIM
	// view space rotation
	float fRotAngle = dot(float4(g_fTime, kInput.vPosId.xyz), float4(g_fTreeSwaySpeed, 1.f, 1.f, 1.f));
	float3 vRotVec = float3(cos(fRotAngle), sin(fRotAngle), 0.f) * g_fTreeSwayRadius;	
#endif

	// view space offset
	float3 vCornerOffset = g_akCornerOffset[kInput.vPosId.w] * kInput.vUVSetSize.z * a_fTreeLeafScale;
	
	// 计算观察空间坐标
#if LEAFANIM	
	float3 vViewPos = vCardViewPos + vRotVec + vCornerOffset;
#else
	float3 vViewPos = vCardViewPos + vCornerOffset;
#endif

	// 计算投影空间坐标
	kOutput.vProjPos = mul(float4(vViewPos, 1.f), g_mProj);
	kOutput.fUVSetDepth = vViewPos.z;
	// @}
	
	
	// 计算法线
	// @{
	float3 vCenterToCorner = normalize(vViewPos - g_mWorldView._m30_m31_m32);
	float3 vCardNormal = lerp(-g_vMainLightDir, vCenterToCorner, g_fTreeLightPassing);
	
	float3 vCornelNormal = g_akCornerNormal[kInput.vPosId.w];
	vCornelNormal.xy *= g_fTreeLeafShading;
	
	float3 vNormal = lerp(vCornelNormal, vCardNormal, saturate(length(vViewPos) / g_fTreeWholeLightDistance));
	
	kOutput.vUVSetNormal = vNormal;
	// @}

	
	// 基础纹理坐标
#if VS_OUTPUT_BASE_UV
	kOutput.vUVSet0 = kInput.vUVSetSize.xy;
#endif

	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
#define PS_SHADER HEADER_KEY(PS_Main, CHANNELS_FLAG)
void PS_SHADER(VS_OUTPUT kInput, 				\
			   out float4 geoData	: COLOR0,	\
			   out float4 matData	: COLOR1)
{
#if (DIFFUSEMAP && VS_OUTPUT_BASE_UV)
	float4 vTexDiffuse = tex2D(sdBaseSampler, kInput.vUVSet0);
	clip(vTexDiffuse.a - 0.5f);
	matData = vTexDiffuse;
	matData.a = 254.f / 255.f;
#else
	clip(-1);	///< if base texture have not loaded, don't display
#endif

	// 模拟AlphaTest/AlphaBlend淡出
#if NOISEFADING
	float3 vNoiseData = tex2Dlod(sdNoiseSampler, float4(kInput.vUVSet0, 0, 0)).rgb;
	float fNoise = dot(vNoiseData, 0.3333f);
	clip(g_fAlpha - fNoise);
#endif

	float fDepth = kInput.fUVSetDepth / g_fCameraFarClip;
	float3 vNormal = normalize(kInput.vUVSetNormal);
	geoData = float4(PackDepth(fDepth), PackNormal(vNormal));
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
#define TECHNIQUE HEADER_KEY(Tree_MRTLeafGeometry, CHANNELS_FLAG)
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