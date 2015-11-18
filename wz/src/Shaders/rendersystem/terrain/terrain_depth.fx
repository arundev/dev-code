//*************************************************************************************************
// 内容:	地形Mesh的深度预绘制
//---------------------------------------------------------
// 作者:		
// 创建:		2012-07-14
// 最后修改:	2013-09-10
//*************************************************************************************************
#include "terrain_common.h"

//---------------------------------------------------------------------------------------
// 全局变量
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// 输入纹理采样器
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// 顶点着色器输出数据流
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;
	float	fDepth			: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// 顶点着色器
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(/* float2 */ uint4 vPos : POSITION0)
{
	VS_OUTPUT kOutput;

	//
	// ************************************************************************
	// 方案一:
	//
	//	原理	vPos.x = ix * 129 + iy
	//			vPos.y = floor(fZ * 20.0f + 0.5f)
	//
	//	实现	float2 vPos : POSITION0
	//			float2 iXY = floor(float2(vPos.x / 129.f, fmod(vPos.x, 129.f) + 0.01f));
	//			float4 vWorldPosition = float4(iXY * a_fQuadScale + a_vQuadOrigin, vPos.y * 0.05f, 1.f);
	// ************************************************************************
	// 方案二:
	//
	//	原理	vPos.x = ix
	//			vPos.y = iy
	//			vPos.z = (floor(fZ * 20.f) + 32768) >> 8;	///< 高度的高八位
	//			vPos.w = (floor(fZ * 20.f) + 32768) & 0xff;	///< 高度的低八位
	//
	// 	实现	uint4 vPos : POSITION0
	//			uint2 iXY = vPos.xy;
	//			float fZ = (vPos.z - 128) * 256 + vPos.w;
	//			float4 vWorldPosition = float4(iXY * a_fQuadScale + a_vQuadOrigin, fZ * 0.05f, 1.f);
	// ************************************************************************
	// 方案三:
	//		
	//	原理	vPos.x = ix
	//			vPos.y = iy
	//			vPos.z = (unsigned short)((fZ - fMin) / (fMax - fMin) * 65535) >> 8;	///< 高度的高八位
	//			vPos.w = (unsigned short)((fZ - fMin) / (fMax - fMin) * 65535)  & 0xff;	///< 高度的低八位
	//
	// 	实现	uint4 vPos : POSITION0
	//			uint2 iXY = vPos.xy;
	//			float fZ = (vPos.z * 256 + vPos.w) / 65535.f * (g_fTerrainMaxHeight - g_fTerrainMinHeight) + g_fTerrainMinHeight;
	//			float4 vWorldPosition = float4(iXY * a_fQuadScale + a_vQuadOrigin, fZ, 1.f);
	// ************************************************************************
	//
	// 提取压缩的XY，并变换到世界坐标
	uint2 iXY = vPos.xy;
	float fZ = (vPos.z * 256 + vPos.w) / 65535.f * (g_fTerrainMaxHeight - g_fTerrainMinHeight) + g_fTerrainMinHeight;
	float4 vWorldPosition = float4(iXY * a_fQuadScale + a_vQuadOrigin, fZ, 1.f);
	
	// 变换到观察坐标
	kOutput.vProjPos = mul(vWorldPosition, g_mViewProj);
	
	// 计算深度(直接乘以矩阵最后一列)
	kOutput.fDepth = dot(vWorldPosition, g_mView._m02_m12_m22_m32);
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// 像素着色器
//---------------------------------------------------------------------------------------
float4	PS_Main(VS_OUTPUT kInput) : COLOR0
{
	// 转换到深度[0, 1.f]，并打包
	return float4(PackDepth(kInput.fDepth / g_fCameraFarClip), 0.f, 0.f);
}

//---------------------------------------------------------------------------------------
// 着色技术
//---------------------------------------------------------------------------------------
technique Terrain_Depth
<
	string Description = "Terrain_Depth";
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