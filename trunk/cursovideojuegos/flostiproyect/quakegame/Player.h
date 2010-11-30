#pragma once

//---Engine Includes----
#include "PhysX/PhysicController.h"
#include "QuakePhysicsData.h"

class CPlayer : public CPhysicController, public CQuakePhysicsData
{
public:
	CPlayer(float radius, float height, float slope, float skinwidth, 
		float stepOffset, uint32 collisionGroups, 
		const Vect3f& pos = Vect3f(0.f,0.f,0.f), float gravity = -9.8f);
	virtual ~CPlayer();
	virtual void																			Update(float elapsedTime);
	virtual void																			RenderScene(CRenderManager* renderManager);
};