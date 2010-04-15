#ifndef INC_CAMERAFPSHOOTER_H_
#define INC_CAMERAFPSHOOTER_H_

#include "Camera.h"

class CCameraFPShooter : public CCamera 
{

public:

	CCameraFPShooter();
	~CCameraFPShooter();
	
	//--- MOVE FUNCTION ---
	void	Move (	int strafe, int forward, bool flag_speed, const float &dt);
		
	//--- ADD FUNCTIONS ---
	void	AddYaw		( const float &radians );
	void	AddPitch	( const float &radians );
	void	AddY		( const float &amount ) { m_pos.y = m_posY + amount; }
	
	//--- GET FUNCTIONS ---
	D3DXVECTOR3 GetPos			( void ) const { return m_pos; }
	D3DXVECTOR3 GetEye			( void ) const { return m_pos; }
	D3DXVECTOR3 GetDirection	( void ) const { return GetLookAt() - m_pos;}
	D3DXVECTOR3 GetLookAt		( void ) const;
	D3DXVECTOR3 GetVecUp		( void ) const;
	
private:

	float m_posY;			//Altura de la camara
	float m_speed_yaw;		//Velocidad a la que gira la camara respecto el angulo Yaw
	float m_speed_pitch;	//Velocidad a la que gita la camara respecto el angulo Pitch
	float m_speed;			//Velocidad a la que se desplaza lateralmente y adelante/atras la camara
	float m_speed_forward;	//Velocidad a la que se despleza hacia adelante/atras la camara
	float m_speed_strafe;	//Velocidad a la que se desplaza lateralmente la camara
};

#endif //INC_CAMERAFPSHOOTER_H_