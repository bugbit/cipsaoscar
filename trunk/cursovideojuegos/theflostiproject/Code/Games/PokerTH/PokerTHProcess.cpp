#include "__PCH_PokerTH.h"

#include "PokerTHProcess.h"

//---Engine Includes---
#include "Input/InputManager.h"
#include "Graphics/RenderManager.h"
#include "Graphics/FontManager.h"
#include "Graphics/Object3D.h"
#include "Graphics/ThPSCamera.h"
#include "Script/ScriptManager.h"
#include "luabind/luabind.hpp"
//----------------------

//---Game Includes-------
//Renders..
//Inputs...
//EndGameConditions...
//----------------------

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
bool CPokerTHProcess::Init ()
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
	}

	if (!CProcess::m_bIsOk)
	{
		Release();
	}
	return CProcess::m_bIsOk;
}

void CPokerTHProcess::Release ()
{
	CHECKED_DELETE(m_pObject3D);
	CHECKED_DELETE(m_pCamera);
}

void CPokerTHProcess::RenderScene (CRenderManager* renderManager, CFontManager* fontManager)
{
	renderManager->DrawAxis(20.f);
	renderManager->DrawGrid(50.f,colWHITE,20,20);
	renderManager->DrawCamera(m_pCamera);
}

uint32 CPokerTHProcess::RenderDebugInfo(CRenderManager* renderManager, CFontManager* fm, float fps)
{
	uint32 posY = 0;
	posY = CProcess::RenderDebugInfo(renderManager, fm, fps);
	if (m_bRenderDebugInfo)
	{
		uint32 posX = m_PosRenderDebugInfo.x;
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"____________Controlador de la cámara esferica___________" );
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- Presionado botón izquierdo->la cámara gira" );
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- Presionado botón derecho->la cámara se desplaza" );
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- Con la rueda del botón-> zoomIn/zoomOut" );
		fm->DrawDefaultText(posX,posY,colWHITE,"- Con la rueda del botón+control-> zoomIn/zoomOut suave" );
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"_____________________________________________________" );
	}
	return posY;
}

void CPokerTHProcess::Update (float elapsedTime)
{
	CProcess::Update(elapsedTime);

	CCore * core = CCore::GetSingletonPtr();
	CInputManager*inputManager = core->GetInputManager();
	UpdateInputActions(inputManager);
}


void CPokerTHProcess::UpdateInputActions	(CInputManager* inputManager)
{

	//----------------------------------------------------------
	//-----ACTUALIZAMOS LA CAMARA ESFERICA AL ESTILO DEL MAX----
	Vect3i deltaMouse;
	deltaMouse = inputManager->GetMouseDelta();
	if( inputManager->IsDown(IDV_MOUSE, MOUSE_BUTTON_LEFT) )
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
void CPokerTHProcess::RegisterFunctions (CScriptManager* scriptManager)
{
	lua_State* l_pLUAState = scriptManager->GetLuaState();
	
	using namespace luabind;
	
	// ahora registramos lo que querramos
  module(l_pLUAState)
    [
			def("getPokerTHGame", GetPokerTHGame),

      // registramos la clase CPokerTHProcess
			class_<CPokerTHProcess>(CScriptRegister::SetClassName("CPokerTHProcess"))
			
      // registramos su constructor
      .def(constructor<const std::string&>())

			// registramos sus funciones
			.def(	CScriptRegister::PushFunctionName(AUTO_COMPLETE), &CScriptRegister::AutoComplete)

			.def(	CScriptRegister::PushFunctionName(HELP,"void","void",
						"Muestra todas las funciones de esta clase"),
						&CScriptRegister::Help)
    ];
}