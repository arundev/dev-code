//*************************************************************************************************
// 内容:	地表高度图
//
// 参考网址:
//		https://en.wikipedia.org/wiki/Natural_neighbor_interpolation
//		https://en.wikipedia.org/wiki/Bilinear_interpolation
//		https://en.wikipedia.org/wiki/Bicubic_interpolation
//		https://en.wikipedia.org/wiki/Cubic_Hermite_spline
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-01
// 最后修改:	2013-05-06
//*************************************************************************************************
#pragma once
#ifndef _TERRAIN_HEIGHT_MAP_H__
#define _TERRAIN_HEIGHT_MAP_H__

#include "sdCubicSampler.h"

class sdQuadMesh;

//
// 地表高度图,
//	1.必须是正方形,大小必须是pow(2,n)个单位
//	2.GB默认世界坐标系,向右为X,向前为Y,向上为Z
//	3.默认高度图从(0,0,0)点开始向XY正方向延伸
//	4.这里只处理了uiSpacing为1的情况
//
// 注释：
//	1.WZ的默认尺寸是pow(2,n) + 1 + 2， 
//		pow(2,n)	是地块大小
//		1			是多出的顶点
//		2			是两边边缘向外延伸的两个裙边
//	2.WZ的默认索引是[-1, pow(2,n)]
//
// 注意：
//	1.目前不支持m_uiSpacing非1的情况
//
// \TODO 创建预过滤的低分辨率高度图,用于创建LOD
//
class sdHeightMap : public NiRefObject
{
	friend sdQuadMesh;
public:
	sdHeightMap(uint uiSize);
	sdHeightMap(uint uiSize, float fMinHeight, float fMaxHeight);
	~sdHeightMap();

	// 属性访问
	uint	GetSpacing() const { return m_uiSpacing;}
	uint	GetSize() const { return m_uiSize;};
	float	GetMinHeight() const { return m_fMinHeight;}
	float	GetMaxHeight() const { return m_fMaxHeight;}
	float	GetAlignHeight() const { return m_fAlignHeight;}

	// 高度(高度图像素坐标，有效范围[0, m_uiSize],原始高度)
	//	0.使用int而不是uint是为了便于处理向下溢出
	//	1.越出边界的读取则读取对应边界点
	//	2.越出边界的写入不处理
	void	SetRawHeight(int iX, int iY, float fHeight);
	float	GetRawHeight(int iX, int iY);

	// 高度(世界坐标整点坐标点处,插值高度)
	float	GetHeight(int iX, int iY);

	// 高度(世界坐标任意位置,插值高度)
	//	1.按照图形渲染的方式分两块三角形,在三角形内部进行插值
	//	2.双线性插值(2x2)
	float	GetHeight(float fX, float fY);
	float	GetHeight_Bilinear(float fX, float fY);

	// 法线(世界坐标整点坐标点处,平均法线)
	void	GetNormal(int iX, int iY, float& fNX, float& fNY, float& fNZ);

	// 高度(世界坐标整点坐标点处,插值坡度, 个人觉得与Normal计算不一致)
	void	GetGradients(int iX, int iY, float& fGX, float& fGY);

	// 扫描精细级网格的点，获取次级网格与精细级网格在指定坐标处误差值(相差一级)
	// (为了提高效率,这里没有检查输入参数)
	//	@param[in]	iStartX,iStartY	网格相对高度图的起始采样位置
	//	@param[in]	iX,iY			精细级网格的像素坐标[0, m_uiSize * m_uiSpacing / uiSpacing]
	//	@param[in]	uiSpacing		精细级网格的网格大小,应该满足power(2,n),即1/2/4/8/16/32
	float	GetMorphDelta(int iStartX, int iStartY, int iX, int iY, uint uiSpacing);
	//float	GetMorphDelta(int iStartX, int iStartY, int iWidth, int iHeight, uint uiSpacing);

protected:
	//NiPoint3	m_kOrigin;			// 高度图起始位置
	//float		m_fScale;			// 高度图整体缩放
	//float		m_fHorizontalScale;	// 水平缩放
	//float		m_fVerticalScale;	// 垂直缩放,基高比(显示时高度夸张比例)
	//float		m_fBaseHeight;		// 高度图基准高度

	// 高度图数据(这是精确格式,可以考虑压缩为unsigned short)
	float	*m_pfHeight;

	// 高度图顶点间距(Cry有此参数,一般是1/2/4/8/16)
	uint	m_uiSpacing;
	float	m_fSpacingDiv;	///< 顶点间距倒数

	// 高度图边长大小
	uint	m_uiSize;		// pow(2,n)
	uint	m_uiAllocSize;	// pow(2,n) + 1

	// 最大与最小高度,压缩数据时使用
	float	m_fMinHeight;
	float	m_fMaxHeight;

	// 高度对齐单位值,即顶点高度的最小改变高度
	// (有时渲染器可能需要压缩地形数据而使用定点浮点数)
	float	m_fAlignHeight;

	// Cubic采样器，用于在iSpacing不为1时采样单位位置的高度
	static sdCubicSampler ms_kCubicSampler;
};
NiSmartPointer(sdHeightMap);
#endif