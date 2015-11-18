//*************************************************************************************************
// ����:	Color Grading
//
//	Color Grading
//	- Require a volume texture 16x16x16 unwrapped in 2d texture 256x16
//	- Use 256 * 16 LUT texture to remapping the LDR color.
//	- LDR color should range between 0-1
//	- Can be optimized by using a volume texture
//---------------------------------------------------------
// ����:		
// ����:		2012-10-13
// ����޸�:
//*************************************************************************************************
#include "postprocess_common.h"

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0, sdColorSampler,	sdColorBuf,	false);		// ��Ļ��ɫ����
SD_POINT_CLAMP_SAMPLE(1, sdWeightSampler,	sdWeightBuf,false);		// ��ĻȨ�ػ���
SD_LINEAR_CLAMP_SAMPLE(2, sdColorGradingForeGroundLUTSampler, sdColorGradingForeGroundLUTTex, false);	// ColorGradingǰ�����ұ������������
SD_LINEAR_CLAMP_SAMPLE(3, sdColorGradingBackgroundLUTSampler, sdColorGradingBackgroundLUTTex, false);	// ColorGrading�������ұ������������

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
	float2	vUVSetScreenTex		: TEXCOORD0;	// ����������ƫ�Ƶ���Ļ��������
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// ͶӰ����
	kOutput.vProjPos = float4(kInput.vPos, 1.0);
	
	// ����������ƫ�Ƶ���Ļ��������
	kOutput.vUVSetScreenTex = kInput.vUVSet0 + g_vHalfPixelOffset;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
// ColorGrading����
float3 ColorGrading(float3 vInLDRColor)
{
	// A 2d LUT texture 256x16
	// ������������ƫ������������
	float2 vOffset = float2(0.5 / 256.0, 0.5 / 16.0);
	float fScale = 15.0 / 16.0;
	
	// Also consider blur value in the blur buffer written by translucency
	float fIntB = floor(vInLDRColor.b / 14.9999) / 16.0;
	float fFracB = (vInLDRColor.b - fIntB) * 16.0;
	
	float fU = fIntB + vInLDRColor.r * fScale / 16.0;
	float fV = vInLDRColor.g * fScale;
	
	float3 vRG0 = tex2Dlod(sdColorGradingForeGroundLUTSampler, float4(vOffset.x + fU, vOffset.y + fV, 0, 0)).rgb;
	float3 vRG1 = tex2Dlod(sdColorGradingForeGroundLUTSampler, float4(vOffset.x + fU + 1.0 / 16.0, vOffset.y + fV, 0, 0)).rgb;
	
	return lerp(vRG0, vRG1, fFracB);
}
//---------------------------------------------------------------------------------------
// ������ȵ�ColorGrading����(�Ա�����ǰ��ʹ�ò�ͬ��LUT)
float3 ColorGradingDepth(float3 vInLDRColor, float fDepth)
{
	// A 2d LUT texture 256x16
	float2 vOffset = float2(0.5 / 256.0, 0.5 / 16.0);
	float fScale = 15.0 / 16.0;
	
	// Also consider blur value in the blur buffer written by translucency
	float fIntB = floor(vInLDRColor.b / 14.9999) / 16.0;
	float fFracB = (vInLDRColor.b - fIntB) * 16.0;
	
	float fU = fIntB + vInLDRColor.r * fScale / 16.0;
	float fV = vInLDRColor.g * fScale;
	
	float3 vRG0 = tex2Dlod(sdColorGradingForeGroundLUTSampler, float4(vOffset.x + fU, vOffset.y + fV, 0, 0)).rgb;
	float3 vRG1 = tex2Dlod(sdColorGradingForeGroundLUTSampler, float4(vOffset.x + fU + 1.0 / 16.0, vOffset.y + fV, 0, 0)).rgb;	
	float3 vRG2 = tex2Dlod(sdColorGradingBackgroundLUTSampler, float4(vOffset.x + fU, vOffset.y + fV, 0, 0)).rgb;
	float3 vRG3 = tex2Dlod(sdColorGradingBackgroundLUTSampler, float4(vOffset.x + fU + 1.0 / 16.0, vOffset.y + fV, 0, 0)).rgb;
	
	return lerp(lerp(vRG0, vRG1, fFracB), lerp(vRG2, vRG3, fFracB), saturate(fDepth));
}
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	// ������������
	float4 vColor = tex2D(sdColorSampler, kInput.vUVSetScreenTex);
	
	// �ָ�ӳ��ϵ��
	float4 vPostWeightMap = tex2D(sdWeightSampler, kInput.vUVSetScreenTex);
	float fT = dot(vPostWeightMap.yz, float2(255.0, 1.0)) * (1.0 / 256.0);
	
	// ����ӳ��
	//vColor.rgb = ColorGrading(saturate(vColor.rgb));
	vColor.rgb = ColorGradingDepth(saturate(vColor.rgb), fT);
	
	return vColor;
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
technique PostEffect_ColorGrading
<
	string Description = "PostEffect_ColorGrading";
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