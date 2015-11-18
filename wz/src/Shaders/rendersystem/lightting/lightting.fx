//*************************************************************************************************
// ����:	�ֲ�������ɫ��
//---------------------------------------------------------
// ����:		
// ����:		2012-10-08
// ����޸�:	2014-04-20
//*************************************************************************************************
#include "../common.h"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
// ��Դ��ɫ(DiffuseColor)
float3		g_vLocalLightColor			: GLOBAL;

// ��Դ�߹�ǿ��
float		g_fLocalLightShiness		: GLOBAL = 70.f;

// ��Դ��Ӱ����(AmbientColor)
float4		g_vLocalLightShadow			: GLOBAL;
#define		g_vLocalLightShadowColor	g_vLocalLightShadow.xyz
#define		g_vLocalLightShadowTotal	g_vLocalLightShadow.w

// ��Դλ��(�۲�����ϵ)
float3		g_vLocalLightViewPos		: GLOBAL;

// ��Դ����������
float4		g_vLocalLightAttenuParams	: GLOBAL;

// �۲�ռ䵽��������ռ�任����
float4x4	g_mViewToLightClipSpace			: GLOBAL;

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
// ������Ļ��Ȼ���
SD_POINT_CLAMP_SAMPLE(0, sdGeomSampler, sdGeomBuf, false);		

//---------------------------------------------------------------------------------------
// ������ɫ������������
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos				: POSITION0;	///< ��������
};
//---------------------------------------------------------------------------------------
// ������ɫ�����������
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos			: POSITION;		///< ͶӰ����
	float4	vUVSetScreenTex		: TEXCOORD0;	///< ����������ƫ�Ƶ���Ļ��������(���Ƿ���PS����)
	float4	vUVFarClipViewPos	: TEXCOORD1;	///< ��ǰ���ӦԶ�ü����ϵĵ�Ĺ۲�����
};
//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// ģ������ת����ͶӰ����
	kOutput.vProjPos = mul(float4(kInput.vPos, 1.0f), g_mWorldViewProj);
	
	// ͶӰ�ռ��������
	float2 vTProjPos = kOutput.vProjPos.xy / kOutput.vProjPos.w;
	
	// ��ǰ���ӦԶ�ü����ϵĵ����������
	float4 vUVFarClipProjPos  = float4(vTProjPos.xy, 1.f, 1.f);
	kOutput.vUVFarClipViewPos = mul(vUVFarClipProjPos, g_mDepthToView) * kOutput.vProjPos.w;
	
	// ͶӰ����ת������Ļ��������(����������ƫ��)
	//float2 vUVSetScreenTex = vTProjPos.xy * float2(0.5f, -0.5f) + float2(0.5f, 0.5f) + g_vHalfPixelOffset;
	float2 vUVSetScreenTex = GetTexCoordForPixel(vTProjPos);
	kOutput.vUVSetScreenTex = float4(vUVSetScreenTex, 0.f, 1.f) * kOutput.vProjPos.w;
	
	return kOutput;
}
//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
// ���Դ
float4 PS_Main_PointLightting(VS_OUTPUT kInput) : COLOR0
{
	// DepthBuf
	// @{
	// ��Ļ��������
	float2 vUVSetScreenTex = kInput.vUVSetScreenTex.xy / kInput.vUVSetScreenTex.w;
	
	// ��ȡ�������
	float4 vGeoTex = tex2D(sdGeomSampler, vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);
	
	// ����۲�����ϵ����
	float3 vViewPos = kInput.vUVFarClipViewPos.xyz / kInput.vUVFarClipViewPos.w * fDepth;
	// @}

	
	// Lightting
	// @{
	// ���߷���
	float3 vLightVec = g_vLocalLightViewPos - vViewPos;
	float fLightDis = length(vLightVec);
	
	// ����˥��(�ȼ��㾶��˥����
	float fAttenu = g_vLocalLightAttenuParams.x * fLightDis + g_vLocalLightAttenuParams.y;
	//clip(fAttenu);
	fAttenu = saturate(fAttenu);
	
	// �������������
	float3 L = vLightVec / fLightDis;
	float3 N = UnpackNormal(vGeoTex.zw);
	float NL = saturate(dot(N, L));
	//clip(g_vLocalLightShadowTotal + NL);
	float3 vDiffuseLight = lerp(g_vLocalLightShadowColor, g_vLocalLightColor, NL) * fAttenu;
	
	// ����߹�
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
// �۹��
float4 PS_Main_SpotLightting(VS_OUTPUT kInput) : COLOR0
{
	// DepthBuf
	// @{
	// ��Ļ��������
	float2 vUVSetScreenTex = kInput.vUVSetScreenTex.xy / kInput.vUVSetScreenTex.w;
	
	// ��ȡ�������
	float4 vGeoTex = tex2D(sdGeomSampler, vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);
	
	// ����۲�����ϵ����
	float3 vViewPos = kInput.vUVFarClipViewPos.xyz / kInput.vUVFarClipViewPos.w * fDepth;
	// @}
	
	
	// Lightting
	// @{
	// �����Դ�ռ�����
	float4 vLightSpacePos = mul(float4(vViewPos, 1.0), g_mViewToLightClipSpace);
	//clip(vLightSpacePos.zw);
	vLightSpacePos.xy /= vLightSpacePos.w;
	vLightSpacePos.z = saturate(vLightSpacePos.z);
	
	// ���ղ���(�ȼ�������˥�����ټ��㾶��˥��)
	float fWeight = length(vLightSpacePos.xy);
	//clip(1.0 - fWeight);
	float fAttenu = saturate(g_vLocalLightAttenuParams.x * fWeight + g_vLocalLightAttenuParams.y);
	fAttenu	*= vLightSpacePos.z;
	
	// �������������
	float3 L = normalize(g_vLocalLightViewPos - vViewPos);
	float3 N = UnpackNormal(vGeoTex.zw);
	float NL = saturate(dot(N, L));
	//clip(g_vLocalLightShadowTotal + NL);
	float3 vDiffuseLight = lerp(g_vLocalLightShadowColor, g_vLocalLightColor, NL) * fAttenu;
	
	// ����߹�
	float3 V = -normalize(vViewPos);
	float3 R = reflect(-L, N);
	float VR = saturate(dot(V, R));
	float3 vSpecularLight = pow(VR, g_fLocalLightShiness) * vDiffuseLight;
	float fSpecularLumin = dot(vSpecularLight, g_vLuminScale);
	// @}
	
	return float4(vDiffuseLight, fSpecularLumin) * g_fLocalLightScale;
}
//---------------------------------------------------------------------------------------
// ���ξ۹��
float4 PS_Main_RectSpotLightting(VS_OUTPUT kInput) : COLOR0
{
	// DepthBuf
	// @{
	// ��Ļ��������
	float2 vUVSetScreenTex = kInput.vUVSetScreenTex.xy / kInput.vUVSetScreenTex.w;
	
	// ��ȡ�������
	float4 vGeoTex = tex2D(sdGeomSampler, vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);
	
	// ����۲�����ϵ����
	float3 vViewPos = kInput.vUVFarClipViewPos.xyz / kInput.vUVFarClipViewPos.w * fDepth;
	// @}
	
	
	// Lightting
	// @{
	// �����Դ�ռ�����
	float4 vLightSpacePos = mul(float4(vViewPos, 1.0), g_mViewToLightClipSpace);
	//clip(vLightSpacePos.zw);
	vLightSpacePos.xy /= vLightSpacePos.w;
	vLightSpacePos.z = saturate(vLightSpacePos.z);
	
	// ���ղ���(�ȼ�������˥�����ټ��㾶��˥��)
	float2 vXYOffset = abs(vLightSpacePos.xy);
	//clip(1.0 - vXYOffset)
	float fWeight = max(vXYOffset.x, vXYOffset.y);
	//clip(1.0 - fWeight);
	
	float fAttenu = saturate(g_vLocalLightAttenuParams.x * fWeight + g_vLocalLightAttenuParams.y);
	fAttenu	*= vLightSpacePos.z;
	
	// �������������
	float3 L = normalize(g_vLocalLightViewPos - vViewPos);
	float3 N = UnpackNormal(vGeoTex.zw);
	float NL = saturate(dot(N, L));
	//clip(g_vLocalLightShadowTotal + NL);
	float3 vDiffuseLight = lerp(g_vLocalLightShadowColor, g_vLocalLightColor, NL) * fAttenu;
	
	// ����߹�
	float3 V = -normalize(vViewPos);
	float3 R = reflect(-L, N);
	float VR = saturate(dot(V, R));
	float3 vSpecularLight = pow(VR, g_fLocalLightShiness) * vDiffuseLight;
	float fSpecularLumin = dot(vSpecularLight, g_vLuminScale);
	// @}
	
	return float4(vDiffuseLight, fSpecularLumin) * g_fLocalLightScale;
}
//---------------------------------------------------------------------------------------
// GB���Դ
float4 PS_Main_GBPointLightting(VS_OUTPUT kInput) : COLOR0
{
	// DepthBuf
	// @{
	// ��Ļ��������
	float2 vUVSetScreenTex = kInput.vUVSetScreenTex.xy / kInput.vUVSetScreenTex.w;
	
	// ��ȡ�������
	float4 vGeoTex = tex2D(sdGeomSampler, vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);
	
	// ����۲�����ϵ����
	float3 vViewPos = kInput.vUVFarClipViewPos.xyz / kInput.vUVFarClipViewPos.w * fDepth;
	// @}
	
	
	// Lightting
	// @{
	// ���߷���
	float3 vLightVec = g_vLocalLightViewPos - vViewPos;
	float fLightDis = length(vLightVec);
	
	// ���ղ���
	float fAttenu = 1.0 / dot(g_vLocalLightAttenuParams.xyz, float3(1.0, fLightDis, fLightDis * fLightDis));
	//clip(fAttenu);
	fAttenu = saturate(fAttenu);
	
	// �������������
	float3 L = vLightVec / fLightDis;
	float3 N = UnpackNormal(vGeoTex.zw);
	float NL = dot(N, L);
	//clip(NL);
	NL = saturate(NL);
	float3 vDiffuseLight = g_vLocalLightColor * NL * fAttenu;
	
	// ����߹�
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
// ��ɫ����
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