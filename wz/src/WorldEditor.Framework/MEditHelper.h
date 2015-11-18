//*************************************************************************************************
// 内容:	编辑帮助类
//---------------------------------------------------------
// 作者:		
// 创建:		2012-10-19
// 最后修改:
//*************************************************************************************************
#pragma once
#include "./SceneType/MMap.h"
//
namespace GameEditEx
{
	class sdEditHelper;
}

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
public ref class MEditHelper
{
public:
	MEditHelper();

	// 渲染属性
	// @{
	void SetEnableMapChannel(int iChannel, bool bEnable);
	// @}

	// 地图属性
	// @{
	property MMap^ Map{ MMap^ get();}
	property MLight^ AmbientLight { MLight^ get();}
	property MLight^ MainLight { MLight^ get();}
	property MLight^ AssistantLight { MLight^ get();}
	// @}

	// 后期特效
	// @{
	property bool MLAA{ void set(bool val);bool get();}
	property bool ColorGrading{ void set(bool val);bool get();}
	property bool Dof{ void set(bool val);bool get();}
	property bool GodRay{ void set(bool val);bool get();}
	property bool HDR{ void set(bool val);bool get();}
	// @}

protected:
	GameEditEx::sdEditHelper* m_pkEditHelper;
};
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK