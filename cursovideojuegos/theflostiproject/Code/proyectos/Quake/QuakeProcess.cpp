#include "__PCH_Quake.h"

#include "QuakeProcess.h"
#include "WorldASE.h"
#include "Player.h"
#include "ActionsPlayerInput.h"
#include "QuakePhysicsData.h"
#include "PlayerRender.h"

//---Engine Includes----
#include "Input/InputManager.h"
#include "Input/ActionToInput.h"
#include "Graphics/RenderManager.h"
#include "Graphics/FontManager.h"
#include "Graphics/Object3D.h"
#include "Graphics/ThPSCamera.h"
#include "Graphics/FPSCamera.h"
#include "Graphics/ASEObject/ASETextureManager.h"
#include "Graphics/ASEObject/ASEObject.h"
#include "Script/ScriptManager.h"
#include "luabind/luabind.hpp"
#include "PhysX/PhysicsManager.h"
#include "Math/Matrix44.h"
//----------------------

//---Game Includes------
//....
//----------------------


//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
bool CQuakeProcess::Init ()
{
	CPhysicsManager *pm=CORE->GetPhysicManager();
	CProcess::m_bIsOk = false;
	uint32 w,h;
	pm->SetDebugRenderMode(true);
	CORE->GetRenderManager()->GetWidthAndHeight(w,h);
	float aspect_ratio = (float)w/h;	
	m_CameraViewObj3D = new CObject3D(Vect3f(10.f,10.f,10.f), 0.f, 0.f);
	CQuakePhysicsData *playerdata=new CQuakePhysicsData("player",CQuakePhysicsData::TYPE3D_PLAYER);
	CPlayer *player=new CPlayer(.5f,3.f,45.f,0.1f,.5f,IMPACT_MASK_1,playerdata,Vect3f(0.f,2.f,0.f));
	player->Init();
	playerdata->SetPaint(true);
	playerdata->SetObject3D(player);
	m_pArena.AddPlayer(player);
	pm->AddPhysicController(player);
	CActionsPlayerInput *inputplayer=new CActionsPlayerInput();
	inputplayer->SetPlayer(player);
	m_PlayerInputs.push_back(inputplayer);
	m_GUIPlayer.Init();
	m_GUIPlayer.SetPlayer(player);
	if (m_GUIPlayer.LoadXML("./Data/Models/ItemsPlayer/GUIPlayer.xml"))
	{
		m_pCamera = new CFPSCamera(0.2f,500.f,mathUtils::Deg2Rad(60.f),aspect_ratio,player);
		//camara (view)
		m_pCameraView = new CThPSCamera(0.2f,500.f,mathUtils::Deg2Rad(60.f),aspect_ratio,m_CameraViewObj3D,10.f);

		if (m_CameraViewObj3D && m_pCamera)
		{
			if (m_pArena.Init())
			{
				CWorldASE *world=new CWorldASE();
				world->SetPhysxGroup(GROUP_BASIC_PRIMITIVES);
				world->Init();
				if (world->LoadXML("./Data/Models/Worlds/First/first.xml"))
				{
					world->LoadModels();
					world->AddActorInPhysxManager();
					m_pArena.SetWorld(world);
					CProcess::m_bIsOk = true;
				}
			}
		}
	}

	if (!CProcess::m_bIsOk)
	{
		Release();
	}
	return CProcess::m_bIsOk;
}

void CQuakeProcess::Release ()
{
	CHECKED_DELETE(m_CameraViewObj3D);
	CHECKED_DELETE(m_pCameraView);
	CHECKED_DELETE(m_pCamera);
	m_GUIPlayer.Done();
	ReleasePlayerInputs();
	ReleasePlayerRenders();
	m_pArena.Done();
}

void CQuakeProcess::ReleasePlayerInputs()
{
	std::vector<CPlayerInput *>::iterator it=m_PlayerInputs.begin(),
		itend=m_PlayerInputs.end();
	for(;it!=itend;it++)
	{
		CPlayerInput *playerinput=*it;
		delete playerinput;
	}
}

void CQuakeProcess::ReleasePlayerRenders	()
{
	std::vector<CPlayerRender *>::iterator it=m_PlayerRenders.begin(),
		itend=m_PlayerRenders.end();
	for(;it!=itend;it++)
	{
		CPlayerRender *playerrender=*it;
		delete playerrender;
	}
}

CCamera* CQuakeProcess::GetCamera() const
{
	return (!m_IsCameraView) ? CProcess::GetCamera() : m_pCameraView;
}


void CQuakeProcess::RenderScene (CRenderManager* renderManager, CFontManager* fontManager)
{
	CPhysicsManager *pm=CORE->GetPhysicManager();

	if (m_drawAxisGrid)
	{
		renderManager->DrawAxis(100.f);
		renderManager->DrawGrid(150.f,colWHITE,20,20);
		renderManager->DrawCamera(m_pCamera);
	}
	m_pArena.RenderScene(renderManager,fontManager);
	RenderPlayers(renderManager,fontManager);
	if (pm->GetDebugRenderMode())
	{
		pm->DebugRender(renderManager);
	}
	CCamera *camera=GetCamera();
	/*Vect3f posI=camera->GetObject3D()->GetPosition();
	Vect3f posF=posI+100.f*camera->GetDirection();
	renderManager->DrawLine(posI,posF,colGREEN);*/
	/*
		A	/------------------\	B
			|              /   |
			|       POS /      |
			|      / *         |
			| /                |
		C	\------------------/	D*/
	Vect3f vup=camera->GetVecUp();
	Vect3f dir=camera->GetDirection();
	/*dir.Normalize();
	Vect3f right=vup ^ dir;
	Vect3f up=dir ^ right;
	Vect3f lookAt=camera->GetEye();*/
	Vect3f up=camera->GetVecUp();
	float yaw  = camera->GetObject3D()->GetYaw();
	Vect3f right=Vect3f(cos(yaw+ D3DX_PI * 0.5f), 0, sin(yaw+ D3DX_PI * 0.5f));
	Vect3f lookAt=camera->GetLookAt();
	right.Normalize();
	up.Normalize();
	float SizeX=1.6f;
	float SizeY=1.1f;
	Vect3f PointA = lookAt - (right*SizeX*0.5f) - (up*SizeY*0.5f);
	Vect3f PointB = lookAt + (right*SizeX*0.5f) - (up*SizeY*0.5f);
	Vect3f PointC = lookAt - (right*SizeX*0.5f) + (up*SizeY*0.5f);
	Vect3f PointD = lookAt + (right*SizeX*0.5f) + (up*SizeY*0.5f);
	renderManager->SetTransform((Mat44f) m44fIDENTITY);
	renderManager->DrawLine(PointA,PointB,colGREEN);
	renderManager->DrawLine(PointA,PointC,colGREEN);
	renderManager->DrawLine(PointC,PointD,colGREEN);
	renderManager->DrawLine(PointD,PointB,colGREEN);
	//D3DXMATRIX matrix;
	//D3DXMATRIX translation;
	//D3DXMATRIX translation2;
	//D3DXMATRIX rotation0;
	//D3DXMATRIX rotation;
	//D3DXMATRIX rotation2;
	//Vect3f position = m_pCamera->GetObject3D()->GetPosition();
	////float yaw =  m_pCamera->GetObject3D()->GetYaw();
	//float m_yawGun=0;
	//float pitch =  m_pCamera->GetObject3D()->GetPitch();

	//D3DXMatrixRotationZ ( &rotation2,  pitch);

	////--------------Paint the FACE------------------------------
	//D3DXMatrixTranslation( &translation, position.x,position.y ,position.z);
	//D3DXMatrixTranslation( &translation2, 0.6f, -0.24f , 0.2f);
	//
	//D3DXMatrixRotationY ( &rotation,  -yaw);
	//D3DXMatrixRotationY ( &rotation0,  sin( -m_yawGun )*0.1f);
	//
	//matrix = rotation0 * translation2 * rotation2 * rotation * translation; 

	//renderManager->SetTransform(matrix);

	
	m_GUIPlayer.RenderScene(renderManager,fontManager,m_pCamera);
}

void CQuakeProcess::RenderPlayers(CRenderManager* renderManager, CFontManager* fontManager)
{
	std::vector<CPlayerRender *>::iterator it=m_PlayerRenders.begin(),
		itend=m_PlayerRenders.end();
	for(;it!=itend;it++)
	{
		CPlayerRender *playerrender=*it;
		if (playerrender->IsOk())
			playerrender->RenderScene(renderManager,fontManager);
	}
}

void CQuakeProcess::RenderScene2D(CRenderManager* renderManager, CFontManager* fm)
{
	m_GUIPlayer.RenderScene2D(renderManager,fm);
}

uint32 CQuakeProcess::RenderDebugInfo(CRenderManager* renderManager, CFontManager* fm, float fps)
{
	CPhysicsManager *pm=CORE->GetPhysicManager();
	uint32 posY = 0;
	posY = CProcess::RenderDebugInfo(renderManager,fm, fps);
	if (m_bRenderDebugInfo)
	{
		uint32 posX = m_PosRenderDebugInfo.x;	
		posY += fm->DrawDefaultText(posX,posY,colWHITE,"Camara: %s",(m_IsCameraView) ? "View" : "FPS");
		if (pm->GetDebugRenderMode())
			posY += fm->DrawDefaultText(posX,posY,colWHITE,"Modo Debug Render in PhysicsManager");
	}
	return posY;
}

void CQuakeProcess::Update (float elapsedTime)
{
	CProcess::Update(elapsedTime);

	CCore * core = CCore::GetSingletonPtr();
	CInputManager*inputManager = core->GetInputManager();	
	UpdateInputActions(inputManager);
	m_pArena.Update(elapsedTime);
	m_GUIPlayer.Update(elapsedTime);
	UpdatePlayerInputs(elapsedTime);
}

void CQuakeProcess::UpdateInputActions	(CInputManager* inputManager)
{
	CActionToInput* input2Action = CORE->GetActionToInput();
	CPhysicsManager *pm=CORE->GetPhysicManager();

	if (input2Action->DoAction("ChangeCamera"))
	{
		m_IsCameraView=!m_IsCameraView;
	}
	if (input2Action->DoAction("paintAxisAndGrid"))
	{
		m_drawAxisGrid=!m_drawAxisGrid;
	}
	if (input2Action->DoAction("DebugRenderPhysx"))
	{
		pm->SetDebugRenderMode(!pm->GetDebugRenderMode());
	}
	UpdateCameraView(inputManager);
}

void CQuakeProcess::UpdateCameraView(CInputManager* inputManager)
{
	//----------------------------------------------------------
	//-----ACTUALIZAMOS LA CAMARA ESFERICA AL ESTILO DEL MAX----
	CCamera *camera=GetCamera();
	CObject3D *object3D=camera->GetObject3D();
	Vect3i deltaMouse;
	deltaMouse = inputManager->GetMouseDelta();
	CActionToInput* input2Action = CORE->GetActionToInput();

	float delta;
	if (input2Action->DoAction("YawViewerCam", delta))
	{
			delta = delta * 0.01f;
			float yaw = object3D->GetYaw();
			object3D->SetYaw(yaw+delta);
	}
	if (input2Action->DoAction("PitchViewerCam", delta))
	{
			delta = delta * 0.01f;
			float pitch = object3D->GetPitch();
			object3D->SetPitch(pitch+delta);
	}

	if (input2Action->DoAction("SlowZoomViewerCam", delta))
	{
		delta = delta * 0.1f;
		float increment = 0.1f;
		static_cast<CThPSCamera*>(camera)->AddZoom(-delta*increment);
	}
	else if (input2Action->DoAction("ZoomViewerCam", delta))
	{
		if (m_IsCameraView)
		{
			delta = delta * 0.1f;
			float increment = 1.0f;
			static_cast<CThPSCamera*>(camera)->AddZoom(-delta*increment);
		}
	}


	if (input2Action->DoAction("MoveXViewerCam", delta))
	{
		//Perpendicularmente a su yaw. Realizamos un strafe
		delta = -delta * 0.1f;
		float yaw = object3D->GetYaw()+ePI2f;
		Vect3f directionXZ(	cos(yaw), 0, sin(yaw) );
		//nos desplazamos a una velocidad de 1unidad x segundo
		object3D->SetPosition(object3D->GetPosition()+directionXZ*delta);
	}

	if (input2Action->DoAction("MoveZViewerCam", delta))
	{
		//Segun su yaw directamente
		float delat = -delta * 0.1f;
		float yaw = object3D->GetYaw();
		Vect3f directionXZ(	cos(yaw), 0, sin(yaw) );
		object3D->SetPosition(object3D->GetPosition()+directionXZ*delat);
	}
}

void  CQuakeProcess::UpdatePlayerInputs(float elapsedTime)
{
	std::vector<CPlayerInput *>::iterator it=m_PlayerInputs.begin(),
		itend=m_PlayerInputs.end();
	for(;it!=itend;it++)
	{
		CPlayerInput *playerinput=*it;
		if (playerinput->IsOk())
			playerinput->UpdateInputAction(elapsedTime);
	}
}

//-----------------ScriptManager------------------------------
void CQuakeProcess::RegisterFunctions (CScriptManager* scriptManager)
{
	lua_State* l_pLUAState = scriptManager->GetLuaState();
	using namespace luabind;
	
	// ahora registramos lo que querramos
	module(l_pLUAState)
    [
		def("getGameQuake", GetGameQuake),
		
		// registramos la clase CQuakeProcess
		class_<CQuakeProcess>(CScriptRegister::SetClassName("CQuakeProcess"))
		
		// registramos su constructor
		.def(constructor<const std::string&>())

		// registramos sus funciones
		.def(	CScriptRegister::PushFunctionName(AUTO_COMPLETE), &CScriptRegister::AutoComplete)

		.def(	CScriptRegister::PushFunctionName(HELP,"void","void",
					"Muestra todas las funciones de esta clase"),
					&CScriptRegister::Help)
    ];
}