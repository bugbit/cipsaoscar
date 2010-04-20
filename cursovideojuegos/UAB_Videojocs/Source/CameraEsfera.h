#ifndef INC_CAMERASPHERIC_H_
#define INC_CAMERASPHERIC_H_

#include "Camera.h"


class CCameraEsfera : public CCamera
{

public:
	
	CCameraEsfera();
	~CCameraEsfera();
	
	//--- ADD FUNCTION ---
	void	AddZoom	( const float &amount ) { m_zoom += amount; }
	
	//--- SET FUNCTION ---
	void	SetZoom	( const float &amount ) { m_zoom = amount; }

	//--- GET FUNCTIONS ---
	D3DXVECTOR3 GetDirection	( void ) const { return m_pos - GetEye();}
	D3DXVECTOR3 GetLookAt		( void ) const { return m_pos; }
	D3DXVECTOR3 GetPos			( void ) const { return GetEye(); }
	D3DXVECTOR3 GetEye			( void ) const;
	D3DXVECTOR3 GetVecUp		( void ) const;
		
private:
	
	float	m_zoom;			//Distancia respecto el centro (0,0,0) a la que se encuentra la camara.
	float	m_speed_zoom;	//Velocidad a la que se acerca/aleja la camara del centro.
	float	m_speed_yaw;	//Velocidad a la que gira la camara respecto el angulo Yaw
	float	m_speed_pitch;	//Velocidad a la que gira la camara respecto el angulo Pitch
};

#endif // INC_CAMERASPHERIC_H_