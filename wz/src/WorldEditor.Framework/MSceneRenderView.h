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
	public ref class MSceneRenderView : public MRenderView
	{
	public:
		MSceneRenderView(System::IntPtr hWndPtr, MRenderViewType eViewType);

		// ����
		//bool	LoadScene();
		//bool	SaveScene();
		//void	DestroyScene();
	};
}