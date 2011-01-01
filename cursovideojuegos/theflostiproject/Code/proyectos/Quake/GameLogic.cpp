#include "__PCH_Quake.h"

#include "GameLogic.h"

//----Engine Includes-------------
#include "Core/Core.h"
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
}

void CGameLogic::OnLeave(CPhysicUserData* trigger1, CPhysicUserData* other_shape)
{
}