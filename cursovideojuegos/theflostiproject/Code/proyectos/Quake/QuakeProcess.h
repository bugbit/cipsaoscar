#pragma once

#include "playerinput.h"
#include "GUIPlayer.h"
#include "GameLogic.h"

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
class CPlayerRender;
//-----------------------

class CQuakeProcess: public CProcess
{
public:
	//---Init and End protocols
	CQuakeProcess(const std::string& processName): CProcess(processName),
																										m_IsCameraView(false),
																										m_pCameraView(NULL),
																										m_CameraViewObj3D(NULL),
																										m_drawAxisGrid(false)
																										{}
	virtual ~CQuakeProcess(void) {Done();}

	//----CScriptRegister Interface---------------------------------------
	virtual void				RegisterFunctions			(CScriptManager* scriptManager);

	//----CProcess Interface---------------------------------------
	virtual bool	      Init	              ();
	virtual void				RenderScene2D				(CRenderManager* renderManager, CFontManager* fm);
	
	//---Update and Render function
	virtual void				Update	            (float elapsedTime);
	virtual void				RenderScene					(CRenderManager* renderManager, CFontManager* fontManager);
	virtual uint32			RenderDebugInfo			(CRenderManager* renderManager, CFontManager* fontManager, float fps);
	//--------------------------------------------------------------

	virtual CCamera*		GetCamera						() const;

	void								ReloadItemModels		();

private:
	//----CProcess Interface---------------------------------------
	virtual void				Release             ();
	//--------------------------------------------------------------

private:
	bool																		m_IsCameraView;
	CThPSCamera	*														m_pCameraView;
	CObject3D *															m_CameraViewObj3D;
	bool																		m_drawAxisGrid;
	CGameLogic															m_GameLogic;
	std::vector<CPlayerInput *>							m_PlayerInputs;
	std::vector<CPlayerRender *>						m_PlayerRenders;
	CGUIPlayer															m_GUIPlayer;

	void																		UpdateCameraView		(CInputManager* inputManager);
	void																		RenderPlayers				(CRenderManager* renderManager, CFontManager* fontManager);
	void																		UpdateInputActions	(CInputManager* inputManager);
	void																		UpdatePlayerInputs	(float elapsedTime);
	void																		ReleasePlayerInputs	();
	void																		ReleasePlayerRenders	();
};

static CQuakeProcess* GetGameQuake() {return static_cast<CQuakeProcess*>(CORE->GetProcess());}
