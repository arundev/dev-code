//*************************************************************************************************
// 内容:	
//---------------------------------------------------------
// 作者:		
// 创建:		2013-01-08
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_MOVIE_SYSTEM_ANIM_AFFECTOR_CONTROL_H__
#define _SD_ENGINE_MOVIE_SYSTEM_ANIM_AFFECTOR_CONTROL_H__

#include "IAnimTrack.h"

ENGINE_NAMESPACE_BEGIN_MOVIESYSTEM

class sdAnimAffectorControl : public IAnimAffectorControl
{
public:
	sdAnimAffectorControl();
	virtual ~sdAnimAffectorControl();

	// 虚函数继承
	virtual bool Load(Engine::Util::sdLuaReadUtil& kLuaStream);
	virtual bool Save(Engine::Util::sdLuaWriteUtil& kLuaStream);
	virtual void Clear();

	virtual int	 AddAffector(EAnimAffectorType eAnimAffectorType, int iTracktype);
	virtual void RemoveAffector(EAnimAffectorType eAnimAffectorType);

	virtual IAnimAffector* GetAffectorByType(EAnimAffectorType eAnimAffectorType);
	virtual IAnimAffector* GetAffectorByIndex(int iIndex);

	virtual int GetAffectorNum() { return m_kAffectorDescVec.size();}

protected:
	class sdAffectorDesc : public NiMemObject
	{
	public:
		EAnimAffectorType	eAffectorType;
		IAnimAffectorControlPtr				pkAffector;

	public:
		sdAffectorDesc() : eAffectorType(E_ANIM_AFFECTOR_DEFAULT), pkAffector(NULL){}
	};

protected:
	typedef std::vector<sdAffectorDesc> AffectorDescVec;
	typedef std::vector<sdAffectorDesc>::iterator AffectorDescVecItr;
	AffectorDescVec m_kAffectorDescVec;
};
NiSmartPointer(sdAnimAffectorControl);

ENGINE_NAMESPACE_END_MOVIESYSTEM
#endif