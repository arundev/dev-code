//*************************************************************************************************
// 内容:	Depth of Field(已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-15
// 最后修改:		2012-10-26
//*************************************************************************************************
#include "common_fun.inl"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
// Coc计算参数
float	g_fFocalDepth		: GLOBAL = 1000.0;	// 焦点深度
float	g_fNoBlurDepth		: GLOBAL = 5.0;		// 清晰范围
float	g_fFullBlurDepth	: GLOBAL = 20.0;	// 弥散距离
float	g_fDofIntensity		: GLOBAL = 1.0;		// 弥散程度

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_LINEAR_CLAMP_SAMPLE(0, sdDepthSampler,			sdDepthBuf,			false);	// 屏幕深度与法线缓存
SD_LINEAR_CLAMP_SAMPLE(1, sdColorSampler,			sdColorBuf,			false);	// 屏幕颜色缓存
SD_LINEAR_CLAMP_SAMPLE(2, sdQuarterColorSampler,	sdQuarterColorBuf,	false);	// 1/4屏幕颜色缓存

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
	float4		vProjPos			: POSITION;		// 投影坐标
	float2		vUVSetScreenTex		: TEXCOORD0;	// 纠正半像素偏移的屏幕纹理坐标
	//float2	vUVSetBlur			: TEXCOORD1;	// 纠正半像素偏移的1/4屏幕纹理坐标
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// 模型坐标转换到投影坐标
	kOutput.vProjPos = float4(kInput.vPos, 1.0);
	
	// 屏幕纹理坐标(修正半像素偏移)
	kOutput.vUVSetScreenTex = kInput.vUVSet0 + g_vHalfPixelOffset;
	
	// 1/4屏幕纹理坐标(修正半像素偏移)
	//kOutput.vUVSetBlur = kInput.vUVSet0 + g_vHalfPixelOffset * 4.0;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	// 采样线性深度,转换为真实深度
	float4 vGeoTex = tex2D(sdDepthSampler, kInput.vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);
	float fRealDepth = fDepth * g_fCameraFarClip;
	
	// 计算Coc
	float fDistance	= max(fRealDepth - g_fFocalDepth, 0.0);
	float fDofFactor = saturate(max(0, fDistance - g_fNoBlurDepth) / (g_fFullBlurDepth - g_fNoBlurDepth));
	
	// 采样原始纹理
	float3 vBaseColor = tex2D(sdColorSampler, kInput.vUVSetScreenTex).xyz;
	
	// 采样模糊纹理
	// @{
	float2 vUVSetLT = kInput.vUVSetScreenTex + float2(-1.f, -1.f) * g_vHalfPixelOffset * 4.f;
	float2 vUVSetRT = kInput.vUVSetScreenTex + float2( 1.f, -1.f) * g_vHalfPixelOffset * 4.f;
	float2 vUVSetLB = kInput.vUVSetScreenTex + float2(-1.f,  1.f) * g_vHalfPixelOffset * 4.f;
	float2 vUVSetRB = kInput.vUVSetScreenTex + float2( 1.f,  1.f) * g_vHalfPixelOffset * 4.f;
	
	float3 vBlurColor = 0;
	vBlurColor += 0.25f * tex2D(sdQuarterColorSampler, vUVSetLT).xyz;
	vBlurColor += 0.25f * tex2D(sdQuarterColorSampler, vUVSetRT).xyz;
	vBlurColor += 0.25f * tex2D(sdQuarterColorSampler, vUVSetLB).xyz;
	vBlurColor += 0.25f * tex2D(sdQuarterColorSampler, vUVSetRB).xyz;
	// @}
	
	// 颜色合成
	return float4(lerp(vBaseColor, vBlurColor, fDofFactor), 1.f);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique PostEffect_tecDof
<
	string Description = "PostEffect_tecDof";
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