//----------------------------------------------------------------------------------
// CProcess class
// Author: Enric Vergara
//
// Description:
// Esta clase se encarga de gestionar el juego Poker Texas Holdem
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_POKER_TEXAS_HOLDEM_PROCESS_H_
#define INC_POKER_TEXAS_HOLDEM_PROCESS_H_

//---Engine Includes---
#include "Core/Process.h"
#include "Core/Core.h"
//---------------------

//--Forward Declaration--
//Engine:
class CRenderManager;
class CObject3D;
class CPhysicActor;
class CInputManager;
//-----------------------


class CPokerTHProcess: public CProcess
{
public:
	//---Init and End protocols
	CPokerTHProcess(const std::string& processName): CProcess(processName), m_pObject3D(NULL){}
	virtual ~CPokerTHProcess(void) {Done();}

		//----CScriptRegister Interface---------------------------------------
	virtual void				RegisterFunctions					(CScriptManager* scriptManager);

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
};

static CPokerTHProcess* GetPokerTHGame() {return static_cast<CPokerTHProcess*>(CORE->GetProcess());}

#endif //INC_POKER_TEXAS_HOLDEM_PROCESS_H_