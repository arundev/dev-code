//*************************************************************************************************
// 内容:	编辑辅助类
//---------------------------------------------------------
// 作者:		
// 创建:		
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMEEDITEX_EDITHELPER_H__
#define _SD_GAMEEDITEX_EDITHELPER_H__

class sdMap;

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
//-------------------------------------------------------------------------------------------------
// 编辑辅助类
//	1.实现一些与状态无关的辅助编辑功能
//	2.对底层功能进行抽象
class sdEditHelper : public NiRefObject
{
public:
	sdEditHelper();

	// 底层抽象访问功能
	sdMap*	GetMap() const { return m_pkMap;};

	// 渲染设置
	// @{
	// 贴图图层开关
	void SetEnableMapChannel(int iChannel, bool bEnable);
	// @}

	// 场景属性设置
	// @{
	
	// @}

	// 后期特效设置
	// @{
	void SetEnableMLAA(bool bEnable);
	bool GetEnableMLAA();

	void SetEnableColorGrading(bool bEnable);
	bool GetEnableColorGrading();

	void SetEnableDof(bool bEnable);
	bool GetEnableDof();

	void SetEnableGodRay(bool bEnable);
	bool GetEnableGodRay();

	void SetEnableHDR(bool bEnable);
	bool GetEnableHDR();
	// @}

protected:
	sdMap*	m_pkMap;	//当前编辑的地图
};
NiSmartPointer(sdEditHelper);
//-------------------------------------------------------------------------------------------------
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX
#endif