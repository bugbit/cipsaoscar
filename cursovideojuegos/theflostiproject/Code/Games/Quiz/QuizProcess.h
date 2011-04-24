//----------------------------------------------------------------------------------
// CQuizProcess class
// Author: Enric Vergara
//
// Description:
// Esta clase se encarga de gestionar el juego QuizGame
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_QUIZ_GAME_PROCESS_H_
#define INC_QUIZ_GAME_PROCESS_H_

//---Engine Includes---
#include "Core/Process.h"
#include "Core/Core.h"
//---------------------

//--Forward Declaration--
//Engine:
class CRenderManager;
class CGameLogicBoard;
class CInputManager;
//-----------------------

class CQuizProcess: public CProcess
{
public:
	//---Init and End protocols
	CQuizProcess(const std::string& processName): CProcess(processName) {}
	virtual ~CQuizProcess(void) {Done();}

	//----CScriptRegister Interface---------------------------------------
	virtual void				RegisterFunctions					(CScriptManager* scriptManager);

	//----CProcess Interface---------------------------------------
	virtual bool	      Init	              ();
	

	//---Update and Render function
	virtual void				Update	            (float elapsedTime);
	virtual void				RenderScene					(CRenderManager* renderManager, CFontManager* fontManager);
	virtual uint32			RenderDebugInfo			(CRenderManager* renderManager, CFontManager* fontManager, float fps);
	//--------------------------------------------------------------
	bool								Start								();
	bool								Exit								();

private:
	//----CProcess Interface---------------------------------------
	virtual void				Release             ();
	//--------------------------------------------------------------

private:
};

static CQuizProcess* GetQuizGame() {return static_cast<CQuizProcess*>(CORE->GetProcess());}

#endif //INC_BOARD_GAME_PROCESS_H_