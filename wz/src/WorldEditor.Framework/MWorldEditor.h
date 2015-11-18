//*************************************************************************************************
// ����:	����༭���й���
//---------------------------------------------------------
// ����:		
// ����:		2012-08-25
// ����޸�:
//*************************************************************************************************
#pragma once
#include "MDisposable.h"

//
class sdWorldEditor;

namespace WorldEditorFramework 
{
	public ref class MWorldEditor : public MDisposable
	{
	public:
		MWorldEditor();
		~MWorldEditor();

		// 
		bool	Initialize(System::IntPtr hWndPtr);
		void	Destroy();
		void	Update();
	
		//
		bool	SaveScene();
		bool	OpenScene();

		// ������Ϣ����
		//void	WndProc(System::Windows::Forms::Message% msg);

	protected:
		sdWorldEditor*	m_pkWorldEditor;
	};
}