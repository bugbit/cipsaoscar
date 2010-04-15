#ifndef _3_EN_RAYA_H
#define _3_EN_RAYA_H

#include "DebugPrintText2D.h"

class CTresEnRaya
{
public:
	CTresEnRaya();
	~CTresEnRaya();

	void		Render		(CDebugPrintText2D& printText2d);
	void		Update		(float dt);

private:

	float	m_iPosx;
	float m_fDirection;
	float m_Angle;
	float m_r;

};

#endif //_3_EN_RAYA_H