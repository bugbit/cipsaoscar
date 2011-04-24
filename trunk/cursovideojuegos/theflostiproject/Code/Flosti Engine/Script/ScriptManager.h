//----------------------------------------------------------------------------------
// CScriptManager class
// Author: Enric Vergara
//
// Description: Clase encargada de gestionar el script.
// Internamente utiliza LUA
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_SCRIPT_MANAGER_H
#define INC_SCRIPT_MANAGER_H

#include <map>
#include <string>
#include <vector>

extern "C"
{
#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"
}

//---Forward Declarations---
class CScriptRegister;
//--------------------------


class CScriptManager
{

public:
	//---Init and End protocols----
	CScriptManager() :m_pLUAState(NULL), m_bIsOk(false){}
	~CScriptManager() {Done();}  

	bool				Init		  						();
	void				Done									();
	bool				IsOk      						() const { return m_bIsOk; }
	//-----------------------------

	bool				LoadFile							(const std::string &FileName);
	bool				RunScript							(const std::string &Script);
	void				AddNewScriptRegister	(CScriptRegister* element);
	void				RegisterFunctions			(const std::string &InitScript);
	lua_State*	GetLuaState						() {return m_pLUAState;}

private:
	void				Release								();

private:
	std::vector<CScriptRegister*>	m_RegisterElements;
	bool													m_bIsOk;
	lua_State*										m_pLUAState;
};

#endif //INC_SCRIPT_MANAGER_H