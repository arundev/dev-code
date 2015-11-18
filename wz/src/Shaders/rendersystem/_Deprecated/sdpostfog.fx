//*************************************************************************************************
// 内容:	Post Fog(已废弃,无用)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-22
// 最后修改:		2012-10-26
//*************************************************************************************************
#include "common_fun.inl"

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0, sdDepthSampler,	sdDepthBuf,	false);	// 屏幕深度与法线缓存
SD_POINT_CLAMP_SAMPLE(1, sdColorSampler,	sdColorBuf,	false);	// 屏幕颜色缓存

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
	float2	vUVSetProjPos	: TEXCOORD1;
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// 转换到投影坐标
	kOutput.vProjPos = float4(kInput.vPos, 1.0);
	
	// 屏幕纹理坐标(修正半像素偏移)
	kOutput.vUVSetScreenTex = kInput.vUVSet0 + g_vHalfPixelOffset;
	
	//
	kOutput.vUVSetProjPos = kInput.vPos.xy;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float UnpackCoarseDepth(float2 enc) 
{ 
	return enc.x * enc.x; 
}
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	// 采样线性深度,转换为真实深度
	// (wz原文就是UnpackCoarseDepth())
	float4 vGeoTex = tex2D(sdDepthSampler, kInput.vUVSetScreenTex);
	float fDepth = UnpackCoarseDepth(vGeoTex.xy);
	float fRealDepth = fDepth * g_fCameraFarClip;
	
	// 
	float3 vViewPos = float3(kInput.vUVSetProjPos, 1.f) * fRealDepth;
	
	// 计算雾系数
	float fFogFactor = CalcFogFactors(vViewPos);
	
	// float3 vBaseColor = tex2D(sdColorSampler, kInput.vUVSetScreenTex).xyz;
	return float4(g_vFogColor, fFogFactor);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique PostEffect_tecPostFog
<
	string Description = "PostEffect_tecPostFog";
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