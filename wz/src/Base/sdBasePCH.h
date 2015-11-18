#pragma once
#ifndef _SD_BASE_STDAFX_H__
#define _SD_BASE_STDAFX_H__

//*********************************************************
// windows 头文件
//*********************************************************
#define NOMINMAX
#include <windows.h>

//*********************************************************
// C头文件
//*********************************************************
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <assert.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <time.h>

//*********************************************************
// C++头文件
//*********************************************************
// 数据流
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>

// 容器
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <set>
#include <map>
#include <hash_set>
#include <hash_map>

// 算法
#include <algorithm>
#include <functional>
#include <utility>
#include <xlocale>
#include <limits>

//*********************************************************
// 当前库文件
//*********************************************************
#define BASE_NAMESPACE_BEGIN_BASE			namespace Base{	
#define BASE_NAMESPACE_END_BASE				}

#define BASE_NAMESPACE_BEGIN_CRYPTOGRAPHY	namespace Cryptography{	
#define BASE_NAMESPACE_END_CRYPTOGRAPHY		}

#define BASE_NAMESPACE_BEGIN_DIAGNOSTICS	namespace Diagnostics{	
#define BASE_NAMESPACE_END_DIAGNOSTICS		}

#define BASE_NAMESPACE_BEGIN_IO				namespace IO{	
#define BASE_NAMESPACE_END_IO				}

#define BASE_NAMESPACE_BEGIN_MATH			namespace Math{	
#define BASE_NAMESPACE_END_MATH				}

#define BASE_NAMESPACE_BEGIN_MEMORY			namespace Memory{	
#define BASE_NAMESPACE_END_MEMORY			}

#define BASE_NAMESPACE_BEGIN_MISC			namespace Misc{	
#define BASE_NAMESPACE_END_MISC				}

#define BASE_NAMESPACE_BEGIN_SECURITY		namespace Security{	
#define BASE_NAMESPACE_END_SECURITY			}

#define BASE_NAMESPACE_BEGIN_THREADING		namespace Threading{	
#define BASE_NAMESPACE_END_THREADING		}

#define BASE_NAMESPACE_BEGIN_UTIL			namespace Util{	
#define BASE_NAMESPACE_END_UTIL				}

//*********************************************************
// 预编译文件
//*********************************************************
#include "sdBaseLibType.h"
#include "sdConfig.h"
#include "sdUniversalTypes.h"
#include "sdPlatform.h"

#include "sdMemObject.h"
#include "sdRefObject.h"
#include "sdSmartPointer.h"

#include "sdAssert.h"
#include "sdDebug.h"

#include "sdCriticalSection.h"
#include "sdMutex.h"
#include "sdSemaphore.h"
#include "sdEvent.h"
#include "sdLock.h"

#include "sdMath.h"
#include "sdVector2.h"
#include "sdVector3.h"
#include "sdVector4.h"
#include "sdMatrix2.h"
#include "sdMatrix3.h"
#include "sdMatrix4.h"
#include "sdQuaternion.h"
#include "sdRay.h"
#include "sdPlane.h"
#include "sdAxisAlignedBox.h"

#endif