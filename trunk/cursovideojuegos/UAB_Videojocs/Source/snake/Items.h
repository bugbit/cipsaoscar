#ifndef _SNAKE_ITEMS_H
#define _SNAKE_ITEMS_H

#include "../DebugPrintText2D.h"

enum ETYPEITEMS
{
	TYPEITEMS_BONUS,TYPEITEMS_WALL
};

class CItems
{
public:
	inline CItems() {} // Borrar cuando separe en clases los Items
	CItems(float posx,float posy,ETYPEITEMS type);
	~CItems(void);

	void		Render							(CDebugPrintText2D& printText2d);
	bool		IsCollision						(float posx,float posy);
	inline		void SetVisible(bool visible)
	{
		m_Visible=visible;
	}
	inline		ETYPEITEMS GetType() const { return m_Type; }

private:
	float				m_PosX,m_PosY;
	bool				m_Visible;
	ETYPEITEMS			m_Type;

};

#endif
