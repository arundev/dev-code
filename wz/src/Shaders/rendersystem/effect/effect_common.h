//*************************************************************************************************
// 内容:	效果着色器的公共部分
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-15
// 最后修改:	
//*************************************************************************************************
#ifndef _EFFECT_COMMON_H__
#define _EFFECT_COMMON_H__

//
#include "../common.h"
#include "../staticmesh/static_mesh_common.h"

//---------------------------------------------------------------------------------------
// 输入变量
//---------------------------------------------------------------------------------------
float	g_fNoiseStrength	: GLOBAL;
float	g_fScrollStrength	: GLOBAL;

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0,	sdGeomSampler,		sdGeomBuf,		false);
SD_POINT_CLAMP_SAMPLE(1,	sdShadingSampler,	sdShadingTex,	false);
SD_LINEAR_CLAMP_SAMPLE(0,	sdCurveFogSampler,	sdCurveFogTex,	false);

//---------------------------------------------------------------------------------------
// 效果着色器渲染通道
//---------------------------------------------------------------------------------------
#define VERTEX_COMPACTVERTEX_FLAG			0x00000001	///< 顶点压缩
#define VERTEX_EHARDWARESKIN_FLAG			0x00000002	///< 骨骼
#define VERTEX_LEAFANIM_FLAG				0x00000004	///< 树叶顶点动画

//---------------------------------------------------------------------------------------
float4 FilterColor(float2 vUVSet, float fOffset, float fDepth)
{
	float4 vColor;
	vColor = tex2D(sdShadingSampler, vUVSet);
	
	float2 vPosition[12] = {
		float2(-0.326212f, -0.40581f),
		float2(-0.840144f, -0.07358f),
		float2(-0.695914f,  0.457137f),
		float2(-0.203345f, -0.620716f),
		float2( 0.96234f,  -0.194983f),
		float2( 0.473434f, -0.480026f),
		float2( 0.519456f,  0.767022f),
		float2( 0.185461f, -0.893124f),
		float2( 0.507431f,  0.064425f),
		float2( 0.89642f,   0.412458f),
		float2(-0.32194f,  -0.932615f),
		float2(-0.791559f, -0.59771f),
		};
		
	float fCount = 1.f;
	for (int tap = 0; tap < 12; ++tap)
	{
		float2 vCoord = vUVSet + (g_vPixelSize * vPosition[tap] * fOffset);
		float fSceneDepth = UnpackDepth(tex2D(sdGeomSampler, vCoord).xy);
		if (fSceneDepth <= 0.000001f)
		{
			fSceneDepth = 1.f;
		}
	
		if (fSceneDepth > fDepth)
		{
			vColor += tex2D(sdShadingSampler, vCoord);
			fCount += 1.f;
		}
	}
	
	return vColor / fCount;
}
//---------------------------------------------------------------------------------------
#endif