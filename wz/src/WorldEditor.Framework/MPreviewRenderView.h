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
	public ref class MPreviewRenderView : public MRenderView
	{
	public:
		MPreviewRenderView(System::IntPtr hWndPtr, MRenderViewType eViewType);

		// 资源
		//bool	LoadNif();
		//bool	LoadEffect();
		//bool	LoadMonster();
		//bool	LoadFXStdioEffect();
	};
}