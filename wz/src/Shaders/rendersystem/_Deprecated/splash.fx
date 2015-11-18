//*************************************************************************************************
// ����:	Splash(�ѷ���)
//---------------------------------------------------------
// ����:		
// ����:		2012-11-07
// ����޸�:
//*************************************************************************************************
#include "common_fun.inl"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
// [w][h][alive][dead]
// [tex_w][tex_h][frame_num][]
// [][][][]
// [][][][]
float4x4	g_mSplashParam	: ATTRIBUTE;

#define		g_fSplashWidth	g_mSplashParam[0][0]
#define		g_fSplashHeight	g_mSplashParam[0][1]
#define		g_fAliveTime	g_mSplashParam[0][2]
#define		g_fDeadTime		g_mSplashParam[0][3]

#define 	g_fTexWidth		g_mSplashParam[1][0]
#define 	g_fTexHeight	g_mSplashParam[1][1]
#define 	g_fFrameNum		g_mSplashParam[1][2]

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
// Splash����
SD_LINEAR_CLAMP_SAMPLE(0, sdSplashSampler,	sdSplashTex,	true);

//---------------------------------------------------------------------------------------
// ������ɫ������������
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos			: POSITION0;
	float2	vUVSet0			: TEXCOORD0;
	float2	vUVSet1			: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// ������ɫ�����������
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4		vProjPos	: POSITION;		// ͶӰ����
	float2		vUVSet		: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;
	
	// ������������
	float3 vWorldPos = kInput.vPos;
	vWorldPos.z += g_fSplashHeight / 2.f;
	
	float fTimePoint = g_fTime + kInput.vUVSet1.x;
	float fF0 = fmod(fTimePoint, g_fAliveTime + g_fDeadTime);
	float fF1 = 1.f - clamp(ceil(fF0 - g_fAliveTime), 0, 1);
	vWorldPos += fF1 * (g_fSplashWidth * kInput.vUVSet0.x * g_mInvView[0].xyz + 
						g_fSplashHeight * kInput.vUVSet0.y * g_mInvView[1].xyz);
		
	// ����ͶӰ����
	kOutput.vProjPos = mul(float4(vWorldPos, 1.f), g_mViewProj);
	
	// ������������
	float fCosTheta = abs(dot(-g_mInvView[2].xyz, float3(0,0,1)));
	float fDegree = degrees(acos(fCosTheta));
	float fBias = clamp(floor(fDegree / 22.f), 0, 3.f);
	float fIndex = GetIndexFromTimePoint(fF0, g_fAliveTime, 4.f);
	float2 vUVSet = GetUVFromPos(fIndex, g_fTexWidth, g_fTexHeight, kInput.vUVSet0 * 2.f, float2(1, 1));
	vUVSet.y += fBias * 1.f / g_fTexHeight;
	kOutput.vUVSet = vUVSet;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
float4 PS_Main(VS_OUTPUT kInput) : COLOR0
{
	float4 vColor = tex2D(sdSplashSampler, kInput.vUVSet);
	vColor.xyz = vColor.w * 0.65f;
	
	return vColor;
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
technique Splash
<
	string Description = "Splash";
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
		//BlendOp = Add;
		//SrcBlend = One;
		//DestBlend = One;
	}
}
//---------------------------------------------------------------------------------------