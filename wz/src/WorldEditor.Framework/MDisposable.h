//*************************************************************************************************
// ����:	�ֶ��ͷ���Դ�ӿ�
//---------------------------------------------------------
// ����:		
// ����:		2012-09-21
// ����޸�:
//*************************************************************************************************
#pragma once
#include "MMacros.h"

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-----------------------------------------------------------------------------
// ��.NET�Ķ�����ʵ���������������ͷ���Դ�ĺ�����Dispose��Finalize��
//		Finalize��Ŀ���������ͷŷ��йܵ���Դ��
//		Dispose�������ͷ�������Դ�������йܵĺͷ��йܵġ�
public ref class MDisposable abstract : public System::IDisposable
{
public:
	MDisposable();
	!MDisposable();			///< finalizer
	virtual ~MDisposable();	///< destructor calls finalizer

protected:
	// �ͷ���Դ������ִ�к���
	virtual void Do_Dispose(bool bDisposing){};	
	
private:
	bool m_bDisposed;
};
//-----------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK