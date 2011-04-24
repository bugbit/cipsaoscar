//----------------------------------------------------------------------------------
// CCore class
// Author: Enric Vergara
//
// Description:
// Esta clase se encarga de gestionar todos los managers del Engine
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_CORE_H_
#define INC_CORE_H_

#include <windows.h>
#include "Singleton.h"
#include "Script/ScriptRegister.h"
#include "EngineDefs.h"
#include "AssetErrors.h"
#include <string>
#include <map>


//--Forward Declaration--
class CFlostiEngine;
class CProcess;
class CConsole;
class CInputManager;
class CActionToInput;
class CRenderManager;
class CFontManager;
class CPhysicsManager;
class CScriptManager;
class CTextureManager;
class CEffectManager;
class CGUIManager;
class CNetworkManager;
class CSoundManager;
class CAviPlayerManager;
class CLanguageManager;
class CComputerVisionManager;
//-----------------------


class CCore: public CSingleton<CCore>, public CScriptRegister
{
public:
	typedef std::vector<std::string>							tStringVec;

public:
	
	//---Init and End protocols-----------------------
	CCore():	m_bIsOk(false), m_pApplication(NULL), m_pInputManager(NULL), m_pConsole(NULL),
						m_pRenderManager(NULL), m_pPhysicsManager(NULL), m_pScriptManager(NULL), 
						m_pTextureManager(NULL), m_pEffectManager(NULL), m_pGUIManager(NULL), 
						m_pNetworkManager(NULL), m_pSoundManager(NULL),  m_pAviPlayerManager(NULL),
						m_pComputerVisionManager(NULL), m_pLanguageManager(NULL), m_pFontManager(NULL), 
						m_pActionToInput(NULL), m_pProcess(NULL) {}

	virtual ~CCore(void) {Done();}

	void	             		 	Done	              				();
	bool	             		 	IsOk	              				() const { return m_bIsOk; }
	bool	             		 	Init												(	HWND hwnd, CFlostiEngine* app, CConsole* console, const SEngineInitParams& params);

	//---Update and Render function------------------------
	void	             			Update	           					(float elapsedTime);
	void										Render											();
	void										RenderAssetErrors						();

	//---Get Managers---------------------------------------
	CInputManager*	    		GetInputManager		  				()									{return m_pInputManager;}
	CActionToInput*	    		GetActionToInput						()									{return m_pActionToInput;}
	CRenderManager*	    		GetRenderManager	  				()									{return m_pRenderManager;}
	CPhysicsManager*				GetPhysicManager						()									{return m_pPhysicsManager;}
	CScriptManager*	    		GetScriptManager	  				()									{return m_pScriptManager;}
	CTextureManager*				GetTextureManager	  				()									{return m_pTextureManager;}
	CEffectManager*					GetEffectManager	  				()									{return m_pEffectManager;}
	CGUIManager*						GetGUIManager								()									{return m_pGUIManager;}
	CNetworkManager*				GetNetworkManager						()									{return m_pNetworkManager;}
	CSoundManager*					GetSoundManager							()									{return m_pSoundManager;}
	CAviPlayerManager*			GetAviPlayerManager					()									{return m_pAviPlayerManager;}
	CComputerVisionManager*	GetComputerVisionManager		()									{return m_pComputerVisionManager;}
	CLanguageManager*				GetLanguageManager					()									{return m_pLanguageManager;}
	CFontManager*						GetFontManager							()									{return m_pFontManager;}

	
	

	//--- Process functions---------------------------------
	CProcess*								GetProcess									()									{return m_pProcess;}
	void										SetProcess									(CProcess* process)	{m_pProcess = process;}
	void										ResetCurrentProcess					();
	void										ChangeProcess								(const std::string& newProcess);

	//--- Error functions------------------------------------
	bool										SaveLogsInFile							();
  void										SetAssetError								(TypeOfAssetError error);
  void										ResetAssetErrors						();

	const tStringVec&				GetGeneralVariables	() const											{return m_InitVariables;}
	void										SetGeneralVariables	(tStringVec& vec)							{m_InitVariables = vec;}
	void										SetAutoComplete			(const std::vector<std::string>& vector);

	//----CScriptRegister interface-------------------
	virtual void						RegisterFunctions						(CScriptManager* scriptManager);

	bool										IsEndTimer									(const std::string& id, float timeOut);
	void										Finished										();			
	void										SetLinesPerPageInLogger			(uint32 lines);

private:
	void										Release											();
	void										UpdateTimers								(float elapsedTime);

private:
	typedef std::map<std::string, float>	tTimerMap;
	
private:
  bool				        		m_bIsOk;													// Initialization boolean control
  CProcess*								m_pProcess;												// Pointer to Process
	CConsole*								m_pConsole;												// Pointer to Console
  tTimerMap								m_Timers;													// Timers
  tStringVec							m_InitVariables;									// Vector of general variables registered in Lua
	CFlostiEngine*					m_pApplication;										// Pointer to Application
	CAssetErrors						m_AssetErrors;										// Errors Assets

  //---Pointers to Manager--------
  CInputManager*		  		m_pInputManager;									// Pointer to Input Manager
	CActionToInput*					m_pActionToInput;									// Pointer to Action To Input
  CRenderManager*		  		m_pRenderManager;									// Pointer to Render Manager
  CPhysicsManager*				m_pPhysicsManager;								// Pointer to Physics Manager
  CScriptManager*	    		m_pScriptManager;	    						// Pointer to Scripting Manager
  CTextureManager*				m_pTextureManager;								// Pointer to Texture Manager
  CEffectManager*					m_pEffectManager;									// Pointer to Effect(shaders) Manager
  CGUIManager*						m_pGUIManager;										// Pointer to GUI Manager
  CNetworkManager*				m_pNetworkManager;								// Pointer to Network Manager
  CSoundManager*					m_pSoundManager;									// Pointer to Sound Manager
	CAviPlayerManager*			m_pAviPlayerManager;							// Pointer to Avi Player Manager
  CComputerVisionManager*	m_pComputerVisionManager;					// Pointer to Computer Vision Manager
	CLanguageManager*				m_pLanguageManager;								// Pointer to Language Manager
	CFontManager*						m_pFontManager;										// Pointer to Font Manager
};

#endif //INC_CORE_H_