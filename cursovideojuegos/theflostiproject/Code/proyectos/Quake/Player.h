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
	virtual							~CPlayer(void);
	void								SetCleanMove();
	void								SetMoveUp(bool speed,float elapsedTime);
	void								SetMoveDown(bool speed,float elapsedTime);
	void								SetMoveLeft(bool speed,float elapsedTime);
	void								SetMoveRight(bool speed,float elapsedTime);
	void								Move(float elapsedTime);
private:
	float								m_fSpeedForward;
	float								m_fSpeed;
	Vect3f							m_MoveDirection;

	void								SetMove(Vect3f vu,bool speed,float elapsedTime);
};
