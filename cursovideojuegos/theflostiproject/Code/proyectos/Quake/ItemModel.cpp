#include "__PCH_Quake.h"

#include "ItemModel.h"

CItemModel::CItemModel(void)
:m_bIsOk(false)
,m_pScale(NULL)
,m_pDesp(NULL)
,m_bDespY(true)
,m_bDebug(false)
{}

CItemModel::~CItemModel(void)
{
	Done();
}

void CItemModel::Done()
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

void CItemModel::Release()
{
	CHECKED_DELETE(m_pScale); 
}

bool CItemModel::Init()
{
	m_bIsOk=true;

	return m_bIsOk;
}
