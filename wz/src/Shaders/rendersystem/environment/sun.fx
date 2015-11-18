//*************************************************************************************************
// ����:	Sun
//---------------------------------------------------------
// ����:		
// ����:		2012-10-23
// ����޸�:
//*************************************************************************************************
#include "environment_common.h"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
float3	g_vGodRayDirection	: GLOBAL = { 0.f, 0.f, 1.f};	// ��Ҫnormalize��
float3	g_vGodRayColor		: GLOBAL = { 1.f, 1.f, 0.8f};
float2	g_vGodRaySize		: GLOBAL = {0.4f, 0.4f};
float	g_fGodRayIntensity	: GLOBAL = 1.f;

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_LINEAR_CLAMP_SAMPLE(0,	sdSunSampler,		sdSunTex,		true);
SD_POINT_CLAMP_SAMPLE(1, 	sdGeomSampler,		sdGeomTex,		false);

//---------------------------------------------------------------------------------------
// ������ɫ������������
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos			: POSITION0;	// ��Ļ���ζ���
	float2	vUVSet0			: TEXCOORD0;	// ��Ļ������������
};

//---------------------------------------------------------------------------------------
// ������ɫ�����������
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;		// ͶӰ����
	float4	vUVSet0			: TEXCOORD0;	// 
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// ����Sun��ͶӰ�ռ�λ��
	float4 vWorldPos = float4(g_vCameraPostion + g_vGodRayDirection, 1.f);
	float4 vProjPos = mul(vWorldPos, g_mViewProj);
	
	// 
	kOutput.vProjPos.xy = vProjPos.xy / vProjPos.w + kInput.vPos.xy / g_vGodRaySize;
	kOutput.vProjPos.zw = float2(0.9999f, 1.f);
	
	//
	kOutput.vUVSet0.xy = kInput.vUVSet0.xy;
	kOutput.vUVSet0.zw = kOutput.vProjPos.xy;
	
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
	// ��ȡ���
	float fDepth = tex2D(sdGeomSampler, GetTexCoordForPixel(kInput.vUVSet0.zw)).w;	
	clip(0.000001f - fDepth);
	
	//
	float3 vColor = tex2D(sdSunSampler, kInput.vUVSet0.xy).xyz;
	
	return float4(vColor * g_vGodRayColor, 1.f);	
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
technique Environment_Sun
<
	string Description = "Environment_Sun";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main ();
	}
}
//---------------------------------------------------------------------------------------