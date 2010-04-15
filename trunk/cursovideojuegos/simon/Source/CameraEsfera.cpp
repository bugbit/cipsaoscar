#include <math.h>
#include <stdio.h>

#include "CameraEsfera.h"


//--------CONSTRUCTOR & DESTRUCTOR-------------//
CCameraEsfera::CCameraEsfera() 
{
	m_id			= ESFERA_CAMERA;
	m_zoom			= 50.f;
	m_pos			= D3DXVECTOR3(0.f, 0.f ,0.f);
	m_yaw			= 0.f;
	m_pitch			= 0.f;             
	m_view_d		= 2.f;
	m_fov_radians	= D3DXToRadian( 50.f );
	m_aspect_ratio	= 640.0f / 480.0f;
	m_zn			= 0.1f;
	m_zf			= 100.f;
	m_speed_zoom	= 2.f;
	m_speed_yaw		= 20.f;
	m_speed_pitch	= 20.f;
}

CCameraEsfera::~CCameraEsfera()
{
	
}
//---------------------------------------------//




//------------GET FUNCTIONS--------------------//
D3DXVECTOR3 CCameraEsfera::GetEye(void) const
{
	//Pasamos de coordenadas esfericas a coordenadas cartesianas
	D3DXVECTOR3 vEyePt( m_zoom * cos(m_yaw) * cos(m_pitch), 
						m_zoom * sin(m_pitch),
						m_zoom * sin(m_yaw) * cos(m_pitch) );

	return -vEyePt; 
}

D3DXVECTOR3 CCameraEsfera::GetVecUp(void) const
{
	D3DXVECTOR3 vUpVec( -cos(m_yaw) * sin(m_pitch), 
						 cos(m_pitch), 
						-sin(m_yaw) * sin(m_pitch) );
	
	return vUpVec;
}

//---------------------------------------------//





