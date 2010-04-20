#ifndef _SPHERE_H
#define _SPHERE_H

#include <d3dx9.h>

class CSphere
{
private:
	float					m_YAW;
	float					m_Pitch;
	float					m_Roll;
	D3DXVECTOR3				m_Position;
	float					m_Radius;
public:
	CSphere(void);
	~CSphere(void);
	void Update(float ElapsedTime);
	void Render(LPDIRECT3DDEVICE9 Device);
};

#endif