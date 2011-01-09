#pragma once

#include "Item.h"

//---Engine Includes----
#include "PhysX/PhysicController.h"

//--Forward Declaration--
class CRenderManager;
class CQuakePhysicsData;
class CItemLife;

class CPlayer :	public CPhysicController
{
public:
	CPlayer(float radius, float height, float slope, float skinwidth, 
		float stepOffset, uint32 collisionGroups, CQuakePhysicsData *data, const Vect3f& pos = Vect3f(0.f,0.f,0.f), float gravity = -9.8f);
	virtual							~CPlayer(void);
	
	bool											Init								();
	void											Done	              ();
	inline bool								IsOk	              () const { return m_bIsOk; }

	void											SetCleanMove();
	void											SetMoveUp(bool speed,float elapsedTime);
	void											SetMoveDown(bool speed,float elapsedTime);
	void											SetMoveLeft(bool speed,float elapsedTime);
	void											SetMoveRight(bool speed,float elapsedTime);
	void											Move(float elapsedTime);
	inline int								GetStatusPlayer() const { return m_life; }
	void											AddStatusPlayer(int amount);
	inline CItem::ETYTE				GetTypeGun() const { return m_gunSelected; }
	void											Catch(CItem *item);
private:
	bool											m_bIsOk;			      // Initialization boolean control
	float											m_fSpeedForward;
	float											m_fSpeed;
	Vect3f										m_MoveDirection;
	int												m_life;
	CItem::ETYTE							m_gunSelected;

	void											Release();
	void											SetMove(Vect3f vu,bool speed,float elapsedTime);
	void											Catch(CItemLife *item);
};
