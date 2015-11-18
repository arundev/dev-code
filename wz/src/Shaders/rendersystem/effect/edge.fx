//*************************************************************************************************
// 内容:	勾边
//
// 注意:
//	1.VERTEX_EHARDWARESKIN_FLAG和VERTEX_LEAFANIM_FLAG不会同时存在
//	2.VERTEX_LEAFANIM_FLAG只能和VERTEX_COMPACTVERTEX_FLAG一起用(需要normal)
//---------------------------------------------------------
// 作者:		
// 创建:		2012-11-15
// 最后修改:	2014-05-06
//*************************************************************************************************
#define CHANNELS_FLAG	0x00000000
#include "edge_implement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000001
#include "edge_implement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000002
#include "edge_implement.h"
#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000003
#include "edge_implement.h"
#undef 	CHANNELS_FLAG

//#define CHANNELS_FLAG	0x00000004
//#include "edge_implement.h"
//#undef 	CHANNELS_FLAG

#define CHANNELS_FLAG	0x00000005
#include "edge_implement.h"
#undef 	CHANNELS_FLAG

//#define CHANNELS_FLAG	0x00000006
//#include "edge_implement.h"
//#undef 	CHANNELS_FLAG

//#define CHANNELS_FLAG	0x00000007
//#include "edge_implement.h"
//#undef 	CHANNELS_FLAG