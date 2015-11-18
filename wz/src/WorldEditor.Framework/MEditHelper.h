//*************************************************************************************************
// ����:	�༭������
//---------------------------------------------------------
// ����:		
// ����:		2012-10-19
// ����޸�:
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

	// ��Ⱦ����
	// @{
	void SetEnableMapChannel(int iChannel, bool bEnable);
	// @}

	// ��ͼ����
	// @{
	property MMap^ Map{ MMap^ get();}
	property MLight^ AmbientLight { MLight^ get();}
	property MLight^ MainLight { MLight^ get();}
	property MLight^ AssistantLight { MLight^ get();}
	// @}

	// ������Ч
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