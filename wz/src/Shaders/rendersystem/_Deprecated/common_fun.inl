//*************************************************************************************************
// 内容:	公共头文件(已废弃)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-20
// 最后修改:	2013-05-02
//*************************************************************************************************
#include "../common.h"

//---------------------------------------------------------------------------------------
// 直线和球的最近交点(???)
//---------------------------------------------------------------------------------------
float GetNearIntersection(float3 vPos, float3 vRay, float fDistance, float fRadius)
{
	float B = 2.f * dot(vPos, vRay);
	float C = fDistance - fRadius;
	float fDet = max(0.f, B * B - 4.f * C);
	return 0.5f * (-B - sqrt(fDet));
}
//---------------------------------------------------------------------------------------
// 直线和球的最远交点(???)
//---------------------------------------------------------------------------------------
float GetFarIntersection(float3 vPos, float3 vRay, float fDistance, float fRadius)
{
	float B = 2.f * dot(vPos, vRay);
	float C = fDistance - fRadius;
	float fDet = max(0.f, B * B - 4.f * C);
	return 0.5f * (-B + sqrt(fDet));
}
//---------------------------------------------------------------------------------------
// (???)
//---------------------------------------------------------------------------------------
float4 QuadCross(float w1, float3 v1, float w2, float3 v2)
{
	float4 q;
	q.x = w1 * w2 - dot(v1, v2);
	q.yzw = w1 * v2 + w2 * v1 + cross(v1, v1);
	return q;
}
//---------------------------------------------------------------------------------------
// 写的真浪费,考虑sincos函数
//---------------------------------------------------------------------------------------
float2 RotateFloat2InPlane(float angle, float2 v)
{
	float2 vv;
	vv.x = v.x * cos(angle) - v.y * sin(angle);
	vv.y = v.x * sin(angle) + v.y * cos(angle);
	return vv;
}
//---------------------------------------------------------------------------------------
// 欧拉角旋转矩阵
//---------------------------------------------------------------------------------------
float3x3 GetXRotMatrix(float sn, float cs)
{
	float3x3 tmp;
	tmp[0][0] = 1.f; 	tmp[0][1] = 0.f;	tmp[0][2] = 0.f;
	tmp[1][0] = 0.f; 	tmp[1][1] = cs;		tmp[1][2] = sn;
	tmp[1][0] = 0.f; 	tmp[2][1] = -sn;	tmp[2][2] = cs;
}
//---------------------------------------------------------------------------------------
float3x3 GetYRotMatrix(float sn, float cs)
{
	float3x3 tmp;
	tmp[0][0] = cs; 	tmp[0][1] = 0.f;	tmp[0][2] = -sn;
	tmp[1][0] = 0.f; 	tmp[1][1] = 1.f;	tmp[1][2] = 0.f;
	tmp[1][0] = sn; 	tmp[2][1] = 0.f;	tmp[2][2] = cs;
}
//---------------------------------------------------------------------------------------
float3x3 GetZRotMatrix(float sn, float cs)
{
	float3x3 tmp;
	tmp[0][0] = cs; 	tmp[0][1] = sn;		tmp[0][2] = 0.f;
	tmp[1][0] = -sn; 	tmp[1][1] = cs;		tmp[1][2] = 0.f;
	tmp[1][0] = 0.f; 	tmp[2][1] = 0.f;	tmp[2][2] = 1.f;
}
//---------------------------------------------------------------------------------------
float3x3 GetMatrixFromSC(float3 sn, float3 cs)
{
	float3x3 mx = GetXRotMatrix(sn.x, cs.x);
	float3x3 my = GetYRotMatrix(sn.y, cs.y);
	float3x3 mz = GetZRotMatrix(sn.z, cs.z);
	return mul(mz, mul(my, mx));
}
//---------------------------------------------------------------------------------------
// 格式转换(depth32-->r8g8b8):
//	1.将值右移8位，取整数作为.x(前8位)
//	2.取小数部分左移8位,取整数作为.y(中间8位)
//	3.取小数部分作为.z(后8位)
//
// 类似于(floor取值为[0,1]):
// 	.r = floor(depth * 256) / 256 									
//	.g = floor((depth - .r) * 256 * 256) / 256
//	.b = floor((fValue - .r - .g / 256) * 256 * 256 * 256) / 256
//
// 	depth = .r + .g / 256 + .b / (256 * 256)
//
// 	1.前八位左移8位取整,再右移8位规格化到[0,1]区间内
//	2.中间8位左移16位取整,再右移8位规格化到[0,1]区间内
//	3.后8位左移24位取整,再右移8位规格化到[0,1]区间内
//
// 参考:
//	http://aras-p.info/blog/2007/06/29/encoding-floats-to-rgba-redux/
//	http://aras-p.info/blog/2008/06/20/encoding-floats-to-rgba-again/
//	http://aras-p.info/blog/2009/07/30/encoding-floats-to-rgba-the-final/
// 	http://stackoverflow.com/questions/6893302/decode-rgb-value-to-single-float-without-bit-shift-in-glsl
//---------------------------------------------------------------------------------------
float4 EncodeFloat2RGBA(float fDepth)
{
	float4 vUnpackInput;
	vUnpackInput.z = modf(fDepth / 256.f, vUnpackInput.x);
	vUnpackInput.z = modf(vUnpackInput.z * 256.f, vUnpackInput.y);
	return float4(vUnpackInput.x / 256.f, vUnpackInput.y / 256.f, vUnpackInput.z, 1.f);
}
//---------------------------------------------------------------------------------------
float DecodeRGBA2Float(float4 vDepth)
{
	return vDepth.x * 65536.f + vDepth.y * 256.f + vDepth.z;
}
//---------------------------------------------------------------------------------------
float4 EncodeFloatRGBA_v2(float fDepth) 
{
	float4 enc = float4(1.0, 255.0, 65025.0, 160581375.0) * fDepth;
	enc = frac(enc);
	enc -= enc.yzww * float4(1.0/255.0, 1.0/255.0, 1.0/255.0, 0.0);
	return enc;
}	
//---------------------------------------------------------------------------------------
float DecodeFloatRGBA_v2(float4 vDepth) 
{
  return dot(vDepth, float4(1.0, 1/255.0, 1/65025.0, 1/160581375.0));
}
//---------------------------------------------------------------------------------------
// 万用纹理坐标生成
// 	@param[in] fTexIndex	纹理序列帧索引
// 	@param[in] w，h			纹理尺寸
//	@param[in] Pos			纹理坐标
//	@param[in] op		
//---------------------------------------------------------------------------------------
float2 GetUVFromPos(float fTexIndex, float w, float h, float2 Pos, float2 op)
{
	float2 vTmp;
	float2 vPos = Pos * op;
	
	if (vPos.x == -1.f && vPos.y == 1.f)
	{
		vTmp.x = fmod(fTexIndex, w) * (1.f / w);
		vTmp.y = floor(fTexIndex / h) * (1.f / h);		
	}
	
	if (vPos.x == 1.f && vPos.y == 1.f)
	{
		vTmp.x = (fmod(fTexIndex, w) + 1.f) * (1.f / w);
		vTmp.y = floor(fTexIndex / h) * (1.f / h);		
	}
	
	if (vPos.x == -1.f && vPos.y == -1.f)
	{
		vTmp.x = fmod(fTexIndex, w) * (1.f / w);
		vTmp.y = (floor(fTexIndex / h) + 1.f) * (1.f / h);		
	}
	
	if (vPos.x == 1.f && vPos.y == -1.f)
	{
		vTmp.x = (fmod(fTexIndex, w) + 1.f) * (1.f / w);
		vTmp.y = (floor(fTexIndex / h) + 1.f) * (1.f / h);		
	}
	
	return vTmp;
}
//---------------------------------------------------------------------------------------
// 根据时间流逝,得到动画索引
//	@param[in]	fTimePoint	相对时间
//	@param[in]	fAniTime	序列帧时长
//	@param[in]	fFrameNum	帧数
//	return					相对时间对应的动画帧索引
//---------------------------------------------------------------------------------------
float GetIndexFromTimePoint(float fTimePoint, float fAniTime, float fFrameNum)
{
		return floor(fTimePoint / (fAniTime / fFrameNum));
}
//---------------------------------------------------------------------------------------
// 弥散射相位函数
//
// 参考:
//	GPU GEM2: http://http.developer.nvidia.com/GPUGems2/gpugems2_chapter16.html
//---------------------------------------------------------------------------------------
float GetMiePhase(float fCos, float fCos2, float g, float g2)
{
	return 1.5f * ((1.f - g2) / (2.f + g2)) * (1.f + fCos2) / pow(1.f + g2 - 2.f * g * fCos, 1.5f);
}
//---------------------------------------------------------------------------------------
// 瑞利散射相位函数
//---------------------------------------------------------------------------------------
float GetRayleighPhase(float fCos2)
{
	return 1.f;
	//return 0.95f + 0.95f * fCos2
}
//---------------------------------------------------------------------------------------
// 雾
//---------------------------------------------------------------------------------------
float CalcFogFactors(float3 vViewPos)
{
	return 0.f;
}
//---------------------------------------------------------------------------------------