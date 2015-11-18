//*************************************************************************************************
// ����:	Tree Leaf Shading��ɫ��(�ѷ���)
//---------------------------------------------------------
// ����:		
// ����:		2012-11-14
// ����޸�:
//*************************************************************************************************
#include "TreeCommon.h"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0, sdLightSampler, 	sdLightBuf, 	false);	// ���ջ���
SD_POINT_CLAMP_SAMPLE(1, sdGeomSampler, 	sdGeomBuf, 		false);	// ���λ���

//---------------------------------------------------------------------------------------
// ������ɫ������������
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float4 	vPosId			: POSITION0;	///< center.x, center.y, center.z, id
	float4	vUVSetSize		: TEXCOORD0;	///< uv0.x, uv0.y, size, 1.f	
};
//---------------------------------------------------------------------------------------
// ������ɫ�����������
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;
	float4	vUVSetProjPos	: TEXCOORD0;
	float3 	vUVSetViewPos	: TEXCOORD1;
	float2 	vUVSet0			: TEXCOORD2;
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	
	// ����ռ�����
	// @{
	// �۲�ռ����ĵ�����
	float3 vCardViewPos = mul(float4(kInput.vPosId.xyz, 1.0f), g_mWorldView).xyz;
	
	// view space rotation
	float fRotAngle = dot(float4(g_fTime, kInput.vPosId.xyz), float4(g_fTreeSwaySpeed, 1.f, 1.f, 1.f));
	float3 vRotVec = float3(cos(fRotAngle), sin(fRotAngle), 0.f) * g_fTreeSwayRadius;	

	// view space offset
	float3 vCornerOffset = g_akCornerOffset[kInput.vPosId.w] * kInput.vUVSetSize.z * a_fTreeLeafScale;
	
	// ����۲�ռ�����	
	float3 vViewPos = vCardViewPos + vRotVec + vCornerOffset;
	kOutput.vUVSetViewPos = vViewPos;

	// ����ͶӰ�ռ�����
	kOutput.vProjPos = mul(float4(vViewPos, 1.f), g_mProj);
	kOutput.vUVSetProjPos = mul(float4(vViewPos, 1.f), g_mProj);	
	// @}
	
	// ������������
	kOutput.vUVSet0 = kInput.vUVSetSize.xy;

	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 PS_Main_LeafCard(VS_OUTPUT kInput) : COLOR0
{
	//clip(tex2D(sdNormalSampler, kInput.vUVSet0).z - 0.5f);
	clip(tex2D(sdNormalSampler, kInput.vUVSet0).a - 0.5f);

	// ��Ļ��������
	float2 vUVSet = GetTexCoordForPixel(kInput.vUVSetProjPos.xy / kInput.vUVSetProjPos.w);
	
	// ����
	float4 vGeomData = tex2D(sdGeomSampler, vUVSet);
	float3 vViewNormal = UnpackNormal(vGeomData.zw);
	
	// ���ع���
	float4 vLocalLight = tex2D(sdLightSampler, vUVSet);
	
	// ȫ�ֹ���
	float3 vDiffuseLight;
	AccumLightingOnlyDiffuse(vViewNormal, vLocalLight, vDiffuseLight);

	// ����
	float3 vDiffuseTex = tex2D(sdBaseSampler, vUVSet).rgb;
#ifdef _EDITOR
	vDiffuseTex.rgb = max(vDiffuseTex.rgb, g_vDiffuseMapMask);
#endif
	
	return float4(vDiffuseLight * g_vTreeLeafMaterial * vDiffuseTex, 1.f);
}
//---------------------------------------------------------------------------------------
float4 PS_Main_LeafCard_NoNormalMap(VS_OUTPUT kInput) : COLOR0
{
	float4 vDiffuseTex = tex2D(sdBaseSampler, kInput.vUVSet0);
	clip(vDiffuseTex.a - 0.5f);
#ifdef _EDITOR
	vDiffuseTex.rgb = max(vDiffuseTex.rgb, g_vDiffuseMapMask);
#endif

	// ��Ļ��������
	float2 vUVSet = GetTexCoordForPixel(kInput.vUVSetProjPos.xy / kInput.vUVSetProjPos.w);
	
	// ����
	float4 vGeomData = tex2D(sdGeomSampler, vUVSet);
	float3 vViewNormal = UnpackNormal(vGeomData.zw);
	
	// ���ع���
	float4 vLocalLight = tex2D(sdLightSampler, vUVSet);
	
	// ȫ�ֹ���
	float3 vDiffuseLight;
	AccumLightingOnlyDiffuse(vViewNormal, vLocalLight, vDiffuseLight);
	
	return float4(vDiffuseLight * g_vTreeLeafMaterial * vDiffuseTex.rgb, 1.f);
}
//---------------------------------------------------------------------------------------
float4 PS_Main_LeafCard_NoTexture(VS_OUTPUT kInput) : COLOR0
{
	// ��Ļ��������
	float2 vUVSet = GetTexCoordForPixel(kInput.vUVSetProjPos.xy / kInput.vUVSetProjPos.w);
	
	// ����
	float4 vGeomData = tex2D(sdGeomSampler, vUVSet);
	float3 vViewNormal = UnpackNormal(vGeomData.zw);
	
	// ���ع���
	float4 vLocalLight = tex2D(sdLightSampler, vUVSet);
	
	// ȫ�ֹ���
	float3 vDiffuseLight;
	AccumLightingOnlyDiffuse(vViewNormal, vLocalLight, vDiffuseLight);
	
	return float4(vDiffuseLight * g_vTreeLeafMaterial, 1.f);
}
//---------------------------------------------------------------------------------------
float4 PS_Main_LeafCard_Density(VS_OUTPUT kInput) : COLOR0
{
	float2 vDDXUV = ddx(kInput.vUVSet0);
	float2 vDDYUV = ddy(kInput.vUVSet0);
	float3 vCrossUV = cross(float3(vDDXUV, 0.f), float3(vDDYUV, 0.f));
	float fUVArea = length(vCrossUV);
	fUVArea *= (g_vDiffuseMapSize.x * g_vDiffuseMapSize.z);
	
	float3 vDDXPos = ddx(kInput.vUVSetViewPos);
	float3 vDDYPos = ddy(kInput.vUVSetViewPos);
	float3 vCrossPos = cross(vDDXPos, vDDYPos);
	float fFaceArea = length(vCrossPos);
	
	float fDensity = fUVArea / fFaceArea;
	//fDensity = 0.00001f * fDensity;
	
	float2 vThreshold = fDensity * g_vDensityThreshold.xy;
	//fDensity = smoothstep(0.f, 1.f, fDensity);
	//return lerp(float4(0.f, 0.f, 1.f, 1.f), float4(1.f, 0.f, 0.f, 1.f), fDensity);
	
	//if (fDensity < fRatio.x)
	//{
	//	float fRatio = saturate(fDensity * 2.f);
	//	return lerp(float4(1.f, 0.f, 0.f, 1.f), float4(0.f, 1.f, 0.f, 1.f), fRatio);
	//}
	//else if (fDensity > fRatio.y)
	//{
	//	float fRatio = saturate(fDensity * 2.f - 1.f);
	//	return lerp(float4(0.f, 1.f, 0.f, 1.f), float4(0.f, 0.f, 1.f, 1.f), fRatio);
	//}
	//else
	//{
	//	float fRatio = saturate(fDensity * 2.f - 1.f);
	//	return lerp(float4(0.f, 1.f, 0.f, 1.f), float4(0.f, 0.f, 1.f, 1.f), fRatio);
	//}
	//return float4(fDensity.xxx, 1.f);
	
	if (vThreshold.x < 1.f)
	{
		float fRatio = saturate((vThreshold.x - 0.5f) * 2.f);
		return lerp(float4(1.f, 0.f, 0.f, 1.f), float4(0.5f, 0.f, 0.f, 1.f), fRatio);
	}
	else if (vThreshold.y > 1.f)
	{
		float fRatio = saturate(vThreshold.y - 1.f);
		return lerp(float4(0.f, 0.f, 0.5f, 1.f), float4(0.f, 0.f, 1.f, 1.f), fRatio);
	}
	else
	{
		float fRatio = vThreshold.y;
		return lerp(float4(0.f, 0.5f, 0.f, 1.f), float4(0.f, 1.f, 01.f, 1.f), fRatio);
	}
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
technique Tree_LeafCard_Shading
<
	string	Description 		= "Tree_LeafCard_Shading";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_2_a	VS_Main();
		PixelShader		= compile	ps_2_a 	PS_Main_LeafCard();
	}
}
//---------------------------------------------------------------------------------------
technique Tree_LeafCard_NoNormalMap_Shading
<
	string	Description 		= "Tree_LeafCard_NoNormalMap_Shading";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_2_a	VS_Main();
		PixelShader		= compile	ps_2_a 	PS_Main_LeafCard_NoNormalMap();
	}
}
//---------------------------------------------------------------------------------------
technique Tree_LeafCard_NoTexture_Shading
<
	string	Description 		= "Tree_LeafCard_NoTexture_Shading";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_2_a	VS_Main();
		PixelShader		= compile	ps_2_a 	PS_Main_LeafCard_NoTexture();
	}
}
//---------------------------------------------------------------------------------------
technique Tree_LeafCard_Density_Shading
<
	string	Description 		= "Tree_LeafCard_Density_Shading";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_2_a	VS_Main();
		PixelShader		= compile	ps_2_a 	PS_Main_LeafCard_Density();
	}
}
//---------------------------------------------------------------------------------------