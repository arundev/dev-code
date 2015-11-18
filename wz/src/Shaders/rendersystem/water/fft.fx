//*************************************************************************************************
// 内容:	Water
//
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-31
// 最后修改:	
//*************************************************************************************************
#include "water_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
float	g_fPass				: GLOBAL;
float	g_fMN 				: GLOBAL;	// 1/(M*N)
float	g_fHeightMapSize	: GLOBAL;
float3	g_vXYZScale			: GLOBAL;

float	g_fWaveTime			: GLOBAL;

// 屏幕像素尺寸
//float2	g_vOnePixelSize		: GLOBAL;

float4 	g_akCornerPoints[4]		: GLOBAL;
float	g_fZScale				: GLOBAL;

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
texture sdHeightTex<string NTM = "shader"; int NTMIndex = 0;>;
texture sdSourceTex<string NTM = "shader"; int NTMIndex = 0;>;
texture sdButterFlyTex<string NTM = "shader"; int NTMIndex = 1;>;
texture sdBaseWaveTex<string NTM = "shader"; int NTMIndex = 0;>;
texture sdFrequencyTex<string NTM = "shader"; int NTMIndex = 1;>;
texture sdMTFTex;

sampler2D sdSourceSampler = sampler_state
{
	Texture = <sdSourceTex>;
	AddressU = Clamp;
	AddressV = Clamp;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = Point;
	SRGBTexture = false;
};

sampler2D sdSourceWrapSampler = sampler_state
{
	Texture = <sdSourceTex>;
	AddressU = Wrap;
	AddressV = Wrap;
	MinFilter = Point;
	MagFilter = Linear;
	MipFilter = Linear;
	SRGBTexture = false;
};

sampler2D sdButterFlySampler = sampler_state
{
	Texture = <sdButterFlyTex>;
	AddressU = Clamp;
	AddressV = Clamp;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = Point;
	SRGBTexture = false;
};

sampler2D sdBaseWaveSampler = sampler_state
{
	Texture = <sdBaseWaveTex>;
	AddressU = Clamp;
	AddressV = Clamp;
	MinFilter = Point;
	MagFilter = Point;
	MipFilter = Point;
	SRGBTexture = false;
};

sampler2D sdFrequencySampler = sampler_state
{
	Texture = <sdFrequencyTex>;
	AddressU = Wrap;
	AddressV = Wrap;
	MinFilter = Point;
	MagFilter = Linear;
	MipFilter = Linear;
	SRGBTexture = false;
};

sampler2D sdMTFSampler = sampler_state
{
	Texture = <sdMTFTex>;
	AddressU = Wrap;
	AddressV = Wrap;
	MinFilter = Point;
	MagFilter = Linear;
	MipFilter = Linear;
	SRGBTexture = false;
};

sampler2D sdHeightSampler = sampler_state
{
	Texture = <sdHeightTex>;
	AddressU = Wrap;
	AddressV = Wrap;
	MinFilter = Linear;
	MagFilter = Linear;
	MipFilter = Linear;
	SRGBTexture = false;
};

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos		: POSITION0;
	float2	vUVSet0		: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;	
	float2	vUVSetScreenPos	: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	kOutput.vProjPos = float4(kInput.vPos.xy, 1.f, 1.f);
	kOutput.vUVSetScreenPos = kInput.vUVSet0 + g_vHalfPixelOffset;
	return kOutput;
}
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main_Normal(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	kOutput.vProjPos = float4(kInput.vPos.xy, 1.f, 1.f);
	kOutput.vUVSetScreenPos = kInput.vUVSet0;
	//kOutput.vUVSetScreenPos.y = 1.f - kOutput.vUVSetScreenPos.y;
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
float4 PS_Main_FFTH(VS_OUTPUT kInput) : COLOR0
{
	float2 vSampleCoord;
	
	vSampleCoord.x = kInput.vUVSetScreenPos.x;
	vSampleCoord.y = g_fPass;
	float4 vButterFly = tex2D(sdButterFlySampler, vSampleCoord);
	
	// sample location A
	vSampleCoord.x = vButterFly.x;
	vSampleCoord.y = kInput.vUVSetScreenPos.y;
	float2 a = tex2D(sdSourceSampler, vSampleCoord).xy;

	// sample location B
	vSampleCoord.x = abs(vButterFly.y);
	vSampleCoord.y = kInput.vUVSetScreenPos.y;
	float2 b = tex2D(sdSourceSampler, vSampleCoord).xy;
	
	// multiply w * b(complex numbers)
	float2 w = vButterFly.ba;
	float temp = w.x * b.x - w.y * b.y;
	b.y = w.y * b.x + w.x * b.y;
	b.x = temp;
	
	// perform a + w * b or a - w * b
	a = a + ((vButterFly.y < 0.f) ? -b : b);
	
	// make it a 4 component output for good measure
	return a.xyyy;
}
//---------------------------------------------------------------------------------------
float4 PS_Main_FFTV(VS_OUTPUT kInput) : COLOR0
{
	float2 vSampleCoord;
	
	vSampleCoord.x = kInput.vUVSetScreenPos.y;
	vSampleCoord.y = g_fPass;
	float4 vButterFly = tex2D(sdButterFlySampler, vSampleCoord);
	
	// sample location A
	vSampleCoord.x = kInput.vUVSetScreenPos.x;
	vSampleCoord.y = vButterFly.x;
	float2 a = tex2D(sdSourceSampler, vSampleCoord).xy;

	// sample location B
	vSampleCoord.x = kInput.vUVSetScreenPos.x;
	vSampleCoord.y = abs(vButterFly.y);
	float2 b = tex2D(sdSourceSampler, vSampleCoord).xy;
	
	// multiply w * b(complex numbers)
	float2 w = vButterFly.ba;
	float temp = w.x * b.x - w.y * b.y;
	b.y = w.y * b.x + w.x * b.y;
	b.x = temp;
	
	// perform a + w * b or a - w * b
	a = a + ((vButterFly.y < 0.f) ? -b : b);
	
	// make it a 4 component output for good measure
	return a.xyyy;
}
//---------------------------------------------------------------------------------------
/*
float4 PS_Main_FFTIH(VS_OUTPUT kInput) : COLOR0
{
	float2 vSampleCoord;
	
	vSampleCoord.x = kInput.vUVSetScreenPos.x;
	vSampleCoord.y = g_fPass;
	float4 vButterFly = tex2D(sdButterFlySampler, vSampleCoord);
	
	// sample location A
	vSampleCoord.x = vButterFly.x;
	vSampleCoord.y = kInput.vUVSetScreenPos.y;
	float2 a = tex2D(sdSourceSampler, vSampleCoord).xy;

	// sample location B
	vSampleCoord.x = abs(vButterFly.y);
	vSampleCoord.y = kInput.vUVSetScreenPos.y;
	float2 b = tex2D(sdSourceSampler, vSampleCoord).xy;
	
	// multiply w * b(complex numbers)
	float2 w = vButterFly.ba;
	float temp = w.x * b.x - w.y * b.y;
	b.y = -w.y * b.x + w.x * b.y;
	b.x = temp;
	
	// perform a + w * b or a - w * b
	a = a + ((vButterFly.y < 0.f) ? -b : b);
	
	// make it a 4 component output for good measure
	return a.xyyy;
}
/*
//---------------------------------------------------------------------------------------
/*
float4 PS_Main_FFTIV(VS_OUTPUT kInput) : COLOR0
{
	float2 vSampleCoord;
	
	vSampleCoord.x = kInput.vUVSetScreenPos.y;
	vSampleCoord.y = g_fPass;
	float4 vButterFly = tex2D(sdButterFlySampler, vSampleCoord);
	
	// sample location A
	vSampleCoord.x = kInput.vUVSetScreenPos.x;
	vSampleCoord.y = vButterFly.x;
	float2 a = tex2D(sdSourceSampler, vSampleCoord).xy;

	// sample location B
	vSampleCoord.x = kInput.vUVSetScreenPos.x;
	vSampleCoord.y = abs(vButterFly.y);
	float2 b = tex2D(sdSourceSampler, vSampleCoord).xy;
	
	// multiply w * b(complex numbers)
	float2 w = vButterFly.ba;
	float temp = w.x * b.x - w.y * b.y;
	b.y = -w.y * b.x + w.x * b.y;
	b.x = temp;
	
	// perform a + w * b or a - w * b
	a = a + ((vButterFly.y < 0.f) ? -b : b);
	
	// make it a 4 component output for good measure
	return a.xyyy;
}
*/
//---------------------------------------------------------------------------------------
float4 PS_Main_FFTX(VS_OUTPUT kInput) : COLOR0
{
	float2 vSampleCoord = float2(kInput.vUVSetScreenPos.x, g_fPass);
	float4 vButterFly = tex2D(sdButterFlySampler, vSampleCoord);
	
	// performs two FFTs on two inputs packed in a single texture 
	// returns two results packed in a single vec4
	float2 i = vButterFly.xy;
	float2 w = vButterFly.zw;
	float2 uv = kInput.vUVSetScreenPos;
	
	float4 input1 = tex2D(sdSourceSampler, float2(i.x, uv.y));
	float4 input2 = tex2D(sdSourceSampler, float2(i.y, uv.y));
	float res1x = w.x * input2.x - w.y * input2.y;
	float res1y = w.y * input2.x + w.x * input2.y;	
	//float res2x = w.x * input2.z - w.y * input2.w;
	//float res2y = w.y * input2.z + w.x * input2.w;	
	
	return input1 + float4(res1x, res1y, 0, 0);
}
//---------------------------------------------------------------------------------------
float4 PS_Main_FFTY(VS_OUTPUT kInput) : COLOR0
{
	float2 vSampleCoord = float2(kInput.vUVSetScreenPos.y, g_fPass);
	float4 vButterFly = tex2D(sdButterFlySampler, vSampleCoord);
	
	// performs two FFTs on two inputs packed in a single texture 
	// returns two results packed in a single vec4
	float2 i = vButterFly.xy;
	float2 w = vButterFly.zw;
	float2 uv = kInput.vUVSetScreenPos;
	
	float4 input1 = tex2D(sdSourceSampler, float2(uv.x, i.x));
	float4 input2 = tex2D(sdSourceSampler, float2(uv.x, i.y));
	float res1x = w.x * input2.x - w.y * input2.y;
	float res1y = w.y * input2.x + w.x * input2.y;	
	//float res2x = w.x * input2.z - w.y * input2.w;
	//float res2y = w.y * input2.z + w.x * input2.w;	
	
	return input1 + float4(res1x, res1y, 0, 0);
}
//---------------------------------------------------------------------------------------
float4 PS_Main_MTF(VS_OUTPUT kInput) : COLOR0
{
	float fMTF = tex2D(sdMTFSampler, kInput.vUVSetScreenPos - 0.5f).x;
	
	float2 vCoord =  kInput.vUVSetScreenPos;
	if (vCoord.x < 0.5f && vCoord.y < 0.5f)
	{
		if (sqrt(vCoord.x * vCoord.x + vCoord.y * vCoord.y) < 0.3f)
			fMTF = 0.f;
	}
	else if (vCoord.x < 0.5f && vCoord.y > 0.5f)
	{
		if (sqrt(vCoord.x * vCoord.x + (vCoord.y - 1.f) * (vCoord.y - 1.f)) < 0.3f)
			fMTF = 0.f;
	}
	else if (vCoord.x > 0.5f && vCoord.y < 0.5f)
	{
		if (sqrt((vCoord.x - 1.f) * (vCoord.x - 1.f) + vCoord.y * vCoord.y) < 0.3f)
			fMTF = 0.f;
	}
	else if (vCoord.x > 0.5f && vCoord.y > 0.5f)
	{
		if (sqrt((vCoord.x - 1.f) * (vCoord.x - 1.f) + (vCoord.y - 1.f) * (vCoord.y - 1.f)) < 0.3f)
			fMTF = 0.f;
	}
	
	float4 vColor = tex2D(sdSourceSampler, kInput.vUVSetScreenPos);
	vColor *= fMTF;
	
	return vColor;
}
//---------------------------------------------------------------------------------------
float4 PS_Main_Debug_Display(VS_OUTPUT kInput) : COLOR0
{
	float4 vColor = tex2D(sdSourceWrapSampler, kInput.vUVSetScreenPos - 0.5f);
	float fMag = vColor.x * vColor.x + vColor.y * vColor.y;
	fMag = 0.05f * sqrt(fMag + 1.f);
	//fMag = UserInput.z * 0.1f * log2(fMag + 1.f);
	
	return fMag.xxxx;
}
//---------------------------------------------------------------------------------------
float4 PS_Main_Debug_Copy(VS_OUTPUT kInput) : COLOR0
{
	int2 vPixelCoord = kInput.vUVSetScreenPos * g_vWindowSize;
	float fScale = (vPixelCoord.x + vPixelCoord.y) % 2 == 1 ? 1.f : -1.f;
	return float4(tex2D(sdSourceWrapSampler, kInput.vUVSetScreenPos).rg, 0, 0) * float4(fScale, 1.f, 0, 0);
}
//---------------------------------------------------------------------------------------
float4 PS_Main_FFT_UpdateWave(VS_OUTPUT kInput) : COLOR0
{
	float2 vBaseWave = 0.f;
	vBaseWave.rg = tex2D(sdBaseWaveSampler, kInput.vUVSetScreenPos).rg * g_fMN;
	
	float2 vBaseWave2 = 0.f;
	vBaseWave2.rg = tex2D(sdBaseWaveSampler, 1.f - kInput.vUVSetScreenPos).rg * g_fMN;
	
	float fFrequency = tex2D(sdFrequencySampler, kInput.vUVSetScreenPos).r * g_fWaveTime;
	float s,c;
	sincos(fFrequency, s, c);
	
	float2 vColor = 0.f;
	vColor.r = vBaseWave.r * c - vBaseWave.g * s + vBaseWave2.r * c - vBaseWave2.g * s;
	vColor.g = vBaseWave.r * s - vBaseWave.g * c + vBaseWave2.r * (-s) + vBaseWave2.g * c;
		
	return float4(vColor.rg, 0, 0);
}
//---------------------------------------------------------------------------------------
float4 PS_Main_RestoreColor(VS_OUTPUT kInput) : COLOR0
{
	return tex2D(sdSourceWrapSampler, kInput.vUVSetScreenPos);
}
//---------------------------------------------------------------------------------------
float4 PS_Main_GenNormal(VS_OUTPUT kInput) : COLOR0
{
	float fSampleXP = tex2D(sdHeightSampler,  kInput.vUVSetScreenPos + float2(g_vPixelSize.x, 0)).r;
	float fSampleXN = tex2D(sdHeightSampler,  kInput.vUVSetScreenPos + float2(-g_vPixelSize.x, 0)).r;
	float fSampleYP = tex2D(sdHeightSampler,  kInput.vUVSetScreenPos + float2(0, g_vPixelSize.x)).r;
	float fSampleYN = tex2D(sdHeightSampler,  kInput.vUVSetScreenPos + float2(0, -g_vPixelSize.x)).r;
	float fSampleCenter = tex2D(sdHeightSampler, kInput.vUVSetScreenPos).r;
	
	float3 v1 = float3(g_vPixelSize.x * 2.f * g_fHeightMapSize * g_vXYZScale.x, 0, (fSampleXP - fSampleXN) * g_vXYZScale.z);
	float3 v2 = float3(0, g_vPixelSize.y * 2.f * g_fHeightMapSize * g_vXYZScale.y, (fSampleYP - fSampleYN) * g_vXYZScale.z);
	float3 normal = normalize(cross(v1, v2));
	
	return float4(normal, fSampleCenter);
}
//---------------------------------------------------------------------------------------
/*
float4 PS_Main_GenNormal(VS_OUTPUT kInput) : COLOR0
{
	float2 vUV0 =  kInput.vUVSetScreenPos + float2(g_vPixelSize.x, 0);
	float2 vUV1 =  kInput.vUVSetScreenPos + float2(-g_vPixelSize.x, 0);
	float2 vUV2 =  kInput.vUVSetScreenPos + float2(0, g_vPixelSize.x);
	float2 vUV3 =  kInput.vUVSetScreenPos + float2(0, -g_vPixelSize.x);	

	float4 vPos0 = lerp(lerp(g_akCornerPoints[0], g_akCornerPoints[1], vUV0.x),
					   lerp(g_akCornerPoints[2], g_akCornerPoints[3], vUV0.x),
					   vUV0.y);
	vPos0.xyz /= vPos0.w;			   

	float4 vPos1 = lerp(lerp(g_akCornerPoints[0], g_akCornerPoints[1], vUV1.x),
					   lerp(g_akCornerPoints[2], g_akCornerPoints[3], vUV1.x),
					   vUV1.y);
	vPos1.xyz /= vPos1.w;	
	
	float4 vPos2 = lerp(lerp(g_akCornerPoints[0], g_akCornerPoints[1], vUV2.x),
					   lerp(g_akCornerPoints[2], g_akCornerPoints[3], vUV2.x),
					   vUV2.y);
	vPos2.xyz /= vPos2.w;	
	
	float4 vPos3 = lerp(lerp(g_akCornerPoints[0], g_akCornerPoints[1], vUV3.x),
					   lerp(g_akCornerPoints[2], g_akCornerPoints[3], vUV3.x),
					   vUV3.y);
	vPos3.xyz /= vPos3.w;	

	//
	float4 vHeight;
	vPos0.z += g_vXYZScale.z * tex2D(sdHeightSampler, Pos0.xy / g_fHeightMapSize).r;
	vPos1.z += g_vXYZScale.z * tex2D(sdHeightSampler, Pos1.xy / g_fHeightMapSize).r;
	vPos2.z += g_vXYZScale.z * tex2D(sdHeightSampler, Pos2.xy / g_fHeightMapSize).r;
	vPos3.z += g_vXYZScale.z * tex2D(sdHeightSampler, Pos3.xy / g_fHeightMapSize).r;	

	float3 normal = normalize(cross(vPos0 - vPos1, vPos2 - vPos3));
	
	return float4(normal * 0.5f + 0.5f);
}
*/
//---------------------------------------------------------------------------------------


//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique FFT_H
<
	string Description = "FFT_H";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main_FFTH ();
	}
}
//---------------------------------------------------------------------------------------
technique FFT_V
<
	string Description = "FFT_V";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main_FFTH ();
	}
}
//---------------------------------------------------------------------------------------
technique FFT_X
<
	string Description = "FFT_X";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main_FFTX ();
	}
}
//---------------------------------------------------------------------------------------
technique FFT_Y
<
	string Description = "FFT_Y";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main_FFTY ();
	}
}
//---------------------------------------------------------------------------------------
technique FFT_MTF
<
	string Description = "FFT_MTF";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main_MTF ();
	}
}
//---------------------------------------------------------------------------------------
technique FFT_Debug_Display
<
	string Description = "FFT_Debug_Display";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main_Debug_Display ();
	}
}
//---------------------------------------------------------------------------------------
technique FFT_Debug_Copy
<
	string Description = "FFT_Debug_Copy";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_3_0 VS_Main ();
		PixelShader 	= compile ps_3_0 PS_Main_Debug_Copy ();
	}
}
//---------------------------------------------------------------------------------------
technique FFT_UpdateWave
<
	string Description = "FFT_UpdateWave";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main_FFT_UpdateWave ();
	}
}
//---------------------------------------------------------------------------------------
technique RestourColor
<
	string Description = "RestourColor";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main_RestoreColor ();
	}
}
//---------------------------------------------------------------------------------------
technique GenWaterNormal
<
	string Description = "GenWaterNormal";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main_GenNormal ();
	}
}
//---------------------------------------------------------------------------------------