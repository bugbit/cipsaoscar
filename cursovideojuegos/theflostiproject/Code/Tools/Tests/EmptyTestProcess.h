//----------------------------------------------------------------------------------
// CEmptyTestProcess class
// Author: Enric Vergara
//
// Description:
// Test para probar el funcionamiento del reproductor de videos AVI.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_EMPTY_TEST_PROCESS_H_
#define INC_EMPTY_TEST_PROCESS_H_

#include <string>

//---Engine Includes----
#include "Core/Core.h"
#include "Core/Process.h"
//-----------------------

//--Forward Declaration--
class CRenderManager;
class CObject3D;  
class CInputManager;
//-----------------------

class CEmptyTestProcess: public CProcess
{
public:
	//---Init and End protocols
	CEmptyTestProcess(const std::string& processName):	CProcess(processName),
		m_pObject3D(NULL) {}
	virtual ~CEmptyTestProcess(void) {Done();}

	//----CProcess Interface---------------------------------------
	virtual bool	      Init	              ();
	virtual bool				Start								()  {m_bStart = true; return true;}

	//---Update and Render function
	virtual void				Update	            (float elapsedTime);
	virtual void				RenderScene					(CRenderManager* renderManager, CFontManager* fm);
	virtual uint32			RenderDebugInfo			(CRenderManager* renderManager, CFontManager* fm, float fps);
	//--------------------------------------------------------------



private:
	//----CProcess Interface---------------------------------------
	virtual void				Release             ();
	//--------------------------------------------------------------

	void								UpdateInputActions	(CInputManager* inputManager);

private:

	CObject3D*				m_pObject3D;
};

static CEmptyTestProcess* GetEmptyTest() {return static_cast<CEmptyTestProcess*>(CORE->GetProcess());}


#endif //INC_EMPTY_TEST_PROCESS_H_