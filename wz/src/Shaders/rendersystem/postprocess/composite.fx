//*************************************************************************************************
// ����:	Dof/GodRay/Bloom/HDR�ϳ�Pass
//		
//		�����е�����UE3��UmbraShader,�ο�����UDK����ɫ��
//		UDK-2010-07/Engine/Shaders/UberPostProcessBlendPixelShader.usf
//---------------------------------------------------------
// ����:		
// ����:		2012-10-22
// ����޸�:
//*************************************************************************************************
#include "postprocess_common.h"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
// Ч���������
bool	g_bDof				: GLOBAL = false;
bool	g_bGodRay			: GLOBAL = false;
bool	g_bBloom			: GLOBAL = false;
bool	g_bUseToneMapping	: GLOBAL = false;

// GodRay
float3	g_vGodRayDirection	: GLOBAL = { 0.f, 0.f, 1.f};

// Bloom
float 	g_fBloomLightIntensity	: GLOBAL = 1.f;

// 
float	g_fSceneFading			: GLOBAL = 1.f;

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0,	sdColorSampler,		sdColorTex,		false);	// ��Ļ��ɫ����
SD_POINT_CLAMP_SAMPLE(1,	sdGeomSampler,		sdGeomTex,		false);	// ��ĻDof-GodRayȨ������
SD_LINEAR_CLAMP_SAMPLE(2,	sdDofSampler,		sdDofTex,		false);	
SD_LINEAR_CLAMP_SAMPLE(3,	sdGodRaySampler,	sdGodRayTex,	false);	
SD_LINEAR_CLAMP_SAMPLE(4,	sdBloomSampler,		sdbloomTex,		false);	

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
	float4	vProjPos			: POSITION;		// ͶӰ����
	float4	vUVSetScreenTex		: TEXCOORD0;	// FullSize��QuadSize����������ƫ�Ƶ���Ļ��������
	float	fFactor				: TEXCOORD1;	// GodRay����
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// ͶӰ����
	kOutput.vProjPos = float4(kInput.vPos, 1.0);
	
	// FullSize��QuadSize����������ƫ�Ƶ���Ļ��������
	kOutput.vUVSetScreenTex.xy = kInput.vUVSet0 + g_vHalfPixelOffset;
	kOutput.vUVSetScreenTex.zw = kInput.vUVSet0 + g_vHalfPixelOffset * 4.f;
	
	// ����GodRay��Factor
	float fDot = max(0.f, dot(g_vGodRayDirection, g_mInvView[2]));
	float fF = 0.5f;
	kOutput.fFactor = max(0, fDot - fF) / (1.f - fF); 
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	// ������������
	float4 vColor = tex2D(sdColorSampler, kInput.vUVSetScreenTex.xy);
	
	// ����Dof-GodRayȨ������
	float4 vPostWeight = tex2D(sdGeomSampler, kInput.vUVSetScreenTex.xy);
	
	// �ϳ�Dof
	if (g_bDof)
	{
		float fCoc = vPostWeight.x;
		float3 vSmallBlurColor = tex2D(sdDofSampler, kInput.vUVSetScreenTex.xy + g_vHalfPixelOffset * 3.f).xyz;
		
		vColor.rgb = lerp(vColor.rgb, vSmallBlurColor, fCoc);
	}	
	
	// �ϳ�GodRay
	if (g_bGodRay)
	{
		float3 vGodRayColor = tex2D(sdGodRaySampler, kInput.vUVSetScreenTex.zw);
		float fGodRayIntensity = kInput.fFactor;
		
		vColor.rgb = vColor.rgb * (1.f - fGodRayIntensity * 0.15f) + 
					 vGodRayColor * fGodRayIntensity * 1.2f;
	}
	
	// �ϳ�Bloom
	if (g_bBloom)
	{
		float3 vBloomColor = tex2D(sdBloomSampler, kInput.vUVSetScreenTex.zw).xyz * g_fBloomLightIntensity;
		vColor.rgb = vColor.rgb + vBloomColor.rgb;
	}
	
	// �ϳ�ToneMap
	if (g_bUseToneMapping)
	{
		vColor.rgb = vColor.rgb / (vColor.rgb + 0.187f) * 1.035f;
	}
	else
	{
		vColor.rgb = saturate(vColor.rgb);
	}
	
	return vColor * g_fSceneFading;
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
technique PostEffect_Composite
<
	string Description = "PostEffect_Composite";
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