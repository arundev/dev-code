//*************************************************************************************************
// ����:	����༭��״̬���й���
//---------------------------------------------------------
// ����:		
// ����:		2012-09-21
// ����޸�:
//*************************************************************************************************
#pragma once
#include "MDisposable.h"

//
namespace GameEditEx
{
	class sdWorldEditFSM;
}

namespace WorldEditorFramework 
{
	// ��װ������sdWorldEditFSM��һЩ�������ṩ��C#ʹ��
	public ref class MWorldEditorFSM : public MDisposable
	{
	public:
		MWorldEditorFSM();

		// ���ñ༭״̬
		void	SetEditState(int iState);

	protected:
		GameEditEx::sdWorldEditFSM*	m_pkWorldEditFSM;
	};
}