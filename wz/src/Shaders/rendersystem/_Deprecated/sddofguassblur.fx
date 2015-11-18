//*************************************************************************************************
// 内容:	Depth of Field Guass Blur(已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-20
// 最后修改:		2012-10-26
//*************************************************************************************************
#include "common_fun.inl"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
// 高斯1D过滤采样点偏移与权重
float2	g_kDOFSamplerOffsets[11]	: GLOBAL;
float	g_kDOFSampleWeights[11]		: GLOBAL;

// 高斯1D过滤采样点偏移缩放
float	g_fDOFBlurScale				: GLOBAL;

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_LINEAR_CLAMP_SAMPLE(0, sdColorSampler,	sdColorBuf,	false);	// 屏幕颜色缓存

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos			: POSITION0;	// 屏幕矩形顶点坐标
	float2	vUVSet0			: TEXCOORD0;	// 屏幕矩形顶点纹理坐标
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;		// 投影坐标
	float2	vUVSetBlur		: TEXCOORD0;	// 纠正半像素偏移的1/4屏幕纹理坐标
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// 转换到投影坐标
	kOutput.vProjPos = float4(kInput.vPos, 1.0);
	
	// Multiplay the g_vHalfPixelOffset with 4.
	// Since the quater size render target pixel's uv span is four times as greate as
	// the full size render target's pixel
	kOutput.vUVSetBlur = kInput.vUVSet0 + g_vHalfPixelOffset * 4.0;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	// 单轴高斯模糊
	float3 vSample = 0.f;
	float3 vColor = 0.f;
	float2 vSamplePosition;
	for (int iSample = 0; iSample < 11; iSample++)
	{
		vSamplePosition = kInput.vUVSetBlur + g_fDOFBlurScale * g_kDOFSamplerOffsets[iSample];
		vColor = tex2D(sdColorSampler, vSamplePosition.xy).xyz;
		vSample += g_kDOFSampleWeights[iSample] * vColor;
	}
	
	return float4(vSample, 1.f);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique PostEffect_tecDofGuassBlur
<
	string Description = "PostEffect_tecDofGuassBlur";
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


