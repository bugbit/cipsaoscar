#include "__PCH_Quake.h"

#include "assert.h"

#include "Item.h"
#include "ItemModel.h"
#include "QuakePhysicsData.h"

//---Engine Includes--------
#include "Core/Core.h"
#include "PhysX/PhysicActor.h"
#include "PhysX/PhysicsManager.h"

const std::string CItem::NameLife="life";
const std::string CItem::NameShotGun="shotgun";
const std::string CItem::NameRocketl="rocketl";
const std::string CItem::NameMachineGun="machinegun";
const std::string CItem::NameAmmoShotGun="ammoshotgun";
const std::string CItem::NameAmmoRocketl="ammorocketl";
const std::string CItem::NameAmmoMachineGun="ammomachinegun";

CItem::CItem()
:m_pPhysxData(NULL)
,m_pActor(NULL)
,m_pModel(NULL)
,m_fTimer(NULL)
,m_bSelected(false)
,m_fCounterTimer(0.f)
{
}

CItem::~CItem()
{
	ReleaseActor();
}

void CItem::ReleaseActor()
{
	CCore::GetSingletonPtr()->GetPhysicManager()->ReleasePhysicActor(m_pActor);
	CHECKED_DELETE(m_pPhysxData);
	CHECKED_DELETE(m_pActor);
	m_pPhysxData=NULL;
	m_pActor=NULL;
}

void CItem::CreateActor(const Vect3f &pos)
{
	assert(m_pModel!=NULL);
	CPhysicsManager* physicManager = CCore::GetSingletonPtr()->GetPhysicManager();
	m_pPhysxData=new CQuakePhysicsData("trigger",this);
	m_pPhysxData->SetPaint(m_pModel->GetDebug());
	m_pActor=new CPhysicActor(m_pPhysxData);
	m_pActor->CreateBoxTrigger(m_pModel->GetSize(),GROUP_BASIC_PRIMITIVES);
	SetPosition(pos);
	physicManager->AddPhysicActor(m_pActor);
}

void CItem::ReCreateActor()
{
	const Vect3f pos=GetPosition();
	ReleaseActor();
	CreateActor(pos);
}

const Vect3f & CItem::GetPosition	()
{
	if (m_pActor!=NULL)
		CObject3D::SetPosition(m_pActor->GetPosition());

	return (Vect3f &) CObject3D::GetPosition();
}

void CItem::SetPosition	( const Vect3f& pos )
{
	CObject3D::SetPosition(pos);
	if (m_pActor!=NULL)
		m_pActor->SetGlobalPosition(pos);
}

void CItem::SetSelected	(bool selected) 
{
	if (selected)
		m_fCounterTimer=0.f;
	m_bSelected=selected;
}

void CItem::Update(float elapsedTime)
{
	if (m_bSelected)
	{
		m_fCounterTimer += elapsedTime;
		if (m_fCounterTimer>m_fTimer)
		{
			m_fCounterTimer=0.f;
			m_bSelected=false;
		}
	}
}

CItemTypeManager::CItemTypeManager()
{
	m_MapTypes[CItem::NameLife]=CItem::LIFE;
	m_MapTypes[CItem::NameShotGun]=CItem::SHOTGUN;
	m_MapTypes[CItem::NameRocketl]=CItem::ROCKETL;
	m_MapTypes[CItem::NameMachineGun]=CItem::MACHINEGUN;
	m_MapTypes[CItem::NameAmmoShotGun]=CItem::AMMOSHOTGUN;
	m_MapTypes[CItem::NameAmmoRocketl]=CItem::AMMOROCKETL;
	m_MapTypes[CItem::NameAmmoMachineGun]=CItem::AMMOMACHINEGUN;
	m_MapTypesNameEtype[CItem::LIFE]=CItem::NameLife;
	m_MapTypesNameEtype[CItem::SHOTGUN]=CItem::NameShotGun;
	m_MapTypesNameEtype[CItem::ROCKETL]=CItem::NameRocketl;
	m_MapTypesNameEtype[CItem::MACHINEGUN]=CItem::NameMachineGun;
	m_MapTypesNameEtype[CItem::AMMOSHOTGUN]=CItem::NameAmmoShotGun;
	m_MapTypesNameEtype[CItem::AMMOROCKETL]=CItem::NameAmmoRocketl;
	m_MapTypesNameEtype[CItem::AMMOMACHINEGUN]=CItem::NameAmmoMachineGun;
	m_MapGunAmmo[CItem::SHOTGUN]=CItem::AMMOSHOTGUN;
	m_MapGunAmmo[CItem::ROCKETL]=CItem::AMMOROCKETL;
	m_MapGunAmmo[CItem::MACHINEGUN]=CItem::AMMOMACHINEGUN;
}

CItemTypeManager & CItemTypeManager::GetInstance()
{
	static CItemTypeManager m_Instance;

	return m_Instance;
}

CItem::ETYTE CItemTypeManager::GetTypeForName(std::string name)
{
	std::map<std::string,CItem::ETYTE>::iterator it=m_MapTypes.find(name);

	if (it==m_MapTypes.end())
		return CItem::NONE;
	return (*it).second;
}

std::string CItemTypeManager::GetNameForType	(CItem::ETYTE type)
{
	std::map<CItem::ETYTE,std::string>::iterator it=m_MapTypesNameEtype.find(type);

	if (it==m_MapTypesNameEtype.end())
		return "";
	return (*it).second;
}

CItem::ETYTE CItemTypeManager::GetAmmo(CItem::ETYTE gun)
{
	std::map<CItem::ETYTE,CItem::ETYTE>::iterator it=m_MapGunAmmo.find(gun);

	if (it==m_MapGunAmmo.end())
		return CItem::NONE;

	return (*it).second;
}