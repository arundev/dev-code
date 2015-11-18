//*************************************************************************************************
// 内容:	编辑辅助标记接口
//---------------------------------------------------------
// 作者:		
// 创建:		2014-03-20
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_GAMEEDITEX_EDITGIZMOHELPER_H__
#define _SD_GAMEEDITEX_EDITGIZMOHELPER_H__

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
//-------------------------------------------------------------------------------------------------
// 编辑辅助标记接口,管理标记的加载更新与绘制
// (统一出来是为了防止在各个编辑状态内部重复，考虑独立为专门管理器)
//	1.支持屏幕左下角坐标系的显示
//-------------------------------------------------------------------------------------------------
class sdEditGizmoHelper : public NiRefObject
{
public:
	sdEditGizmoHelper();
	virtual ~sdEditGizmoHelper();

	virtual void Update();
	virtual void RenderGizmo();

	NiCamera* GetOrthCamera() { return m_spOrthCamera;}
	NiCamera* GetOrthScreenCamera() { return m_spOrthScreenCamera;}

	

protected:
	static NiNodePtr LoadGizmoNif(const char* szFileName);

	static void DisableZTest(NiAVObject* pkAVObject);
	static void EnableDrawBothMode(NiAVObject* pkAVObject);

protected:
	NiCameraPtr m_spOrthScreenCamera;	///< 正交相机，用于绘制屏幕对象
	NiCameraPtr m_spOrthCamera;			///< 正交相机
	NiAVObjectPtr m_spAxisObject;		///< 左下角坐标轴
};
NiSmartPointer(sdEditGizmoHelper);
//-------------------------------------------------------------------------------------------------
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX
#endif