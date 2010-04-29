#ifndef _SNAKE_ITEMS_BONUS_H
#define _SNAKE_ITEMS_BONUS_H

#include "Items.h"

class CItemsBonus : public CItems
{
public:
	CItemsBonus(float posx,float posy);
	CItemsBonus();
	~CItemsBonus(void);

	virtual		void		Render							(CDebugPrintText2D& printText2d);
	virtual		void		Update							(float dt);

	void		GeneratePosition();

private:

	float		m_TimePhoenix;
	bool		m_bIsPhoenix;

	void		Init							();

};

#endif
