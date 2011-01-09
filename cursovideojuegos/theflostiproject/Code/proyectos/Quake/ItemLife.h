#pragma once
#include "item.h"

class CItemLife :
	public CItem
{
public:
	CItemLife																	(void);
	virtual ~CItemLife												(void);
	inline int								GetAmountLife		() const { return mi_AmountLife; }
	inline void								SetAmountLife		(int amount) { mi_AmountLife=amount; }
protected:
	int												mi_AmountLife;
};
