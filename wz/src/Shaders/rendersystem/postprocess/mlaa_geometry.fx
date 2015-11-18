//*************************************************************************************************
// ����:	����PostWeight
//---------------------------------------------------------
// ����:		
// ����:		2012-10-17
// ����޸�:
//*************************************************************************************************
#include "postprocess_common.h"
  
//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
float	g_fMLAANoBlurDepth			: GLOBAL = 5.0f;
float	g_fMLAAFullBlurDepth		: GLOBAL = 20.0f;
float	g_fMLAADofSkyDepth			: GLOBAL = 1.0f;
float 	g_fMLAAColorForegroundDepth	: GLOBAL = 5.0f;
float 	g_fMLAAColorBackgroundDepth	: GLOBAL = 20.0f;
float 	g_fMLAAColorSkyDepth		: GLOBAL = 0.0f;

#define PIXEL_SIZE 			(g_vHalfPixelOffset * 2.0)
  
//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0, sdGeomSampler,		sdGeomBuf, 		false);		// ��Ļ����뷨�߻���
SD_POINT_CLAMP_SAMPLE(0, sdColorSampler,	sdColorBuf, 	false);		// ��Ļ��ɫ����
SD_POINT_CLAMP_SAMPLE(1, sdWeightSampler,	sdWeightBuf, 	false);		// ��Ļ��ɫ����
 
//---------------------------------------------------------------------------------------
// ������ɫ������������
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3 vPos 	: POSITION0;
	float2 vUVSet 	: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// ������ɫ�����������
//---------------------------------------------------------------------------------------
struct VS_OUTPUT_SIMPLE
{
	float4	vProjPos			: POSITION;		// ͶӰ����
	float2	vUVSetScreenTex		: TEXCOORD0;	// ����������ƫ�Ƶ���Ļ��������
};
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos			: POSITION;		// ͶӰ����
	float2	vUVSetScreenTex		: TEXCOORD0;	// ����������ƫ�Ƶ���Ļ��������
	float4	vUVSetOffset[2]		: TEXCOORD1;	// ��ǰ�����������
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
// �򵥵�"����",ֻ��������ƫ�ƴ���
VS_OUTPUT_SIMPLE VS_Main_Simple(VS_INPUT kInput) 
{
	VS_OUTPUT_SIMPLE kOutput;
	kOutput.vProjPos = float4(kInput.vPos, 1.0);
	kOutput.vUVSetScreenTex = kInput.vUVSet + g_vHalfPixelOffset;
	
	return kOutput;
}
//---------------------------------------------------------------------------------------
// ����4���������
VS_OUTPUT VS_Main_Offset(VS_INPUT kInput) 
{
	VS_OUTPUT kOutput;
	kOutput.vProjPos = float4(kInput.vPos, 1.0);
	kOutput.vUVSetScreenTex = kInput.vUVSet + g_vHalfPixelOffset;
    kOutput.vUVSetOffset[0] = kOutput.vUVSetScreenTex.xyxy + PIXEL_SIZE.xyxy * float4(-1.0, 0.0, 0.0, -1.0);
    kOutput.vUVSetOffset[1] = kOutput.vUVSetScreenTex.xyxy + PIXEL_SIZE.xyxy * float4( 1.0, 0.0, 0.0,  1.0);
	
	return kOutput;
}
  
//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------  
 //
//---------------------------------------------------------------------------------------
// ����ColorMap������Ҷ�ֵ��Alphaͨ��
float4 PS_Main_CopyAndCalcGrayToAlpha(float2 texcoord : TEXCOORD0) : COLOR
{
	float3 vColor = tex2D(sdColorSampler, texcoord).rgb;
	float fGray = dot(vColor, g_vLuminScale3);
	
	return float4(vColor, fGray);
}
//---------------------------------------------------------------------------------------
// ԭ������
float4 PS_Main_Copy(float2 texcoord : TEXCOORD0) : COLOR
{
	return tex2D(sdColorSampler, texcoord);
}
//---------------------------------------------------------------------------------------
// ����Ȩֵ
float4 PS_Main_PostWeightCalculation(float2 texcoord : TEXCOORD0) : COLOR
{
	float4 vOutput;

	// ��ȡ�������
	float4 vGeoTex = tex2D(sdGeomSampler, texcoord);
	float fDepth = UnpackDepth(vGeoTex.xy);
	
	//
	vOutput.w = (fDepth == 0.0 ? 0.0: 1.0);
	
	// ת������ʵ���
	fDepth *= g_fCameraFarClip;
	
	float fDistance = max(fDepth, 0.0);
	
	// Dof
	float fDofStrength = saturate(max(0, fDistance - g_fMLAANoBlurDepth) / (g_fMLAAFullBlurDepth - g_fMLAANoBlurDepth));
	fDofStrength = lerp(g_fMLAADofSkyDepth, fDofStrength, vOutput.w);
	
	// ColorGrading
	float fColorGradingStrength = saturate(max(0, fDistance - g_fMLAAColorForegroundDepth) / (g_fMLAAColorBackgroundDepth - g_fMLAAColorForegroundDepth));
	fColorGradingStrength = lerp(g_fMLAAColorSkyDepth, fColorGradingStrength, vOutput.w);
	
	float fCGInt, fCGFrac;
	fCGFrac = modf(fColorGradingStrength * 256, fCGInt);
	
	//
	vOutput.x = fDofStrength;
	vOutput.y = fCGInt/ 255.0;
	vOutput.z = fCGFrac;
	
	return vOutput;
}
//---------------------------------------------------------------------------------------
// ���Ȩֵ
float4 PS_Main_PostWeightBlending(float2 texcoord : TEXCOORD0,
								  float4 offset[2]: TEXCOORD1) : COLOR0 
{
   // Fetch the blending weights for current pixel:
    float4 topLeft = tex2D(sdWeightSampler, texcoord);
    float bottom = tex2D(sdWeightSampler, offset[1].zw).g;
    float right = tex2D(sdWeightSampler, offset[1].xy).a;
    float4 a = float4(topLeft.r, bottom, topLeft.b, right);

    // Up to 4 lines can be crossing a pixel (one in each edge). So, we perform
    // a weighted average, where the weight of each line is 'a' cubed, which
    // favors blending and works well in practice.
    float4 w = a * a * a;

    // There is some blending weight with a value greater than 0.0?
    float sum = dot(w, 1.0);
    if (sum < 1e-5)
        discard;

    float4 color = 0.0;

	float4 C = tex2D(sdColorSampler, texcoord);
	float4 Cleft = tex2D(sdColorSampler, offset[0].xy);
	float4 Ctop = tex2D(sdColorSampler, offset[0].zw);
	float4 Cright = tex2D(sdColorSampler, offset[1].xy);
	float4 Cbottom = tex2D(sdColorSampler, offset[1].zw);
	color = mad(lerp(C, Ctop, a.r), w.r, color);
	color = mad(lerp(C, Cbottom, a.g), w.g, color);
	color = mad(lerp(C, Cleft, a.b), w.b, color);
	color = mad(lerp(C, Cright, a.a), w.a, color);

    // Normalize the resulting color and we are finished!
    return color / sum; 
}

//---------------------------------------------------------------------------------------
// ��ɫ����, Time for some techniques!
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
// ����
technique PostEffect_Copy
<
	string Description = "PostEffect_Copy";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
    pass 
	{
        VertexShader 	= compile vs_2_0 VS_Main_Simple();
        PixelShader 	= compile ps_2_0 PS_Main_Copy();
    }
}
//---------------------------------------------------------------------------------------
// ����������Ҷ�
technique PostEffect_CopyAndCalcGrayToAlpha
<
	string Description = "PostEffect_CopyAndCalcGrayToAlpha";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
    pass 
	{
        VertexShader 	= compile vs_2_0 VS_Main_Simple();
        PixelShader 	= compile ps_2_0 PS_Main_CopyAndCalcGrayToAlpha();
    }
}
//---------------------------------------------------------------------------------------
// ����Ȩֵ
technique PostEffect_PostWeightCalculation
<
	string Description = "PostEffect_PostWeightCalculation";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
    pass 
	{
        VertexShader 	= compile vs_3_0 VS_Main_Simple();
        PixelShader 	= compile ps_3_0 PS_Main_PostWeightCalculation();
    }
}
//---------------------------------------------------------------------------------------
// Ȩֵ���
technique PostEffect_PostWeightBlending 
<
	string Description = "PostEffect_PostWeightBlending";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
    pass NeighborhoodBlending 
	{
        VertexShader 	= compile vs_3_0 VS_Main_Offset();
        PixelShader 	= compile ps_3_0 PS_Main_PostWeightBlending();
       
		ZEnable = false;
        AlphaBlendEnable = false;
		
        StencilEnable = true;
        StencilPass = KEEP;
        StencilFunc = EQUAL;
        StencilRef = 1;
		StencilMask = 0xffffffff;
    }
}
//---------------------------------------------------------------------------------------