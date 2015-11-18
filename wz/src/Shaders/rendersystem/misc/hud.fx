//*************************************************************************************************
// ����:	3d hud ui, ʹ��vertex constants instancing
//---------------------------------------------------------
// ����:		
// ����:		2012-11-23
// ����޸�:	
//*************************************************************************************************
#include "../common.h"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
//float4x4	g_mWorld			: WORLD;
//float4x4	g_mInvView			: INVVIEW;
//float4x4	g_mWorldViewProj	: WORLDVIEWPROJECTION;
//float4	g_mEyePos			: EYEPOS;
//float2	g_vWindowSize		: GLOBAL = float2(1024.f, 768.f);

float		a_akHudParam[12]	: ATTRIBUTE;
#define		a_fHudWidth			a_akHudParam[0]
#define		a_fHudHeight		a_akHudParam[1]
#define		a_vHudSize			float2(a_akHudParam[0], a_akHudParam[1])
#define		a_vHudOffset		float3(a_akHudParam[2], a_akHudParam[3], a_akHudParam[4])
#define		a_fHudAlpha			a_akHudParam[5]
#define		a_fHudScale			a_akHudParam[6]	// �Ƿ���ʾ����Ļ�ռ�, 0:true 1:false
#define		a_fHudDist			a_akHudParam[7]
#define		a_vHudColor			float3(a_akHudParam[9], a_akHudParam[9], a_akHudParam[10])

float4		a_vPickColor		: ATTRIBUTE;

float2		g_vHudPickSampleUV	: GLOBAL = float2(0.f, 0.f);
float4		g_akHudConstant[160]: GLOBAL;
float2		g_vHudTexSize		: GLOBAL;
float2		g_vHudColRow		: GLOBAL;

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(1, sdPickMaskSampler, sdPickMaskTex, false); 	// ʰȡ
SD_POINT_CLAMP_SAMPLE(1, sdGeomSampler, sdGeomBuf, false); 			// ��Ļ��ȷ��߻���

texture sdHUDTex
<
	string NTM = "base";
	int NTMIndex = 0;
>;

sampler sdHUDSampler = sampler_state
{
	Texture = (sdHUDTex);
	AddressU = CLAMP;
	AddressV = CLAMP;	
	AddressW = CLAMP;	
	MipFilter = POINT;
	MinFilter = POINT;
	MagFilter = POINT;
	SRGB_DISABLE;
};

//---------------------------------------------------------------------------------------
// ������ɫ�����������
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos	: POSITION;	
	float2	vUVSet		: TEXCOORD0;
	float2	vUVSetAlpha	: TEXCOORD1;
};
//---------------------------------------------------------------------------------------
struct VS_OUTPUT_INSTANCE
{
	float4	vProjPos	: POSITION;	
	float3	vUVSet		: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main_Hud(float3 vPos : POSITION0, float2 vUVSet : TEXCOORD0)
{
	VS_OUTPUT kOutput = (VS_OUTPUT)0;

//	vPos.xy = trunc(vPos.xy);
	float4 vProjPos = mul(float4(vPos, 1.f), g_mWorldViewProj);
	float2 vPixelSpace = vProjPos.xy * g_vWindowSize;
	vPixelSpace = trunc(vPixelSpace);
	vProjPos.xy = vPixelSpace / g_vWindowSize;
	
	float fZ = vProjPos.w * (1.f - a_fHudScale) + a_fHudDist * a_fHudScale;
	float2 vCDCSpace = a_vHudSize / g_vWindowSize;	// ��Ϊ��Ļ�����Ҿ���Ϊ2,���Բ��ó�����	
	float2 vBias = float2(vUVSet.x, vUVSet.y + 1.f) - 2.f * (a_vHudOffset.xy / a_vHudSize);	// �ӵײ�����
	vProjPos.xy += vBias * vCDCSpace * fZ;	// ��Ϊ��ʽ��CDCҪ��Z��
	kOutput.vProjPos = vProjPos;
	
	vUVSet = vUVSet / 2.f + float2(0.5f, 0.5f);
	vUVSet.y = 1.f - vUVSet.y;
	kOutput.vUVSet = vUVSet;
	
	kOutput.vUVSetAlpha = float2(a_fHudAlpha, a_fHudAlpha);
	
	return kOutput;
}
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main_Hud_PointSample(float3 vPos : POSITION0, float2 vUVSet : TEXCOORD0)
{
	VS_OUTPUT kOutput = (VS_OUTPUT)0;
	kOutput.vProjPos = float4(vPos, 1.f);
	kOutput.vUVSet = vUVSet;
	kOutput.vUVSetAlpha = 0.f;
	return kOutput;
}
//---------------------------------------------------------------------------------------
VS_OUTPUT_INSTANCE VS_Main_Hud_Instance(float4 vPos : POSITION0, float2 vUVSet : TEXCOORD0)
{
	VS_OUTPUT_INSTANCE kOutput = (VS_OUTPUT_INSTANCE)0;

	int iGeoId = (int)(vPos.w + 0.5f) * g_vHudColRow.x + (int)(vPos.z + 0.5f);
	float2 vSize = g_akHudConstant[iGeoId * 2].xy / g_vWindowSize;
	float2 vSizeTex = g_akHudConstant[iGeoId * 2].xy / g_vHudTexSize;
	float3 vOffset = g_akHudConstant[iGeoId * 2 + 1].xyz;
	
	vPos.y = vPos.y * 0.5f + 0.5f;
	kOutput.vProjPos.x = vPos.x * vSize.x + vOffset.x;
	kOutput.vProjPos.y = vPos.y * vSize.y * 2.f + vOffset.y;
	kOutput.vProjPos.z = vOffset.z;
	kOutput.vProjPos.w = 1.f;	
	
	float2 vUVSet0 = g_akHudConstant[iGeoId * 2].zw;
	vPos.x = vPos.x * 0.5f + 0.5f;
	kOutput.vUVSet.x = vUVSet0.x + vPos.x * vSizeTex.x; 
	kOutput.vUVSet.y = vUVSet0.y + vSizeTex.y * (1.f - vPos.y);
	kOutput.vUVSet.z = g_akHudConstant[iGeoId * 2 + 1].w;	
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 PS_Main_Hud(VS_OUTPUT kInput) : COLOR0
{	
	float4 vColor = tex2D(sdHUDSampler, kInput.vUVSet);
	vColor.rgb *= a_vHudColor;
	vColor.a *= kInput.vUVSetAlpha.x;
	if (vColor.a == 0.f)
		clip(-1);
		
	return vColor;
}
//---------------------------------------------------------------------------------------
void PS_Main_Hud_MRT(VS_OUTPUT kInput,
		out float4 vDiffuseColor : COLOR0,
		out float4 vPickColor	 : COLOR1)
{	
	float4 vColor = tex2D(sdHUDSampler, kInput.vUVSet);
	vColor.rgb *= a_vHudColor;
	vColor.a *= kInput.vUVSetAlpha.x;
	if (vColor.a == 0.f)
	{
		clip(-1);
	}
	else
	{
		vDiffuseColor = vColor;
		vPickColor = a_vPickColor;
	}
}
//---------------------------------------------------------------------------------------
float4 PS_Main_Hud_PointSample() : COLOR0
{	
	return tex2D(sdPickMaskSampler, g_vHudPickSampleUV);
}
//---------------------------------------------------------------------------------------
float4 PS_Main_Hud_Instance(VS_OUTPUT_INSTANCE kInput) : COLOR0
{	
	float4 vColor = tex2D(sdHUDSampler, kInput.vUVSet.xy);
	vColor.w = kInput.vUVSet.z;
	
	float3 vColorKey = abs(vColor.xyz - float3(0.f, 0.f, 0.f));
	clip(sign(vColorKey.x + vColorKey.y + vColorKey.z) - 0.5f);
	
	return vColor;
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
technique HUD_Power
<
	string Description = "HUD_Power";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main_Hud ();
		PixelShader 	= compile ps_2_0 PS_Main_Hud ();
		
		//CullMode = None;
		//ZEnable = TRUE;
		//ZWriteEnable = TRUE;
		//AlphaTestEnable = FALSE;
		//AlphaBlendEnable = TRUE;
		//SrcBlend = SRCALPHA;
		//DestBlend = INVSRCALPHA;
		//StencilEnable = FALSE;
	}
}
//---------------------------------------------------------------------------------------
technique HUD_MRT
<
	string Description = "HUD_MRT";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main_Hud ();
		PixelShader 	= compile ps_3_0 PS_Main_Hud_MRT ();
		
		//CullMode = None;
		//ZEnable = TRUE;
		//ZWriteEnable = TRUE;
		//AlphaTestEnable = FALSE;
		//AlphaBlendEnable = TRUE;
		//SrcBlend = SRCALPHA;
		//DestBlend = INVSRCALPHA;
		//StencilEnable = FALSE;
	}
}
//---------------------------------------------------------------------------------------
technique HUD_PointSample
<
	string Description = "HUD_PointSample";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main_Hud_PointSample ();
		PixelShader 	= compile ps_2_0 PS_Main_Hud_PointSample ();
		
		//CullMode = None;
		//ZEnable = FALSE;
		//ZWriteEnable = FALSE;
		//AlphaTestEnable = FALSE;
		//AlphaBlendEnable = FALSE;
		//StencilEnable = FALSE;
	}
}
//---------------------------------------------------------------------------------------
technique HUD_Instance
<
	string Description = "HUD_Instance";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main_Hud_Instance ();
		PixelShader 	= compile ps_3_0 PS_Main_Hud_Instance ();
		
		//CullMode = None;
		//ZEnable = TRUE;
		//ZWriteEnable = TRUE;
		//AlphaTestEnable = FALSE;
		//AlphaBlendEnable = TRUE;
		//SrcBlend = SRCALPHA;
		//DestBlend = INVSRCALPHA;
		//StencilEnable = FALSE;
	}
}
//---------------------------------------------------------------------------------------