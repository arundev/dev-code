//*************************************************************************************************
// ����:	����ȫ����
//---------------------------------------------------------
// ����:		
// ����:		2012-10-09
// ����޸�:
//*************************************************************************************************
#include "postprocess_common.h"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
// 	-.x tile num
//	-.y speed
//	-.z mapsize
//	-.w intensity
float4 	g_vLightFuncParam	: GLOBAL;		

//
float3 	g_vOcclusionColor 	: GLOBAL;


//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0, 	sdGeomSampler,		sdGeomBuf,		false);		// ��Ļ����뷨�߻���
SD_LINEAR_CLAMP_SAMPLE(1,	sdAOSampler,		sdAOBuf,		false);
SD_LINEAR_WRAP_SAMPLE(2, 	sdCloudSampler,		sdCloudTex,		false);
SD_LINEAR_CLAMP_SAMPLE(3,	sdCurveFogSampler,	sdCurveFogTex,	false);

//---------------------------------------------------------------------------------------
// ������ɫ������������
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos			: POSITION0;	// ��Ļ���ζ�������
	float2	vUVSet0			: TEXCOORD0;	// ��Ļ���ζ�����������
};

//---------------------------------------------------------------------------------------
// ������ɫ�����������
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos			: POSITION;		// ͶӰ����
	float2	vUVSetScreenTex		: TEXCOORD0;	// ����������ƫ�Ƶ���Ļ��������
	float3	vUVFarClipWorldPos	: TEXCOORD1;	// ��ǰ���ӦԶ�ü����ϵĵ����������
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// ת����ͶӰ����
	kOutput.vProjPos = float4(kInput.vPos.xy, 0, 1.0);
	
	// ����������ƫ�Ƶ���Ļ��������
	kOutput.vUVSetScreenTex = kInput.vUVSet0 + g_vHalfPixelOffset;
	
	// ��ǰ���ӦԶ�ü����ϵĵ����������
	float4 vUVFarClipProjPos  = float4(kInput.vPos.xy, 1.0, 1.0);
	float4 vUVFarClipWorldPos = mul(vUVFarClipProjPos, g_mDepthToWorld);
	kOutput.vUVFarClipWorldPos = vUVFarClipWorldPos.xyz;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
// 
//---------------------------------------------------------------------------------------
// �߶������
float4 CalcHeightFogCurveFactor(float4 vWorldPos, bool bSky)
{
	float fSkyHeight = g_fFogCameraFar / 5000.0 * vWorldPos.z;
	float fDist = bSky ? sqrt(g_fFogCameraFar * g_fFogCameraFar - fSkyHeight * fSkyHeight) * g_fFogSkyDensity : vWorldPos.w;
	float fDistInFront = fDist - g_fFogStartDistance;
	if (fDistInFront < 0)
		return float4(0, 0, 0, 1.0);
		
	// 
	float2 vRelativeZ = float2(g_fFogStartHeight - g_vViewPos.z, g_fFogEndHeight - g_vViewPos.z);
	float2 vHeightPercent = saturate(LinePlaneIntersection(vWorldPos.z - g_vViewPos.z, vRelativeZ));
	
	// fog layer distance based on % over max/min fog heights
	float fLayerDistance = fDistInFront * abs(vHeightPercent.y - vHeightPercent.x);
	float fNegFogLineIntegral = g_fFogDistanceScale * fLayerDistance;
	
	// clamp anything beyond the extinction distance to 0 scattering
	// scattering falloff is exponential based on normalized layer distance
	float fScattering = 0.0;
	if (fLayerDistance < g_fFogExtinctionDistance)
		fScattering = 1.0 - GetBezierValue(sdCurveFogSampler, float2(g_fFogStartDistance, g_fFogEndDistance), fLayerDistance, 1.0);

	// radio of the inscattering color to be used
	float fInScattering = fScattering - 1.0;
	float3 vFogColor = fInScattering * g_vFogColor;
	
	return float4(vFogColor, fScattering);
}
//---------------------------------------------------------------------------------------
// 
float4 PS_Main_Fog(VS_OUTPUT kInput) : COLOR0
{
	// ��ȡ���
	float4 vGeoTex = tex2D(sdGeomSampler, kInput.vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);

	// �������
	bool bSky = false;
	if (fDepth <= 0.000001)
	{
		fDepth = 1.0;
		bSky = true;
	}
	
	// ������������
	//	1.�����������,�����λ�ú�Զƽ���Ӧ��λ�ý��в�ֵ
	//	2.�����ʹ����ʵ���������Z���
	float4 vWorldPos;
	vWorldPos.xyz = lerp(g_vViewPos, kInput.vUVFarClipWorldPos, fDepth);
	vWorldPos.w = distance(g_vViewPos, vWorldPos.xyz);
	
	//
	floatt4 vFogColorAndFactor = CalcHeightFogCurveFactor(vWorldPos, bSky);
	
	//
	float2 vLightFuncUV = (vWorldPos.xy / g_vLightFuncParam.zz) * g_vLightFuncParam.x * 0.5 +
						float2(-g_fTime * 0.1 * g_vLightFuncParam.y, 0.0);
	float fLightFuncValue = bSky ? 1.0 : (1 - tex2D(sdCloudSampler, vLightFuncUV).a * g_vLightFuncParam.w);
	float fFogFactor = vFogColorAndFactor.w;
	
	// AO is blend as FinalColor = OcclusionColor * (1.f - OcclusionFactor) + DestColor * OcclusionFactor;
	// Fog is blended as FinaColor = FogColorAndFactor.rgb + DestColor * vFogColorAndFactor.w
	// Combine these blend functions, treating ambient occlusion as being filtered through the fog
	// So using a blend mode One,SourceAlpha
	
	float fOcclusion = fLightFuncValue;
	
	// As OcclusionColor is black so optimize it
	// float3 vAOAndFogColor = vFogColorAndFactor.rgb + fFogFactor * OcclusionColor.rgb * (1.0 - fOcclusion);
	float3 vAOAndFogColor = vFogColorAndFactor.rgb;

	return float4(vAOAndFogColor, fFogFactor * fOcclusion);
	
	return vFogColorAndFactor;
}
//---------------------------------------------------------------------------------------
// 
float4 PS_Main_CurveFog(VS_OUTPUT kInput) : COLOR0
{
	// ��ȡ���
	float4 vGeoTex = tex2D(sdGeomSampler, kInput.vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);

	// �������
	bool bSky = false;
	if (fDepth <= 0.000001)
	{
		fDepth = 1.0;
		bSky = true;
	}
	
	// ������������
	//	1.�����������,�����λ�ú�Զƽ���Ӧ��λ�ý��в�ֵ
	//	2.�����ʹ����ʵ���������Z���
	float4 vWorldPos;
	vWorldPos.xyz = lerp(g_vViewPos, kInput.vUVFarClipWorldPos, fDepth);
	vWorldPos.w = distance(g_vViewPos, vWorldPos.xyz);
	
	// 
	float4 vFogColorAndFactor;
	float fSkyHeight = g_fFogCameraFar / 5000.0 * vWorldPos.z;
	float fDist = bSky ? sqrt(g_fFogCameraFar * g_fFogCameraFar - fSkyHeight * fSkyHeight) * g_fFogSkyDensity : vWorldPos.w;
	float fDistInFront = fDist - g_fFogStartDistance;
	if (fDistInFront < 0)
	{
		vFogColorAndFactor = float4(0, 0, 0, 1.0);
	}
	else
	{ 
		float2 vRelativeZ = float2(g_fFogStartHeight - g_vViewPos.z, g_fFogEndHeight - g_vViewPos.z);
		float2 vHeightPercent = saturate(LinePlaneIntersection(vWorldPos.z - g_vViewPos.z, vRelativeZ));
	
		// fNegFogLineIntegral = clamp(g_fFogDistanceScale * fLayerDistance, log2(g_fFogExp, 0.0))
		float fLayerDistance = fDistInFront * abs(vHeightPercent.y - vHeightPercent.x);
		float fNegFogLineIntegral = g_fFogDistanceScale * fLayerDistance;
		
		// fOldScattering = exp2(NegFogLineIntegral)
		// fScattering = 1.0 - GetBezierValue(sdCurveFogSampler, float2(0.0, g_fFogEndDistance), fLayerDistance, 1.0)
		float fScattering = 0.0;
		if (fLayerDistance < g_fFogExtinctionDistance)
			fScattering = 1.0 - GetBezierValue(sdCurveFogSampler, float2(g_fFogStartDistance, g_fFogEndDistance), fLayerDistance, 1.0);

		float fInScattering = fScattering - 1.0;
		vFogColorAndFactor = float4(fInScattering * g_vFogColor, fScattering);
	}
	
	// 
	float2 vLightFuncUV = (vWorldPos.xy / g_vLightFuncParam.zz) * g_vLightFuncParam.x * 0.5 +
						float2(-g_fTime * 0.1 * g_vLightFuncParam.y, 0.0);
	float fLightFuncValue = bSky ? 1.0 : (1 - tex2D(sdCloudSampler, vLightFuncUV).a * g_vLightFuncParam.w);
	float fFogFactor = vFogColorAndFactor.w;
	
	float fOcclusion = fLightFuncValue;
	// float3 vAOAndFogColor = vFogColorAndFactor.rgb + fFogFactor * OcclusionColor.rgb * (1.0 - fOcclusion);
	float3 vAOAndFogColor = vFogColorAndFactor.rgb;

	return float4(vAOAndFogColor, fFogFactor * fOcclusion);
	
	return vFogColorAndFactor;
}
//---------------------------------------------------------------------------------------
// 
float4 PS_Main_CurveFogAndAO(VS_OUTPUT kInput) : COLOR0
{
	// ��ȡ���
	float4 vGeoTex = tex2D(sdGeomSampler, kInput.vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);

	// �������
	bool bSky = false;
	if (fDepth <= 0.000001)
	{
		fDepth = 1.0;
		bSky = true;
	}
	
	// ������������
	//	1.�����������,�����λ�ú�Զƽ���Ӧ��λ�ý��в�ֵ
	//	2.�����ʹ����ʵ���������Z���
	float4 vWorldPos;
	vWorldPos.xyz = lerp(g_vViewPos, kInput.vUVFarClipWorldPos, fDepth);
	vWorldPos.w = distance(g_vViewPos, vWorldPos.xyz);
	
	//
	floatt4 vFogColorAndFactor = CalcHeightFogCurveFactor(vWorldPos, bSky);
	float fFogFactor = vFogColorAndFactor.w;
	
	//
	float2 vLightFuncUV = (vWorldPos.xy / g_vLightFuncParam.zz) * g_vLightFuncParam.x * 0.5 +
						float2(-g_fTime * 0.1 * g_vLightFuncParam.y, 0.0);
	float fLightFuncValue = bSky ? 1.0 : (1 - tex2D(sdCloudSampler, vLightFuncUV).a * g_vLightFuncParam.w);
	
	// As OcclusionColor is black so optimize it
	// float3 vAOAndFogColor = vFogColorAndFactor.rgb + fFogFactor * OcclusionColor.rgb * (1.0 - fOcclusion);
	float fOcclusion = fLightFuncValue;
	float3 vAOAndFogColor = vFogColorAndFactor.rgb;

	return float4(vAOAndFogColor, fFogFactor * fOcclusion);
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
// 
technique PostEffect_Fog
<
	string Description = "PostEffect_Fog";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main_Fog ();
	}
}
//---------------------------------------------------------------------------------------
//
technique PostEffect_CurveFog
<
	string Description = "PostEffect_CurveFog";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main_CurveFog ();
	}
}
//---------------------------------------------------------------------------------------
//
technique PostEffect_CurveFogAndAO
<
	string Description = "PostEffect_CurveFogAndAO";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main_CurveFogAndAO ();
	}
}
//---------------------------------------------------------------------------------------

