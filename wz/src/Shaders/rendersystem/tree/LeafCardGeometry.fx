//*************************************************************************************************
// ����:	Tree Leaf Geometry��ɫ��(�ѷ���)
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
	float	fUVSetDepth		: TEXCOORD0;
	float3 	vUVSetNormal	: TEXCOORD1;
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
	kOutput.vUVSet0 = kInput.vUVSetSize.xy;

	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 PS_Main_DiffuseMapAlphaTest(VS_OUTPUT kInput) : COLOR0
{
	float4 vDiffuseTex = tex2D(sdBaseSampler, kInput.vUVSet0);
	clip(vDiffuseTex.a - 0.5f);
	
	float fNoise = dot(tex2Dlod(sdNoiseSampler, float4(kInput.vUVSet0, 0, 0)).rgb, 0.3333f);
	clip(g_fAlpha - fNoise);
	
	float fDepth = kInput.fUVSetDepth / g_fCameraFarClip;
	float3 vNormal = normalize(kInput.vUVSetNormal);
	return float4(PackDepth(fDepth), PackNormal(vNormal));
}
//---------------------------------------------------------------------------------------
float4 PS_Main_NormalMapAlphaTest(VS_OUTPUT kInput) : COLOR0
{
	float4 vDiffuseTex = tex2D(sdNormalSampler, kInput.vUVSet0);
	clip(vDiffuseTex.a - 0.5f);
	
	float fNoise = dot(tex2Dlod(sdNoiseSampler, float4(kInput.vUVSet0, 0, 0)).rgb, 0.3333f);
	clip(g_fAlpha - fNoise);
	
	float fDepth = kInput.fUVSetDepth / g_fCameraFarClip;
	float3 vNormal = normalize(kInput.vUVSetNormal);
	return float4(PackDepth(fDepth), PackNormal(vNormal));
}
//---------------------------------------------------------------------------------------
float4 PS_Main_Solid(VS_OUTPUT kInput) : COLOR0
{
	float fNoise = dot(tex2Dlod(sdNoiseSampler, float4(kInput.vUVSet0, 0, 0)).rgb, 0.3333f);
	clip(g_fAlpha - fNoise);
	
	float fDepth = kInput.fUVSetDepth / g_fCameraFarClip;
	float3 vNormal = normalize(kInput.vUVSetNormal);
	return float4(PackDepth(fDepth), PackNormal(vNormal));
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Tree_LeafCard_DiffuseMapAlphaTest_Geometry
<
	string	Description 		= "Tree_LeafCard_DiffuseMapAlphaTest_Geometry";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_3_0	VS_Main();
		PixelShader		= compile	ps_3_0 	PS_Main_DiffuseMapAlphaTest();
	}
}
//---------------------------------------------------------------------------------------
technique Tree_LeafCard_NormalMapAlphaTest_Geometry
<
	string	Description 		= "Tree_LeafCard_NormalMapAlphaTest_Geometry";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_3_0	VS_Main();
		PixelShader		= compile	ps_3_0 	PS_Main_NormalMapAlphaTest();
	}
}
//---------------------------------------------------------------------------------------
technique Tree_LeafCard_Solid_Geometry
<
	string	Description 		= "Tree_LeafCard_Solid_Geometry";
	bool	UsesNiRenderState 	= false;
	bool	UsesNiLightState	= false;
>
{
	pass P0
	{
		VertexShader	= compile 	vs_3_0	VS_Main();
		PixelShader		= compile	ps_3_0 	PS_Main_Solid();
	}
}
//---------------------------------------------------------------------------------------