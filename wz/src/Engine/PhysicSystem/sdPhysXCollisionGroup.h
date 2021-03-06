//*************************************************************************************************
// 内容:	PhysX的Actor分组
//---------------------------------------------------------
// 作者:		
// 创建:		2013-07-04
// 最后修改:
//*************************************************************************************************
#pragma once
#ifndef _SD_ENGINE_PHYSICS_SYSTEM_PHYSX_COLLISION_GROUP_H__
#define _SD_ENGINE_PHYSICS_SYSTEM_PHYSX_COLLISION_GROUP_H__

ENGINE_NAMESPACE_BEGIN_PHYSICSYSTEM
//-----------------------------------------------------------------------------
enum EPhysXCollisionGroupShift
{
	E_COLLISION_GROUP_SHIFT_DEFAULT,
	E_COLLISION_GROUP_SHIFT_TERRAIN,	///< 地形
	E_COLLISION_GROUP_SHIFT_WATER,		///< 水
	E_COLLISION_GROUP_SHIFT_FBDAREA,
	E_COLLISION_GROUP_SHIFT_CARRIER,
	E_COLLISION_GROUP_SHIFT_RAGDOLL,
	E_COLLISION_GROUP_SHIFT_PLAYER_CHARACTER,
	E_COLLISION_GROUP_SHIFT_STATIC,
	E_COLLISION_GROUP_SHIFT_BUILDING,
	E_COLLISION_GROUP_SHIFT_PROP,
	E_COLLISION_GROUP_SHIFT_VEHICLE,
	E_COLLISION_GROUP_SHIFT_PROJECTILE,
	E_COLLISION_GROUP_SHIFT_TRIGGER,
	E_COLLISION_GROUP_SHIFT_NPC,
	E_COLLISION_GROUP_SHIFT_CAMERAHITTEST,
	E_COLLISION_GROUP_SHIFT_NOCAMERAHITTEST,
	E_COLLISION_GROUP_SHIFT_NOPUSH,
	E_COLLISION_GROUP_SHIFT_CLOTH,
	E_COLLISION_GROUP_SHIFT_DOOR,
	E_COLLISION_GROUP_SHIFT_ORION_COUNT,
};
//-----------------------------------------------------------------------------
enum EPhysXCollisionGroupMASK
{
	E_COLLISION_GROUP_MASK_DEFAULT			= 1 << E_COLLISION_GROUP_SHIFT_DEFAULT,
	E_COLLISION_GROUP_MASK_TERRAIN			= 1 << E_COLLISION_GROUP_SHIFT_TERRAIN,	
	E_COLLISION_GROUP_MASK_WATER			= 1 << E_COLLISION_GROUP_SHIFT_WATER,		
	E_COLLISION_GROUP_MASK_FBDAREA			= 1 << E_COLLISION_GROUP_SHIFT_FBDAREA,
	E_COLLISION_GROUP_MASK_CARRIER			= 1 << E_COLLISION_GROUP_SHIFT_CARRIER,
	E_COLLISION_GROUP_MASK_RAGDOLL			= 1 << E_COLLISION_GROUP_SHIFT_RAGDOLL,
	E_COLLISION_GROUP_MASK_PLAYER_CHARACTER = 1 << E_COLLISION_GROUP_SHIFT_PLAYER_CHARACTER,
	E_COLLISION_GROUP_MASK_STATIC			= 1 << E_COLLISION_GROUP_SHIFT_STATIC,
	E_COLLISION_GROUP_MASK_BUILDING			= 1 << E_COLLISION_GROUP_SHIFT_BUILDING,
	E_COLLISION_GROUP_MASK_PROP				= 1 << E_COLLISION_GROUP_SHIFT_PROP,
	E_COLLISION_GROUP_MASK_VEHICLE			= 1 << E_COLLISION_GROUP_SHIFT_VEHICLE,
	E_COLLISION_GROUP_MASK_PROJECTILE		= 1 << E_COLLISION_GROUP_SHIFT_PROJECTILE,
	E_COLLISION_GROUP_MASK_TRIGGER			= 1 << E_COLLISION_GROUP_SHIFT_TRIGGER,
	E_COLLISION_GROUP_MASK_NPC				= 1 << E_COLLISION_GROUP_SHIFT_NPC,
	E_COLLISION_GROUP_MASK_CAMERAHITTEST	= 1 << E_COLLISION_GROUP_SHIFT_CAMERAHITTEST,
	E_COLLISION_GROUP_MASK_NOCAMERAHITTEST	= 1 << E_COLLISION_GROUP_SHIFT_NOCAMERAHITTEST,
	E_COLLISION_GROUP_MASK_CLOTH			= 1 << E_COLLISION_GROUP_SHIFT_NOPUSH,
	E_COLLISION_GROUP_MASK_DOOR				= 1 << E_COLLISION_GROUP_SHIFT_CLOTH,
	E_COLLISION_GROUP_MASK_ALL				= 0xffffffff,
};
//-----------------------------------------------------------------------------
ENGINE_NAMESPACE_END_PHYSICSYSTEM
#endif