#ifndef _SNAKE_H
#define _SNAKE_H

#include <vector>

#include "../DebugPrintText2D.h"

struct SBody
{
	float	m_fPosX;
	float	m_fPosY;
};
typedef enum Direction {DIR_RIGHT, DIR_LEFT, DIR_UP, DIR_DOWN, DIR_NOTHING};

class CSnake
{
public:
	CSnake(void);
	~CSnake(void);

	void		Render							(CDebugPrintText2D& printText2d);
	void		Update							(float dt);

	inline SBody		GetBodyHead() const
	{
		return m_Snake[0];
	}

	inline void     SetDirection(Direction direction)
	{
		m_Direction=direction;
	}

	inline void		Move() { m_bMove=true; }
	inline void		Grow() { m_bGrow=true; }

private:

	std::vector<SBody>	m_Snake;

	void		UpdateInputActions	(float dt);
	Direction						m_Direction;
	bool								m_bMove;
	bool								m_bGrow;
	
};

#endif
