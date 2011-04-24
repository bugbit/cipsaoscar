//----------------------------------------------------------------------------------
// CProcess class
// Author: Enric Vergara
//
// Description:
// Esta clase se encarga la aplicacion Viewer
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_VIEWER_PROCESS_H_
#define INC_VIEWER_PROCESS_H_

//---Engine Includes---
#include "Core/Core.h"
#include "Core/Process.h"
//-----------------------

//--Forward Declaration--
class CRenderManager;
class CObject3D;  
class CInputManager;
//-----------------------

class CViewerProcess: public CProcess
{
public:
	//---Init and End protocols
	CViewerProcess(const std::string& processName):  CProcess(processName), m_pObject3D(NULL){}
	virtual ~CViewerProcess(void) {Done();}

	//----CProcess Interface---------------------------------------
	virtual bool	      Init	              ();
	
	//---Update and Render function
	virtual void				Update	            (float elapsedTime);
	virtual void				RenderScene					(CRenderManager* renderManager, CFontManager* fm);
	virtual uint32			RenderDebugInfo			(CRenderManager* renderManager, CFontManager* fm, float fps);
	//--------------------------------------------------------------

	//----CScriptRegister Interface---------------------------------------
	virtual void				RegisterFunctions			(CScriptManager* scriptManager);

private:
	//----CProcess Interface---------------------------------------
	virtual void				Release             ();
	//--------------------------------------------------------------

	void								UpdateInputActions	(CInputManager* inputManager);

private:
	CObject3D*				m_pObject3D;
};

static CViewerProcess* GetViewer() {return static_cast<CViewerProcess*>(CORE->GetProcess());}

#endif //INC_VIEWER_PROCESS_H_