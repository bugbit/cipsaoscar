//----------------------------------------------------------------------------------
// CAviPlayerManager class
// Author: Enric Vergara
//
// Description:
// Clase...
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_AVI_PLAYER_MANAGER_H
#define INC_AVI_PLAYER_MANAGER_H

#include <map>
#include "Script/ScriptRegister.h"

//---Forward Declarations---
class CAviPlayer;
class CTexture;
class CScriptManager;
class CRenderManager;
//--------------------------

class CAviPlayerManager:public CScriptRegister
{
public:
	//--- Init and End protocols--------------
	CAviPlayerManager():  m_bIsOk(false), m_pRM(NULL) {}
  virtual ~CAviPlayerManager() { Done(); }  

	bool		      Init			        (CRenderManager* rm);
	void		      Done			        ();
	bool		      IsOk			        () const { return m_bIsOk; }
	//---------------------------------------------------------------

	
	bool					LoadAvis								(const std::string& xmlAviFile);
	void					UnLoadAvis							();
	void					Update									(float deltaTime);
	
	//-----------AviPlayer Functions---------------------
	void					PlayAvi									(const std::string& aviName);
	void					StopAvi									(const std::string& aviName);
	void					PauseAvi								(const std::string& aviName);
	void					SetAviTexture						(const std::string& aviName, CTexture* texture);

		//----CScriptRegister interface-------------------
	virtual void	RegisterFunctions				(CScriptManager* scriptManager);

private:
	void					Release									();

private:
	bool																		m_bIsOk;
	std::map<std::string, CAviPlayer*>			m_AviPlayers;  
	CRenderManager*													m_pRM;
};
#endif //INC_AVI_PLAYER_MANAGER_H