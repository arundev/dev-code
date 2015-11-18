//*************************************************************************************************
// 内容:	计算PostWeight
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-17
// 最后修改:
//*************************************************************************************************
#include "postprocess_common.h"
  
//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
float	g_fMLAANoBlurDepth			: GLOBAL = 5.0f;
float	g_fMLAAFullBlurDepth		: GLOBAL = 20.0f;
float	g_fMLAADofSkyDepth			: GLOBAL = 1.0f;
float 	g_fMLAAColorForegroundDepth	: GLOBAL = 5.0f;
float 	g_fMLAAColorBackgroundDepth	: GLOBAL = 20.0f;
float 	g_fMLAAColorSkyDepth		: GLOBAL = 0.0f;

#define PIXEL_SIZE 			(g_vHalfPixelOffset * 2.0)
  
//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0, sdGeomSampler,		sdGeomBuf, 		false);		// 屏幕深度与法线缓存
SD_POINT_CLAMP_SAMPLE(0, sdColorSampler,	sdColorBuf, 	false);		// 屏幕颜色缓存
SD_POINT_CLAMP_SAMPLE(1, sdWeightSampler,	sdWeightBuf, 	false);		// 屏幕颜色缓存
 
//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3 vPos 	: POSITION0;
	float2 vUVSet 	: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT_SIMPLE
{
	float4	vProjPos			: POSITION;		// 投影坐标
	float2	vUVSetScreenTex		: TEXCOORD0;	// 纠正半像素偏移的屏幕纹理坐标
};
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos			: POSITION;		// 投影坐标
	float2	vUVSetScreenTex		: TEXCOORD0;	// 纠正半像素偏移的屏幕纹理坐标
	float4	vUVSetOffset[2]		: TEXCOORD1;	// 当前坐标的四邻域
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
// 简单的"流过",只做半像素偏移处理
VS_OUTPUT_SIMPLE VS_Main_Simple(VS_INPUT kInput) 
{
	VS_OUTPUT_SIMPLE kOutput;
	kOutput.vProjPos = float4(kInput.vPos, 1.0);
	kOutput.vUVSetScreenTex = kInput.vUVSet + g_vHalfPixelOffset;
	
	return kOutput;
}
//---------------------------------------------------------------------------------------
// 计算4邻域的坐标
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
// 像素着色器
//---------------------------------------------------------------------------------------  
 //
//---------------------------------------------------------------------------------------
// 拷贝ColorMap并计算灰度值到Alpha通道
float4 PS_Main_CopyAndCalcGrayToAlpha(float2 texcoord : TEXCOORD0) : COLOR
{
	float3 vColor = tex2D(sdColorSampler, texcoord).rgb;
	float fGray = dot(vColor, g_vLuminScale3);
	
	return float4(vColor, fGray);
}
//---------------------------------------------------------------------------------------
// 原样拷贝
float4 PS_Main_Copy(float2 texcoord : TEXCOORD0) : COLOR
{
	return tex2D(sdColorSampler, texcoord);
}
//---------------------------------------------------------------------------------------
// 计算权值
float4 PS_Main_PostWeightCalculation(float2 texcoord : TEXCOORD0) : COLOR
{
	float4 vOutput;

	// 获取线性深度
	float4 vGeoTex = tex2D(sdGeomSampler, texcoord);
	float fDepth = UnpackDepth(vGeoTex.xy);
	
	//
	vOutput.w = (fDepth == 0.0 ? 0.0: 1.0);
	
	// 转换到真实深度
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
// 混合权值
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
// 着色技术, Time for some techniques!
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
// 拷贝
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
// 拷贝并计算灰度
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
// 计算权值
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
// 权值混合
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