//*************************************************************************************************
// ����:	����Mesh�����Ԥ����
//---------------------------------------------------------
// ����:		
// ����:		2012-07-14
// ����޸�:	2013-09-10
//*************************************************************************************************
#include "terrain_common.h"

//---------------------------------------------------------------------------------------
// ȫ�ֱ���
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------

//---------------------------------------------------------------------------------------
// ������ɫ�����������
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos		: POSITION;
	float	fDepth			: TEXCOORD0;
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(/* float2 */ uint4 vPos : POSITION0)
{
	VS_OUTPUT kOutput;

	//
	// ************************************************************************
	// ����һ:
	//
	//	ԭ��	vPos.x = ix * 129 + iy
	//			vPos.y = floor(fZ * 20.0f + 0.5f)
	//
	//	ʵ��	float2 vPos : POSITION0
	//			float2 iXY = floor(float2(vPos.x / 129.f, fmod(vPos.x, 129.f) + 0.01f));
	//			float4 vWorldPosition = float4(iXY * a_fQuadScale + a_vQuadOrigin, vPos.y * 0.05f, 1.f);
	// ************************************************************************
	// ������:
	//
	//	ԭ��	vPos.x = ix
	//			vPos.y = iy
	//			vPos.z = (floor(fZ * 20.f) + 32768) >> 8;	///< �߶ȵĸ߰�λ
	//			vPos.w = (floor(fZ * 20.f) + 32768) & 0xff;	///< �߶ȵĵͰ�λ
	//
	// 	ʵ��	uint4 vPos : POSITION0
	//			uint2 iXY = vPos.xy;
	//			float fZ = (vPos.z - 128) * 256 + vPos.w;
	//			float4 vWorldPosition = float4(iXY * a_fQuadScale + a_vQuadOrigin, fZ * 0.05f, 1.f);
	// ************************************************************************
	// ������:
	//		
	//	ԭ��	vPos.x = ix
	//			vPos.y = iy
	//			vPos.z = (unsigned short)((fZ - fMin) / (fMax - fMin) * 65535) >> 8;	///< �߶ȵĸ߰�λ
	//			vPos.w = (unsigned short)((fZ - fMin) / (fMax - fMin) * 65535)  & 0xff;	///< �߶ȵĵͰ�λ
	//
	// 	ʵ��	uint4 vPos : POSITION0
	//			uint2 iXY = vPos.xy;
	//			float fZ = (vPos.z * 256 + vPos.w) / 65535.f * (g_fTerrainMaxHeight - g_fTerrainMinHeight) + g_fTerrainMinHeight;
	//			float4 vWorldPosition = float4(iXY * a_fQuadScale + a_vQuadOrigin, fZ, 1.f);
	// ************************************************************************
	//
	// ��ȡѹ����XY�����任����������
	uint2 iXY = vPos.xy;
	float fZ = (vPos.z * 256 + vPos.w) / 65535.f * (g_fTerrainMaxHeight - g_fTerrainMinHeight) + g_fTerrainMinHeight;
	float4 vWorldPosition = float4(iXY * a_fQuadScale + a_vQuadOrigin, fZ, 1.f);
	
	// �任���۲�����
	kOutput.vProjPos = mul(vWorldPosition, g_mViewProj);
	
	// �������(ֱ�ӳ��Ծ������һ��)
	kOutput.fDepth = dot(vWorldPosition, g_mView._m02_m12_m22_m32);
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
float4	PS_Main(VS_OUTPUT kInput) : COLOR0
{
	// ת�������[0, 1.f]�������
	return float4(PackDepth(kInput.fDepth / g_fCameraFarClip), 0.f, 0.f);
}

//---------------------------------------------------------------------------------------
// ��ɫ����
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