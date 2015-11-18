//*************************************************************************************************
// 内容:	Rain(已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-30
// 最后修改:
//*************************************************************************************************
#include "common_fun.inl"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
//
// [fHeight] [fHeightSize] [fWHRatio] [fSpeed]
// [/] [fRainAlpha] [fFallTime] [fSpread]
// [d.x] [d.y] [d.z] [/]
// [/] [/] [/] [/]
float4x4 	a_mRainParam		: ATTRIBUTE;
//float4x4 	a_mRainSplashParam	: ATTRIBUTE;

#define		fRainHeight 		a_mRainParam[0][0]
#define 	fRainDropHeight		a_mRainParam[0][1]
#define 	fRainDropWidth		a_mRainParam[0][2]
#define 	fRainSpeed			a_mRainParam[0][3]

#define		fRainAlpha			a_mRainParam[1][1]
#define		fRainFallTime		a_mRainParam[1][2]
#define		fRainSpread			a_mRainParam[1][3]

#define 	vRainDirection		float3(a_mRainParam[2][3], a_mRainParam[2][1], a_mRainParam[2][2])

// 地图大小
float		a_fMapSize			: ATTRIBUTE;

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_SAMPLE_EX2(0, "base", sdRainSampler, sdRainTex, CLAMP, LINEAR, true);
SD_LINEAR_CLAMP_SAMPLE(0,	sdHeightSampler,	sdHeightTex,	false);

//---------------------------------------------------------------------------------------
// 顶点着色器输入数据流
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos			: POSITION0;
};

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;		// 投影坐标
	float3	vUVSetWorldPos	: TEXCOORD0;
	float2	vUVSet0			: TEXCOORD1;
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// 伪随机雨点分布
	float3 vWorldPos;
	vWorldPos.x = fRainSpread * cos(25.f * kInput.vPos.z);
	vWorldPos.y = fRainSpread * cos(25.f * kInput.vPos.z);
	vWorldPos.z = fRainHeight;
	
	//
	//float3 vRainDirection = normalize(vRainDirection);
	float fStartTime = sin(170.f * (vWorldPos.x + vWorldPos.y + vWorldPos.z)) * fRainFallTime;
	float fElapseTime = fmod(g_fTime - fStartTime, fRainFallTime);
	float3 vFallPos = fElapseTime * fRainSpeed * vRainDirection;
	
	vWorldPos += vFallPos;
	
	// 旋转公告板
	float3 vUp = -vRainDirection;
	vWorldPos += fRainDropWidth * kInput.vPos.x * g_mInvView[0].xyz + 
				 fRainDropHeight * kInput.vPos.y * vUp;
	
	float fCosSita = dot(vRainDirection, float3(0,0,-1));
	float fHH = max(0, fRainHeight - g_vCameraPostion.z);
	vWorldPos.xy += (g_vCameraPostion.xy - (vRainDirection * fHH / fCosSita).xy);
	//float fB = 18.f * sin((vWorldPos.z + g_fTime) * 0.017f);
	//vWorldPos.xy += fB;
	
	//
	kOutput.vUVSetWorldPos = vWorldPos;
	kOutput.vProjPos = mul(float4(vWorldPos, 1.f), g_mViewProj);
	kOutput.vProjPos.xy += float2(0.5f, 0.5f);
	kOutput.vUVSet0 = float2(kInput.vPos.x, 1.f - kInput.vPos.y);
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	float2 vUVSet;
	vUVSet.x = kInput.vUVSetWorldPos.x / a_fMapSize;
	vUVSet.y = 1.f - kInput.vUVSetWorldPos.y / a_fMapSize;
	
	float fHeight = DecodeRGBA2Float(tex2D(sdHeightSampler, vUVSet));
	
	//float fAlphaFactor = clamp((kInput.vUVSetWorldPos.z - fHeight), 0, 1.f);
	float fAlphaFactor = 1.f;
	
	//
	float4 vColor = tex2D(sdRainSampler, kInput.vUVSet0);
	vColor.xyz = vColor.w * 0.4f * fAlphaFactor;
	
	return vColor;
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique Environment_RainPower
<
	string Description = "Environment_RainPower";
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
		//SrcBlend = One;
		//DestBlend = One;
	}
}
//---------------------------------------------------------------------------------------