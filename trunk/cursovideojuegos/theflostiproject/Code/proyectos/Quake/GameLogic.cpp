#include "__PCH_Quake.h"

#include "GameLogic.h"
#include "QuakePhysicsData.h"
#include "Player.h"
#include "Item.h"
#include "Logger/Logger.h"

//----Engine Includes-------------
#include "Core/Core.h"
//Includes para el test de físicas:
#include "PhysX/PhysicsManager.h"

CGameLogic::CGameLogic(void)
{	
}

CGameLogic::~CGameLogic(void)
{
	Done();
}

void CGameLogic::Done()
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

bool CGameLogic::Init()
{
	CPhysicsManager* physicManager = CORE->GetPhysicManager();
	assert(physicManager);
	physicManager->SetTriggerReport(this);
	m_bIsOk=true;

	return m_bIsOk;
}

void CGameLogic::Release()
{
	m_Arena.Done();
}

void CGameLogic::Update(float elapsedTime)
{
	m_Arena.Update(elapsedTime);
}

void CGameLogic::RenderScene(CRenderManager* renderManager, CFontManager* fontManager)
{
	m_Arena.RenderScene(renderManager,fontManager);
}

void CGameLogic::OnEnter(CPhysicUserData* trigger1, CPhysicUserData* other_shape)
{
	CQuakePhysicsData *t=static_cast<CQuakePhysicsData *>(trigger1);
	CQuakePhysicsData *s=static_cast<CQuakePhysicsData *>(other_shape);
	CObject3D *to=t->GetObject3D();
	CObject3D *so=s->GetObject3D();
	
	// El player ha cogido un item
	CPlayer *player=dynamic_cast<CPlayer *>(so);
	CItem *item=dynamic_cast<CItem *>(to);
	if (player!=NULL && item!=NULL)
	{
		if (!item->GetSelected())
		{
			item->SetSelected(true);
			player->Catch(item);
		}
	}
}

void CGameLogic::OnLeave(CPhysicUserData* trigger1, CPhysicUserData* other_shape)
{
}

void CGameLogic::Shot(CPlayer &player)
{
	if (player.IsGunReady())
	{
		if (player.Shot())
		{
			CItem::ETYTE type=player.GetTypeGun();
			switch (type)
			{
				case CItem::MACHINEGUN:
				case CItem::SHOTGUN:
					ShotRay(type,player);
					break;
				case CItem::ROCKETL:
					ShotOverlapSphere(type,player);
					break;
			}
		}
	}
}

void CGameLogic::ShotRay(CItem::ETYTE argType,CPlayer &player)
{
	SCollisionInfo info;
	Vect3f dir=player.GetVectDir();
	Vect3f pos=player.GetPosition()+.6f*dir;
	CQuakePhysicsData *data=(CQuakePhysicsData *) CORE->GetPhysicManager()->RaycastClosestActor(pos,dir,IMPACT_MASK_1,NULL,info);
	if (data!=NULL)
	{
		LOGGER->AddNewLog(ELL_INFORMATION, "CGameLogic::ShotRay: Impacto en %s", data->GetName().c_str());
	}
}

void CGameLogic::ShotOverlapSphere	(CItem::ETYTE argType,CPlayer &player)
{
}