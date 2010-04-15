#include "CameraThPShooter.h"


//--------CONSTRUCTOR & DESTRUCTOR-------------//
CCameraThPShooter::CCameraThPShooter() 
{
	m_id			= THPS_CAMERA;
	m_dist2player	= 2.f;
	m_pos			= D3DXVECTOR3(0.f, 1.f, 0.f);
	m_yaw			= 0.f;
	m_pitch			= 0.f;
	m_view_d		= 2.f;
	m_fov_radians	= D3DXToRadian( 50.f );
	m_aspect_ratio	= 640.0f / 480.0f;
	m_zn			= 1.f;
	m_zf			= 100.f;
	m_speed_yaw		= 20.f;
	m_speed_pitch	= -20.f;
	m_speed			= 2.f;
	m_speed_forward	= 10.f;
	m_speed_strafe	= 5.f;
	m_speed_zoom	= 2.f;
}

CCameraThPShooter::~CCameraThPShooter()
{
}
//---------------------------------------------//



//------------MOVE FUNCTION---------------------//
void CCameraThPShooter::Move( int strafe, int forward, bool flag_speed, const float &dt)
{
	D3DXVECTOR3 addPos;
	addPos.y = 0.f;
	addPos.x =	forward * (  cos(m_yaw) )+
				strafe  * (  cos(m_yaw+ D3DX_PI * 0.5f) );
				

	addPos.z =	forward * (  sin(m_yaw) )+
				strafe  * (  sin(m_yaw+ D3DX_PI * 0.5f) );
	
	float constant = dt * m_speed_forward;
	if( flag_speed )
		constant *= m_speed;
	addPos *= constant;
	m_pos += addPos;
}
//---------------------------------------------//



//------------ADD FUNCTIONS---------------------//
void CCameraThPShooter::AddPitch( const float &radian ) 
{
	float degrees = D3DXToRadian( radian * m_speed_pitch );
	//Hemos de limitar el ángulo yaw entre 90º  y -90º
	if( (m_pitch + degrees <  (D3DX_PI*0.5)) &&  
		(m_pitch + degrees > -(D3DX_PI*0.5)) )
	{	
		m_pitch += degrees;
	}
}
//---------------------------------------------//


//------------------GET FUNCTIONS--------------------//
D3DXVECTOR3 CCameraThPShooter::GetEye( void ) const
{
	//Pasamos de coordenadas esfericas a coordenadas cartesianas
	D3DXVECTOR3 vEyePt(  m_dist2player * cos(m_yaw) * cos(m_pitch), 
						 m_dist2player * sin(m_pitch),
						 m_dist2player * sin(m_yaw) * cos(m_pitch) );

	return -vEyePt + m_pos; 
}

D3DXVECTOR3 CCameraThPShooter::GetVecUp( void ) const
{
		
	D3DXVECTOR3 vUpVec( -cos(m_yaw) * sin(m_pitch), 
						 cos(m_pitch), 
						-sin(m_yaw) * sin(m_pitch) );
		
	return vUpVec;
}
//---------------------------------------------//




