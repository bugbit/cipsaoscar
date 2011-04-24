#include "__PCH_Tests.h"

#include "SpriteTestProcess.h"
#include "Core/Core.h"
#include "Input/InputManager.h"
#include "Graphics/RenderManager.h"
#include "Graphics/Object3D.h"
#include "Graphics/ThPSCamera.h"
#include "Graphics/Textures/TextureManager.h"
#include "Script/ScriptManager.h"
#include "luabind/luabind.hpp"

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
bool CSpriteTestProcess::Init ()
{
	CProcess::m_bIsOk = false;
	m_pObject3D = new CObject3D(Vect3f(0.f,0.f,0.f), 0.f, 0.f);
	uint32 w,h;
	CORE->GetRenderManager()->GetWidthAndHeight(w,h);
	float aspect_ratio = (float)w/h;
	m_pCamera = new CThPSCamera(1.f,500.f,mathUtils::Deg2Rad(60.f),aspect_ratio,m_pObject3D,50.f);

	if (m_pObject3D && m_pCamera) 
	{
		CProcess::m_bIsOk = true;
	}

	if (m_bIsOk)
	{
		m_Sprite.SetPosition(Vect2f(200.f,100.f));
		m_Sprite.SetTexture(CORE->GetTextureManager()->GetTexture("./Data/Textures/fragile1.jpg"));


		m_Sprite2.SetPosition(Vect2f(200.f,300.f));
		m_Sprite2.SetTexture(CORE->GetTextureManager()->GetTexture("./Data/Textures/explosion_30_128.tga"));
		m_Sprite2.SetTotalFrames(30);
		m_Sprite2.SetColumns(6);
		m_Sprite2.SetSize(128,128);
		m_Sprite2.SetFrameTimer(40);
	}

	if (!CProcess::m_bIsOk)
	{
		Release();
	}
	return CProcess::m_bIsOk;
}

void CSpriteTestProcess::Release ()
{
	CHECKED_DELETE(m_pObject3D);
	CHECKED_DELETE(m_pCamera);
}

void CSpriteTestProcess::RenderScene (CRenderManager* renderManager, CFontManager* fm)
{
	renderManager->DrawAxis(20.f);
	renderManager->DrawGrid(50.f,colWHITE,20,20);
	renderManager->DrawCamera(m_pCamera);
}

void CSpriteTestProcess::RenderScene2D(CRenderManager* renderManager, CFontManager* fm)
{
	m_Sprite.Draw(renderManager);
	m_Sprite2.Draw(renderManager);
}

uint32 CSpriteTestProcess::RenderDebugInfo(CRenderManager* renderManager, CFontManager* fm, float fps)
{
	uint32 posY = 0;
	posY = CProcess::RenderDebugInfo(renderManager, fm, fps);
	if (m_bRenderDebugInfo)
	{
		uint32 posX = m_PosRenderDebugInfo.x;
		//...
	}
	return posY;
}

void CSpriteTestProcess::Update (float elapsedTime)
{
	CProcess::Update(elapsedTime);

	CCore * core = CCore::GetSingletonPtr();
	CInputManager*inputManager = core->GetInputManager();

	m_fSpriteAngle += elapsedTime;
	m_Sprite.SetScaleX(fabs(sin(m_fSpriteAngle*0.1)));
	m_Sprite.SetScaleY(fabs(sin(m_fSpriteAngle*0.1)));
	m_Sprite.SetRotation(m_fSpriteAngle);

	
	m_Sprite2.Animate();

	UpdateInputActions(inputManager);	
}


void CSpriteTestProcess::UpdateInputActions	(CInputManager* inputManager)
{
	
}



//-----------------ScriptManager------------------------------
void CSpriteTestProcess::RegisterFunctions (CScriptManager* scriptManager)
{
	lua_State* l_pLUAState = scriptManager->GetLuaState();
	using namespace luabind;

	// ahora registramos lo que querramos
	module(l_pLUAState)
		[
			def("getSpriteTest", GetSpriteTest),

			// registramos la clase CSpriteTestProcess
			class_<CSpriteTestProcess>(CScriptRegister::SetClassName("CSpriteTestProcess"))


			// registramos su constructor
			.def(constructor<const std::string&>())

			// registramos sus funciones
			// registramos sus funciones
			.def(	CScriptRegister::PushFunctionName(AUTO_COMPLETE), &CScriptRegister::AutoComplete)

			.def(	CScriptRegister::PushFunctionName(HELP,"void","void",
			"Muestra todas las funciones de esta clase"),
			&CScriptRegister::Help)
		];
}
