//*************************************************************************************************
// ����:	StaticMesh��MRT3ShadingPass��ɫ������ʵ��(V3��)
//---------------------------------------------------------
// ����:		
// ����:		2012-07-19
// ����޸�:	2013-05-27
//*************************************************************************************************
// 1.���ﲻ��Ҫ��������ظ��ĺ�,��Ϊ���ļ��ᱻ�ظ�����
// 2.�������Flag��ͬ��̬��������,������������ظ�
#include "static_mesh_common.h"

//---------------------------------------------------------------------------------------
// ȡ��һЩ�ڲ����õ���Ԥ�����
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
// ����CHANNELS_FLAG��ֵ����ȡ��ǰ���õ�ͨ�����
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
// ������ɫ�����������
//---------------------------------------------------------------------------------------
#define VS_OUTPUT HEADER_KEY(VS_Output, CHANNELS_FLAG)
struct VS_OUTPUT
{
	float4	vPos			: POSITION;
	float2 	vProjPos		: TEXCOORD0;
	float3	vViewPos		: TEXCOORD1;
};

//---------------------------------------------------------------------------------------
// ������ɫ��
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
// ������ɫ��
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
	// ����뷨������
	// @{
	float4 vGeomData 	= tex2D(sdGeomBufferSampler, kInput.vProjPos);
	float3 vViewNormal 	= UnpackNormal(vGeomData.zw);
	//float  fDepth		= UnpackDepth(vGeomData.xy);
	// @}
	
	
	// ������ͼ����
	// @{
	// g_fLocalLightRadio��Ϊ���޸ĵ��Դ��,�������ĳ��λ�õ�ʱ��,���������һ���Ӱ������Ĺ���
	float4 vLocalLight	= tex2D(sdLightBufferSampler, kInput.vProjPos) * g_fLocalLightRadio;
	// }@
	
	
	// ��������
	// @{
	float4 vMatData0	= tex2D(sdMatBuffer0Sampler, kInput.vProjPos);
	
	// ��ѹDiffuse
	float3 vDiffuseTex	= vMatData0.rgb;
	
	// decode lightmap and edge sign
	// ��ѹLightMap��Edge���
	float fLMAndEdge 	= vMatData0.a * 255.0f;
	float fLightMap		= 0.0f;
	float fRimLightMask	= modf(fLMAndEdge / 2.0f, fLightMap);
	fRimLightMask		= fRimLightMask * 2.0f;
	fLightMap			/= 127.0f;
	
#if SPECLIGHT
	float4 vMatData1	= tex2D(sdMatBuffer1Sampler, kInput.vProjPos);
	
	// ��ѹSpecular/Shiness
	float3 vSpecularTex	= vMatData1.rgb;
	float fShiness		= vMatData1.a * 255.0f;
#endif

#if GLOWLIGHT
	float4 vMatData2	= tex2D(sdMatBuffer2Sampler, kInput.vProjPos);
	
	// ��ѹGlow
	float3 vGlowTex 	= vMatData2.rgb;
#endif
	// @}
	
	
	// ���ռ���
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
	
	
	// RimLight����
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
	
	
	// �ϳɹ���
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
// ��ɫ����
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