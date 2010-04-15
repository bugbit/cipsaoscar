#ifndef INC_CAMERAMANAGER_H_
#define INC_CAMERAMANAGER_H_

#include <map>
#include <string>
#include <d3dx9.h>
#include "XMLParser.h"
#include "defines.h"

//--- Declaraciones avanzadas---
class CTimer;
class CCamera;
class CFrustum;

//------------------Definicion de nuevos tipos-----------------//
typedef std::pair <std::string, CCamera* >			Camera_Pair;
typedef std::map< std::string, CCamera* >			MapCam;
typedef std::map< std::string, CCamera* >::iterator	It_MapCam;
//-------------------------------------------------------------//

class CCameraManager: public CXMLParser
{

public:

	CCameraManager(); 
	~CCameraManager();

	//--- SINGLETON FUNCTIONS ---
	static CCameraManager*	GetInstance		( void );	
	void					CleanUP			( void );

	//--- RESET FUNCTION ---
	void	Reset	( const std::string &xmlSoundsFile );
	
	//--- UPDATE FUNCTIONS ---
	void	Update				( const float &dt );
	void	UpdateESFERACamera	( const float &dt, CCamera * camControl );
	void	UpdateTHPSCamera	( const float &dt, CCamera * camControl );
	void	UpdateFPSCamera		( const float &dt, CCamera * camControl );

	//--- ADD FUNCTION ---
	void	AddViewD	( const float &amount );

	//--- GET FUNCTIONS ---
	CCamera*		GetCamera		( const std::string &name );
	std::string		GetCamVision	( void ) const { return m_camVision; }
	std::string		GetCamControl	( void ) const { return m_camControl; }
	std::string		GetCamFrustum	( void ) const { return m_camFrustum; }
	D3DXMATRIX		GetMatrixView	( void );
	D3DXMATRIX		GetMatrixProj	( void ); 

	// Create & Destroy Cameras
	void	NewCamera	( CamerType id_cam, const std::string &name, 
						  const D3DXVECTOR3 &position, float zoom_dist2player = 0 );
	void	DeleteCamera	( const std::string &name );
	
	//--- Change InputControl/Vision/Frustum of the Camera ---
	void	ChangeVision		( void );
	//TODO 2.2: Implementar ChangeControl--> asignamos como camara de control la camara de visión
	void	ChangeControl		( void ) {;}
	//TODO 2.3: Implementar ChangeFrustum--> asignamos como camara de frustum la camara de visión
	void	ChangeFrustum		( void ) {;}

	//--- FRUSTUM FUNCTIONS ---
	void	UpdateFrustumCamera	( void );
	bool	SphereVisible		( const D3DXVECTOR3 &p, const float &radius ) const;
	bool	BoxVisible			( const D3DXVECTOR3 &max, const D3DXVECTOR3 &min ) const;

	//--- RENDER CAMERAS FUNCTIONS ---
	void	ChangeModePrintCamera	( void ) { m_RenderCameras = !m_RenderCameras; }
	void	RenderCameras			( LPDIRECT3DDEVICE9	mDevice );
	void	Initgeom_box			( LPDIRECT3DDEVICE9 mDevice );

	//--- PARSERFILE FUNCTION ---
	void	Load	(const std::string &xmlSoundsFile);
	

private:

	static CCameraManager*		myCameraManager;	//Variable para hacer de CameraManager un Singleton.
	
	MapCam		m_mapCameras;	//Estructura de datos con las camaras existentes en el mundo.
	It_MapCam	it_mapCameras;	//Iterador de la estructura m_mapCameras.
	
	std::string	m_camControl;	//Nombre de la camara que responde al control de entrada (raton, teclado..)
	std::string	m_camVision;	//Nombre de la camara por la cual visualizamos el mundo.
	std::string	m_camFrustum;	//Nombre de la camara a la cual se le aplica el Frustum.

	LPD3DXMESH	m_box;		//Representacion geometrica de la camara (una caja)
	
	CFrustum*	m_frustum;			//Variable que contiene el frustum de la camara m_camFrustum
	bool		m_RenderCameras;	//Flag que indica si se ha de pintar o no la posicion fisica de las camaras.
	void		onStartElement		(const std::string &elem, MKeyValue &atts);
};

#endif // INC_CAMERAMANAGER_H_