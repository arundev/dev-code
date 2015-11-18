//*************************************************************************************************
// 内容:	Rain/Snow
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-30
// 最后修改:
//*************************************************************************************************
#include "environment_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
// 
// [speed][/][size_w][size_h]
// [r][g][b][a]
// [speed_rand][bias_x][bias_y][bias_z]
// [leash_axis][size_Rand][alphaIn_time][alphaOut_time]
float4x4	a_mPDParam		: ATTRIBUTE;
float3x3	a_mPDRM			: ATTRIBUTE;

#define		fPDSpeed		a_mPDParam[0][0]
#define		fPDSizeW		a_mPDParam[0][2]
#define		fPDSizeH		a_mPDParam[0][3]	

#define		vPDColor		float3(a_mPDParam[1][0],a_mPDParam[1][1],a_mPDParam[1][2])
#define		fPDAlpha		a_mPDParam[1][3]

#define		fPDSpeedRand	a_mPDParam[2][0]
#define		fPDBiasVec		float3(a_mPDParam[2][1],a_mPDParam[2][2],a_mPDParam[2][3])

#define		fPDLeashAxis	a_mPDParam[3][0]
#define		fPDSizeRand		a_mPDParam[3][1]
#define		fPDAlphaIn		a_mPDParam[3][2]	// 百分比
#define		fPDAlphaOut		a_mPDParam[3][3]	// 百分比

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_SAMPLE_EX2(0, "base", sdRainSnowSampler, sdRainSnowTex, CLAMP, LINEAR, true);
SD_LINEAR_CLAMP_SAMPLE(0,	sdHeightSampler,	sdHeightTex,	false);
SD_POINT_CLAMP_SAMPLE(1,	sdDepthSampler,		sdDepthBuf,		false);

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos			: POSITION0;
	float2	vUVSet0			: TEXCOORD0;	
	float3	vUVSet1			: TEXCOORD1;	// LifeTime, BirthTime, TexIndex
	float3	vUVSet2			: TEXCOORD2;
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;		// 投影坐标
	float4	vUVSetProjPos	: TEXCOORD0;	// 
	float3	vUVSetWorldPos	: TEXCOORD1;
	float2	vUVSet0			: TEXCOORD2;
	float	fTP				: TEXCOORD3;
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
#define fPDLifeTime		kInput.vUVSet1.x
#define fPDBirthTime	kInput.vUVSet1.y
#define fPDTexTime		kInput.vUVSet1.z

	VS_OUTPUT kOutput;

	float3 vRotPos = kInput.vPos;
	float fRF = sin((vRotPos.x + vRotPos.y + vRotPos.z) * 10.f);
	float fSpeedRand = 1.f - fPDSpeed * fRF;
	
	// 计算相对时间
	float fTimePoint = g_fTime - fPDBirthTime;
	float fTimeAnchor = clamp(fTimePoint, 0, fPDLifeTime);
	kOutput.fTP = fTimeAnchor / fPDLifeTime;
	
	// 旋转(可以再CPU/GPU计算)
	float3 vEmitVec = mul(-kInput.vUVSet2, a_mPDRM) + fPDBiasVec;
	float3 vMoveVec = vEmitVec * fPDSpeedRand * fPDSpeedRand *  fTimeAnchor;
	
	float fSizeRand = 1.f - fPDSizeRand * fRF;
	float3 vLeashAxis = lerp(-normalize(vEmitVec), g_mInvView[1].xyz, fPDLeashAxis);
	float3 vBillboardPos = fSizeRand * 
		(fPDSizeW * kInput.vUVSet0.x * g_mInvView[0].xyz + fPDSizeH * kInput.vUVSet0.y * fPDLeashAxis);
		
	//
	float3 vVec = vRotPos + vMoveVec;
	vVec = mul(float4(vVec, 1.f), g_mWorld).xyz;
	kOutput.vUVSetWorldPos = vVec;
	
	float3 vWorldPos = vBillboardPos + vVec;
	kOutput.vProjPos = mul(float4(vWorldPos, 1.f), g_mViewProj);
	
	float4 vHPos = kOutput.vProjPos;
	vHPos.y = -vHPos.y;
	kOutput.vUVSetProjPos.xy = (vHPos.xy + vHPos.w) * 0.5f;
	kOutput.vUVSetProjPos.zw = float2(1.f, vHPos.w);
	
	// 不支持复杂纹理计算
	float2 vTex = kInput.vUVSet0 + float2(0.5f, 0.5f);
	vTex.y = 1.f - vTex.y;
	kOutput.vUVSet0 = vTex;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	float4 vBaseColor = tex2D(sdRainSnowSampler, kInput.vUVSet0);
	float3 vBiasColor = vBaseColor.rgb * vPDColor;
	
	float fInRatio  = clamp(kInput.fTP / fPDAlphaIn, 0, 1.f);
	float fOutRatio = clamp((1.f - kInput.fTP) / (1.f - fPDAlphaOut), 0, 1.f);
	float fRadio = min(fOutRatio, fInRatio);
	
	float fAlphaFactor = 1.f;
	float4 vColor = float4(vBiasColor * fRadio * fPDAlpha * fAlphaFactor, 1.f);
	
	return vColor;
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique Environment_RainSnow
<
	string Description = "Environment_RainSnow";
	bool UsesNiRenderState = true;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_0 VS_Main ();
		PixelShader 	= compile ps_2_0 PS_Main ();
		
		//CullMode = None;
		//ZEnable = true;
		//ZWriteEnable = false;
		//AlphaTestEnable = false;
		//AlphaBlendEnable = true;
		//SrcBlend = One;
		//DestBlend = One;
	}
}
//---------------------------------------------------------------------------------------