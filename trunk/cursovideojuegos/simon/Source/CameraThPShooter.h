#ifndef INC_CAMERATHPSHOOTER_H_
#define INC_CAMERATHPSHOOTER_H_

#include "Camera.h"


class CCameraThPShooter : public CCamera 
{

public:

	CCameraThPShooter();
	~CCameraThPShooter();

	//--- MOVE FUNCTION ---
	void	Move (	int strafe, int forward, bool flag_speed, const float &dt);
	
	//--- ADD FUNCTION ---
	void	AddPitch		( const float &radian );
	void	AddYaw			( const float &radian ) { m_yaw -= D3DXToRadian(radian * m_speed_yaw); }
	void	AddDist2Player	( const float &amount ) { m_dist2player += amount * m_speed_zoom; }

	//--- SET FUNCTION ---
	void	SetDist2Player	( const float &amount ) { m_dist2player = amount; }
	
	//--- GET FUNCTIONS ---
	D3DXVECTOR3	GetPos			( void ) const { return GetEye(); }
	D3DXVECTOR3 GetDirection	( void ) const { return m_pos - GetEye();}
	D3DXVECTOR3 GetLookAt		( void ) const { return m_pos; }
	D3DXVECTOR3 GetEye			( void ) const;
	D3DXVECTOR3 GetVecUp		( void ) const;

private:
	
	float	m_dist2player;		//Distancia de la camara respecto su centro 
	float	m_speed_yaw;		//Velocidad a la que gira la camara respecto el angulo Yaw
	float	m_speed_pitch;		//Velocidad a la que gira la camara respecto el angulo Pitch
	float	m_speed;			//Velocidad a la que se desplaza lateralmente y adelante/atras la camara
	float	m_speed_forward;	//Velocidad a la que se despleza hacia adelante/atras la camara
	float	m_speed_strafe;		//Velocidad a la que se desplaza lateralmente la camara
	float	m_speed_zoom;		//Velocidad a la que se acerca/aleja la camara del centro

};
#endif //INC_CAMERATHPSHOOTER_H_
		
