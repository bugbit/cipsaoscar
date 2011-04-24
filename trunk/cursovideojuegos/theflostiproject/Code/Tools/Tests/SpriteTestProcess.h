//----------------------------------------------------------------------------------
// CSpriteTestProcess class
// Author: Enric Vergara
//
// Description:
// Test para probar el funcionamiento....
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_SPRITE_TEST_PROCESS_H_
#define INC_SPRITE_TEST_PROCESS_H_

#include <string>

//---Engine Includes----
#include "Core/Core.h"
#include "Core/Process.h"
#include "Graphics/Sprite.h"
//-----------------------

//--Forward Declaration--
class CRenderManager;
class CObject3D;  
class CInputManager;
//-----------------------

class CSpriteTestProcess: public CProcess
{
public:
	//---Init and End protocols
	CSpriteTestProcess(const std::string& processName):	CProcess(processName),
		m_pObject3D(NULL), m_fSpriteAngle(0.f) {}

	virtual ~CSpriteTestProcess(void) {Done();}

	//----CScriptRegister Interface---------------------------------------
	virtual void				RegisterFunctions			(CScriptManager* scriptManager);

	//----CProcess Interface---------------------------------------
	virtual bool	      Init	              ();
	virtual bool				Start								()  {m_bStart = true; return true;}
	virtual void				RenderScene2D				(CRenderManager* renderManager, CFontManager* fm);

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

	CSprite						m_Sprite;
	CSprite						m_Sprite2;
	float							m_fSpriteAngle;
};

static CSpriteTestProcess* GetSpriteTest() {return static_cast<CSpriteTestProcess*>(CORE->GetProcess());}


#endif //INC_SPRITE_TEST_PROCESS_H_
