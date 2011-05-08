#include "__PCH_Quake.h"

#include "GameLogic.h"
#include "QuakePhysicsData.h"
#include "Player.h"
#include "Item.h"
#include "ItemLife.h"
#include "ItemGun.h"
#include "ItemAmmo.h"

//----Engine Includes-------------
#include "Core/Core.h"
#include "Logger/Logger.h"
#include "Sound/SoundManager.h"
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
			Catch(item,*player);
		}
	}
}

void CGameLogic::OnLeave(CPhysicUserData* trigger1, CPhysicUserData* other_shape)
{
}

void CGameLogic::Catch(CItem *item,CPlayer &player)
{
	CItemLife *itemlife=dynamic_cast<CItemLife *>(item);

	if (itemlife!=NULL)
	{
		player.Catch(itemlife);
		CORE->GetSoundManager()->PlayAction2D("getLife");
	}
	else
	{
		CItemAmmo *itemammo=dynamic_cast<CItemAmmo *>(item);
		if (itemammo!=NULL)
		{
			player.Catch(itemammo);
			CORE->GetSoundManager()->PlayAction2D("getGun");
		}
		else
		{
			CItemGun *itemgun=dynamic_cast<CItemGun *>(item);
			if (itemgun!=NULL)
			{
				player.Catch(itemgun);
				CORE->GetSoundManager()->PlayAction2D("getGun");
			}
		}
	}
}

void CGameLogic::Shot(CPlayer &player)
{
	GUN *gun=player.GetGunSelected();
	std::string sound;
	
	if (gun==NULL)
		CORE->GetSoundManager()->PlayAction2D("noAmmo");
	else if (player.IsGunReady())
	{
		if (player.Shot())
		{			
			switch (gun->type)
			{
				case CItem::MACHINEGUN:
				case CItem::SHOTGUN:
					ShotRay(*gun,player);
					break;
				case CItem::ROCKETL:
					//ShotOverlapSphere(gun,player);
					break;
			}
			sound=CItemTypeManager::GetInstance().GetSound(gun->type);
			if (!sound.empty())
				CORE->GetSoundManager()->PlayAction2D(sound);
		}
		else
			CORE->GetSoundManager()->PlayAction2D("noAmmo");
	}
}

void CGameLogic::ChangeSelectedGun(CPlayer &player)
{
	GUN *gun=player.ChangeSelectedGun();

	if (gun!=NULL)
		CORE->GetSoundManager()->PlayAction2D("changeGun");
}

void CGameLogic::ShotRay(GUN &gun,CPlayer &player)
{
	SCollisionInfo info;
	Vect3f dir=player.GetVectDir();
	Vect3f pos=player.GetPosition()+.6f*dir;
	CQuakePhysicsData *data=(CQuakePhysicsData *) CORE->GetPhysicManager()->RaycastClosestActor(pos,dir,IMPACT_MASK_1,NULL,info);
	if (data!=NULL)
	{
		if(info.m_fDistance<=gun.distshot)
			LOGGER->AddNewLog(ELL_INFORMATION, "CGameLogic::ShotRay: Impacto en %s dist: %f", data->GetName().c_str(),info.m_fDistance);
	}
}

void CGameLogic::ShotOverlapSphere	(CItem::ETYTE argType,CPlayer &player)
{
}