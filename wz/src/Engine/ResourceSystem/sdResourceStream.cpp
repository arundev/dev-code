#include "sdResourceSystemPCH.h"
#include "sdResourceStream.h"
#include "sdResourceSystem.h"

#include <NiFilename.h>

ENGINE_NAMESPACE_BEGIN_ENGINE
ENGINE_NAMESPACE_BEGIN_RESOURCESYSTEM
//-------------------------------------------------------------------------------------------------
// sdResourceStream
//-------------------------------------------------------------------------------------------------
sdResourceStream::sdResourceStream()
{
	SetSearchPath(NiNew sdResourceSearchPath);
}
//-------------------------------------------------------------------------------------------------
// sdResourceStream
//-------------------------------------------------------------------------------------------------
sdResourceSearchPath::sdResourceSearchPath()
: m_uiBaseNextPath(0)
{

}
//-------------------------------------------------------------------------------------------------
bool sdResourceSearchPath::GetNextSearchPath(char* pcPath, unsigned int uiStringLen)
{
	NiFilename kFullPath(m_acFilePath);
	switch (m_uiNextPath)
	{
	case 0:
		{
			// ת��ResName->ResPath
			char szNewPath[MAX_PATH_LEN];
			sdResourceSystem* pkResourceSystem = (sdResourceSystem*)sdResourceSystem::GetResourceSystem();
			pkResourceSystem->Convert2ResPath(m_acFilePath, szNewPath);

			kFullPath.SetDrive("");
			kFullPath.SetDir("");
			kFullPath.SetPlatformSubDir("");
			kFullPath.SetExt("");
			kFullPath.SetFilename(szNewPath);
			break;
		}
	case 1:
		{
			// ��ʱ�߼�,��Nif·���е�"/model/"ǰ�����texture
			// (���ڸ�Nif�Զ��������)
			std::string kTexPath = m_acReferencePath;
			size_t iIndex = kTexPath.find("\\model\\");
			if (iIndex != std::string::npos)
			{
				kTexPath = kTexPath.insert(iIndex, "\\texture");
				kTexPath = kTexPath.substr(0, iIndex + strlen("\\texture\\model\\") - 1);
			}
			
			// Ĭ��Ŀ¼ + �ļ���Ŀ¼ + �ļ���
			kFullPath.SetDrive("");
			kFullPath.SetDir(kTexPath.c_str());
			kFullPath.SetPlatformSubDir(NiFilename(m_acFilePath).GetDir());

			break;
		}
	default:
		{
			// ʹ�ø����GetNextSearchPath�߼�
			m_uiNextPath = m_uiBaseNextPath;
			bool bRet = NiSearchPath::GetNextSearchPath(pcPath, uiStringLen);
			m_uiBaseNextPath = m_uiNextPath;
			return bRet;
		}
	}

	kFullPath.GetFullPath(pcPath, uiStringLen);
	NiPath::Standardize(pcPath);

	m_uiNextPath++;

	return true;
}
//-------------------------------------------------------------------------------------------------
ENGINE_NAMESPACE_END_RESOURCESYSTEM
ENGINE_NAMESPACE_END_ENGINE