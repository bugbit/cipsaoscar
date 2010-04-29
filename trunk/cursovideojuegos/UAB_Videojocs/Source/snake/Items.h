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
	CItems(float posx,float posy,ETYPEITEMS type);
	CItems(ETYPEITEMS type);
	~CItems(void);

	virtual		void		Render							(CDebugPrintText2D& printText2d)=0;
	virtual     void		Update							(float dt)=0;
	bool					IsCollision						(float posx,float posy);
	inline		void		SetVisible						(bool visible)
	{
		m_Visible=visible;
	}
	inline		ETYPEITEMS GetType() const { return m_Type; }

protected:
	float				m_PosX,m_PosY;
	bool				m_Visible;
	ETYPEITEMS			m_Type;

	virtual	void		Init							();

};

#endif
