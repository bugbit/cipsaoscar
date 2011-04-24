//----------------------------------------------------------------------------------
// CFlostiEngine class
// Author: Enric Vergara
//
// Description:
// Esta clase se encarga de poner en marcha una aplicacion Win32 haciendo uso de nuestro Core
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_FLOSTI_ENGINE_H_
#define INC_FLOSTI_ENGINE_H_

#include <Windows.h>
#include "LogRender.h"
#include "Timer/Timer.h"
#include "EngineDefs.h"

//---Forward Declarations---
class CCore;
class CCamera;
class CProcess;
class CConsole;
//--------------------------



class CFlostiEngine
{
public:
	//---Init and End protocols
	CFlostiEngine():	m_bIsOk(false), m_pCore(NULL), m_Timer(30), 
										m_pCurrentProcess(NULL), m_bDrawDebugInf(true),
										m_pConsole(NULL), m_bScreenshot(false) {}

	virtual ~CFlostiEngine(void) {Done();}

	void	            Done		          				();
	bool	            IsOk		          				() const { return m_bIsOk; }
	bool							LoadInitParams						(const std::string& xmlFile);
	bool							Init											(std::vector<CProcess*>& processes, HWND hwnd);
	bool							Exit											();

	//---Update and Render functions
	void	            Update	          				();
	void	            Render	          				();
	void							RenderScene								(CRenderManager* renderManager);
	void							Screenshot								() {m_bScreenshot=true;}

	void							MsgProc										(WPARAM wp, LPARAM lp);
	const SEngineInitParams&	GetInitParams	() const {return m_sInitParams;}

	//---Process functions
	void							ResetCurrentProcess				();
	void							ChangeProcess							(const std::string& newProcess);

	//--Logger---
	void							SetLinesPerPageInLogger	(uint32 lines) {m_LogRender.SetLinePerPage(lines);}


private:            
	void	            Release										();
	void							RenderLogger							();
	void							UpdateInputActions				();

private:
	bool			       					m_bIsOk;						// Initialization boolean control
	bool											m_bDrawDebugInf;		// Boolean to control debug info render
	CTimer										m_Timer;						// Timer
	CCore*			     					m_pCore;						// Pointer to Core
	CLogRender								m_LogRender;				// Logger render
	CConsole*									m_pConsole;					// Pointer to Console
	std::vector<CProcess*>		m_pProcesses;				// Pointer to all processes 
	CProcess*									m_pCurrentProcess;	// Pointer to current process
	SEngineInitParams					m_sInitParams;			// Init parameters
	bool											m_bScreenshot;
};

#endif //INC_APPLICATION_H_