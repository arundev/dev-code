//*************************************************************************************************
// ����:	��������ϵͳ�����ӿ�
//---------------------------------------------------------
// ����:		
// ����:		2013-01-05
// ����޸�:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_MOVIE_SYSTEM_MOVIE_SYSTEM_H__
#define _SD_ENGINE_MOVIE_SYSTEM_MOVIE_SYSTEM_H__

#include <sdSingleton.h>

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM

class IMovie;

class IMovieSystem : public Base::sdTSingleton<IMovieSystem>
{
public:
	virtual ~IMovieSystem() {};

	// ���ش��������ٹ�������
	virtual IMovie* CreateMovie() = 0;
	virtual IMovie* LoadMovie(const std::string& strMovieFile) = 0;
	virtual void DestroyMovie(IMovie* pkMovie) = 0;

	// ������������
	virtual void UpdateFrame(float fAccumTime, float fFrameTime) = 0;

	// ��ȡ�����ù�������ȫ�ֲ���
	virtual void SetParam(const std::string& key, const std::string& value) = 0;
	virtual void GetParam(const std::string& key, std::string& value) = 0;

	// ��ȡ�������ӿ�
	static IMovieSystem* GetMovieSystem(){ return InstancePtr();}
};

ENGINE_NAMESPACE_END_MOVIESYSTEM
#endif