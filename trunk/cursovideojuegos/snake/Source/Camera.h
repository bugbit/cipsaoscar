#ifndef INC_CAMERA_H_
#define INC_CAMERA_H_

#include <d3dx9.h>
#include "defines.h"

class CCamera
{

public:

	CCamera(){};
	virtual ~CCamera(){};

	// Render (para debug) de la posicion y la orientacion de la camara
	void	RenderCamera	( LPDIRECT3DDEVICE9 device , LPD3DXMESH	box);

	//--- GET FUNCTIONS ---
	virtual D3DXVECTOR3 GetDirection	( void ) const = 0;
	virtual D3DXVECTOR3 GetLookAt		( void ) const = 0;
	virtual D3DXVECTOR3 GetEye			( void ) const = 0;
	virtual D3DXVECTOR3 GetVecUp		( void ) const = 0;
	virtual D3DXVECTOR3 GetPos			( void ) const = 0;
	float				GetYaw			( void ) const { return m_yaw; }
	float				GetPitch		( void ) const { return m_pitch; }
	CamerType			GetId			( void ) const { return m_id; }
	D3DXMATRIX			GetMatrixView	( void );
	D3DXMATRIX			GetMatrixProj	( void );
	
	//--- SET FUNCTIONS ---
	void	SetYaw		( const float &new_yaw )		{ m_yaw  = new_yaw; }
	void	SetPitch	( const float &new_pitch )		{ m_pitch  = new_pitch; }
	void	SetPos		( const D3DXVECTOR3 &new_pos )	{ m_pos = new_pos; }
	void	SetZn		( const float &amount )			{ m_zn = amount; }
	
	//--- ADD FUNCTIONS ---
	void	AddPos		( const D3DXVECTOR3 &position ) { m_pos += position; }
	void	AddYaw		( const float &radian )			{ m_yaw += D3DXToRadian(radian); }
	void	AddPitch	( const float &radian )			{ m_pitch += D3DXToRadian(radian); }
	void	AddZf		( const float &amount )			{ m_zf += amount; }
	void	AddFov		( const float &delta_fov )		{ m_fov_radians += delta_fov; }
	void	AddViewD	( const float &amount )			{ if( m_view_d + amount > 1) m_view_d += amount; }

	//--- AUXILIARY FUNCTIONS ---
	D3DXVECTOR3			Yaw2vector		( const float &yaw_angle ) const;
	D3DXVECTOR3			YawPitch2vector ( const float &yaw_angle, const float &pitch_angle ) const;		


protected:
	
	CamerType		m_id;			
	D3DXVECTOR3		m_pos;			// posicion de la camara
	float			m_yaw;			// angulo yaw de la camara
	float			m_pitch;		// angulo pitch de la camara

	float			m_view_d;		// variable de debug utilizada para pintar el objeto de la camara.	
	float			m_fov_radians;
	float			m_aspect_ratio;
	float			m_zn;			// valor del z near (a partir de que vemos)
	float			m_zf;			// valor del z far (hasta donde podemos ver)
	
	D3DXMATRIXA16	m_view;			// matriz vista
	D3DXMATRIXA16	m_proj;			// matriz proyeccion
};

#endif // INC_CAMERA_H_