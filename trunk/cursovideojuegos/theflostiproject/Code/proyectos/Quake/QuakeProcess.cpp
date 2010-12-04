#include "__PCH_Quake.h"

//---Engine Includes----
#include "QuakeProcess.h"
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
bool CQuakeGameProcess::Init ()
{
	CProcess::m_bIsOk = false;
	
	CProcess::m_bIsOk = true;

	if (!CProcess::m_bIsOk)
	{
		Release();
	}
	return CProcess::m_bIsOk;
}

void CQuakeGameProcess::Release ()
{
	
}

void CQuakeGameProcess::RenderScene (CRenderManager* renderManager, CFontManager* fontManager)
{
	
}

uint32 CQuakeGameProcess::RenderDebugInfo(CRenderManager* renderManager, CFontManager* fm, float fps)
{
	uint32 posY = 0;
	posY = CProcess::RenderDebugInfo(renderManager,fm, fps);
	if (m_bRenderDebugInfo)
	{
		uint32 posX = m_PosRenderDebugInfo.x;	
	}
	return posY;
}

void CQuakeGameProcess::Update (float elapsedTime)
{
	CProcess::Update(elapsedTime);

	CCore * core = CCore::GetSingletonPtr();
	CInputManager*inputManager = core->GetInputManager();	
}

//-----------------ScriptManager------------------------------
void CQuakeGameProcess::RegisterFunctions (CScriptManager* scriptManager)
{
	lua_State* l_pLUAState = scriptManager->GetLuaState();
	using namespace luabind;
	
	// ahora registramos lo que querramos
	module(l_pLUAState)
    [
		def("getGameQuake", GetGameQuake),
		
		// registramos la clase CQuakeGameProcess
		class_<CQuakeGameProcess>(CScriptRegister::SetClassName("CQuakeGameProcess"))
		
		// registramos su constructor
		.def(constructor<const std::string&>())

		// registramos sus funciones
		.def(	CScriptRegister::PushFunctionName(AUTO_COMPLETE), &CScriptRegister::AutoComplete)

		.def(	CScriptRegister::PushFunctionName(HELP,"void","void",
					"Muestra todas las funciones de esta clase"),
					&CScriptRegister::Help)
    ];
}