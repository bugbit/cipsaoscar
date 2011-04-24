#include "__PCH_Script.h"

#include "ScriptRegister.h"
#include "Logger/Logger.h"
#include "Core/Core.h"
#include "Exceptions/Exception.h"
#include <algorithm>



bool SFunctionInfoSortPredicate(const SFunctionInfo& d1, const SFunctionInfo& d2)
{
	return d1.m_sName < d2.m_sName;
}


const char* CScriptRegister::PushFunctionName	(	const std::string& nameFunction,
																								const std::string& argsIn,
																								const std::string& argOut,
																								const std::string& description )
{
	return nameFunction.c_str();
	//Primero de todo nos aseguramos que no se haya registrado una función con el mismo nombre:
	std::vector<SFunctionInfo>::iterator it		= m_FuntionsInfo.begin();
	std::vector<SFunctionInfo>::iterator itEnd	= m_FuntionsInfo.end();

	bool exist = false;
	for(; it != itEnd; ++it)
	{
		SFunctionInfo info = *it;
		std::string name = info.m_sName;
		if (name.compare(nameFunction)==0)
		{
			exist = true; //Error, ya exitse una función registrada con el mismo nombre:	
			break;
		}
	}

	if (exist)
	{
		LOGGER->AddNewLog(ELL_ERROR, "CScriptRegister::PushFunctionName Ya se ha registrado la funcion:%s en la clase %s",
																					nameFunction.c_str(), m_sClassName.c_str()); 
	}
	else
	{
		SFunctionInfo newInfo;
		newInfo.m_sName		= nameFunction;
		newInfo.m_sArgsIn	= argsIn;
		newInfo.m_sArgOut	= argOut;
		newInfo.m_sDesc		= description;

		m_FuntionsInfo.push_back(newInfo);

		// Sort the vector using predicate and std::sort
		std::sort(m_FuntionsInfo.begin(), m_FuntionsInfo.end(), SFunctionInfoSortPredicate);

	}

	return nameFunction.c_str();
}

const char* CScriptRegister::SetClassName (const std::string& className)
{
	m_sClassName = className;
	return m_sClassName.c_str();
}

void CScriptRegister::Help ()
{
	LOGGER->AddNewLog(ELL_INFORMATION, "------Funciones de la clase:%s-------", m_sClassName.c_str()); 
	std::vector<SFunctionInfo>::iterator it			= m_FuntionsInfo.begin();
	std::vector<SFunctionInfo>::iterator itEnd	= m_FuntionsInfo.end();
	uint8 index = 0;
	for(; it != itEnd; ++it)
	{
		SFunctionInfo info = *it;
	
		if (info.m_sName.compare(AUTO_COMPLETE) != 0)
		{
			LOGGER->AddNewLog(ELL_INFORMATION, "[%s]",  info.m_sName.c_str());
			LOGGER->AddNewLog(ELL_INFORMATION, "Args: %s  function  (%s)",  info.m_sArgsIn.c_str(), info.m_sArgOut.c_str());
			LOGGER->AddNewLog(ELL_INFORMATION, "Description: %s",  info.m_sDesc.c_str());
			LOGGER->AddNewLog(ELL_INFORMATION, " ");
		}		
		index++;
	}
	LOGGER->AddNewLog(ELL_INFORMATION, "-----------------End-----------------"); 
}


void CScriptRegister::AutoComplete ()
{

	//Quitamos la funcion AUTO_COMPLETE del vector-->
	m_FunctionNames.clear();
	std::vector<SFunctionInfo>::iterator it			= m_FuntionsInfo.begin();
	std::vector<SFunctionInfo>::iterator itEnd	= m_FuntionsInfo.end();
	for (; it!=itEnd; ++it)
	{
		SFunctionInfo info = *it;
		if (info.m_sName.compare(AUTO_COMPLETE) == 0)
		{
			m_FuntionsInfo.erase(it);
			break;
		}
	}

	it		= m_FuntionsInfo.begin();
	itEnd	= m_FuntionsInfo.end();
	for (; it!=itEnd; ++it)
	{
		SFunctionInfo info = *it;
		m_FunctionNames.push_back(info.m_sName);
	}
	
	CORE->SetAutoComplete(m_FunctionNames);
}