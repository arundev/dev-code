//*************************************************************************************************
// 内容:	公共头文件(已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-22
// 最后修改:	2013-05-02
//*************************************************************************************************

// MRT
#define MRT	0

//*****************************************************************************
// 全局雾
//*****************************************************************************
//
//-----------------------------------------------------------------------------
// 系数
float	g_fFogDensity	: GLOBAL = 1.f;
float	g_fFogNear		: GLOBAL = 50.f;
float	g_fFogFar		: GLOBAL = 200.f;
float3	g_vFogColor		: GLOBAL = { 0.5f, 0.5f, 0.5f};

//-----------------------------------------------------------------------------
void CalculateFog(float3 vViewPosition, out float fFog)
{
	float fDistance = length(vViewPosition);
	float fRange = g_fFogFar - g_fFogNear;
	if (fDistance < g_fFogNear || fDistance < 1.f)
	{
		fFog = 0.f;
	}
	else
	{
		float fVal = (fDistance - g_fFogNear) / g_fFogFar;
		fFog = clamp((fVal * g_fFogDensity), 0.f, g_fFogDensity)
	}
}
//-----------------------------------------------------------------------------
void ApplayFog(float3 vUnfoggedColor, float3 vFogColor, float fFogAmount, out float3 vFoggedColor)
{
	vFoggedColor = lerp(vUnfoggedColor, vFogColor, fFogAmount);
}
//*****************************************************************************