#include "CameraManager.h"
#include "CameraEsfera.h"
#include "CameraThPShooter.h"
#include "CameraFPShooter.h"
#include "Frustum.h"
#include "InputManager.h"
#include "Timer.h"

#pragma warning( disable : 4996 ) // disable deprecated warning 

CCameraManager *CCameraManager::myCameraManager = 0;


//--------CONSTRUCTOR & DESTRUCTOR------------------//
CCameraManager::CCameraManager()
{
	//---Por defecto insertamos la camara del player----
	CCamera * FPS_cam = new CCameraFPShooter();
	std::string name_Cameraplayer = "player";
	m_mapCameras.insert( Camera_Pair(name_Cameraplayer, FPS_cam) );
	//--------------------------------------------------
	
	
	m_RenderCameras = false; //Por defecto no pintamos la representacion de las camaras
	
	m_camControl = name_Cameraplayer;	 //Por defecto asignamos como camara
	m_camVision  = name_Cameraplayer;	 //de control, vision y frustum a la del player
	m_camFrustum = name_Cameraplayer;
	
	m_frustum = new CFrustum();	// Creamos un objeto Frustum 
	m_box = NULL;	// Objeto box para pintar la representacion de las camaras
}

CCameraManager::~CCameraManager()
{
}
//----------------------------------------------//



//--------------Reset FUNCTION------------------//
void CCameraManager::Reset( const std::string &xmlSoundsFile )
{
		
		//--------Eliminamos todas las camaras actuales------------//
		// Primero eliminamos todas las camaras del mapa (ya que han sido creadas con memoria dinamica)
		for( it_mapCameras = m_mapCameras.begin(); it_mapCameras != m_mapCameras.end(); it_mapCameras++ )
			delete it_mapCameras->second;

		// Una vez eliminado las camaras que contenía ya podemos hacer un clear
		m_mapCameras.clear(); 
		
		//---------Reseteamos todas la variables de la clase--------//
		//Por defecto insertamos la camara del player
		CCamera * FPS_cam = new CCameraFPShooter();
		std::string name = "player";
		m_mapCameras.insert( Camera_Pair(name, FPS_cam) );
	
		m_RenderCameras = false;
		m_camControl = name;
		m_camVision  = name;
		m_camFrustum = name;

		//----------Leemos de nuevo las camaras de un fichero-------//
		Load( xmlSoundsFile );
}



//--------------SINGLETON FUNCTIONS------------------//
void CCameraManager::CleanUP( void )
{
	if( myCameraManager != NULL )
	{
		// Primero eliminamos todas las camaras del mapa (ya que han sido creadas con memoria dinamica)
		for( it_mapCameras = m_mapCameras.begin(); it_mapCameras != m_mapCameras.end(); it_mapCameras++ )
			delete it_mapCameras->second;

		// Una vez eliminado las camaras que contenía ya podemos hacer un clear
		m_mapCameras.clear();  
	
		if( m_box != NULL )
		{
			m_box->Release();
			m_box = NULL;
		}
		delete m_frustum;

		delete myCameraManager;
		myCameraManager = NULL;
	}
}

CCameraManager *CCameraManager::GetInstance()
{
	if( myCameraManager == NULL )
	{
		myCameraManager = new CCameraManager();
	};
	return myCameraManager;
}

//----------------------------------------------------//


//--------------ADD FUNCTION------------------//
//Modificacion de la representacion de las camaras, en particular
//esto modifica el paralalepipedo que representa cada frustum de las
//camaras existentes en la escena
void CCameraManager::AddViewD( const float &amount )
{
	for( it_mapCameras = m_mapCameras.begin(); it_mapCameras != m_mapCameras.end(); it_mapCameras++ )
	{
		it_mapCameras->second->AddViewD(amount);
	}
}
//---------------------------------------------//


//------------GET FUNCTIONS--------------------//
// Dado un string que identifica una camara, esta funcion retorna 
// un puntero si existe tal camara y un NULL en caso contrario.
CCamera* CCameraManager::GetCamera( const std::string &name )
{
	it_mapCameras = m_mapCameras.find( name );
	if(it_mapCameras != m_mapCameras.end())
	{
		return it_mapCameras->second;
	}
	else
	{
		return NULL;
	}
}

//Estas dos funciones retornan la matriz de Proyeccion y Vista de la
//camara que esta como m_camVision. Es decir,retorna las matrices 
//de la camara con la que pintaremos la escena
D3DXMATRIX	CCameraManager::GetMatrixView( void )
{
	return GetCamera(m_camVision)->GetMatrixView(); 
}

D3DXMATRIX	CCameraManager::GetMatrixProj( void )
{
	return GetCamera(m_camVision)->GetMatrixProj(); 
}


//---------CREATE & DESTROY CAMERAS-----------------------//
// Cada vez que se lea del fichero XML la configuracion de una nueva camara que ha de 
//existir en la escena, se llamara a esta funcion para que la cree y la introduzca 
//en la estructura de datos (m_mapCameras) donde se almacenaran las camaras
void CCameraManager::NewCamera(	CamerType id_cam, 
								const std::string &name, 
								const D3DXVECTOR3 &position, 
								float zoom_dist2player )
{
	CCamera *new_camera;
		
	switch(id_cam)
	{
		case ESFERA_CAMERA:
			    new_camera = new CCameraEsfera();
				static_cast<CCameraEsfera*>(new_camera)->SetZoom( zoom_dist2player );
				break;
		case THPS_CAMERA:
			  	new_camera = new CCameraThPShooter();
				static_cast<CCameraThPShooter*>(new_camera)->SetDist2Player( zoom_dist2player );
				break;
		case FPS_CAMERA:
				new_camera =new CCameraFPShooter();
				break;
	}

	new_camera->SetPos(position);
	m_mapCameras.insert( Camera_Pair(name, new_camera) );
}



void CCameraManager::DeleteCamera(const std::string &name)
{
	//Eliminamos la camara y pasamos el control a la camara del player
	if(name != "player")
	{
		it_mapCameras = m_mapCameras.find(name);
		if(it_mapCameras != m_mapCameras.end())
		{
			delete it_mapCameras->second;
			m_camControl = "player";
			m_camVision = "player";
		}
	}
}	
//------------------------------------------------------//
	



//-------CHANGE INPUTCONTROL/VISION/FRUSTUM OF THE CAMERA------------//
void CCameraManager::ChangeVision( void )
{
	//Importante: Los todos 2.1,2.2 y 2.3 los podremos testear cuando implementemos el 2.4
	//TODO 2.1: Cambiar de forma cíclica, la cámara de visión a la siguiente existente.
	//1. Encontrar con  un iterador la posición de la cámara m_camVision.
	//2. De existir esa cámara en el mapa.
		//2.1 Incrementar el iterador en uno y comprobar si está apuntando a una posición válida.
		//2.2 Si es válida la posición simplemente asignar a m_camVision el nombre al que apunta el iterador.
		//2.3 De haber estado el iterador apuntando la última posición y al incrementar en uno, debemos
		//    colocar el iterador en la primera posición.
			//	2.3.1 Colocar el iterador en la primera posición y asignar a m_CamVision el nombre que
			//        apunta el iterador.

	
}	
//----------------------------------------------------------------//


//-------------PARSERFILE FUNCTIONS---------------------------//
void CCameraManager::Load (const std::string &xmlSoundsFile)
{
	//Read the xml camera file
	xmlParseFile(xmlSoundsFile);
}

// Esta funcion permite leer de un fichero XML las camaras que han de existir, 
//definiendo para cada una de ellas su tipo de camara, su posicion y su identificador.
//
// Un ejemplo de fichero XML podria ser el siguiente:
//
//<CameraManager>
//	<EsferaCamera	name="debug_esferica"	posx="0" posy="0"	posz="0" zoom="20"/>
//	<THPSCamera		name="debug_thps"		posx="0" posy="2"	posz="0" dist2player="1"/>
//	<FPSCamera		name="debug_fps"		posx="0" posy="2"	posz="0"/>
//<CameraManager/>

void CCameraManager::onStartElement( const std::string &elem, MKeyValue &atts )
{
	if( elem == "EsferaCamera" )
    {
	  float zoom;
	  D3DXVECTOR3 position;

      std::string s_name  = atts["name"];
      std::string s_posx  = atts["posx"];
	  std::string s_posy  = atts["posy"];
	  std::string s_posz  = atts["posz"];
	  std::string s_zoom  = atts["zoom"];
	  	  
	  sscanf(s_posx.c_str(),"%f",&position.x);
      sscanf(s_posy.c_str(),"%f",&position.y);
	  sscanf(s_posz.c_str(),"%f",&position.z);
      sscanf(s_zoom.c_str(),"%f",&zoom);
	  
	  NewCamera(ESFERA_CAMERA, s_name, position, zoom);	
    }
    
	if( elem == "FPSCamera" )
    {
	  D3DXVECTOR3 position;

      std::string s_name  = atts["name"];
      std::string s_posx  = atts["posx"];
	  std::string s_posy  = atts["posy"];
	  std::string s_posz  = atts["posz"];
	 
	  sscanf(s_posx.c_str(),"%f",&position.x);
      sscanf(s_posy.c_str(),"%f",&position.y);
	  sscanf(s_posz.c_str(),"%f",&position.z);
	   
	  NewCamera(FPS_CAMERA, s_name, position);	
		
    }
	
	if( elem == "THPSCamera" )
    {
	  float dist2player;
	  D3DXVECTOR3 position;

      std::string s_name  = atts["name"];
      std::string s_posx  = atts["posx"];
	  std::string s_posy  = atts["posy"];
	  std::string s_posz  = atts["posz"];
	  std::string s_dist  = atts["dist2player"];
	  	  
	  sscanf(s_posx.c_str(),"%f",&position.x);
      sscanf(s_posy.c_str(),"%f",&position.y);
	  sscanf(s_posz.c_str(),"%f",&position.z);
      sscanf(s_dist.c_str(),"%f",&dist2player);
	  
	  NewCamera(THPS_CAMERA, s_name, position, dist2player);	
		
    }
}
//------------------------------------------------------//



//--------------------RENDER CAMERAS FUNCTIONS------------------//
void  CCameraManager::Initgeom_box(LPDIRECT3DDEVICE9 mDevice )
{
	struct CUSTOMVERTEX
	{
		D3DXVECTOR3 pos;
		DWORD color;
		D3DXVECTOR2 tex;
		
		static unsigned int getFlags()
		{
			return (D3DFVF_XYZ|D3DFVF_DIFFUSE|D3DFVF_TEX1); 
		}
	};

	LPDIRECT3DVERTEXBUFFER9 pTempVertexBuffer;

	
	//----inicializamos la box de las camaras-------------//
	D3DXCreateBox( mDevice, 1, 1, 1, &m_box, NULL);

	

	m_box->CloneMeshFVF( D3DXMESH_VB_WRITEONLY|D3DXMESH_IB_WRITEONLY,
								CUSTOMVERTEX::getFlags(), mDevice, 
								&m_box );

	if( SUCCEEDED( m_box->GetVertexBuffer( &pTempVertexBuffer ) ) )
	{
		int nNumVerts = m_box->GetNumVertices();
		CUSTOMVERTEX *pVertices = NULL;

		pTempVertexBuffer->Lock( 0, 0, (void**)&pVertices, 0 );
		{
			for( int i = 0; i < nNumVerts; ++i )
				pVertices[i].color = D3DCOLOR_COLORVALUE( 0.0, 1.0, 0.0, 1.0 );
		}

		pTempVertexBuffer->Unlock();
		pTempVertexBuffer->Release();
		pTempVertexBuffer = NULL;
	}
}


void CCameraManager::RenderCameras( LPDIRECT3DDEVICE9 mDevice )
{
	//Si el flag que indica si se han de pintar las representaciones de las
	//camaras esta a true => debemos recorrer todas las camaras existentes 
	//y llamar a su correspondiente metodo de pintado.
	if(m_RenderCameras == true)
	{
		for( it_mapCameras = m_mapCameras.begin(); it_mapCameras != m_mapCameras.end(); it_mapCameras++ )
		{
			it_mapCameras->second->RenderCamera( mDevice, m_box );
		}
	}
}
//------------------------------------------------------//


//--------------------FRUSTUM FUNCTIONS------------------//
// Con esta funcion obtenemos los 6 planos del paralepipedo que forma el frustum 
//de la camara m_camFrustum. 
void CCameraManager::UpdateFrustumCamera( void )
{	
	D3DXMATRIX matView;
	D3DXMATRIX matProj;

	it_mapCameras = m_mapCameras.find( m_camFrustum );

	matView = it_mapCameras->second->GetMatrixView();
	matProj  = it_mapCameras->second->GetMatrixProj();
	
	m_frustum->Update( matView * matProj ); 	
}

// Una vez tenemos los 6 planos del frustum, podemos efectuar muy rapidamente la
//comprobacion (sin mucho calculo computacional) si una esfera esta dentro 
//del frustum, es decir si la esfera comparando con todos los planos se encuentra dentro.
bool CCameraManager::SphereVisible( const D3DXVECTOR3 &p, const float &radius ) const
{
	return m_frustum->SphereVisible(p, radius);
}
// Lo mismo que la anterior funcion pero comparando con una caja.
bool CCameraManager::BoxVisible( const D3DXVECTOR3 &max, const D3DXVECTOR3 &min ) const
{
	return m_frustum->BoxVisible( max, min);
}
//------------------------------------------------------//



//--------------UPDATE FUNCTION------------------//
// Con esta funcion actualizamos la posicion y orientacion de la camara m_camControl,
//es decir la camara que esta bajo el control del input(mouse y teclado)
void CCameraManager::Update( const float &dt )
{
	//Primero de todo obtenemos la direccion de la camara m_camControl
	CCamera * camControl = GetCamera( m_camControl );
	//Dependiendo del tipo de camara que sea llamamos a su Update correspondiente
	switch( camControl->GetId() )
	{
		case FPS_CAMERA:	UpdateFPSCamera(dt, camControl);
							break;

		case THPS_CAMERA:	UpdateTHPSCamera(dt, camControl);
							break;

		case ESFERA_CAMERA: UpdateESFERACamera(dt, camControl);
							break;
	}
}

// Si la camara m_camControl es de tipo ESFERA_CAMERA entonces llamaremos a esta
//funcion para actualizar su posicion y orientacion.
// Se actualiza el yaw, pitch y zoom de una camara ESFERA_CAMERA (igual que la camara
//que se utiliza en la practica de graficos por computador II).
void CCameraManager::UpdateESFERACamera( const float &dt, CCamera * camControl )
{
	float amount;
	CInputManager * input = CInputManager::GetInstance();
	
	if( input->DoAction("move_yaw",amount) )
	{
		camControl->AddYaw(amount * dt * 30 );
	}
	if( input->DoAction("move_pitch",amount) )
	{
		camControl->AddPitch(amount * dt * 30  );
	}
	if( input->DoAction("zoom", amount) )
	{
		static_cast<CCameraEsfera*>(camControl)->AddZoom(-amount * 2*  dt);
	}
}


// Si la camara m_camControl es de tipo THPS_CAMERA se llama a esta funcion.
void CCameraManager::UpdateTHPSCamera( const float &dt, CCamera * camControl )
{
	float amount;
	bool flag_speed;
	CInputManager * input = CInputManager::GetInstance();
	CCameraThPShooter* camera = static_cast<CCameraThPShooter*>(camControl);

	//Definimos el movimiento del Yaw, Pitch y Zoom de la camara
	if( input->DoAction("move_yaw", amount) )
	{
		camera->AddYaw( amount * dt * 4 );
	}
	if( input->DoAction("move_pitch" ,amount) )
	{
		camera->AddPitch( amount * dt * 4 );
	}
	if( input->DoAction("zoom", amount) )
	{
		camera->AddDist2Player( -amount * dt );
	}
	
	//Definimos el movimiento de posicion (X,y,Z) de la camara
	int forward = 0;
	int strafe = 0;
	flag_speed = input->DoAction("speed");

	if( input->DoAction("move_up") )
		forward += 1;
	if( input->DoAction("move_down") )
		forward -= 1;
	if( input->DoAction("move_left") )
		strafe += 1;
	if( input->DoAction("move_right") )
		strafe -= 1;
	
	camera->Move(strafe, forward ,flag_speed, dt);
}

// Si la camara m_camControl es de tipo FPS_CAMERA se llama a esta funcion.
void CCameraManager::UpdateFPSCamera( const float &dt, CCamera * camControl)
{
	CInputManager * input = CInputManager::GetInstance();
	CCameraFPShooter* camera = static_cast<CCameraFPShooter*>(camControl);
	
	float amount;
	bool flag_speed;
		
	//Definimos el movimiento del Yaw y Pitch de la camara
	if( input->DoAction("move_yaw",amount) )
	{
		camera->AddYaw( amount * dt );
	}
	if( input->DoAction("move_pitch",amount) )
	{
		camera->AddPitch( amount * dt );
	}
	
	//Definimos el movimiento de posicion (X,y,Z) de la camara
	int forward = 0;
	int strafe = 0;
	flag_speed = input->DoAction("speed",amount);

	if( input->DoAction("move_up") )
		forward += 1;
	if( input->DoAction("move_down") )
		forward -= 1;
	if( input->DoAction("move_left") )
		strafe += 1;
	if( input->DoAction("move_right") )
		strafe -= 1;
	
	camera->Move(strafe, forward ,flag_speed, dt);
}