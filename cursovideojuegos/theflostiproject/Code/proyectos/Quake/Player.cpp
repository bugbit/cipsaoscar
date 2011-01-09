#include "__PCH_Quake.h"

#include "Player.h"
#include "QuakePhysicsData.h"
#include "ItemLife.h"

//---Engine Includes--------
#include "Core/Core.h"
#include "PhysX/PhysicActor.h"
#include "PhysX/PhysicsManager.h"
#include "PhysX/PhysicController.h"

CPlayer::CPlayer(float radius, float height, float slope, float skinwidth, float stepOffset, uint32 collisionGroups, CQuakePhysicsData *data, 
								 const Vect3f& pos, float gravity)
:CPhysicController(radius, height, slope, skinwidth, stepOffset, collisionGroups, data,pos,gravity)
,m_bIsOk(false)
,m_fSpeedForward(3.5f)
,m_fSpeed(7.f)
,m_life(50)
,m_gunSelected(CItem::SHOTGUN)
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
}

bool CPlayer::Init()
{
	m_bIsOk=true;

	return m_bIsOk;
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

void CPlayer::Catch(CItem *item)
{
	CItemLife *itemlife=dynamic_cast<CItemLife *>(item);

	if (itemlife!=NULL)
		Catch(itemlife);
}

void CPlayer::Catch(CItemLife *item)
{
	AddStatusPlayer(item->GetAmountLife());
}