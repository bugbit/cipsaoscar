#pragma once

#include "Item.h"

//---Engine Includes----
#include "PhysX/PhysicController.h"

//--Forward Declaration--
class CRenderManager;
class CQuakePhysicsData;
class CItemLife;
class CItemGun;
class CItemAmmo;

class CPlayer :	public CPhysicController
{
public:
	CPlayer																				(float radius, float height, float slope, float skinwidth, 
																									float stepOffset, uint32 collisionGroups, CQuakePhysicsData *data, 
																									const Vect3f& pos = Vect3f(0.f,0.f,0.f), float gravity = -9.8f);
	virtual	~CPlayer															(void);

	virtual void							Update	            (float elapsedTime);
	
	bool											Init								();
	void											Done	              ();
	inline bool								IsOk	              () const { return m_bIsOk; }

	std::string								GetName							() const { return m_sName; }
	void											SetName							(const std::string name) { m_sName=name; }
	void											SetCleanMove				();
	void											SetMoveUp						(bool speed,float elapsedTime);
	void											SetMoveDown					(bool speed,float elapsedTime);
	void											SetMoveLeft					(bool speed,float elapsedTime);
	void											SetMoveRight				(bool speed,float elapsedTime);
	void											Move								(float elapsedTime);
	inline GUN *							GetGunSelected			() const { return m_GunSelected; }
	inline int								GetStatusPlayer			() const { return m_life; }
	void											AddStatusPlayer			(int amount);
	CItem::ETYTE							GetTypeGun					();
	int												GetStatusGun				();
	GUN *											ChangeSelectedGun		();
	void											SetGuns							(std::vector<GUN> &guns);
	void											SetGunSelected			(CItem::ETYTE type);
	inline const Vect3f &			GetVectDir					() const { return m_VectDir; }
	void											SetVectDir					(Vect3f &v);
	inline bool								IsShot							() const { return m_bIsShot; }
	inline bool               IsGunReady          () const { return !m_bIsShot; }
	bool											Shot								();
private:
	friend										class CGameLogic;

	bool											m_bIsOk;			      // Initialization boolean control
	std::string								m_sName;
	bool											m_bIsShot;
	float											m_fSpeedForward; 
	float											m_fSpeed;
	Vect3f										m_MoveDirection,m_VectDir;
	int												m_life;
	std::vector<GUN>					m_vecGuns;
	GUN *											m_GunSelected;
	float											m_fTimerShotting;

	void											Release();
	void											SetMove(Vect3f vu,bool speed,float elapsedTime);
	void											Catch(CItemLife *item);
	void											Catch(CItemGun *item);
	void											Catch(CItemAmmo *item);
};
