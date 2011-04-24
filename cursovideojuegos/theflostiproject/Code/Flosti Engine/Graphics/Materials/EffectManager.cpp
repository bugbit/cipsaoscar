#include "__PCH_Graphics.h"
#include "Script/ScriptManager.h"
#include "Materials/effectmanager.h"
#include "Materials/effect.h"
#include "Core/Core.h"
#include "RenderManager.h"
#include "VertexType/VertexType.h"
#include <assert.h>
#include "luabind/luabind.hpp"

CEffectManager::CEffectManager()
{
}

CEffectManager::~CEffectManager()
{
	CleanUp();
}

void CEffectManager::UnLoadEffects()
{
	for(TEffectMapIterator b=m_Effects.begin();b!=m_Effects.end();++b)
	{
		CHECKED_DELETE(b->second);
	}
	m_Effects.clear();
}

void CEffectManager::CleanUp()
{
	UnLoadEffects();
}

void CEffectManager::LoadEffects()
{
	for(TEffectFileNameMapIterator b=m_EffectsFileNames.begin();b!=m_EffectsFileNames.end();++b)
	{
		CEffect *l_Effect=new CEffect;
		if(l_Effect->Load(b->second.c_str()))
			m_Effects[b->first]=l_Effect;
		else
		{
			CHECKED_DELETE(l_Effect);
		}
	}
}

bool CEffectManager::Reload()
{
	for(TEffectMapIterator b=m_Effects.begin();b!=m_Effects.end();++b)
	{
		b->second->Reload();
	}
	return true;
}

bool CEffectManager::AddEffect(const std::string &Name, const std::string &FileName)
{
	m_EffectsFileNames[Name]=FileName;
	return true;
}

bool CEffectManager::Load(const std::string &FileName)
{
	AddEffect("normalmapeffect","data\\effect.fx");
	LoadEffects();
	return true;
}

CEffect * CEffectManager::GetEffect(const std::string &EffectName)
{
	TEffectMapIterator l_It=m_Effects.find(EffectName);
	if(l_It!=m_Effects.end())
		return l_It->second;
	return NULL;
}

void CEffectManager::Init()
{
	//Creamos los VertexDeclarations
	TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX::GetVertexDeclaration(CCore::GetSingleton().GetRenderManager());
	Load("./data/effects.xml");
}

void CEffectManager::Destroy()
{
	//Destruimos los VertexDeclarations
	TNORMAL_TANGENT_BINORMAL_TEXTURED_VERTEX::ReleaseVertexDeclaration();
}


//----------------------------------------------------------------------------
// LUA BIND
//----------------------------------------------------------------------------
void CEffectManager::RegisterFunctions (CScriptManager* scriptManager)
{
	lua_State* l_pLUAState = scriptManager->GetLuaState();

	using namespace luabind;

	// ahora registramos lo que querramos
	module(l_pLUAState)
		[
			// registramos la clase CEffectManager
			class_<CEffectManager>(CScriptRegister::SetClassName("CEffectManager"))
			
			// registramos su constructor
			.def(constructor<>())

			//-----------GENERAL FUNCTIONS---------------------
			.def(	CScriptRegister::PushFunctionName(AUTO_COMPLETE), &CScriptRegister::AutoComplete)

			.def(	CScriptRegister::PushFunctionName(HELP,"void","void",
						"Muestra todas las funciones de esta clase"), 
						&CScriptRegister::Help)
		];
}