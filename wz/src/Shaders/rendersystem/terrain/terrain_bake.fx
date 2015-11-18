//*************************************************************************************************
// ����:	���κ決LightMap
//---------------------------------------------------------
// ����:		
// ����:		2012-08-24
// ����޸�:	2013-09-10
//*************************************************************************************************
#include "terrain_common.h"

//---------------------------------------------------------------------------------------
// ȫ�ֱ���
//---------------------------------------------------------------------------------------
float4x4 g_mWorldToShadowSpace		: GLOBAL;

float2	g_vRecipLightMapSize		: GLOBAL = { 1.f / 256.f, 1.f / 256.f};
float2	g_vRecipShadowMapSize		: GLOBAL = { 1.f / 256.f, 1.f / 256.f};

float	g_fShadowCameraFarPlane		: GLOBAL;

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
// ShadowMap
texture sdShadowBuf
<
	string NTM = "shader";
	int NTMIndex = 3;
>;

sampler sdShadowSampler = sampler_state
{
	Texture = (sdShadowBuf);
	AddressU = CLAMP;
	AddressV = CLAMP;	
	AddressW = CLAMP;	
	MipFilter = NONE;
	MinFilter = POINT;
	MagFilter = POINT;
	SRGBTexture = false;
};

//---------------------------------------------------------------------------------------
// ������ɫ�����������
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;
	float3	vWorldPos		: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(/* float2 */ uint4 vPos : POSITION0)
{
	VS_OUTPUT kOutput;

	uint2 iXY = vPos.xy;
	float fZ = (vPos.z * 256 + vPos.w) / 65535.f * (g_fTerrainMaxHeight - g_fTerrainMinHeight) + g_fTerrainMinHeight;
	float4 vWorldPosition = float4(iXY * a_fQuadScale + a_vQuadOrigin, fZ * 0.05f, 1.f);
	
	float2 vUVSet = vWorldPosition.xy * g_vRecipTerrainSize;
	vUVSet = vUVSet * float2(2.f, -2.f) + float2(-1.f, 1.f);
	vUVSet = vUVSet + g_vRecipLightMapSize * float2(-1.f, 1.f);
	
	kOutput.vProjPos = float4(vUVSet, 0.f, 1.f);
	kOutput.vWorldPos = vWorldPosition.xyz;
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
float PCF(sampler sdShadowSampler, float fShadowPixelDepth, float fRecipShadowMapSize, float2 vUVSetShadow, float fShadowBias)
{
	// x x x
	// x . x
	// x x x

	fShadowPixelDepth -=  fShadowBias;	// ����z-fighting
	float3 vDepth = 0.f;
	float3 vOcclusion = 0.f;
	float fOcclusionAddition = 0.f;
	float2 vUVSet = 0.f;
	float2 vEncodedDepth = 0.f;
	
	// row0 - 00 01 02
	vUVSet.y = vUVSetShadow.y - fRecipShadowMapSize;
	
	vUVSet.x = vUVSetShadow.x - fRecipShadowMapSize;
	vEncodedDepth = tex2D(sdShadowSampler, vUVSet).xy;
	vDepth.x = UnpackDepth(vEncodedDepth) * g_fShadowCameraFarPlane;
	vDepth.x = lerp(9999.f, vDepth.x, sign(vDepth.x));
	
	vUVSet.x = vUVSetShadow.x;
	vEncodedDepth = tex2D(sdShadowSampler, vUVSet).xy;
	vDepth.y = UnpackDepth(vEncodedDepth) * g_fShadowCameraFarPlane;
	vDepth.y = lerp(9999.f, vDepth.y, sign(vDepth.y));	
	
	vUVSet.x = vUVSetShadow.x + fRecipShadowMapSize;
	vEncodedDepth = tex2D(sdShadowSampler, vUVSet).xy;
	vDepth.z = UnpackDepth(vEncodedDepth) * g_fShadowCameraFarPlane;
	vDepth.z = lerp(9999.f, vDepth.z, sign(vDepth.z));	
	
	vOcclusion = fShadowPixelDepth <= vDepth;
	fOcclusionAddition += dot(vOcclusion, float3(1.f, 1.f, 1.f));
	
	// row1 - 10 11 12
	vUVSet.y = vUVSetShadow.y;
	vUVSet.x = vUVSetShadow.x - fRecipShadowMapSize;

	vEncodedDepth = tex2D(sdShadowSampler, vUVSet).xy;
	vDepth.x = UnpackDepth(vEncodedDepth) * g_fShadowCameraFarPlane;
	vDepth.x = lerp(9999.f, vDepth.x, sign(vDepth.x));
	
	vUVSet.x = vUVSetShadow.x;
	vEncodedDepth = tex2D(sdShadowSampler, vUVSet).xy;
	vDepth.y = UnpackDepth(vEncodedDepth) * g_fShadowCameraFarPlane;
	vDepth.y = lerp(9999.f, vDepth.y, sign(vDepth.y));	
	
	vUVSet.x = vUVSetShadow.x + fRecipShadowMapSize;
	vEncodedDepth = tex2D(sdShadowSampler, vUVSet).xy;
	vDepth.z = UnpackDepth(vEncodedDepth) * g_fShadowCameraFarPlane;
	vDepth.z = lerp(9999.f, vDepth.z, sign(vDepth.z));	
	
	vOcclusion = fShadowPixelDepth <= vDepth;
	fOcclusionAddition += dot(vOcclusion, float3(1.f, 1.f, 1.f));
	
	// row2 - 20 21 22
	vUVSet.y = vUVSetShadow.y + fRecipShadowMapSize;
	vUVSet.x = vUVSetShadow.x - fRecipShadowMapSize;

	vEncodedDepth = tex2D(sdShadowSampler, vUVSet).xy;
	vDepth.x = UnpackDepth(vEncodedDepth) * g_fShadowCameraFarPlane;
	vDepth.x = lerp(9999.f, vDepth.x, sign(vDepth.x));
	
	vUVSet.x = vUVSetShadow.x;
	vEncodedDepth = tex2D(sdShadowSampler, vUVSet).xy;
	vDepth.y = UnpackDepth(vEncodedDepth) * g_fShadowCameraFarPlane;
	vDepth.y = lerp(9999.f, vDepth.y, sign(vDepth.y));	
	
	vUVSet.x = vUVSetShadow.x + fRecipShadowMapSize;
	vEncodedDepth = tex2D(sdShadowSampler, vUVSet).xy;
	vDepth.z = UnpackDepth(vEncodedDepth) * g_fShadowCameraFarPlane;
	vDepth.z = lerp(9999.f, vDepth.z, sign(vDepth.z));	
	
	vOcclusion = fShadowPixelDepth <= vDepth;
	fOcclusionAddition += dot(vOcclusion, float3(1.f, 1.f, 1.f));
	
	return fOcclusionAddition / 9.f;
}
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	// ���㵱ǰ����ͶӰ����ϵλ��
	float4 vShadowSpacePos = mul(float4(kInput.vWorldPos, 1.f), g_mWorldToShadowSpace);
	
	// ���㵱ǰ����PCF��Ӱ
	float 	fPixelDepth = vShadowSpacePos.w;
	float2	vUVSetShadow = vShadowSpacePos.xy;
	float fShadow = PCF(sdShadowSampler, fPixelDepth, g_vRecipShadowMapSize, vUVSetShadow, 0.5f);
	
	return fShadow;
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
technique Terrain_Bake
<
	string Description = "Terrain_Bake";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main ();
	}
}
//---------------------------------------------------------------------------------------