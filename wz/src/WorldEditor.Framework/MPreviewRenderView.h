//*************************************************************************************************
// ����:	�༭������
//---------------------------------------------------------
// ����:		
// ����:		2012-11-07
// ����޸�:
//*************************************************************************************************
#pragma once
#include "MRenderView.h"

namespace WorldEditorFramework 
{
	public ref class MPreviewRenderView : public MRenderView
	{
	public:
		MPreviewRenderView(System::IntPtr hWndPtr, MRenderViewType eViewType);

		// ��Դ
		//bool	LoadNif();
		//bool	LoadEffect();
		//bool	LoadMonster();
		//bool	LoadFXStdioEffect();
	};
}