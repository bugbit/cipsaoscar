#include "__PCH_Quake.h"

#include "assert.h"

#include "Item.h"
#include "ItemModel.h"
#include "QuakePhysicsData.h"

//---Engine Includes--------
#include "Core/Core.h"
#include "PhysX/PhysicActor.h"
#include "PhysX/PhysicsManager.h"

CItem::CItem()
:m_pPhysxData(NULL)
,m_pActor(NULL)
,m_pModel(NULL)
,m_fTimer(NULL)
,m_bSelected(false)
{
}

CItem::~CItem()
{
	CHECKED_DELETE(m_pPhysxData);
	CHECKED_DELETE(m_pActor);
}

void CItem::CreateActor(const Vect3f &pos)
{
	assert(m_pModel!=NULL);
	CPhysicsManager* physicManager = CCore::GetSingletonPtr()->GetPhysicManager();
	m_pPhysxData=new CQuakePhysicsData("trigger");
	m_pPhysxData->SetPaint(true);
	m_pActor=new CPhysicActor(m_pPhysxData);
	m_pActor->CreateBoxTrigger(m_pModel->GetSize(),GROUP_BASIC_PRIMITIVES);
	m_pActor->SetGlobalPosition(pos);
	physicManager->AddPhysicActor(m_pActor);
}
