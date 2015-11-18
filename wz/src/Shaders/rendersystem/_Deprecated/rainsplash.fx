//*************************************************************************************************
// 内容:	Rain(已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-09
// 最后修改:
//*************************************************************************************************
#include "common_fun.inl"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
//
// [speed][rot_speed][size_w][size_h]
// [r][g][b][alpha_factor]
// [speed_random][rot_rand][size_rand][add_factor]
// [tex_w][tex_h][alphaIn_time][alphaOut_time]
float4x4	a_mPDParam		: ATTRIBUTE;
//float3x3	a_mPDRM			: ATTRIBUTE;

#define 	a_fPDSpeed			a_mPDParam[0][0]
#define		a_fPDRotSpeed		a_mPDParam[0][1]
#define		a_fPDSizeW			a_mPDParam[0][2]
#define		a_fPDSizeH			a_mPDParam[0][3]

#define 	a_fPDSpeedRand		a_mPDParam[2][0]
#define		a_fPDRotSpeedRand	a_mPDParam[2][1]
#define		a_fPDSizeRand		a_mPDParam[2][2]
#define		a_fPDAddFactor		a_mPDParam[2][3]

#define		a_vPDColor			float3(a_mPDParam[1][0], a_mPDParam[1][1], a_mPDParam[1][2])
#define		a_fPDAlphaFactor	a_mPDParam[1][3]
#define		a_fPDAlphaIn		a_mPDParam[3][2]
#define		a_fPDAlphaOut		a_mPDParam[3][3]

#define		a_fPDTexSizeW		a_mPDParam[3][0]
#define		a_fPDTexSizeH		a_mPDParam[3][1]

// 地图大小
float		a_fMapSize		: ATTRIBUTE;

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_SAMPLE_EX2(0, "base", sdRainSampler, sdRainTex, CLAMP, LINEAR, true);
//SD_LINEAR_CLAMP_SAMPLE(0,	sdHeightSampler,	sdHeightTex,	false);

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos	: POSITION0;
	float3	vUVSet0	: TEXCOORD0;
	float3	vUVSet1	: TEXCOORD0;
	float3	vUVSet2	: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;		// 投影坐标
	float3	vUVSetWorldPos	: TEXCOORD0;
	float2	vUVSet0			: TEXCOORD1;	
	float3	vUVSetSpeed		: TEXCOORD2;
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
#define	fPDIndex		kInput.vUVSet0.z
#define	fPDBirthTime	kInput.vUVSet1.y
#define	fPDLifeTime		kInput.vUVSet1.x
#define	fPDTexIndex		kInput.vUVSet1.z

	VS_OUTPUT kOutput = (VS_OUTPUT)0;
	
	float fRF = sin((kInput.vPos.x + kInput.vPos.y + kInput.vPos.z) * 10.f);
	float fSpeedRand = 1.f - a_fPDSpeedRand * fRF;
	float fTimePoint = g_fTime - fPDBirthTime;
	float fF0 = clamp(fTimePoint , 0, fPDLifeTime);
	
	float3 vEmitVec = kInput.vUVSet2;
	float3 vSpeed = vEmitVec * a_fPDSpeed * fF0;
	kOutput.vUVSetSpeed = vSpeed;
	
	float3 vMoveVec = vSpeed * fF0;
	
	float fSizeRand = 1.f - a_fPDSizeRand * fRF;
	float3 vBillboardPos = fSizeRand * (a_fPDSizeW * kInput.vUVSet0.x * g_mInvView[0].xyz + 
		a_fPDSizeH * kInput.vUVSet0.y * float3(0,0,1));
		
	float3 vVec = kInput.vPos + vMoveVec;
	kOutput.vUVSetWorldPos = vVec;
	
	float3 vWorldPos = vBillboardPos + vVec;
	kOutput.vProjPos = mul(float4(vWorldPos, 1.f), g_mViewProj);
	
	// 计算纹理,固定取第一帧动画
	float2 vUVSet = GetUVFromPos(0, a_fPDTexSizeW, a_fPDTexSizeH, kInput.vUVSet0.xy * 2.f, float2(1, 1));
	float fCosTheta = abs(dot(-g_mInvView[2].xyz, float3(0,0,1)));
	float fDegree = degrees(acos(fCosTheta));
	float fBias = clamp(floor(fDegree / 22.f), 0, 3);
	vUVSet.y += fBias * 1.f / a_fPDTexSizeH;	// x方向将在PS中计算
	kOutput.vUVSet0 = vUVSet;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
// 纹理位移补偿
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
#define	ALIVE_TIME  0.4f

	//float2 vUVSet;
	//vUVSet.x = kInput.vUVSetWorldPos.x / a_fMapSize;
	//vUVSet.y = 1.f - kInput.vUVSetWorldPos.y / a_fMapSize;

	//float fHeight = DecodeRGBA2Float(tex2D(sdHeightSample, vUVSet));
	
	float4 vColor = 0;
	float fF0 = (/*fHeight*/ - kInput.vUVSetWorldPos.z + 0.2f) / kInput.vUVSetSpeed.z;
	if (fF0 >= 0 && fF0 <= ALIVE_TIME)
	{
		float fIndex = GetIndexFromTimePoint(fF0 , ALIVE_TIME, 4.f);// 只播放0.5s
		
		float2 vUVSet0 = kInput.vUVSet0;
		vUVSet0.x += fIndex * 1.f / a_fPDTexSizeW;
		
		vColor = tex2D(sdRainSampler, vUVSet0);
	}
	
	return vColor;
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique Environment_RainSplash
<
	string Description = "Environment_RainSplash";
	bool UsesNiRenderState = false;
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
		//SrcBlend = SrcAlpha;
		//DestBlend = InvSrcAlpha;
	}
}
//---------------------------------------------------------------------------------------