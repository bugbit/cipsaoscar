#include "__PCH_Quake.h"

#include "Player.h"
#include "QuakePhysicsData.h"

//---Engine Includes--------
#include "PhysX/PhysicActor.h"
#include "PhysX/PhysicsManager.h"
#include "PhysX/PhysicController.h"

CPlayer::CPlayer(float radius, float height, float slope, float skinwidth, 
		float stepOffset, uint32 collisionGroups, CQuakePhysicsData *data, 
		const Vect3f& pos, float gravity):CPhysicController(radius, height, slope, skinwidth, 
		stepOffset, collisionGroups, data,pos,gravity)
{
	data->SetObject3D(this);
}

CPlayer::~CPlayer(void)
{
}
