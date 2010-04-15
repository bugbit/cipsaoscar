#include "TresEnRaya.h"

#include <math.h>

#define M_PI       3.14159265358979323846


CTresEnRaya::CTresEnRaya()
{
	m_iPosx = 0.f;
	m_fDirection = 1.f;
	m_Angle=0;
	m_r=100;
}

CTresEnRaya::~CTresEnRaya()
{

}

void CTresEnRaya::Render		(CDebugPrintText2D& printText2d)
{
	int dy = 320;
	float x,y;

	x=400+m_r*cos(m_Angle);
	y=400+m_r*sin(m_Angle);

	//dy += printText2d.PrintText((int)m_iPosx,dy,0xffffffff,"XXX");

	dy += printText2d.PrintText((int)x,(int) y,0xffffffff,"XXX");
}

void CTresEnRaya::Update		(float dt)
{
	if (m_iPosx<=0) 
	{
		m_fDirection = 1;
	}
	else if(m_iPosx>=400)
	{
		m_fDirection = -1;
	}

	if (dt!=0.0)
		m_Angle += M_PI/(4000*dt*180.0);

	if (m_Angle>2*M_PI)
		m_Angle=m_Angle-2*M_PI;

	m_iPosx +=dt*40*m_fDirection;//10pixeles por segundo
}