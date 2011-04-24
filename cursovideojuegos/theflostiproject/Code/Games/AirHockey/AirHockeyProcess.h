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
//-----------------------

class CAirHockeyGameProcess: public CProcess
{
public:
	//---Init and End protocols
	CAirHockeyGameProcess(const std::string& processName): CProcess(processName), m_pObject3D(NULL){}
	virtual ~CAirHockeyGameProcess(void) {Done();}

	//----CScriptRegister Interface---------------------------------------
	virtual void				RegisterFunctions			(CScriptManager* scriptManager);

	//----CProcess Interface---------------------------------------
	virtual bool	      Init	              ();
	
	//---Update and Render function
	virtual void				Update	            (float elapsedTime);
	virtual void				RenderScene					(CRenderManager* renderManager, CFontManager* fontManager);
	virtual uint32			RenderDebugInfo			(CRenderManager* renderManager, CFontManager* fontManager, float fps);
	//--------------------------------------------------------------



private:
	//----CProcess Interface---------------------------------------
	virtual void				Release             ();
	//--------------------------------------------------------------

	void								UpdateInputActions	(CInputManager* inputManager);

private:

	CObject3D*				m_pObject3D;
	CASEObject*				m_pASEBaseBoard;
};

static CAirHockeyGameProcess* GetGameAirHockey() {return static_cast<CAirHockeyGameProcess*>(CORE->GetProcess());}

#endif //INC_AIR_HOCKEY_GAME_PROCESS_H_