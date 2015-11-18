//*************************************************************************************************
// 内容:	编辑帮助类
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-07
// 最后修改:
//*************************************************************************************************
#pragma once
#include "MRenderView.h"

namespace WorldEditorFramework 
{
	public ref class MSceneRenderView : public MRenderView
	{
	public:
		MSceneRenderView(System::IntPtr hWndPtr, MRenderViewType eViewType);

		// 场景
		//bool	LoadScene();
		//bool	SaveScene();
		//void	DestroyScene();
	};
}