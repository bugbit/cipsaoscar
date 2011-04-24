//----------------------------------------------------------------------------------
// CTextureManager class
// Author: Jordi Arnal
//
// Description:
// Clase...
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_TEXTURE_MANAGER_H
#define INC_TEXTURE_MANAGER_H

#include <d3dx9.h>

#include "Script/ScriptRegister.h"

//---Forward Declarations---
class CTexture;
class CScriptManager;
//--------------------------

class CTextureManager:public CScriptRegister
{
public:
  CTextureManager(): m_pDefaultTexture(NULL) {}
  ~CTextureManager();

  void				Init							();
  CTexture *	GetTexture				(const std::string &FileName);
	CTexture *	GetDefualtTexture	() {return m_pDefaultTexture;}
  void				Reload						();

	//----CScriptRegister interface-------------------
	virtual void	RegisterFunctions	(CScriptManager* scriptManager);

private:
  void				UnLoad						();

private:
  std::map<std::string,CTexture *>	m_Textures;
  CTexture*													m_pDefaultTexture;
};

#endif //END INC_TEXTURE_MANAGER_H