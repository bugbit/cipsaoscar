#include "__PCH_Graphics.h"

#include "Texture.h"
#include "TextureManager.h"
#include "Core/Core.h"
#include "RenderManager.h"
#include "Logger/Logger.h"
#include "Exceptions/Exception.h"
#include "Script/ScriptManager.h"
#include "luabind/luabind.hpp"

CTextureManager::~CTextureManager()
{
  UnLoad();
}

void CTextureManager::Init ()
{
  m_pDefaultTexture = new CTexture;
	m_pDefaultTexture->SetAsDefaultTexture();
  bool isOk = false;
  if (m_pDefaultTexture->CreateTexture(50, 50))
  {
    if (m_pDefaultTexture->FillTextureWithColor(50, 50, colGREEN))
    {
      isOk = true;
    }
  }

  if(!isOk)
  {
		std::string msg_error = "CTextureManager::Init-> Error al intentar crear la defualtTexture en la inicialización de CTextureManager";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
    throw CException(__FILE__, __LINE__, msg_error);
  }
}

void CTextureManager::UnLoad ()
{
  for(std::map<std::string,CTexture*>::iterator it=m_Textures.begin();it!=m_Textures.end();++it)
  {
		if (!it->second->IsDefaultTexture())
		{
			CHECKED_DELETE(it->second);
		}
  }

  CHECKED_DELETE(m_pDefaultTexture)	
}

CTexture * CTextureManager::GetTexture (const std::string &FileName)
{
  if(m_Textures.find(FileName)==m_Textures.end())
  {
    CTexture *l_Texture=new CTexture;
    if(!l_Texture->Load(FileName))
    {
      CHECKED_DELETE(l_Texture);
      l_Texture = m_pDefaultTexture;
      CORE->SetAssetError(ASSET_ERROR_TEXTUREFILE);
    }
		m_Textures[FileName]=l_Texture;
		
  }
  return m_Textures[FileName];
}


void CTextureManager::Reload ()
{
  for(std::map<std::string,CTexture*>::iterator it=m_Textures.begin();it!=m_Textures.end();++it)
  {
    it->second->Reload();
  }
}


//----------------------------------------------------------------------------
// LUA BIND
//----------------------------------------------------------------------------
void CTextureManager::RegisterFunctions (CScriptManager* scriptManager)
{
	lua_State* l_pLUAState = scriptManager->GetLuaState();

	using namespace luabind;

	// ahora registramos lo que querramos
	module(l_pLUAState)
		[
			// registramos la clase CTextureManager
			class_<CTextureManager>(CScriptRegister::SetClassName("CTextureManager"))
			
			// registramos su constructor
			.def(constructor<>())

			//-----------GENERAL FUNCTIONS---------------------
			.def(	CScriptRegister::PushFunctionName(AUTO_COMPLETE), &CScriptRegister::AutoComplete)

			.def(	CScriptRegister::PushFunctionName(HELP,"void","void",
						"Muestra todas las funciones de esta clase"), 
						&CScriptRegister::Help)
		];
}