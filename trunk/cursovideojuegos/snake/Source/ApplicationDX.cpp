
//---Engine Include--
#include ".\applicationdx.h"
#include <assert.h>
#include "defines.h"
#include <d3dx9.h>
#include "DebugRender.h"
#include "Camera.h"
#include "CameraManager.h"
#include "InputManager.h"
//------------------

//---Games Include--
#include "TresEnRaya.h"
#include "Simon.h"
#include "Snake.h"
//------------------

//--------CONSTRUCTOR & DESTRUCTOR------------------//
CApplicationDX::CApplicationDX(void)
: m_pD3D( NULL )
, m_pD3DDevice( NULL )
, m_PaintSolid( true )
, m_hWnd( NULL )
, m_paintInfoGame( true )
, m_paintInfoInput ( false )
, m_drawAxisGrid ( true )
, m_timer(300)

{
	m_CurrentGame = (CTresEnRaya*) new CTresEnRaya();
	m_VectorGames.push_back(m_CurrentGame);

	m_CurrentGame = (CSimon*) new CSimon();
	m_VectorGames.push_back(m_CurrentGame);

	m_CurrentGame = (CSnake*) new CSnake();
	m_VectorGames.push_back(m_CurrentGame);
}

CApplicationDX::~CApplicationDX(void)
{
	//En esta funcion llamaremos a la funcion de destruccion (CleanUP) de todos los Singletons
	CInputManager::GetInstance()->CleanUP();
	CCameraManager::GetInstance()->CleanUP();
		
	CHECKED_RELEASE(m_pD3DDevice);
  CHECKED_RELEASE(m_pD3D);
}
//----------------------------------------------------//


void CApplicationDX::InitAndLoad(HWND hWnd)
{
	m_hWnd = hWnd;
	HRESULT hr = InitDX( hWnd ); //Inicializamos todas las variables DirectX
	assert( SUCCEEDED( hr ) );

	hr = InitData();	//Cargamos en memoria todo lo necesario en el juego
	assert( SUCCEEDED( hr ) );
}

//-----------------------------------------------------------------------------
// Name: InitDX()
// Desc: Initializes DirectX
//-----------------------------------------------------------------------------
HRESULT CApplicationDX::InitDX( HWND hWnd )
{
    // Create the D3D object.
    if( NULL == ( m_pD3D = Direct3DCreate9( D3D_SDK_VERSION ) ) )
        return E_FAIL;

    // Set up the structure used to create the D3DDevice
    D3DPRESENT_PARAMETERS d3dpp;
    ZeroMemory( &d3dpp, sizeof(d3dpp) );
    d3dpp.Windowed = TRUE;
    d3dpp.SwapEffect = D3DSWAPEFFECT_DISCARD;
    d3dpp.BackBufferFormat = D3DFMT_UNKNOWN;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D16;
		d3dpp.EnableAutoDepthStencil = TRUE;
		d3dpp.AutoDepthStencilFormat = D3DFMT_D24S8;
		d3dpp.Flags                  = D3DPRESENTFLAG_DISCARD_DEPTHSTENCIL;
		d3dpp.PresentationInterval   = D3DPRESENT_INTERVAL_IMMEDIATE;

    // Create the D3DDevice
    if( FAILED( m_pD3D->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, hWnd,
                                      D3DCREATE_HARDWARE_VERTEXPROCESSING,
                                      &d3dpp, &m_pD3DDevice ) ) )
    {
        return E_FAIL;
    }

    // Turn off culling, so we see the front and back of the triangle
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(1, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(1, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pD3DDevice->SetSamplerState(1, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);

    m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW );
	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL );


	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE,D3DZB_TRUE);
	m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);

    // Turn off D3D lighting, since we are providing our own vertex colors
    m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );

	m_printText2D.SetFont(m_pD3DDevice);
	
	//Inicializamos los objetos de DirectInput (Keyboard y Mouse)
	CInputManager::GetInstance()->OnCreateDevice( hWnd );
	
    return S_OK;
}


//Cargamos de disco a memoria (RAM Y GPU) todos los objetos necesarios durante la partida
HRESULT CApplicationDX::InitData()
{
	CInputManager::GetInstance()->Load( "./DATA/XML/input.xml" );
	CCameraManager::GetInstance()->Load( "./DATA/XML/cameras.xml" );
	CCameraManager::GetInstance()->Initgeom_box( m_pD3DDevice );

	return S_OK;
}


//-----------------------------------------------------------------------------
// Name: SetupMatrices()
// Desc: Sets up the world, view, and projection transform matrices.
//-----------------------------------------------------------------------------
void CApplicationDX::SetupMatrices()
{
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProject;
	//Obtenemos las matrices de Vista y Proyeccion del CamaraManager
	//CamaraManager contiene varias camaras de diferente tipo, pero una de ellas (m_camVision) es 
	//la que utilizaremos para pintar
	m_matView		= CCameraManager::GetInstance()->GetMatrixView();
	m_matProject	= CCameraManager::GetInstance()->GetMatrixProj();
	
	m_pD3DDevice->SetTransform( D3DTS_VIEW, &m_matView );
	m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &m_matProject );

	
	//TODO 1: Practicar el uso de las cámaras en DirectX llamadas D3DXMatrixPerspectiveFovLH y D3DXMatrixLookAtLH
	// Antes de empezar comentar el código de arriba
	//1.1 Conseguir visualizar la escena de las 3 maneras que se muestra en el documento

	/*D3DXMATRIX matView;
	D3DXMATRIX matProject;
	D3DXMatrixPerspectiveFovLH(&matProject, D3DXToRadian( 50.f ), 640.f/480.f, 0.1f, 1000.f );

	D3DXVECTOR3 position_Camera(,,);
	D3DXVECTOR3 point_to_look(,,);
	D3DXVECTOR3 vecUp(,,);
	D3DXMatrixLookAtLH( &matView, &position_Camera, &point_to_look, &vecUp );
	
	m_pD3DDevice->SetTransform( D3DTS_VIEW, &matView );
	m_pD3DDevice->SetTransform( D3DTS_PROJECTION, &matProject );*/

	//1.2 Utilizar el input de entrada para modificar la posición de la cámara:
	// - Crear una variable privada en esta clase D3DXVECTOR3 position_Camera;
	// - En el constructor inicializar la posición 	position_Camera.x = 5;	position_Camera.y = 1; position_Camera.z = 5;
	// - Utilizar los valores D3DXVECTOR3 point_to_look(0,0,0);	D3DXVECTOR3 vecUp(0,1,0);
	// - Obtener el mapa de teclas presionadas char * keys = CInputManager::GetInstance()->GetKeyboard().GetKey();
	// - Añadir la lógica-> 
	//		-De presionarse la tecla DIK_W añadir al valor position_Camera.x  0.01f unidades.
	//		-De presionarse la tecla DIK_S añadir al valor position_Camera.x  -0.01f unidades.
	//		-De presionarse la tecla DIK_A añadir al valor position_Camera.z  0.01f unidades.
	//		-De presionarse la tecla DIK_D añadir al valor position_Camera.z  -0.01f unidades.

}


void CApplicationDX::BeginRenderDX()
{
	
	#ifdef _DEBUG // Clear the backbuffer to a blue color in a Debug mode
		m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0xff), 1.0f, 0 );
	#else // Clear the backbuffer to a black color in a Release mode
		m_pD3DDevice->Clear( 0, NULL, D3DCLEAR_TARGET|D3DCLEAR_ZBUFFER, D3DCOLOR_XRGB(0,0,0), 1.0f, 0 );
	#endif


    // Begin the scene
    HRESULT hr=m_pD3DDevice->BeginScene();
	assert( SUCCEEDED( hr ) );
	m_pD3DDevice->SetRenderState( D3DRS_CULLMODE, D3DCULL_CCW);
	m_pD3DDevice->SetRenderState( D3DRS_ZENABLE,D3DZB_TRUE);
	m_pD3DDevice->SetRenderState( D3DRS_ZFUNC, D3DCMP_LESSEQUAL);
	m_pD3DDevice->SetRenderState( D3DRS_ZWRITEENABLE, TRUE);
	m_pD3DDevice->SetRenderState( D3DRS_LIGHTING, FALSE );
	m_pD3DDevice->SetRenderState( D3DRS_DITHERENABLE, TRUE );
	m_pD3DDevice->SetRenderState( D3DRS_SPECULARENABLE, FALSE );
	if(m_PaintSolid)
	{
		m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_SOLID  );
	}
	else
	{
		m_pD3DDevice->SetRenderState( D3DRS_FILLMODE, D3DFILL_WIREFRAME  );
	}
}

void CApplicationDX::EndRenderDX()
{
	m_pD3DDevice->EndScene();
    // Present the backbuffer contents to the display
    m_pD3DDevice->Present( NULL, NULL, NULL, NULL );
}


// En esta funcion compone la escena a pintar, es decir, hace todas las llamadas Render
//de los objetos a dibujar con Direct3D
void CApplicationDX::Render()
{
	BeginRenderDX();
	SetupMatrices();
	//--------------------------------------------------------------------------------------//
	//-------AQUI HACEMOS LAS LLAMADAS DE RENDER DE TODOS LOS ELEMENTOS A DIBUJAR-----------//
	//--------------------------------------------------------------------------------------//
		// >>>> INIT


	//dy += m_printText2D.PrintText(400,dy,0xffffffff,"XXX");
	
	m_CurrentGame->Render(m_printText2D);

		// >>>>> END
	//--------------------------------------------------------------------------------------//		
	EndRenderDX();
}

//Funcion para pintar toda la informacion de texto2D en pantalla
void CApplicationDX::RenderInfoGame()
{
	if( m_paintInfoGame )
	{
		CCameraManager * camerManager = CCameraManager::GetInstance();
	
		float l_ElapsedTime = m_timer.GetElapsedTime();
		float l_FPS			= m_timer.GetFPS();
		
		std::string camera_frustum = camerManager->GetCamFrustum(); 
		std::string camera_control = camerManager->GetCamControl();
		std::string camera_vision = camerManager->GetCamVision();
		
		D3DXVECTOR3 pos_CamControl = camerManager->GetCamera(camera_control)->GetPos();
		D3DXVECTOR3 pos_CamVision = camerManager->GetCamera(camera_vision)->GetPos();

		// Codigo de pintado 2d //
		int dy = 0;
	
		
	}
	if( m_paintInfoInput )
	{
		//TODO 3.1: Hemos de recorrer el vector  m_vecInfoInput (del Singleton InputManager) e imprimir 
		//la información de cada elemento en una linea.
		
		//IMPORTANTE: Para imprimir una variable string (todos los elementos de este todo lo son), tenéis que 
		//utilizar la función c_str() de los string. Es decir, si el string a imprimir es pepito --> pepito.c_str()
		
	}

}

//-----------------------------------------------------------------------------
// Name: Update()
// Desc: Update the scene
//-----------------------------------------------------------------------------
void CApplicationDX::Update()
{
	
	//Primero de todo calculamos el update del timer
	m_timer.Update();
	float l_ElapsedTime = m_timer.GetElapsedTime();

	m_CurrentGame->Update(l_ElapsedTime);
	
	CInputManager::GetInstance()->ReadInput( m_hWnd );
	
	CCameraManager::GetInstance()->Update( l_ElapsedTime );	

	CCameraManager::GetInstance()->UpdateFrustumCamera();

	UpdateActions(l_ElapsedTime);
}

// Esta funcion ejecuta las acciones asociadas a los inputs que estan activos. No todas las acciones
//se ejecutan aqui, algunas se ejecutan por ejemplo en el CamaraManager.
void CApplicationDX::UpdateActions( float l_ElapsedTime )
{
	CCameraManager* cameraManager = CCameraManager::GetInstance();
	CInputManager * input = CInputManager::GetInstance();
	
	if( input->DoAction("Simon1") && input->DoAction("Simon2") )
	{
		m_CurrentGame->DeInit();
		m_CurrentGame = m_VectorGames[1];
		m_CurrentGame->Init();
	}
	if( input->DoAction("TresEnRaya1") &&input->DoAction("TresEnRaya2") )
	{
		m_CurrentGame->DeInit();
		m_CurrentGame = m_VectorGames[0];
		m_CurrentGame->Init();
	}
	if( input->DoAction("Snake1") &&input->DoAction("Snake2") )
	{
		m_CurrentGame->DeInit();
		m_CurrentGame = m_VectorGames[2];
		m_CurrentGame->Init();
	}

	//TODO 2.4: Hemos de comprobar si se están realizando los inputs (de teclado) asociados a las 
	//acciones ChangeVision(), ChangeFrustum() y ChangeFrustum()
	//	1. Hemos de ir al archivo /DATA/input.XML y añadir un input para cada una de las 3 acciones
	//		1.1 Por ejemplo --> <KEYBOARD name_action="identificador_accion" name_input="DIK_..." debug="yes"/>
	//	2. Hemos de comprobar si estas 3 acciones se están realizando
	//		2.1 if( input->DoAction("identificador_accion")...
	//	3. Si dejaramos el código así, aunque presionáramos el input relacionado con ChangeVision() 
	//	muy rápido ,internamente llamaría muchísimas veces a ChangeVision(). Para que
	//	esto no suceda hemos de controlarlo con un timer--> m_timer.IsEndInputTimer("identificador",sg).
	//	Como ejemplo podéis ver como lo hace paintCameras.
	


	if( input->DoAction("camViewDist-") )
		cameraManager->AddViewD(-l_ElapsedTime);
	
	if( input->DoAction("camViewDist+") )
		cameraManager->AddViewD(l_ElapsedTime);

	if( input->DoAction("paintCameras") && m_timer.IsEndInputTimer("printCameras",0.5) )
		cameraManager->ChangeModePrintCamera();
	
	if( input->DoAction("changeSolidWire") && m_timer.IsEndInputTimer("changeSolidWire",0.8f) )
	{
		if( m_PaintSolid )
			m_PaintSolid = false;
		else
			m_PaintSolid = true;
	}
	if( input->DoAction("paintAxisAndGrid") && m_timer.IsEndInputTimer("paintAxisAndGrid",0.8f) )
	{
		if( m_drawAxisGrid )
            m_drawAxisGrid = false;
		else
		    m_drawAxisGrid = true;
	}

	if( input->DoAction("paintInfoGame") && m_timer.IsEndInputTimer("paintInfoGame",0.8f) )
	{
		if( m_paintInfoGame )
		{
			m_paintInfoGame = false;
		}
		else
		{
			m_paintInfoGame = true;
			m_paintInfoInput = false;
		}
	}

	if( input->DoAction("paintInfoInput") && m_timer.IsEndInputTimer("paintInfoInput",0.8f) )
	{
		if( m_paintInfoInput )
		{
			m_paintInfoInput = false;
		}
		else
		{
			m_paintInfoInput = true;
			m_paintInfoGame = false;
		}
	}
}