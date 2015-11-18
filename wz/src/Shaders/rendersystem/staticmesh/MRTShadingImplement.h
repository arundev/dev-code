//*************************************************************************************************
// ����:	StaticMesh��MRTShadingPass��ɫ������ʵ��(V2��,�ѷ���)
//---------------------------------------------------------
// ����:		
// ����:		2012-11-19
// ����޸�:	2013-05-28
//*************************************************************************************************
#include "static_mesh_common.h"

//---------------------------------------------------------------------------------------
// ȡ��һЩ�ڲ����õ���Ԥ�����
//---------------------------------------------------------------------------------------
#undef VS_INPUT
#undef VS_OUTPUT
#undef VS_SHADER
#undef PS_SHADER
#undef TECHNIQUE

#undef RIMLIGHT

//---------------------------------------------------------------------------------------
// ����CHANNELS_FLAG��ֵ����ȡ��ǰ���õ�ͨ�����
//---------------------------------------------------------------------------------------
#if TEST_CHANNEL(CHANNELS_FLAG, RIM_LIGHTTING_CHANNEL)
	#define RIMLIGHT 1
#else
	#define RIMLIGHT 0
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
float4 PS_SHADER(VS_OUTPUT kInput) : COLOR0
{
	// ����뷨������
	// @{
	float4 vGeomData 	= tex2D(sdGeomBufferSampler, kInput.vProjPos);
	float3 vViewNormal 	= UnpackNormal(vGeomData.zw);
	float  fDepth		= UnpackDepth(vGeomData.xy);
	// @}
	
	
	// ������ͼ����
	// @{
	// g_fLocalLightRadio��Ϊ���޸ĵ��Դ��,�������ĳ��λ�õ�ʱ��,���������һ���Ӱ������Ĺ���
	float4 vLocalLight	= tex2D(sdLightBufferSampler, kInput.vProjPos) * g_fLocalLightRadio;
	// }@
	
	
	// ��������
	// @{
	float4 vMatData	= tex2D(sdMatBufferSampler, kInput.vProjPos);
	
	// ���shiness
	float3 bits = frac(vMatData.rgb * 255.f / 2.f) * 2.f;
	
#if RIMLIGHT
	// ֻ�н�ɫӵ��rimlightЧ��,�ڽ�ɫ��shiness����rimlight
	float fRimlightAttenuration = dot(bits.rgb, float3(4.f, 2.f, 1.f)) / 7.f;
	float fShiness = 20.f;
#else
	float fShiness = dot(bits.rgb, float3(4.f, 2.f, 1.f)) / 7.f * 70.f;
#endif

	// ���gloss��lightmap
	float inter = vMatData.a * 255.f;
	float fGlossSpec = 0.f;
	float fLightMap = modf(inter / 8.f, fGlossSpec);
	fLightMap  = fLightMap * 8.f / 7.f;
	fGlossSpec /= 31.f;
	// @}
	
	
	// ���ռ���
	// @{
	float3	vDiffuseLight;
	float3	vSpecularLight;
	float3	vViewVector = -normalize(kInput.vViewPos);
	AccumLighting(vViewVector, vViewNormal, fShiness, fLightMap, vLocalLight, vDiffuseLight, vSpecularLight);
	// @}
	
	
	// RimLight����
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
	
	
	// �ϳɹ���
	float3 vColor = vMatData.rgb * vDiffuseLight + fGlossSpec * vSpecularLight + vRimLight;

	return float4(vColor, 0.f);
};

//---------------------------------------------------------------------------------------
// ��ɫ����
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