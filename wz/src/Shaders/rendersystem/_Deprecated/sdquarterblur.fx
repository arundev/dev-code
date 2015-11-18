//*************************************************************************************************
// 内容:	Quarter Blur(已废弃,无用)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-22
// 最后修改:		2012-10-26
//*************************************************************************************************
#include "common_fun.inl"

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_LINEAR_CLAMP_SAMPLE(0,	sdColorSampler,	sdColorTex,	false);	// 屏幕颜色缓存

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
	// 计算4领域纹理坐标
	float2 vUVSetLT = kInput.vUVSetBlur + float2(-2.f, -2.f) * g_vHalfPixelOffset;
	float2 vUVSetRT = kInput.vUVSetBlur + float2( 2.f, -2.f) * g_vHalfPixelOffset;
	float2 vUVSetLB = kInput.vUVSetBlur + float2(-2.f,  2.f) * g_vHalfPixelOffset;
	float2 vUVSetRB = kInput.vUVSetBlur + float2( 2.f,  2.f) * g_vHalfPixelOffset;
	
	// 采样4领域纹理并混合
	float3 vBlurColor = 0;
	vBlurColor += 0.25f * tex2D(sdColorSampler, vUVSetLT).xyz;
	vBlurColor += 0.25f * tex2D(sdColorSampler, vUVSetRT).xyz;
	vBlurColor += 0.25f * tex2D(sdColorSampler, vUVSetLB).xyz;
	vBlurColor += 0.25f * tex2D(sdColorSampler, vUVSetRB).xyz;
	
	return float4(vBlurColor, 1.f);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique PostEffect_tecQuarterBlur
<
	string Description = "PostEffect_tecQuarterBlur";
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