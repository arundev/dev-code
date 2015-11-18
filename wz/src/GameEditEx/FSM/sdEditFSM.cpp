#include "sdGameEditExPCH.h"
#include "sdEditFSM.h"
#include "sdEditMode.h"

SD_GAMEEDITEX_NAMESPACE_BEGIN_GAMEEDITEX
//-------------------------------------------------------------------------------------------------
void sdEditFSM::OnKeyDown(WPARAM wParam, LPARAM lParam)
{
	sdEditMode* pkEditMode = (sdEditMode*)GetCurrentState();
	if (pkEditMode)
	{
		pkEditMode->OnKeyDown(wParam, lParam);
	}
}
//-------------------------------------------------------------------------------------------------
void sdEditFSM::OnKeyUp(WPARAM wParam, LPARAM lParam)
{
	sdEditMode* pkEditMode = (sdEditMode*)GetCurrentState();
	if (pkEditMode)
	{
		pkEditMode->OnKeyUp(wParam, lParam);
	}
}
//-------------------------------------------------------------------------------------------------
void sdEditFSM::OnMouseWheel(WPARAM wParam, LPARAM lParam)
{
	sdEditMode* pkEditMode = (sdEditMode*)GetCurrentState();
	if (pkEditMode)
	{
		pkEditMode->OnMouseWheel(wParam, lParam);
	}
}
//-------------------------------------------------------------------------------------------------
void sdEditFSM::OnMouseMove(WPARAM wParam, LPARAM lParam)
{
	sdEditMode* pkEditMode = (sdEditMode*)GetCurrentState();
	if (pkEditMode)
	{
		pkEditMode->OnMouseMove(wParam, lParam);
	}
}
//-------------------------------------------------------------------------------------------------
void sdEditFSM::OnLeftButtonDown(WPARAM wParam, LPARAM lParam)
{
	sdEditMode* pkEditMode = (sdEditMode*)GetCurrentState();
	if (pkEditMode)
	{
		pkEditMode->OnLeftButtonDown(wParam, lParam);
	}
}
//-------------------------------------------------------------------------------------------------
void sdEditFSM::OnLeftButtonUp(WPARAM wParam, LPARAM lParam)
{
	sdEditMode* pkEditMode = (sdEditMode*)GetCurrentState();
	if (pkEditMode)
	{
		pkEditMode->OnLeftButtonUp(wParam, lParam);
	}
}
//-------------------------------------------------------------------------------------------------
void sdEditFSM::OnLeftButtonDBClick(WPARAM wParam, LPARAM lParam)
{
	sdEditMode* pkEditMode = (sdEditMode*)GetCurrentState();
	if (pkEditMode)
	{
		pkEditMode->OnLeftButtonDBClick(wParam, lParam);
	}
}
//-------------------------------------------------------------------------------------------------
void sdEditFSM::OnRightButtonDown(WPARAM wParam, LPARAM lParam)
{
	sdEditMode* pkEditMode = (sdEditMode*)GetCurrentState();
	if (pkEditMode)
	{
		pkEditMode->OnRightButtonDown(wParam, lParam);
	}
}
//-------------------------------------------------------------------------------------------------
void sdEditFSM::OnRightButtonUp(WPARAM wParam, LPARAM lParam)
{
	sdEditMode* pkEditMode = (sdEditMode*)GetCurrentState();
	if (pkEditMode)
	{
		pkEditMode->OnRightButtonUp(wParam, lParam);
	}
}
//-------------------------------------------------------------------------------------------------
void sdEditFSM::OnRightButtonDBClick(WPARAM wParam, LPARAM lParam)
{
	sdEditMode* pkEditMode = (sdEditMode*)GetCurrentState();
	if (pkEditMode)
	{
		pkEditMode->OnRightButtonDBClick(wParam, lParam);
	}
}
//-------------------------------------------------------------------------------------------------
void sdEditFSM::OnMiddleButtonDown(WPARAM wParam, LPARAM lParam)
{
	sdEditMode* pkEditMode = (sdEditMode*)GetCurrentState();
	if (pkEditMode)
	{
		pkEditMode->OnMiddleButtonDown(wParam, lParam);
	}
}
//-------------------------------------------------------------------------------------------------
void sdEditFSM::OnMiddleButtonUp(WPARAM wParam, LPARAM lParam)
{
	sdEditMode* pkEditMode = (sdEditMode*)GetCurrentState();
	if (pkEditMode)
	{
		pkEditMode->OnMiddleButtonUp(wParam, lParam);
	}
}
//-------------------------------------------------------------------------------------------------
void sdEditFSM::OnMiddleButtonDBClick(WPARAM wParam, LPARAM lParam)
{
	sdEditMode* pkEditMode = (sdEditMode*)GetCurrentState();
	if (pkEditMode)
	{
		pkEditMode->OnMiddleButtonDBClick(wParam, lParam);
	}
}
//-------------------------------------------------------------------------------------------------
SD_GAMEEDITEX_NAMESPACE_END_GAMEEDITEX