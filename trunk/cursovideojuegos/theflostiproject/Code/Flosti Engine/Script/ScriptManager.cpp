#include "__PCH_Script.h"

#include "ScriptManager.h"
#include "Logger/Logger.h"
#include "Core/Core.h"
#include "ScriptRegister.h"
#include "luabind/luabind.hpp"



//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CScriptManager::Release()
{
	if (m_pLUAState)
	{
		lua_close(m_pLUAState);
		//delete m_pLUAState; //:S Peta... 
		m_pLUAState = NULL;
	}
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CScriptManager::Done ()
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}


//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
int32	luaError (lua_State *LUAState)
{
	std::string errorMessage = "ScriptManager:: ";
	errorMessage += lua_tostring(LUAState, -1);
	LOGGER->AddNewLog(ELL_WARNING, errorMessage.c_str());
	lua_pop(LUAState, 1);
	return 0;
}

bool CScriptManager::Init()
{
	LOGGER->AddNewLog(ELL_INFORMATION, "ScriptManager:: Inicializando el manager de scripting");
	m_bIsOk = false;

	m_pLUAState = lua_open();
	m_bIsOk = (m_pLUAState!=NULL);
	if (m_bIsOk)
	{
		luaopen_base(m_pLUAState);           // abrimos la biblioteca base
		luaopen_math(m_pLUAState);           // abrimos la biblioteca de mate
		luabind::open(m_pLUAState);          // requerido para luabind
		
		using namespace luabind;
		//Reescribimos la función _ALERT de LUA
		lua_register(m_pLUAState,"_ALERT",luaError);
	}

	if (!m_bIsOk)
	{
		LOGGER->AddNewLog(ELL_ERROR, "ScriptManager:: Error en la inicializacion.");

		Release();
	}

	return m_bIsOk;
}

bool CScriptManager::LoadFile(const std::string &FileName)
{
	bool isOk = true;

	if(lua_dofile(m_pLUAState,FileName.c_str()))
	{
		isOk = false;
		LOGGER->AddNewLog(ELL_ERROR, "CScriptManager:: Error loading ScriptFile %s", FileName.c_str());
		CORE->SetAssetError(ASSET_ERROR_SCRIPTFILE);
	}

	return isOk;
}

bool CScriptManager::RunScript(const std::string &Script)
{
	bool isOk = true;
	if(lua_dostring(m_pLUAState,Script.c_str()))
	{
		bool isOk = false;
	}
	return isOk;
}

void	CScriptManager::AddNewScriptRegister	(CScriptRegister* element)
{
	std::vector<CScriptRegister*>::iterator it(m_RegisterElements.begin());
	std::vector<CScriptRegister*>::iterator itEnd(m_RegisterElements.end());
	bool exist = false;
	while (it != itEnd)
	{
		CScriptRegister* scriptRegister = *it;
		if (scriptRegister == element)
		{
			//ERROR ya existe
			LOGGER->AddNewLog(ELL_ERROR,"CScriptManager::RegisterFunctions: Error al introducir un IScriptingRegister ya existente");
			exist = true;
		}
		it++;
	}

	if (!exist) 
	{
		m_RegisterElements.push_back(element);
	}
}

//-------------------------------------------------------------------------
//--------------------REGISTER FUNCTIONS-----------------------------------
int ShowMessage (lua_State *L)
{
	MessageBox(NULL,"ShowMessage","",MB_OK);
	return 0;
}
// bool function_1_parameter(std::string &texto)
int ShowMessage2 (lua_State *L)
{
	//1 parameter function que devuelve un booleano
	std::string texto=lua_tostring(L,1);
	MessageBox(NULL,"function_1_parameter",texto.c_str(),MB_OK);
	bool l_Ret=true;
	lua_pushboolean(L, l_Ret);
	return 1;
}

int AutoComplete (lua_State *L)
{
	std::vector<std::string> initVariables = CORE->GetGeneralVariables();
	CORE->SetAutoComplete(initVariables);
	return 0;
}

int Help (lua_State *L)
{
	std::vector<std::string> initVariables = CORE->GetGeneralVariables();
	LOGGER->AddNewLog(ELL_INFORMATION, "------Variables Generales-------"); 
	std::vector<std::string>::iterator it			= initVariables.begin();
	std::vector<std::string>::iterator itEnd	= initVariables.end();
	for(; it != itEnd; ++it)
	{
		std::string variable = *it;
		LOGGER->AddNewLog(ELL_INFORMATION, "    [%s]", variable.c_str()); 
	}
	LOGGER->AddNewLog(ELL_INFORMATION, "-----------------End------------"); 

	return 0;
}


void CScriptManager::RegisterFunctions (const std::string& initScript)
{
	LOGGER->AddNewLog(ELL_INFORMATION, "ScriptManager:: Inicializando el registro de funciones.");
	
	//Vamos a recorrer el string initScript en busca de las variables generales:
	std::vector<std::string> initVariables;
	std::string::size_type pos, end;
	std::string sub = initScript;
	pos = initScript.find( "_", 0 );
	while( pos != std::string::npos ) 
	{
			end = sub.find( " ", 0 );
			if (end == std::string::npos) break;
			std::string variable = sub.substr(pos, end);
			initVariables.push_back(variable);
			sub = sub.substr(end);
			pos = sub.find( "_", 0 );
			if (pos == std::string::npos) break;
			sub = sub.substr(pos);
			pos = sub.find( "_", 0 );
	}
	initVariables.push_back(HELP);
	CORE->SetGeneralVariables(initVariables);
	

	//Registramos las funciones de todos los CScriptRegister:
	std::vector<CScriptRegister*>::iterator it(m_RegisterElements.begin());
	std::vector<CScriptRegister*>::iterator itEnd(m_RegisterElements.end());
	bool exist = false;
	while (it != itEnd)
	{
		CScriptRegister* scriptRegister = *it;
		scriptRegister->RegisterFunctions(this);
		it++;
	}
	
	RunScript(initScript);

	//--- Register Functions
	lua_register(m_pLUAState,"showMessage",		ShowMessage);
	lua_register(m_pLUAState,"showMessage2",	ShowMessage2);
	lua_register(m_pLUAState,HELP,					Help);
	lua_register(m_pLUAState,AUTO_COMPLETE,		AutoComplete);
	
	LOGGER->AddNewLog(ELL_INFORMATION, "ScriptManager:: Finalizado el registro de funciones.");

}
