#pragma once
#include "item.h"

class CItemGun :
	public CItem
{
public:
	CItemGun																(CItem::ETYTE type);
	virtual ~CItemGun												(void);
	inline					CItem::ETYTE GetTypeGun	() const { return m_TypeGun; }
protected:
	CItem::ETYTE														m_TypeGun;
};
