#include "__PCH_Quake.h"

#include "Player.h"
#include "QuakePhysicsData.h"
#include "ItemLife.h"
#include "ItemGun.h"
#include "ItemAmmo.h"

//---Engine Includes--------
#include "Core/Core.h"
#include "PhysX/PhysicActor.h"
#include "PhysX/PhysicsManager.h"
#include "PhysX/PhysicController.h"

CPlayer::CPlayer(float radius, float height, float slope, float skinwidth, float stepOffset, uint32 collisionGroups, CQuakePhysicsData *data, 
								 const Vect3f& pos, float gravity)
:CPhysicController(radius, height, slope, skinwidth, stepOffset, collisionGroups, data,pos,gravity)
,m_bIsOk(false)
,m_bIsShot(false)
,m_fSpeedForward(3.5f)
,m_fSpeed(7.f)
,m_life(50)
//,m_GunSelected((GUN *) &CItemTypeManager::GetMachineGun())
,m_GunSelected(NULL)
,m_fTimerShotting(0.f)
{
	data->SetObject3D(this);
}

CPlayer::~CPlayer(void)
{
	Done();
}

void CPlayer::Done()
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

void CPlayer::Release()
{
	CQuakePhysicsData *data=(CQuakePhysicsData *) GetUserData();
	CHECKED_DELETE(data);
	m_vecGuns.clear();
}

bool CPlayer::Init()
{
	m_bIsOk=true;

	return m_bIsOk;
}

void CPlayer::Update(float elapsedTime)
{
	if (m_bIsShot)
	{
		m_fTimerShotting -= elapsedTime;
		if (m_fTimerShotting<0)
			m_bIsShot=false;
	}
}

void CPlayer::SetCleanMove()
{
	m_MoveDirection=v3fZERO;
}

void CPlayer::SetMove(Vect3f vu,bool speed,float elapsedTime)
{
	float fspeed=(speed) ? m_fSpeed : m_fSpeedForward;
	m_MoveDirection=fspeed*elapsedTime*vu;
}

void CPlayer::SetMoveUp(bool speed,float elapsedTime)
{
	SetMove(Vect3f(	cos(m_fYaw), 0, sin(m_fYaw) ),speed,elapsedTime);
}

void CPlayer::SetMoveDown(bool speed,float elapsedTime)
{
	SetMove(Vect3f(	-cos(m_fYaw), 0, -sin(m_fYaw) ),speed,elapsedTime);
}

void CPlayer::SetMoveLeft(bool speed,float elapsedTime)
{
	SetMove(Vect3f(	sin(m_fYaw), 0, cos(m_fYaw) ),speed,elapsedTime);
}

void CPlayer::SetMoveRight(bool speed,float elapsedTime)
{
	SetMove(Vect3f(	-sin(m_fYaw), 0, -cos(m_fYaw) ),speed,elapsedTime);
}

void CPlayer::Move(float elapsedTime)
{
	CPhysicController::Move(m_MoveDirection,elapsedTime);
}

void CPlayer::AddStatusPlayer(int amount)
{
	if( m_life + amount > 100)
	{
		m_life = 100;
	}
	else if( m_life + amount < 0)
	{
		m_life = 0;
	}
	else m_life += amount; 
	
	if( amount < 0 )
	{
		// Activar muerte del player
	}
}

void CPlayer::ChangeSelectedGun()
{
	GUN *gun=NULL;

	if (m_GunSelected!=NULL)
	{
		std::vector<GUN>::iterator it=m_vecGuns.begin(),itend=m_vecGuns.end();
		for (;it!=itend;it++)
		{
			GUN *gun_i=&*it;
			if (gun_i->type==m_GunSelected->type)
			{
				it++;
				break;
			}
			else
				if (gun_i->selected)
				{
					if (gun==NULL)
						gun=gun_i;
				}
		}
		for (;it!=itend;it++)
		{
			GUN *gun_i=&*it;
			if (gun_i->selected)
			{
				gun=gun_i;
				break;
			}
		}
		if (gun!=NULL)
			m_GunSelected=gun;
	}
}

void CPlayer::Catch(CItem *item)
{
	CItemLife *itemlife=dynamic_cast<CItemLife *>(item);

	if (itemlife!=NULL)
		Catch(itemlife);
	else
	{
		CItemAmmo *itemammo=dynamic_cast<CItemAmmo *>(item);
		if (itemammo!=NULL)
			Catch(itemammo);
		else
		{
			CItemGun *itemgun=dynamic_cast<CItemGun *>(item);
			if (itemgun!=NULL)
				Catch(itemgun);
		}
	}
}

void CPlayer::Catch(CItemLife *item)
{
	AddStatusPlayer(item->GetAmountLife());
}

void CPlayer::Catch(CItemGun *item)
{
	SetGunSelected(item->GetTypeGun());
}

void CPlayer::Catch(CItemAmmo *item)
{
	std::vector<GUN>::iterator it=m_vecGuns.begin(),itend=m_vecGuns.end();
	CItem::ETYTE type=item->GetTypeGun();

	for(;it!=itend;it++)
	{
		GUN &gun=*it;
		if (gun.type==type)
			gun.gunState += item->GetGunState();
	}
}

CItem::ETYTE CPlayer::GetTypeGun()
{
	if (m_GunSelected==NULL)
		return CItem::NONE;

	return m_GunSelected->type;
}


int CPlayer::GetStatusGun()
{
	if (m_GunSelected==NULL)
		return 0;

	return m_GunSelected->gunState;
}

void CPlayer::SetGuns(std::vector<GUN> &guns)
{
	std::vector<GUN>::iterator it=guns.begin(),itend=guns.end();

	m_vecGuns.clear();
	for(;it!=itend;it++)
		m_vecGuns.push_back(*it);
}

void CPlayer::SetGunSelected(CItem::ETYTE type)
{
	std::vector<GUN>::iterator it=m_vecGuns.begin(),itend=m_vecGuns.end();

	for(;it!=itend;it++)
	{
		GUN &gun=*it;
		if (gun.type==type)
		{
			gun.selected=true;
			m_GunSelected=&gun;
		}
	}
}

void CPlayer::SetVectDir(Vect3f &v)
{
	m_VectDir=v.Normalize();
}

void CPlayer::Shot()
{
	if (GetStatusGun()>0)
	{
		if (!m_bIsShot)
		{
			m_bIsShot=true;
			m_GunSelected->gunState--;
			m_fTimerShotting=m_GunSelected->timeShot;
		}
	}
}