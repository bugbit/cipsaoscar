#pragma once

//---Engine Includes----
#include "PhysX/PhysicController.h"

//--Forward Declaration--
class CRenderManager;
class CQuakePhysicsData;

class CPlayer :	public CPhysicController
{
public:
	CPlayer(float radius, float height, float slope, float skinwidth, 
		float stepOffset, uint32 collisionGroups, CQuakePhysicsData *data, const Vect3f& pos = Vect3f(0.f,0.f,0.f), float gravity = -9.8f);
	virtual ~CPlayer(void);
};
