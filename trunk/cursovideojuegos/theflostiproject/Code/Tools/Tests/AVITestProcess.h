//----------------------------------------------------------------------------------
// CAVITestProcess class
// Author: Enric Vergara
//
// Description:
// Test para probar el funcionamiento del reproductor de videos AVI.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_AVI_TEST_PROCESS_H_
#define INC_AVI_TEST_PROCESS_H_

//---Engine Includes----
#include "Core/Core.h"
#include "Core/Process.h"
#include "Math/LerpAnimator1D.h"
//-----------------------

//--Forward Declaration--
class CRenderManager;
class CFontManager;
class CObject3D;  
class CInputManager;
class CAviPlayer;
class CTexture;
//-----------------------

class CAVITestProcess: public CProcess
{
public:
	//---Init and End protocols
	CAVITestProcess(const std::string& processName):	CProcess(processName),
																										m_pObject3D(NULL),
																										m_pQuadTextureFragile1(NULL),
																										m_pAviTexture1(NULL),
																										m_pAviTexture2(NULL) {}
	virtual ~CAVITestProcess(void) {Done();}

	//----CScriptRegister Interface---------------------------------------
	virtual void				RegisterFunctions			(CScriptManager* scriptManager);

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
	CTexture*					m_pQuadTextureFragile1;
	CTexture*					m_pAviTexture1;
	CTexture*					m_pAviTexture2;
	CLerpAnimator1D		m_LerpAnimator1D;
	uint32						m_uSoundSource;
};

static CAVITestProcess* GetAVITest() {return static_cast<CAVITestProcess*>(CORE->GetProcess());}

#endif //INC_AVI_TEST_PROCESS_H_