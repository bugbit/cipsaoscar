#include "__PCH_Quake.h"

#include "ItemManager.h"

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
}
