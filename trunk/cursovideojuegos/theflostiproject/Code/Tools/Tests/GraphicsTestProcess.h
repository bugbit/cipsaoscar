//----------------------------------------------------------------------------------
// CGraphicsTestProcess class
// Author: Enric Vergara
//
// Description:
// Test para probar el funcionamiento del modulo Grpahics.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_GRAPHICS_TEST_PROCESS_H_
#define INC_GRAPHICS_TEST_PROCESS_H_

//---Engine Includes----
#include "Core/Core.h"
#include "Core/Process.h"
#include "Graphics/Mesh/StaticMesh.h"
#include "Graphics/Materials/EffectManager.h"
#include "Graphics/Textures/TextureManager.h"
//-----------------------


//--Forward Declaration--
class CRenderManager;
class CObject3D;  
class CTexture;
class CInputManager;
//-----------------------

class CGraphicsTestProcess: public CProcess
{
public:
	//---Init and End protocols
	CGraphicsTestProcess(const std::string& processName): CProcess(processName),
																												m_pObject3D(NULL), 
																												m_pMesh(NULL), 
																												m_fTranslate(0.f),
																												m_pQuadTexture(NULL),
																												m_pQuadTextureFragile1(NULL),
																												m_pQuadTextureFragile2(NULL) {}
	virtual ~CGraphicsTestProcess(void) {Done();}

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
	void								HardCodedLoadMesh		();

private:
	CObject3D*				m_pObject3D;
	CStaticMesh*			m_pMesh;
	float							m_fTranslate;
	CTexture*					m_pQuadTexture;
	CTexture*					m_pQuadTextureFragile1;
	CTexture*					m_pQuadTextureFragile2;
};

static CGraphicsTestProcess* GetGrpahicTest() {return static_cast<CGraphicsTestProcess*>(CORE->GetProcess());}

#endif //INC_GRAPHICS_TEST_PROCESS_H_