#include "__PCH_Tests.h"

#include "QuakePlayer.h"
#include "QuakePhysicsData.h"

//---Engine Includes--------
#include "PhysX/PhysicActor.h"
#include "PhysX/PhysicsManager.h"
#include "PhysX/PhysicController.h"

CQuakePlayer::CQuakePlayer(float radius, float height, float slope, float skinwidth, 
		float stepOffset, uint32 collisionGroups,CQuakePhysicsData *userData,  const Vect3f& pos, float gravity)
:CPhysicController(radius, height, slope, skinwidth, stepOffset, collisionGroups,userData,pos,gravity)
{
}

CQuakePlayer::~CQuakePlayer() {}

void CQuakePlayer::Update(float elapsedTime)
{
}

void CQuakePlayer::RenderScene(CRenderManager* renderManager)
{
}