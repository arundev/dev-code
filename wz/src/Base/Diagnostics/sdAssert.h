//*************************************************************************************************
// 内容:	错误捕捉
//---------------------------------------------------------
// 作者:	
// 创建:		2012-08-04
// 最后修改:	
//*************************************************************************************************
#pragma once
#ifndef _SD_BASE_DIAGNOSTICS_ASSERT_H__
#define _SD_BASE_DIAGNOSTICS_ASSERT_H__

BASE_NAMESPACE_BEGIN_BASE
BASE_NAMESPACE_BEGIN_DIAGNOSTICS
//-------------------------------------------------------------------------------------------------
//#if (SD_ASSERT_MODE == 1)
//	#ifdef SDDEBUG
//		#define sdAssert(a, b) assert((a) && (b))
//	#else
//		#define sdAssert(a, b) //if( !(a) ) OGRE_EXCEPT( Ogre::Exception::ERR_RT_ASSERTION_FAILED, (b), "no function info")
//	#endif
//#elif SD_ASSERT_MODE == 2
//	#define sdAssert(a, b)	//if(!(a)) OGRE_EXCEPT(Ogre::Exception::ERR_RT_ASSERTION_FAILED, (b), "no function info")
//#else
//	#define sdAssert(a, b)	assert((a) && (b))	
//#endif
//-------------------------------------------------------------------------------------------------
BASE_NAMESPACE_END_DIAGNOSTICS
BASE_NAMESPACE_END_BASE
#endif