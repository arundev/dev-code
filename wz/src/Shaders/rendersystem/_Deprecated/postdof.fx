//*************************************************************************************************
// 内容:	Depth of Field(已废弃,无用)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-09
// 最后修改:		2012-10-26
//*************************************************************************************************
#include "common_fun.inl"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
// Coc计算参数
float3 	g_vDofStartEnd	: GLOBAL;
#define	g_fDofStart		g_vDofStartEnd.x
#define	g_fDofEnd		g_vDofStartEnd.y
#define	g_fDofStartZ	g_vDofStartEnd.z

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0, 	sdGeomSampler,			sdGeomBuf,			false);	// 屏幕深度与法线缓存
SD_LINEAR_CLAMP_SAMPLE(1,	sdQuarterColorSampler,	sdQuarterColorBuf,	false);	// 1/4屏幕颜色缓存

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
	float2	vUVSetScreenTex	: TEXCOORD0;	// 纠正半像素偏移的屏幕纹理坐标
	float2	vUVSetBlur		: TEXCOORD1;	// 纠正半像素偏移的1/4屏幕纹理坐标
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// 转换到投影坐标
	kOutput.vProjPos = float4(kInput.vPos.xy, g_fDofStartZ, 1.0);
	
	// 屏幕纹理坐标(修正半像素偏移)
	kOutput.vUVSetScreenTex = kInput.vUVSet0 + g_vHalfPixelOffset;
	
	// Multiply the g_vHalfPixelOffset with 4.
	// Since the quater size rendertarget pixel's uv span is four times as 
	// greate as the full size rendertarget's pixel
	kOutput.vUVSetBlur = kInput.vUVSet0 + g_vHalfPixelOffset * 4.0;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float CalcDofCoc(float fDepth)
{
	return saturate(fDepth * g_fDofStart + g_fDofEnd);
}
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	// 获取深度
	float4 vGeoTex = tex2D(sdGeomSampler, kInput.vUVSetScreenTex);
	float fDepth = UnpackDepth(vGeoTex.xy);
	
	// 计算模糊
	float fCoc = CalcDofCoc(fDepth);
	
	// 采样模糊纹理
	float3 vBlurColor = tex2D(sdQuarterColorSampler, kInput.vUVSetBlur).rgb;
	
	return float4(vBlurColor, fCoc);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique PostEffect_PostDof
<
	string Description = "PostEffect_PostDof";
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