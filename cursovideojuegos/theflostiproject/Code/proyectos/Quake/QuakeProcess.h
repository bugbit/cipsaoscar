//----------------------------------------------------------------------------------
// CConsoleAndLoggerProcess class
// Author: Enric Vergara
//
// Description:
// Esta clase se encarga la aplicacion Console And Logger
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_AIR_HOCKEY_GAME_PROCESS_H_
#define INC_AIR_HOCKEY_GAME_PROCESS_H_

#include "Arena.h"
#include "playerinput.h"

//---Engine Includes---
#include "Core/Process.h"
#include "Core/Core.h"
//---------------------


//--Forward Declaration--
//Engine:
class CRenderManager;
class CObject3D;  
class CInputManager;
class CASEObject;
class CThPSCamera;
class CObject3D;
//-----------------------

class CQuakeProcess: public CProcess
{
public:
	//---Init and End protocols
	CQuakeProcess(const std::string& processName): CProcess(processName),
																										m_IsCameraView(false),
																										m_pCameraView(NULL),
																										m_CameraViewObj3D(NULL),
																										m_drawAxisGrid(true)
																										{}
	virtual ~CQuakeProcess(void) {Done();}

	//----CScriptRegister Interface---------------------------------------
	virtual void				RegisterFunctions			(CScriptManager* scriptManager);

	//----CProcess Interface---------------------------------------
	virtual bool	      Init	              ();
	
	//---Update and Render function
	virtual void				Update	            (float elapsedTime);
	virtual void				RenderScene					(CRenderManager* renderManager, CFontManager* fontManager);
	virtual uint32			RenderDebugInfo			(CRenderManager* renderManager, CFontManager* fontManager, float fps);
	//--------------------------------------------------------------

	virtual CCamera*		GetCamera						() const;

private:
	//----CProcess Interface---------------------------------------
	virtual void				Release             ();
	//--------------------------------------------------------------

private:
	bool																		m_IsCameraView;
	CThPSCamera	*														m_pCameraView;
	CObject3D *															m_CameraViewObj3D;
	bool																		m_drawAxisGrid;
	CArena																	m_pArena;
	std::vector<CPlayerInput *>							m_PlayerInputs;

	void																		UpdateCameraView		(CInputManager* inputManager);
	void																		UpdateInputActions	(CInputManager* inputManager);
	void																		UpdatePlayerInputs	(float elapsedTime);
};

static CQuakeProcess* GetGameQuake() {return static_cast<CQuakeProcess*>(CORE->GetProcess());}

#endif //INC_AIR_HOCKEY_GAME_PROCESS_H_