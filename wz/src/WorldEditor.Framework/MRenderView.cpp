#include "WorldEditor.FrameworkPCH.h"
#include "MRenderView.h"

//
#include "sdWorldEditor.h"

namespace WorldEditorFramework 
{
	//-----------------------------------------------------------------------------
	MRenderView::MRenderView(System::IntPtr hWndPtr, MRenderViewType eViewType)
		: m_hWndPtr(hWndPtr), m_eRenderViewType(eViewType)
	{
		
	}
	//-----------------------------------------------------------------------------
	void MRenderView::WndProc(System::Windows::Forms::Message% msg)
	{
		if (sdWorldEditor::InstancePtr() && sdWorldEditor::Instance().IsInitialized())
		{
			sdWorldEditor::Instance().WndProc(msg.Msg, (HWND)(msg.HWnd.ToPointer()), 
				msg.WParam.ToInt32(), msg.LParam.ToInt32());
		}
	}
	//-----------------------------------------------------------------------------
}