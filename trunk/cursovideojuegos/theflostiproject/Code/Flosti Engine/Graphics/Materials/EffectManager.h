//----------------------------------------------------------------------------------
// CEffectManager class
// Author: Enric Vergara
//
// Description:
// Clase...
//----------------------------------------------------------------------------------
#pragma once
#ifndef EFFECT_MANAGER_H
#define EFFECT_MANAGER_H

#include <d3dx9.h>
#include <map>
#include <string>

#include "Script/ScriptRegister.h"

//---Forward Declarations---
class CEffect;
class CScriptManager;
//--------------------------

class CEffectManager:public CScriptRegister
{
public:
	CEffectManager();
	~CEffectManager();

	void						Init					();
	void						Destroy				();
	bool						Reload				();
	bool						Load					(const std::string &FileName);
	CEffect *				GetEffect			(const std::string &EffectName);

		//----CScriptRegister interface-------------------
	virtual void	RegisterFunctions	(CScriptManager* scriptManager);

private:
	void						CleanUp				();
	void						LoadEffects		();
	void						UnLoadEffects	();
	bool						AddEffect			(const std::string &Name, const std::string &FileName);

private:
	//--Types---
	typedef std::map<std::string,std::string>		TEffectFileNameMap;
	typedef TEffectFileNameMap::iterator				TEffectFileNameMapIterator;
	typedef std::map<std::string,CEffect *>			TEffectMap;
	typedef TEffectMap::iterator								TEffectMapIterator;
	//----------

	TEffectMap					m_Effects;
	TEffectFileNameMap	m_EffectsFileNames;
};

#endif //EFFECT_MANAGER_H