//----------------------------------------------------------------------------------
// CScriptRegister interface
// Author: Enric Vergara
//
// Description: clase con la funcionalidad para registrar funciones c en el scriptManager.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_SCRIPT_REGISTER_H
#define INC_SCRIPT_REGISTER_H

#include <vector>

#define AUTO_COMPLETE "autoComplete"
#define HELP					"help"

//---Forward Declarations---
class CScriptManager;
//--------------------------

//----Declaracion de nuevos tipos---
struct SFunctionInfo
{
	std::string	m_sName;
	std::string m_sArgsIn;
	std::string	m_sArgOut;
	std::string	m_sDesc;
};

//----------------------------------

class CScriptRegister
{
public:
	CScriptRegister(): m_sClassName("") {}
	virtual ~CScriptRegister() { /*Nothing*/; }

	virtual void										RegisterFunctions	(CScriptManager* scriptManager) = 0;

	const char*											PushFunctionName	(	const std::string& nameFunction,
																											const std::string& argsIn = "",
																											const std::string& argOut = "",
																											const std::string& description = "");

	const char*											SetClassName			(const std::string& className);
	void														Help							();
	void														AutoComplete			();
	const std::vector<std::string>&	GetFunctionNames	() const {return m_FunctionNames;}

private:
	//bool														SFunctionInfoSortPredicate	(const SFunctionInfo& d1, const SFunctionInfo& d2);
private:
	std::vector<SFunctionInfo>	m_FuntionsInfo;
	std::vector<std::string>		m_FunctionNames;
	std::string									m_sClassName;
};

#endif //INC_SCRIPT_REGISTER_H