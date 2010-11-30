#include "__PCH_Tests.h"

#include "Player.h"

//---Engine Includes--------
#include "PhysX/PhysicActor.h"
#include "PhysX/PhysicsManager.h"
#include "PhysX/PhysicController.h"

CPlayer::CPlayer(float radius, float height, float slope, float skinwidth, 
		float stepOffset, uint32 collisionGroups, 
		const Vect3f& pos, float gravity):CPhysicController(radius, height, slope, skinwidth, 
		stepOffset, collisionGroups, this,pos,gravity):CQuakePhysicsData(TYPE3D_PLAYER,this)
{
}

void CPlayer::Update(float elapsedTime) {}

void CPlayer::RenderScene(CRenderManager* renderManager) {}