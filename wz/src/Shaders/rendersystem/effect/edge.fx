//*************************************************************************************************
// ����:	����
//
// ע��:
//	1.VERTEX_EHARDWARESKIN_FLAG��VERTEX_LEAFANIM_FLAG����ͬʱ����
//	2.VERTEX_LEAFANIM_FLAGֻ�ܺ�VERTEX_COMPACTVERTEX_FLAGһ����(��Ҫnormal)
//---------------------------------------------------------
// ����:		
// ����:		2012-11-15
// ����޸�:	2014-05-06
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