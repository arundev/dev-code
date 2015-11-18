//*************************************************************************************************
// ����:	�������������ӿ�
//---------------------------------------------------------
// ����:		
// ����:		2013-01-05
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_MOVIE_SYSTEM_MOVIE_H__
#define _SD_ENGINE_MOVIE_SYSTEM_MOVIE_H__

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM

class IAnimNode;

class IMovie : public NiRefObject
{
public:
	// �����������ŷ�ʽ
	enum EMoviePlayMode
	{
		E_MOVIE_PLAYONCE,		///< һ�β���,�������,�ص���һ֡
		E_MOVIE_PLAYONCEEND,	///< һ�β���,�������,ͣ�����һ֡
		E_MOVIE_PLAYLOOP,		///< ѭ������,��Ҫ�ֶ�ֹͣ
	};

public:
	virtual ~IMovie() = 0;

	// ���������ļ�����������
	virtual bool Load(const std::string& strMovieFile) = 0;
	virtual bool Save(const std::string& strMovieFile) = 0;

	// ������������
	virtual void UpdateFrame(float fAccumTime, float fFrameTime) = 0;

	// ������������
	virtual void Start() = 0;
	virtual void Stop() = 0;
	virtual void Pause() = 0;
	virtual void Resume() = 0;

	// ��ǰ�Ƿ��ڲ���״̬
	virtual bool IsPlaying() = 0;

	// ��ȡ�������ڵ�
	virtual IAnimNode*	GetAnimNode() = 0;

	// ��ȡ����������ʱ��
	virtual float GetTotalTime() = 0;

	// ��ȡ����������ǰ����λ��
	virtual float GetCurrentTime() = 0;

	// ��ת����������ָ��ʱ���
	virtual void GotoTime(float fTime) = 0;

	// ��ת��ָ��֡
	virtual void GotoFrame(uint uiFrameId) = 0;

	// ��ȡ�����ù����������ŷ�ʽ
	virtual void SetPlayMode(EMoviePlayMode ePlayMode) = 0;
	virtual EMoviePlayMode GetPlayMode() = 0;

	// ��ȡ�����ù����������
	virtual void SetCamera(NiCamera* spCamera) = 0;
	virtual NiCamera* GetCamera() = 0;

	// ��ȡ�����ù�����������
	virtual void SetParam(const std::string& key, const std::string& value) = 0;
	virtual void GetParam(const std::string& key, std::string& value) = 0;
};
NiSmartPointer(IMovie);

ENGINE_NAMESPACE_END_MOVIESYSTEM
#endif