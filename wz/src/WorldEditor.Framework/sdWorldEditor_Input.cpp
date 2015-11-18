#include "WorldEditor.FrameworkPCH.h"
#include "sdWorldEditor.h"
#include <sdCameraEditState.h>

using namespace GameEditEx;
//-------------------------------------------------------------------------------------------------
void sdWorldEditor::WndProc(int iMsg, HWND hWnd, WPARAM wParam, LPARAM lParam)
{
	// 
	sdEditCameraState* pkCurEditCamState = (sdEditCameraState*)m_kCameraFSM.GetState();
	NIASSERT(pkCurEditCamState);

	// 分发消息
	if (iMsg >= WM_KEYFIRST && iMsg <= WM_KEYLAST)
	{
		// 键盘输入
		switch (iMsg)
		{
			case WM_KEYDOWN:	
				m_kWorldEditFSM.OnKeyDown(wParam, lParam);	
				pkCurEditCamState->OnKeyDown(wParam, lParam);
				break;

			case WM_KEYUP:		
				m_kWorldEditFSM.OnKeyUp(wParam, lParam);	
				pkCurEditCamState->OnKeyUp(wParam, lParam);
				break;	
		}
	}
	else if (iMsg >= WM_MOUSEFIRST && iMsg <= WM_MOUSELAST)
	{
		if (WM_MOUSEWHEEL == iMsg)
		{
			// 处理鼠标滚轮消息
			m_kWorldEditFSM.OnMouseWheel(wParam, lParam);	
			pkCurEditCamState->OnMouseWheel(wParam, lParam);
		}
		else
		{
			// 鼠标输入
			switch (iMsg)
			{
			case WM_MOUSEMOVE:		
				m_kWorldEditFSM.OnMouseMove(wParam, lParam);	
				pkCurEditCamState->OnMouseMove(wParam, lParam);
				break;

			case WM_LBUTTONDOWN:	
				m_kWorldEditFSM.OnLeftButtonDown(wParam, lParam);		
				break;

			case WM_LBUTTONUP:		
				m_kWorldEditFSM.OnLeftButtonUp(wParam, lParam);			
				break;	

			case WM_LBUTTONDBLCLK:	
				m_kWorldEditFSM.OnLeftButtonDBClick(wParam, lParam);	
				break;	

			case WM_RBUTTONDOWN:	
				m_kWorldEditFSM.OnRightButtonDown(wParam, lParam);	
				pkCurEditCamState->OnRightButtonDown(wParam, lParam);
				break;

			case WM_RBUTTONUP:		
				m_kWorldEditFSM.OnRightButtonUp(wParam, lParam);	
				pkCurEditCamState->OnRightButtonUp(wParam, lParam);
				break;	

			case WM_RBUTTONDBLCLK:	
				m_kWorldEditFSM.OnRightButtonDBClick(wParam, lParam);	
				break;	

			case WM_MBUTTONDOWN:	
				m_kWorldEditFSM.OnMiddleButtonDown(wParam, lParam);		
				break;

			case WM_MBUTTONUP:		
				m_kWorldEditFSM.OnMiddleButtonUp(wParam, lParam);		
				break;	

			case WM_MBUTTONDBLCLK:	
				m_kWorldEditFSM.OnMiddleButtonDBClick(wParam, lParam);	
				break;	
			}
		}
	}
	else
	{
		//// 鼠标切换窗口
		//switch (iMsg)
		//{
		//case WM_MOUSELEAVE:	
		//	break;
		//case WM_MOUSEHOVER:		
		//	break;	
		//}
	}
}
//-------------------------------------------------------------------------------------------------