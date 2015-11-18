//*************************************************************************************************
// ����:	��ԴϵͳһЩ���湫������
//---------------------------------------------------------
// ����:		
// ����:		2012-08-11
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_DEFINE_H__
#define _SD_ENGINE_RESOURCE_DEFINE_H__

namespace ResourceSystem
{
	// ��Դ������������
	enum eResTaskType
	{
		E_RES_TASK_MODEL	= 0x0001,	// nif�ļ�
		E_RES_TASK_KEYFRAME	= 0x0002,	// kf�ļ�
		E_RES_TASK_ACTOR	= 0x0004,	// kfm�ļ�
		E_RES_TASK_TEXTURE	= 0x0008,	// texture�ļ�
	};

	// ��Դ��������״̬
	enum eResTaskStatus
	{
		E_RES_TASK_WAIT,
		E_RES_TASK_LOADING,
	};

	// ��Դ�������ȼ�
	enum eResTaskPrior
	{
		E_RES_TASK_NORMAL	= 0,
		E_RES_TASK_HIGH		= 1,
		E_RES_TASK_HIGHEST	= 2,
	};

	// ��Դ��������
	enum eResLifeTime
	{
		E_RES_LT_APP,
		E_RES_LT_SCENE,
	};
}
#endif