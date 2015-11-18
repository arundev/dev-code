//*************************************************************************************************
// 内容:	世界编辑器托管类
//---------------------------------------------------------
// 作者:		
// 创建:		2012-08-25
// 最后修改:
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

		// 输入消息处理
		//void	WndProc(System::Windows::Forms::Message% msg);

	protected:
		sdWorldEditor*	m_pkWorldEditor;
	};
}