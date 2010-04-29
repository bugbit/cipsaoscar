#ifndef _SNAKE_ITEMS_ITEMS_H
#define _SNAKE_ITEMS_ITEMS_H

#include "Items.h"

class CItemsWall : public CItems
{
public:
	CItemsWall(float posx,float posy);
	~CItemsWall(void);

	virtual		void		Render							(CDebugPrintText2D& printText2d);
	virtual		void		Update							(float dt);
};

#endif
