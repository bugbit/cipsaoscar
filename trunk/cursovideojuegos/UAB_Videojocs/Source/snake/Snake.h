#ifndef _SNAKE_H
#define _SNAKE_H

#include <vector>

#include "../DebugPrintText2D.h"

#define BODY_SIZE 10.f
#define MOVE_TIME 0.1f

struct SBody
{
	float	m_fPosX;
	float	m_fPosY;
};
typedef enum Direction {DIR_RIGHT, DIR_LEFT, DIR_UP, DIR_DOWN, DIR_NOTHING};

class CSnake
{
public:
	CSnake(float posx,float posy,int nsnake);
	~CSnake(void);

	void SetDirection(Direction direction);

	void		Render							(CDebugPrintText2D& printText2d);
	void		Update							(float dt);

	inline SBody		GetBodyHead() const
	{
		return m_Snake[0];
	}

	inline int			GetNSnake() const { return m_NSnake; }

	inline void		Move() { m_bMove=true; }
	inline void		Grow() { m_bGrow=true; }
	bool		IsCollision						(float posx,float posy);

private:

	std::vector<SBody>	m_Snake;

	Direction						m_Direction;
	Direction						m_DirectionOld;
	bool								m_bMove;
	bool								m_bGrow;
	float								m_fMoveTime;
	int									m_NSnake;

	Direction			GetDireccionContraria(const Direction &) const;
	
};

#endif
