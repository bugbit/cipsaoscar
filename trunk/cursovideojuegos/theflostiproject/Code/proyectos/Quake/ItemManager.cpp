#include "__PCH_Quake.h"

#include "ItemManager.h"
#include "itemmodel.h"
#include "Item.h"

//---Engine Includes--------
#include "Graphics/RenderManager.h"
#include "Math/Matrix44.h"
#include "PhysX/PhysicActor.h"

const std::string CItemManager::NameLife="LIFE";

CItemManager::CItemManager(void)
:m_bIsOk(false)
{
}

CItemManager::~CItemManager(void)
{
	Done();
}

void CItemManager::Done()
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

bool CItemManager::Init()
{
	m_bIsOk=true;

	return m_bIsOk;
}

void CItemManager::Release()
{
	ReleaseItems();
	ReleaseModels();
}

void CItemManager::ReleaseModels()
{
	std::map<CItemManager::ETYTE,CItemModel *>::iterator it=m_Models.begin(),itend=m_Models.end();
	
	for(;it!=itend;it++)
	{
		CItemModel *itemmanager=(*it).second;
		CHECKED_DELETE(itemmanager);
	}
	m_Models.clear();
}

void CItemManager::ReleaseItems()
{
	std::vector<CItem *>::iterator it=m_Items.begin(),itend=m_Items.end();
	
	for(;it!=itend;it++)
	{
		CItem *item=*it;
		CHECKED_DELETE(item);
	}
	m_Items.clear();
}

void CItemManager::Update(float elapsedTime)
{
}

void CItemManager::RenderScene(CRenderManager* renderManager, CFontManager* fontManager)
{
	std::vector<CItem *>::iterator it=m_Items.begin(),itend=m_Items.end();
	
	for(;it!=itend;it++)
	{
		CItem *item=*it;
		Mat44f mat;
		item->GetActor().GetMat44(mat);
		renderManager->SetTransform(mat);
		item->GetModel().RenderModel(renderManager,fontManager);
	}
}
