//*************************************************************************************************
// ����:	Tree Leaf Geometry��ɫ������ʵ��
//---------------------------------------------------------
// ����:		
// ����:		2012-11-14
// ����޸�:
//*************************************************************************************************
#include "TreeCommon.h"

//---------------------------------------------------------------------------------------
// ȡ��һЩ�ڲ����õ���Ԥ�����
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
// ���ݵ�ǰ���õ�ͨ����������������������
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
// ���ݵ�ǰ���õ�ͨ����������������������
//---------------------------------------------------------------------------------------
// ���û��NormalMap��Ҫ������㷨��
#if (NORMALMAP)
	#define VS_OUTPUT_VERTEX_NORMAL	0
#else
	#define VS_OUTPUT_VERTEX_NORMAL 1
#endif

// DiffuseMap/NormalMap/GlossMap������������
#if (DIFFUSEMAP || NORMALMAP || NOISEFADING)
	#define VS_OUTPUT_BASE_UV	1
#else
	#define VS_OUTPUT_BASE_UV 	0
#endif

//---------------------------------------------------------------------------------------
// ������ɫ������������
//---------------------------------------------------------------------------------------
#define VS_INPUT HEADER_KEY(VS_Input, CHANNELS_FLAG)
struct VS_INPUT
{
	float4 	vPosId			: POSITION0;	///< center.x, center.y, center.z, id
	float4	vUVSetSize		: TEXCOORD0;	///< uv0.x, uv0.y, size, 1.f	
};

//---------------------------------------------------------------------------------------
// ������ɫ�����������
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
// ������ɫ��
//---------------------------------------------------------------------------------------
#define VS_SHADER HEADER_KEY(VS_Main, CHANNELS_FLAG)
VS_OUTPUT VS_SHADER(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	
	// ����ռ�����
	// @{
	// �۲�ռ����ĵ�����
	float3 vCardViewPos = mul(float4(kInput.vPosId.xyz, 1.0f), g_mWorldView).xyz;
	
#if LEAFANIM
	// view space rotation
	float fRotAngle = dot(float4(g_fTime, kInput.vPosId.xyz), float4(g_fTreeSwaySpeed, 1.f, 1.f, 1.f));
	float3 vRotVec = float3(cos(fRotAngle), sin(fRotAngle), 0.f) * g_fTreeSwayRadius;	
#endif

	// view space offset
	float3 vCornerOffset = g_akCornerOffset[kInput.vPosId.w] * kInput.vUVSetSize.z * a_fTreeLeafScale;
	
	// ����۲�ռ�����
#if LEAFANIM	
	float3 vViewPos = vCardViewPos + vRotVec + vCornerOffset;
#else
	float3 vViewPos = vCardViewPos + vCornerOffset;
#endif

	// ����ͶӰ�ռ�����
	kOutput.vProjPos = mul(float4(vViewPos, 1.f), g_mProj);
	kOutput.fUVSetDepth = vViewPos.z;
	// @}
	
	
	// ���㷨��
	// @{
	float3 vCenterToCorner = normalize(vViewPos - g_mWorldView._m30_m31_m32);
	float3 vCardNormal = lerp(-g_vMainLightDir, vCenterToCorner, g_fTreeLightPassing);
	
	float3 vCornelNormal = g_akCornerNormal[kInput.vPosId.w];
	vCornelNormal.xy *= g_fTreeLeafShading;
	
	float3 vNormal = lerp(vCornelNormal, vCardNormal, saturate(length(vViewPos) / g_fTreeWholeLightDistance));
	
	kOutput.vUVSetNormal = vNormal;
	// @}

	
	// ������������
#if VS_OUTPUT_BASE_UV
	kOutput.vUVSet0 = kInput.vUVSetSize.xy;
#endif

	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
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

	// ģ��AlphaTest/AlphaBlend����
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
// ��ɫ����
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