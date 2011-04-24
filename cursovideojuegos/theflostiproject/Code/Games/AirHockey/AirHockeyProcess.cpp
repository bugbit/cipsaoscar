#include "__PCH_AirHockey.h"

//---Engine Includes----
#include "AirHockeyProcess.h"
#include "Input/InputManager.h"
#include "Graphics/RenderManager.h"
#include "Graphics/Object3D.h"
#include "Graphics/ThPSCamera.h"
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
bool CAirHockeyGameProcess::Init ()
{
	CProcess::m_bIsOk = false;
	m_pObject3D = new CObject3D(Vect3f(0.f,0.f,0.f), 0.f, 0.f);
	uint32 w,h;
	CORE->GetRenderManager()->GetWidthAndHeight(w,h);
	float aspect_ratio = (float)w/h;
	m_pCamera = new CThPSCamera(1.f,500.f,mathUtils::Deg2Rad(60.f),aspect_ratio,m_pObject3D,50.f);

	if (m_pObject3D && m_pCamera) 
	{
		CProcess::m_bIsOk = true;
		
		CPhysicsManager* physicManager = CORE->GetPhysicManager();
		assert(physicManager);
		CRenderManager* rm = CORE->GetRenderManager();
		assert(rm);
		CASETextureManager::GetInstance()->SetTexturePath("./Data/3ds/");
		
		//-----Board Base----
		m_pASEBaseBoard = new CASEObject();
		assert(m_pASEBaseBoard);
		m_pASEBaseBoard->Load("./Data/escena.ASE", rm, false);
		//LoadOrCreatePhysXMesh(meshBoardBase,physxBoardBase,"escena");

	}

	if (!CProcess::m_bIsOk)
	{
		Release();
	}
	return CProcess::m_bIsOk;
}

void CAirHockeyGameProcess::Release ()
{
	CHECKED_DELETE(m_pObject3D);
	CHECKED_DELETE(m_pCamera);

	if (m_pASEBaseBoard)
	{
		m_pASEBaseBoard->CleanUp();
		delete m_pASEBaseBoard;
		m_pASEBaseBoard = NULL;
	}
}

void CAirHockeyGameProcess::RenderScene (CRenderManager* renderManager, CFontManager* fontManager)
{
	if (m_pASEBaseBoard)
	{
		m_pASEBaseBoard->Render(renderManager);
	}
}

uint32 CAirHockeyGameProcess::RenderDebugInfo(CRenderManager* renderManager, CFontManager* fm, float fps)
{
	uint32 posY = 0;
	posY = CProcess::RenderDebugInfo(renderManager,fm, fps);
	if (m_bRenderDebugInfo)
	{
		uint32 posX = m_PosRenderDebugInfo.x;	
	}
	return posY;
}

void CAirHockeyGameProcess::Update (float elapsedTime)
{
	CProcess::Update(elapsedTime);

	CCore * core = CCore::GetSingletonPtr();
	CInputManager*inputManager = core->GetInputManager();
	UpdateInputActions(inputManager);
}


void CAirHockeyGameProcess::UpdateInputActions	(CInputManager* inputManager)
{
		//----------------------------------------------------------
	//-----ACTUALIZAMOS LA CAMARA ESFERICA AL ESTILO DEL MAX----
	Vect3i deltaMouse;
	deltaMouse = inputManager->GetMouseDelta();
	if( inputManager->IsDown(IDV_MOUSE,MOUSE_BUTTON_LEFT) )
	{
		if (deltaMouse.x != 0)
		{
			float deltaX = deltaMouse.x * 0.01f;
			float yaw = m_pObject3D->GetYaw();
			m_pObject3D->SetYaw(yaw+deltaX);
		}
		if (deltaMouse.y != 0)
		{
			float deltaY = deltaMouse.y * 0.01f;
			float pitch = m_pObject3D->GetPitch();
			m_pObject3D->SetPitch(pitch+deltaY);
		}
	}

	if (deltaMouse.z != 0)
	{
		float deltaZ = deltaMouse.z * 0.1f;
		float increment = 1.f;
		if (inputManager->IsDown(IDV_KEYBOARD, KEY_RCTRL) || inputManager->IsDown(IDV_KEYBOARD, KEY_LCTRL))
		{
			increment = 0.1f;
		}
		static_cast<CThPSCamera*>(m_pCamera)->AddZoom(-deltaZ*increment);
	}
	//- Si se presiona el boton del medio y se mueve el mouse, la camara se desplaza por el plano (X,Z) segun su yaw
	if( inputManager->IsDown(IDV_MOUSE, MOUSE_BUTTON_MIDDLE) )
	{
		if (deltaMouse.y != 0)
		{
			//Segun su yaw directamente
			float delatY = deltaMouse.y * 0.1f;
			float yaw = m_pObject3D->GetYaw();
			Vect3f directionXZ(	cos(yaw), 0, sin(yaw) );
			m_pObject3D->SetPosition(m_pObject3D->GetPosition()+directionXZ*delatY);
		}
		if (deltaMouse.x != 0)
		{
			//Perpendicularmente a su yaw. Realizamos un strafe
			float deltaX = deltaMouse.x * 0.1f;
			float yaw = m_pObject3D->GetYaw()+ePI2f;
			Vect3f directionXZ(	cos(yaw), 0, sin(yaw) );
			//nos desplazamos a una velocidad de 1unidad x segundo
			m_pObject3D->SetPosition(m_pObject3D->GetPosition()+directionXZ*deltaX);
		}
	}
	//----------------------------------------------------------
	//----------------------------------------------------------
}


//-----------------ScriptManager------------------------------
void CAirHockeyGameProcess::RegisterFunctions (CScriptManager* scriptManager)
{
	lua_State* l_pLUAState = scriptManager->GetLuaState();
	using namespace luabind;
	
	// ahora registramos lo que querramos
	module(l_pLUAState)
    [
		def("getGameAirHockey", GetGameAirHockey),
		
		// registramos la clase CAirHockeyGameProcess
		class_<CAirHockeyGameProcess>(CScriptRegister::SetClassName("CAirHockeyGameProcess"))
		
		// registramos su constructor
		.def(constructor<const std::string&>())

		// registramos sus funciones
		.def(	CScriptRegister::PushFunctionName(AUTO_COMPLETE), &CScriptRegister::AutoComplete)

		.def(	CScriptRegister::PushFunctionName(HELP,"void","void",
					"Muestra todas las funciones de esta clase"),
					&CScriptRegister::Help)
    ];
}