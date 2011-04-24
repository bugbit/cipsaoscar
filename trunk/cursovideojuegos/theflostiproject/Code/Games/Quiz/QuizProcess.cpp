#include "__PCH_Quiz.h"


#include "QuizProcess.h"

//---Engine Includes---
#include "Input/InputManager.h"
#include "Graphics/RenderManager.h"
#include "Logger/Logger.h"
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
bool CQuizProcess::Init ()
{
	//Load sounds, meshes, 
	return true;
}

bool CQuizProcess::Start ()
{
	return false;
}

void CQuizProcess::Release ()
{
	
}

void CQuizProcess::RenderScene (CRenderManager* renderManager, CFontManager* fontManager)
{
	if (CProcess::m_bStart)
	{
		/*Nothing...*/
	}
}

uint32 CQuizProcess::RenderDebugInfo(CRenderManager* renderManager, CFontManager* fontManager, float fps)
{
	return CProcess::RenderDebugInfo(renderManager,fontManager, fps);
}

void CQuizProcess::Update (float elapsedTime)
{
	if (CProcess::m_bStart)
	{
		CProcess::Update(elapsedTime);

		
	}
}

bool CQuizProcess::Exit ()
{
	CProcess::m_bStart = false;
	return true;
}


//-----------------ScriptManager------------------------------
void CQuizProcess::RegisterFunctions (CScriptManager* scriptManager)
{
	lua_State* l_pLUAState = scriptManager->GetLuaState();
	
	using namespace luabind;
	
	// ahora registramos lo que querramos
  module(l_pLUAState)
    [
			def("getQuizGame", GetQuizGame),

      // registramos la clase CQuizProcess
			class_<CQuizProcess>(CScriptRegister::SetClassName("CQuizProcess"))
			
      // registramos su constructor
      .def(constructor<const std::string&>())

			// registramos sus funciones
			.def(	CScriptRegister::PushFunctionName(AUTO_COMPLETE), &CScriptRegister::AutoComplete)

			.def(	CScriptRegister::PushFunctionName(HELP,"void","void",
						"Muestra todas las funciones de esta clase"),
						&CScriptRegister::Help)

			.def(	CScriptRegister::PushFunctionName("start", "bool", "void", 
						"Ejecuta el inicio del proceso, retorna si la inicializacion ha ido bien"),									
						&CQuizProcess::Start)
    ];
}