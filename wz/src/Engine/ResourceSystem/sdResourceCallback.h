//*************************************************************************************************
// ����:	��Դ���ػص�
//---------------------------------------------------------
// ����:		
// ����:		2012-08-11
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_RESOURCE_CALLBACK_H__
#define _SD_ENGINE_RESOURCE_CALLBACK_H__

namespace ResourceSystem
{
	// ��Դ���ػص��ӿ�
	class IResCallback : public NiMemObject
	{
	public:
		IResCallback();
		virtual IResCallback() = 0;

		// ������ɻص�
		// (���첽������ɺ���Դ����������)
		virtual void OnLoadingOK() = 0;

	protected:
		// ȡ�����ػص�
		// (�ɼ��������ߵ���)
		//void CancelLoadingEvent();

	};
}
#endif