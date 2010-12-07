#include "__PCH_Quake.h"

#include "WorldASE.h"

//---Engine Includes----
#include "QuakeProcess.h"
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
//----------------------

//---Game Includes------
//....
//----------------------


//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
bool CQuakeProcess::Init ()
{
	CProcess::m_bIsOk = false;
	uint32 w,h;
	CORE->GetRenderManager()->GetWidthAndHeight(w,h);
	float aspect_ratio = (float)w/h;	
	m_CameraViewObj3D = new CObject3D(Vect3f(10.f,10.f,10.f), 0.f, 0.f);
	m_pCamera = new CFPSCamera(0.2f,500.f,mathUtils::Deg2Rad(60.f),aspect_ratio,m_CameraViewObj3D);
	//camara (view)
	m_pCameraView = new CThPSCamera(0.2f,500.f,mathUtils::Deg2Rad(60.f),aspect_ratio,m_CameraViewObj3D,10.f);

	if (m_CameraViewObj3D && m_pCamera)
	{
		if (m_pArena.Init())
		{
			CWorldASE *world=new CWorldASE();
			world->Init();
			world->LoadWorld("./Data/Models/Worlds/First/first.xml");
			world->LoadModels();
			m_pArena.SetWorld(world);
			CProcess::m_bIsOk = true;
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
	m_pArena.Done();
}

CCamera* CQuakeProcess::GetCamera() const
{
	return (!m_IsCameraView) ? CProcess::GetCamera() : m_pCameraView;
}


void CQuakeProcess::RenderScene (CRenderManager* renderManager, CFontManager* fontManager)
{
	if (m_drawAxisGrid)
	{
		renderManager->DrawAxis(100.f);
		renderManager->DrawGrid(150.f,colWHITE,20,20);
		renderManager->DrawCamera(m_pCamera);
	}
	m_pArena.RenderScene(renderManager,fontManager);
}

uint32 CQuakeProcess::RenderDebugInfo(CRenderManager* renderManager, CFontManager* fm, float fps)
{
	uint32 posY = 0;
	posY = CProcess::RenderDebugInfo(renderManager,fm, fps);
	if (m_bRenderDebugInfo)
	{
		uint32 posX = m_PosRenderDebugInfo.x;	
		posY += fm->DrawDefaultText(posX,posY,colWHITE,"Camara: %s",(m_IsCameraView) ? "View" : "FPS");
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
}

void CQuakeProcess::UpdateInputActions	(CInputManager* inputManager)
{
	CActionToInput* input2Action = CORE->GetActionToInput();

	if (input2Action->DoAction("ChangeCamera"))
	{
		m_IsCameraView=!m_IsCameraView;
	}
	if (input2Action->DoAction("paintAxisAndGrid"))
	{
		m_drawAxisGrid=!m_drawAxisGrid;
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