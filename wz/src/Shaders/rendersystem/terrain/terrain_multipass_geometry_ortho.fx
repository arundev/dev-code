//*************************************************************************************************
// ����:	����Normal/Depth���Ƶ�G-Buffer
//---------------------------------------------------------
// ����:		
// ����:		2012-08-23
// ����޸�:	2013-05-02
//*************************************************************************************************
#include "terrain_common.h"

//---------------------------------------------------------------------------------------
// �������
//---------------------------------------------------------------------------------------
float4 g_vRecipUVRepeats[3]	: GLOBAL;

//---------------------------------------------------------------------------------------
// �������������
//---------------------------------------------------------------------------------------
SD_POINT_CLAMP_SAMPLE(0, sdDepthSampler,		sdDepthBuf, 		false);		// ������Ļ��Ȼ���
SD_LINEAR_WRAP_SAMPLE(1, sdBaseNormalSampler, 	sdBaseNormalTex, 	false);		// ���λ���������ͼ
SD_LINEAR_WRAP_SAMPLE(2, sdBlendSampler,		sdBlendTex,			false);		// ���λ��Ȩ����ͼ

SD_LINEAR_WRAP_SAMPLE( 3, sdNormalSampler00, sdNormalTex00, false)
SD_LINEAR_WRAP_SAMPLE( 4, sdNormalSampler01, sdNormalTex01, false)
SD_LINEAR_WRAP_SAMPLE( 5, sdNormalSampler02, sdNormalTex02, false)
SD_LINEAR_WRAP_SAMPLE( 6, sdNormalSampler03, sdNormalTex03, false)
SD_LINEAR_WRAP_SAMPLE( 7, sdNormalSampler04, sdNormalTex04, false)
SD_LINEAR_WRAP_SAMPLE( 8, sdNormalSampler05, sdNormalTex05, false)
SD_LINEAR_WRAP_SAMPLE( 9, sdNormalSampler06, sdNormalTex06, false)
SD_LINEAR_WRAP_SAMPLE(10, sdNormalSampler07, sdNormalTex07, false)
SD_LINEAR_WRAP_SAMPLE(11, sdNormalSampler08, sdNormalTex08, false)
SD_LINEAR_WRAP_SAMPLE(12, sdNormalSampler09, sdNormalTex09, false)
SD_LINEAR_WRAP_SAMPLE(13, sdNormalSampler10, sdNormalTex10, false)
SD_LINEAR_WRAP_SAMPLE(14, sdNormalSampler11, sdNormalTex11, false)

//---------------------------------------------------------------------------------------
// ������ɫ������������
//---------------------------------------------------------------------------------------
struct VS_INPUT
{
	float3	vPos			: POSITION0;	// ��Ļ���ζ���
	float2	vUVSet0			: TEXCOORD0;	// ��Ļ������������
};

//---------------------------------------------------------------------------------------
// ������ɫ�����������
//---------------------------------------------------------------------------------------
struct VS_OUTPUT
{
	float4	vProjPos			: POSITION;		// ͶӰ����
	float2	vUVSetScreenTex		: TEXCOORD0;	// ����������ƫ�Ƶ���Ļ��������
	float3	vUVNearClipWorldPos	: TEXCOORD1;	// ��ǰ���Ӧ���ü����ϵĵ����������
	float3	vUVFarClipWorldPos	: TEXCOORD2;	// ��ǰ���ӦԶ�ü����ϵĵ����������
};

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
VS_OUTPUT VS_Main(VS_INPUT kInput)
{
	VS_OUTPUT kOutput;

	// ͶӰ����
	kOutput.vProjPos = float4(kInput.vPos, 1.f);
	
	// ����������ƫ�Ƶ���Ļ��������
	kOutput.vUVSetScreenTex = kInput.vUVSet0 + g_vHalfPixelOffset;
	
	// ��ǰ���Ӧ��Զ�ü����ϵĵ����������
	kOutput.vUVNearClipWorldPos = mul(float4(kInput.vPos.xy, 0.f, 1.f), g_mDepthToWorld).xyz;
	kOutput.vUVFarClipWorldPos = mul(float4(kInput.vPos.xy, 1.f, 1.f), g_mDepthToWorld).xyz;
	
	return kOutput;
}

//---------------------------------------------------------------------------------------
// ������ɫ��
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
// ��ȾԶ��,ֻ��Ⱦ����������ͼ
float4 PS_Main_Far_BaseNormal(VS_OUTPUT kInput) : COLOR0
{
	// ��ȡ�������
	float2 vPackedDepth = tex2D(sdDepthSampler, kInput.vUVSetScreenTex).xy;
	float fDepth = UnpackDepth(vPackedDepth.xy);
	
	// ������������(�����������,�����λ�ú�Զƽ���Ӧ��λ�ý��в�ֵ)
	float3 vWorldPos = lerp(kInput.vUVNearClipWorldPos, kInput.vUVFarClipWorldPos, fDepth);
	
	// �ü���ָ����ƽ�����ڵ�����
	clip(length(vWorldPos - kInput.vUVNearClipWorldPos) - g_fTerrainFarStart);
	
	// ���㵱ǰ��ĵ������UV(ע��,����û��ƫ��)
	float2 vUVSet = vWorldPos.xy * g_vRecipTerrainSize;
	
	
	// BaseNormalMap
	// @{
	// ����UV����BaseNormalMap(ע��,����û��ƫ�ư�����,��ΪBaseNormalMap��Linear������)
	float4 vBaseNormalTex = tex2D(sdBaseNormalSampler, vUVSet);
	
	// �������ռ䷨��
	float3 vWorldNormal;
	vWorldNormal.xy	= vBaseNormalTex.xy * 2.f - 1.f;
	vWorldNormal.z 	= sqrt(dot(float3(1.f, vWorldNormal.xy), float3(1.f, -vWorldNormal.xy)));
	
	// �任Normal���۲�ռ�
	// 	1.��Ҫ������ת�þ���,
	//	2.ViewMatrix��ת��������������,ƽ�Ʋ��ֲ���,����ֻ��Ҫ��ת�任
	//	3.g_mView��ת����(3x3)������������ת�þ��������Լ�
	float3 vViewNormal = mul(float4(vWorldNormal.xyz, 0.f), g_mView).xyz;
	// @}
	
	
	// �������ķ��ߺ����
	return float4(vPackedDepth, PackNormal(vViewNormal));
}

//---------------------------------------------------------------------------------------
// ��Ⱦ����,��Ⱦ�������ߺͷ�����ͼ, XYƽ��
float4 PS_Main_Near_BaseNormalAndNormalMap_XY(VS_OUTPUT kInput) : COLOR0
{
	// ��ȡ�������
	float2 vPackedDepth = tex2D(sdDepthSampler, kInput.vUVSetScreenTex).xy;
	float fDepth = UnpackDepth(vPackedDepth.xy);
	
	// ������������(�����������,�����λ�ú�Զƽ���Ӧ��λ�ý��в�ֵ)
	float3 vWorldPos = lerp(kInput.vUVNearClipWorldPos, kInput.vUVFarClipWorldPos, fDepth);
	
	// ���㵱ǰ��ĵ������UV(ע��,����û��ƫ��)
	float2 vUVSet = vWorldPos.xy * g_vRecipTerrainSize;
	
	
	// BaseNormalMap
	// @{
	// ����UV����BaseNormalMap(ע��,����û��ƫ�ư�����,��ΪBaseNormalMap��Linear������)
	float4 vBaseNormalTex = tex2D(sdBaseNormalSampler, vUVSet);
	
	// �����б���
	float3 vPlanarWeight;
	vPlanarWeight.xy 	= vBaseNormalTex.zw;
	vPlanarWeight.z 	= saturate(1.f - vPlanarWeight.x - vPlanarWeight.y);	
	
	clip(vPlanarWeight.z - 0.001f);
	
	// �������ռ䷨��
	float3 vWorldNormal;
	vWorldNormal.xy	= vBaseNormalTex.xy * 2.f - 1.f;
	vWorldNormal.z 	= sqrt(dot(float3(1.f, vWorldNormal.xy), float3(1.f, -vWorldNormal.xy)));
	
	// ���㵱ǰ������߿ռ�
	float3 vWorldBinormal 	= cross(float3(1.f, 0.f, 0.f), vWorldNormal);
	float3 vWorldTangent 	= cross(vWorldNormal, vWorldBinormal);
	// @}
	
	
	// BlendMap
	// @{
	// ����UV
	float2 vUVSet1 = vUVSet * 0.5f;
	
	// ����BlendMap
	float4 vBlendWeight00 = tex2D(sdBlendSampler, vUVSet1);
	float4 vBlendWeight01 = tex2D(sdBlendSampler, vUVSet1 + float2(0.5f, 0.f));
	float4 vBlendWeight02 = tex2D(sdBlendSampler, vUVSet1 + float2(0.f,  0.5f));
	// @}
	
	
	// NormalMap
	// @{
	// ����UV
	float2 vUVSet2 = float2(vWorldPos.x, -vWorldPos.y);
	
	// ����NormalMap
	float3 vNormal = tex2D(sdNormalSampler00, vUVSet2 * g_vRecipUVRepeats[0].x) * vBlendWeight00.a;
	vNormal += tex2D(sdNormalSampler01, vUVSet2 * g_vRecipUVRepeats[0].y) * vBlendWeight00.r;
	vNormal += tex2D(sdNormalSampler02, vUVSet2 * g_vRecipUVRepeats[0].z) * vBlendWeight00.g;
	vNormal += tex2D(sdNormalSampler03, vUVSet2 * g_vRecipUVRepeats[0].w) * vBlendWeight00.b;
	vNormal += tex2D(sdNormalSampler04, vUVSet2 * g_vRecipUVRepeats[1].x) * vBlendWeight01.a;
	vNormal += tex2D(sdNormalSampler05, vUVSet2 * g_vRecipUVRepeats[1].y) * vBlendWeight01.r;
	vNormal += tex2D(sdNormalSampler06, vUVSet2 * g_vRecipUVRepeats[1].z) * vBlendWeight01.g;
	vNormal += tex2D(sdNormalSampler07, vUVSet2 * g_vRecipUVRepeats[1].w) * vBlendWeight01.b;
	vNormal += tex2D(sdNormalSampler08, vUVSet2 * g_vRecipUVRepeats[2].x) * vBlendWeight02.a;
	vNormal += tex2D(sdNormalSampler09, vUVSet2 * g_vRecipUVRepeats[2].y) * vBlendWeight02.r;
	vNormal += tex2D(sdNormalSampler10, vUVSet2 * g_vRecipUVRepeats[2].z) * vBlendWeight02.g;
	vNormal += tex2D(sdNormalSampler11, vUVSet2 * g_vRecipUVRepeats[2].w) * vBlendWeight02.b;
	
	vNormal = vNormal * 2.f - 1.f;
	
	// ��һ��
	//vNormal = normalize(vNormal);
	vNormal.z = sqrt(dot(float3(1.f, vNormal.xy), float3(1.f, -vNormal.xy)));
	
	// DetailNormal�����߿ռ�ת������������(http://www.terathon.com/code/tangent.html)
	//	|Tx Ty Tz|   |X|
	//	|Bx By Bz| * |Y|
	//  |Nx Ny Nz|   |Z|
	vNormal = vNormal.z * vWorldNormal + vNormal.y * vWorldBinormal + vNormal.x * vWorldTangent;
	
	// �任Normal���۲�ռ�
	// 	1.��Ҫ������ת�þ���,
	//	2.ViewMatrix��ת��������������,ƽ�Ʋ��ֲ���,����ֻ��Ҫ��ת�任
	//	3.g_mView��ת����(3x3)������������ת�þ��������Լ�
	float3 vViewNormal = mul(float4(vWorldNormal.xyz, 0.f), g_mView).xyz;
	// @}
	
	return float4(vPackedDepth, PackNormal(vViewNormal) * vPlanarWeight.z);
}

//---------------------------------------------------------------------------------------
// ��Ⱦ����,��Ⱦ�������ߺͷ�����ͼ, YZƽ��
float4 PS_Main_Near_BaseNormalAndNormalMap_YZ(VS_OUTPUT kInput) : COLOR0
{
	// ��ȡ�������
	float2 vPackedDepth = tex2D(sdDepthSampler, kInput.vUVSetScreenTex).xy;
	float fDepth = UnpackDepth(vPackedDepth.xy);
	
	// ������������(�����������,�����λ�ú�Զƽ���Ӧ��λ�ý��в�ֵ)
	float3 vWorldPos = lerp(kInput.vUVNearClipWorldPos, kInput.vUVFarClipWorldPos, fDepth);
	
	// ���㵱ǰ��ĵ������UV(ע��,����û��ƫ��)
	float2 vUVSet = vWorldPos.xy * g_vRecipTerrainSize;
	
	
	// BaseNormalMap
	// @{
	// ����UV����BaseNormalMap(ע��,����û��ƫ�ư�����,��ΪBaseNormalMap��Linear������)
	float4 vBaseNormalTex = tex2D(sdBaseNormalSampler, vUVSet);
	
	// �����б���
	float3 vPlanarWeight;
	vPlanarWeight.xy 	= vBaseNormalTex.zw;
	vPlanarWeight.z 	= saturate(1.f - vPlanarWeight.x - vPlanarWeight.y);	
	
	clip(vPlanarWeight.x - 0.001f);
	
	// �������ռ䷨��
	float3 vWorldNormal;
	vWorldNormal.xy	= vBaseNormalTex.xy * 2.f - 1.f;
	vWorldNormal.z 	= sqrt(dot(float3(1.f, vWorldNormal.xy), float3(1.f, -vWorldNormal.xy)));
	
	// ���㵱ǰ������߿ռ�
	float3 vWorldBinormal 	= cross(float3(0.f, sign(vWorldNormal.x), 0.f), vWorldNormal);
	float3 vWorldTangent 	= cross(vWorldNormal, vWorldBinormal);
	// @}
	
	
	// BlendMap
	// @{
	// ����UV
	float2 vUVSet1 = vUVSet * 0.5f;
	
	// ����BlendMap
	float4 vBlendWeight00 = tex2D(sdBlendSampler, vUVSet1);
	float4 vBlendWeight01 = tex2D(sdBlendSampler, vUVSet1 + float2(0.5f, 0.f));
	float4 vBlendWeight02 = tex2D(sdBlendSampler, vUVSet1 + float2(0.f,  0.5f));
	// @}
	
	
	// NormalMap
	// @{
	// ����UV
	float2 vUVSet2 = float2(vWorldPos.y * sign(vWorldNormal.x), -vWorldPos.z);
	
	// ����NormalMap
	float3 vNormal = tex2D(sdNormalSampler00, vUVSet2 * g_vRecipUVRepeats[0].x) * vBlendWeight00.a;
	vNormal += tex2D(sdNormalSampler01, vUVSet2 * g_vRecipUVRepeats[0].y) * vBlendWeight00.r;
	vNormal += tex2D(sdNormalSampler02, vUVSet2 * g_vRecipUVRepeats[0].z) * vBlendWeight00.g;
	vNormal += tex2D(sdNormalSampler03, vUVSet2 * g_vRecipUVRepeats[0].w) * vBlendWeight00.b;
	vNormal += tex2D(sdNormalSampler04, vUVSet2 * g_vRecipUVRepeats[1].x) * vBlendWeight01.a;
	vNormal += tex2D(sdNormalSampler05, vUVSet2 * g_vRecipUVRepeats[1].y) * vBlendWeight01.r;
	vNormal += tex2D(sdNormalSampler06, vUVSet2 * g_vRecipUVRepeats[1].z) * vBlendWeight01.g;
	vNormal += tex2D(sdNormalSampler07, vUVSet2 * g_vRecipUVRepeats[1].w) * vBlendWeight01.b;
	vNormal += tex2D(sdNormalSampler08, vUVSet2 * g_vRecipUVRepeats[2].x) * vBlendWeight02.a;
	vNormal += tex2D(sdNormalSampler09, vUVSet2 * g_vRecipUVRepeats[2].y) * vBlendWeight02.r;
	vNormal += tex2D(sdNormalSampler10, vUVSet2 * g_vRecipUVRepeats[2].z) * vBlendWeight02.g;
	vNormal += tex2D(sdNormalSampler11, vUVSet2 * g_vRecipUVRepeats[2].w) * vBlendWeight02.b;
	
	// �任DetailNormal(��[0,1]��[-1,1])
	vNormal = vNormal * 2.f - 1.f;
	
	// ��һ��
	//vNormal = normalize(vNormal);
	vNormal.z = sqrt(dot(float3(1.f, vNormal.xy), float3(1.f, -vNormal.xy)));
	
	// DetailNormal�����߿ռ�ת������������(http://www.terathon.com/code/tangent.html)
	//	|Tx Ty Tz|   |X|
	//	|Bx By Bz| * |Y|
	//  |Nx Ny Nz|   |Z|
	vNormal = vNormal.z * vWorldNormal.xyz + vNormal.y * vWorldBinormal + vNormal.x * vWorldTangent;
	
	// �任Normal���۲�ռ�
	// 	1.��Ҫ������ת�þ���,
	//	2.ViewMatrix��ת��������������,ƽ�Ʋ��ֲ���,����ֻ��Ҫ��ת�任
	//	3.g_mView��ת����(3x3)������������ת�þ��������Լ�
	float3 vViewNormal = mul(float4(vWorldNormal.xyz, 0.f), g_mView).xyz;
	// @}
	
	return float4(vPackedDepth * step(vPlanarWeight.z, 0.001f), PackNormal(vViewNormal) * vPlanarWeight.x);
}

//---------------------------------------------------------------------------------------
// ��Ⱦ����,��Ⱦ�������ߺͷ�����ͼ, XZƽ��
float4 PS_Main_Near_BaseNormalAndNormalMap_XZ(VS_OUTPUT kInput) : COLOR0
{
	// ��ȡ�������
	float2 vPackedDepth = tex2D(sdDepthSampler, kInput.vUVSetScreenTex).xy;
	float fDepth = UnpackDepth(vPackedDepth.xy);
	
	// ������������(�����������,�����λ�ú�Զƽ���Ӧ��λ�ý��в�ֵ)
	float3 vWorldPos = lerp(kInput.vUVNearClipWorldPos, kInput.vUVFarClipWorldPos, fDepth);
	
	// ���㵱ǰ��ĵ������UV(ע��,����û��ƫ��)
	float2 vUVSet = vWorldPos.xy * g_vRecipTerrainSize;
	
	
	// BaseNormalMap
	// @{
	// ����UV����BaseNormalMap(ע��,����û��ƫ�ư�����,��ΪBaseNormalMap��Linear������)
	float4 vBaseNormalTex = tex2D(sdBaseNormalSampler, vUVSet);
	
	// �����б���
	float3 vPlanarWeight;
	vPlanarWeight.xy 	= vBaseNormalTex.zw;
	vPlanarWeight.z 	= saturate(1.f - vPlanarWeight.x - vPlanarWeight.y);	
	
	clip(vPlanarWeight.y - 0.001f);
	
	// �������ռ䷨��
	float3 vWorldNormal;
	vWorldNormal.xy	= vBaseNormalTex.xy * 2.f - 1.f;
	vWorldNormal.z 	= sqrt(dot(float3(1.f, vWorldNormal.xy), float3(1.f, -vWorldNormal.xy)));
	
	// ���㵱ǰ������߿ռ�
	float3 vWorldBinormal 	= cross(float3(-sign(vWorldNormal.y), 0.f, 0.f), vWorldNormal);
	float3 vWorldTangent 	= cross(vWorldNormal, vWorldBinormal);
	// @}
	
	
	// BlendMap
	// @{
	// ����UV
	float2 vUVSet1 = vUVSet * 0.5f;
	
	// ����BlendMap
	float4 vBlendWeight00 = tex2D(sdBlendSampler, vUVSet1);
	float4 vBlendWeight01 = tex2D(sdBlendSampler, vUVSet1 + float2(0.5f, 0.f));
	float4 vBlendWeight02 = tex2D(sdBlendSampler, vUVSet1 + float2(0.f,  0.5f));
	// @}
	
	
	// NormalMap
	// @{
	// ����UV
	float2 vUVSet2 = float2(-vWorldPos.x * sign(vWorldNormal.y), -vWorldPos.z);
	
	// ����NormalMap
	float3 vNormal = tex2D(sdNormalSampler00, vUVSet2 * g_vRecipUVRepeats[0].x) * vBlendWeight00.a;
	vNormal += tex2D(sdNormalSampler01, vUVSet2 * g_vRecipUVRepeats[0].y) * vBlendWeight00.r;
	vNormal += tex2D(sdNormalSampler02, vUVSet2 * g_vRecipUVRepeats[0].z) * vBlendWeight00.g;
	vNormal += tex2D(sdNormalSampler03, vUVSet2 * g_vRecipUVRepeats[0].w) * vBlendWeight00.b;
	vNormal += tex2D(sdNormalSampler04, vUVSet2 * g_vRecipUVRepeats[1].x) * vBlendWeight01.a;
	vNormal += tex2D(sdNormalSampler05, vUVSet2 * g_vRecipUVRepeats[1].y) * vBlendWeight01.r;
	vNormal += tex2D(sdNormalSampler06, vUVSet2 * g_vRecipUVRepeats[1].z) * vBlendWeight01.g;
	vNormal += tex2D(sdNormalSampler07, vUVSet2 * g_vRecipUVRepeats[1].w) * vBlendWeight01.b;
	vNormal += tex2D(sdNormalSampler08, vUVSet2 * g_vRecipUVRepeats[2].x) * vBlendWeight02.a;
	vNormal += tex2D(sdNormalSampler09, vUVSet2 * g_vRecipUVRepeats[2].y) * vBlendWeight02.r;
	vNormal += tex2D(sdNormalSampler10, vUVSet2 * g_vRecipUVRepeats[2].z) * vBlendWeight02.g;
	vNormal += tex2D(sdNormalSampler11, vUVSet2 * g_vRecipUVRepeats[2].w) * vBlendWeight02.b;
	
	// �任DetailNormal(��[0,1]��[-1,1])
	vNormal = vNormal * 2.f - 1.f;
	
	// ��һ��
	//vNormal = normalize(vNormal);
	vNormal.z = sqrt(dot(float3(1.f, vNormal.xy), float3(1.f, -vNormal.xy)));
	
	// DetailNormal�����߿ռ�ת������������(http://www.terathon.com/code/tangent.html)
	//	|Tx Ty Tz|   |X|
	//	|Bx By Bz| * |Y|
	//  |Nx Ny Nz|   |Z|
	vNormal = vNormal.z * vWorldNormal.xyz + vNormal.y * vWorldBinormal + vNormal.x * vWorldTangent;
	
	// �任Normal���۲�ռ�
	// 	1.��Ҫ������ת�þ���,
	//	2.ViewMatrix��ת��������������,ƽ�Ʋ��ֲ���,����ֻ��Ҫ��ת�任
	//	3.g_mView��ת����(3x3)������������ת�þ��������Լ�
	float3 vViewNormal = mul(float4(vWorldNormal.xyz, 0.f), g_mView).xyz;
	// @}
	
	float2 vOutputDepth = step(vPlanarWeight.xz, 0.001f);
	return float4(vPackedDepth * vOutputDepth.x * vOutputDepth.y, PackNormal(vViewNormal) * vPlanarWeight.y);
}

//---------------------------------------------------------------------------------------
// ��ɫ����
//---------------------------------------------------------------------------------------
//
//---------------------------------------------------------------------------------------
technique Terrain_MultiPassGeometry_Far_BaseNormal_Ortho
<
	string Description = "Terrain_MultiPassGeometry_Far_BaseNormal_Ortho";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main ();
		PixelShader 	= compile ps_2_a PS_Main_Far_BaseNormal ();
	}
}
//---------------------------------------------------------------------------------------
technique Terrain_MultiPassGeometry_Near_BaseNormalAndNormalMap_XY_Ortho
<
	string Description = "Terrain_MultiPassGeometry_Near_BaseNormalAndNormalMap_XY_Ortho";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main ();
		PixelShader 	= compile ps_2_a PS_Main_Near_BaseNormalAndNormalMap_XY ();
	}
}
//---------------------------------------------------------------------------------------
technique Terrain_MultiPassGeometry_Near_BaseNormalAndNormalMap_YZ_Ortho
<
	string Description = "Terrain_MultiPassGeometry_Near_BaseNormalAndNormalMap_YZ_Ortho";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main ();
		PixelShader 	= compile ps_2_a PS_Main_Near_BaseNormalAndNormalMap_YZ ();
	}
}
//---------------------------------------------------------------------------------------
technique Terrain_MultiPassGeometry_Near_BaseNormalAndNormalMap_XZ_Ortho
<
	string Description = "Terrain_MultiPassGeometry_Near_BaseNormalAndNormalMap_XZ_Ortho";
	bool UsesNiRenderState = false;
	bool UsesNiLightState = false;
>
{
	pass P0
	{
		VertexShader 	= compile vs_2_a VS_Main ();
		PixelShader 	= compile ps_2_a PS_Main_Near_BaseNormalAndNormalMap_XZ ();
	}
}
//---------------------------------------------------------------------------------------