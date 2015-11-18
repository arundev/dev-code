//*************************************************************************************************
// ����:	Dynamic Weather(��ʱľ��ʹ��)
//---------------------------------------------------------
// ����:		
// ����:		2012-10-31
// ����޸�:
//*************************************************************************************************
#include "environment_common.h"


//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
// ǰһ���任����
matrix	g_mWorldViewProjPrev	: GLOBAL;

// �߶�ƫ��,���ڵ���λ��
float4 	g_vPositionOffset	: GLOBAL;	

float4	g_vVelocity			: GLOBAL;
float4	g_vAlpha			: GLOBAL;
float4	g_vSizeScale		: GLOBAL;
float4	g_vLighting			: GLOBAL;
float4	g_vForward			: GLOBAL;

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_LINEAR_CLAMP_SAMPLE(0,	sdBlobSampler,	sdBlobTex,	true);

//---------------------------------------------------------------------------------------
// ������ɫ������������
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos			: POSITION0;
	float2	vUVSet0			: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// ������ɫ�����������
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos			: POSITION;		// ͶӰ����
	float2	vUVSet0				: TEXCOORD0;
	float3	vUVSetWorldPosition	: TEXCOORD1;
	float4	vUVSetColor			: TEXCOORD2;
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
#define PARTICLE_BOX 30.f

	VS_OUTPUT kOutput;

	//
	kOutput.vUVSet0 = kInput.vUVSet0;
	
	//
	float3 vPosition = kInput.vPos;
	vPosition.xyz -= g_vPositionOffset.xyz;
	vPosition.xyz = fmod(vPosition, PARTICLE_BOX);
	vPosition.xyz -= (PARTICLE_BOX / 2.f);
	vPosition.xyz += g_vForward.xyz;
	
	// 
	float fSubtractColor = length(vPosition.xyz) / PARTICLE_BOX;
	float fDistanceColorScale = 1.f - fSubtractColor * fSubtractColor;
	
	float4 vWorldPositionBottom = float4(vPosition.xyz + g_vCameraPostion.xyz, 1.f);
	float4 vWorldPositionTop = float4(vWorldPositionBottom.xyz + g_vVelocity.xyz * g_vSizeScale.y, 1.f);
	
	float4 vBottom = mul(vWorldPositionBottom, g_mViewProj);
	float4 vTop = mul(vWorldPositionTop, g_mViewProj);
	float4 vTopPrev = mul(vWorldPositionTop, g_mWorldViewProjPrev);

	float2 vDir = (vTop.xy / vTop.w) - (vBottom.xy / vBottom.w);
	float2 vDirPrev = (vTopPrev.xy / vTopPrev.w) - (vBottom.xy / vBottom.w);

	float fLen = length(vDir);
	float fLenPrev = length(vDirPrev);
	
	float fLenColorScale = saturate(fLen / fLenPrev);
	
	float fDirPerp = normalize(float2(-vDirPrev.y, vDirPrev.x));
	
	kOutput.vProjPos = lerp(vTopPrev, vBottom, kOutput.vUVSet0.y);
	kOutput.vProjPos.xy += (0.5f -kOutput.vUVSet0.x) * fDirPerp * g_vSizeScale.x;
	
	kOutput.vUVSetWorldPosition = vWorldPositionBottom.xyz;
	
	kOutput.vUVSetColor = g_vLighting * fDistanceColorScale * fLenColorScale * g_vAlpha.x;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
// ���²���
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	float4 vColor = tex2D(sdBlobSampler, kInput.vUVSet0) * kInput.vUVSetColor;
	clip(vColor.w - 0.01f);
	
	return vColor;
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
technique Environment_DynamicWeather
<
	string Description = "Environment_DynamicWeather";
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