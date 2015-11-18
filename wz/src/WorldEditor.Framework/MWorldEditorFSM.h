//*************************************************************************************************
// 内容:	世界编辑器状态机托管类
//---------------------------------------------------------
// 作者:		
// 创建:		2012-09-21
// 最后修改:
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
	// 封装了以下sdWorldEditFSM的一些功能以提供给C#使用
	public ref class MWorldEditorFSM : public MDisposable
	{
	public:
		MWorldEditorFSM();

		// 设置编辑状态
		void	SetEditState(int iState);

	protected:
		GameEditEx::sdWorldEditFSM*	m_pkWorldEditFSM;
	};
}