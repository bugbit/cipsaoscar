#include "CameraFPShooter.h"

//--------CONSTRUCTOR & DESTRUCTOR-------------//
CCameraFPShooter::CCameraFPShooter() 
{
	m_id			= FPS_CAMERA;
	m_posY			= 2.f;
	m_pos			= D3DXVECTOR3(0.f, m_posY, 0.f);
	m_yaw			= 0.f;
	m_pitch			= 0.f;
	m_view_d		=  2.f;
	m_fov_radians	= D3DXToRadian( 50.f );
	m_aspect_ratio	= 640.f / 480.f;
	m_zn			= 0.1f;
	m_zf			= 1000.f;
	m_speed_yaw		=  100.f;
	m_speed_pitch	= 60.f;
	m_speed_forward = 10.f;
	m_speed_strafe	= 5.f;
	m_speed			= 2.f;
}

CCameraFPShooter::~CCameraFPShooter()
{
	
}
//---------------------------------------------//



//------------MOVE FUNCTION---------------------//
void CCameraFPShooter::Move( int strafe, int forward, bool flag_speed, const float &dt)
{
	D3DXVECTOR3 addPos;
	addPos.y =  0.0f;
	addPos.x =	forward * (  cos(m_yaw) )+
				strafe  * (  cos(m_yaw+ D3DX_PI * 0.5f) );
				

	addPos.z =	forward * (  sin(m_yaw) )+
				strafe  * (  sin(m_yaw+ D3DX_PI * 0.5f) );
				
	
	D3DXVec3Normalize(&addPos, &addPos);
	float constant = dt * m_speed_forward;
	if( flag_speed )
		constant *= m_speed;
	addPos *= constant;
	m_pos += addPos;
}


//------------ADD FUNCTIONS---------------------//
void CCameraFPShooter::AddYaw( const float &degree ) 
{
	m_yaw -= D3DXToRadian(degree * m_speed_yaw);
	
	//Para controlar que el valor de m_yaw siempre este acotado
	if( m_yaw > 2*D3DX_PI)
		m_yaw -= 2*D3DX_PI;	
	else if( m_yaw < -2*D3DX_PI ) 
		m_yaw += 2*D3DX_PI;

}

void CCameraFPShooter::AddPitch( const float &radian ) 
{
	float degrees = D3DXToRadian(radian * m_speed_pitch);
	//Hemos de limitar el ángulo yaw entre 90º  y -90º
	if( (m_pitch - degrees <  (D3DX_PI*0.5)) &&  
		(m_pitch - degrees > -(D3DX_PI*0.5)) )
	{	
		m_pitch -= degrees;
	}
}
//---------------------------------------------//



//------------GET FUNCTIONS--------------------//
D3DXVECTOR3 CCameraFPShooter::GetLookAt(void) const
{	
	//Pasamos de coordenadas esfericas a coordenadas cartesianas
	D3DXVECTOR3 look( cos(m_yaw) * cos(m_pitch), 
					  sin(m_pitch),
					  sin(m_yaw) * cos(m_pitch) );

	return m_pos + look;
}

D3DXVECTOR3 CCameraFPShooter::GetVecUp(void) const
{
	D3DXVECTOR3 vUpVec( -cos(m_yaw) * sin(m_pitch), 
						 cos(m_pitch), 
						-sin(m_yaw) * sin(m_pitch) );
	
	return vUpVec;
}
//---------------------------------------------//


