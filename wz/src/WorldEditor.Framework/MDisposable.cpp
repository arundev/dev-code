#include "WorldEditor.FrameworkPCH.h"
#include "MDisposable.h"

SD_EDITOR_NAMESPACE_BEGIN_WORLDEDITORFRAMEWORK
//-----------------------------------------------------------------------------
MDisposable::MDisposable(): m_bDisposed(false)
{

}
//-----------------------------------------------------------------------------
MDisposable::~MDisposable()
{
	// �ͷ��й���Դ�ͷ��й���Դ
	__try
	{
		System::Threading::Monitor::Enter(this);

		if (!m_bDisposed)
		{
			// �ͷŷ��й���Դ
			Do_Dispose(true);

			// �ͷ��й���Դ
			System::GC::SuppressFinalize(this);

			m_bDisposed = true;
		}
	}
	__finally
	{
		System::Threading::Monitor::Exit(this);
	}
}
//-----------------------------------------------------------------------------
MDisposable::!MDisposable()
{
	// �ͷŷ��й���Դ
	__try
	{
		System::Threading::Monitor::Enter(this);

		if (!m_bDisposed)
		{
			// �ͷŷ��й���Դ
			Do_Dispose(false);

			m_bDisposed = true;
		}
	}
	__finally
	{
		System::Threading::Monitor::Exit(this);
	}
}
//-----------------------------------------------------------------------------
SD_EDITOR_NAMESPACE_END_WORLDEDITORFRAMEWORK