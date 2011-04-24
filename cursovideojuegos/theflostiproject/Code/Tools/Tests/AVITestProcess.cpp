#include "__PCH_Tests.h"

#include "AVITestProcess.h"

//---Engine Includes--------
#include "Core/Core.h"
#include "Language/LanguageManager.h"
#include "Input/InputManager.h"
#include "Input/ActionToInput.h"
#include "Graphics/RenderManager.h"
#include "Graphics/FontManager.h"
#include "Logger/Logger.h"
#include "Graphics/Object3D.h"
#include "Graphics/ThPSCamera.h"
#include "Graphics/Textures/TextureManager.h"
#include "Graphics/Textures/Texture.h"
#include "Graphics/AviPlayer.h"
#include "Graphics/AviPlayerManager.h"
#include "Script/ScriptManager.h"
#include "Sound/SoundManager.h"
#include "luabind/luabind.hpp"
//--------------------------


//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
bool CAVITestProcess::Init ()
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
		CRenderManager* rm = CORE->GetRenderManager();
		
		CAviPlayerManager* aviM = CORE->GetAviPlayerManager();
	
		m_pAviTexture1 = new CTexture();
		aviM->SetAviTexture("lost",m_pAviTexture1);
		aviM->PlayAvi("lost");

		m_pAviTexture2 = new CTexture();
		aviM->SetAviTexture("intro",m_pAviTexture2);
		aviM->PlayAvi("intro");
				

		CTextureManager* textureM = CORE->GetTextureManager();
		m_pQuadTextureFragile1 = textureM->GetTexture("./Data/Textures/fragile2.jpg");
		m_uSoundSource = CORE->GetSoundManager()->CreateSource();
		//CORE->GetSoundManager()->PlaySource2D(m_uSoundSource,"help",true);
	}

	if (!CProcess::m_bIsOk)
	{
		Release();
	}
	return CProcess::m_bIsOk;
}

void CAVITestProcess::Release ()
{
	CHECKED_DELETE(m_pObject3D);
	CHECKED_DELETE(m_pCamera);
	CHECKED_DELETE(m_pAviTexture1);	
	CHECKED_DELETE(m_pAviTexture2);	
	CORE->GetAviPlayerManager()->UnLoadAvis();
}

void CAVITestProcess::RenderScene (CRenderManager* renderManager, CFontManager* fm)
{
	renderManager->DrawAxis(40.f);
	//renderManager->DrawGrid(50.f,colWHITE,20,20);
	renderManager->DrawGrid(50.f,colWHITE,20,20, PAINT_WIREFRAME);
	renderManager->DrawCamera(m_pCamera);
	//renderManager->DrawSphere(5.f);
	//renderManager->DrawCylinder(5.f, 5.f, 10.f, 10, colBLUE, PAINT_BOTH, true);
	//renderManager->DrawCylinder(3.f, 3.f, 6.f, 10, colRED, PAINT_WIREFRAME, true);
	//renderManager->DrawSphere(3.f,colRED,10,PAINT_BOTH);

	//renderManager->DrawSphere(3.f,colRED,10,PAINT_BOTH);
	
	//renderManager->DrawSphere(3.f,colRED,10,PAINT_BOTH);
	//renderManager->DrawSphere(3.f,colRED,10,PAINT_BOTH);
	//renderManager->DrawSphere(3.f,colRED,10,PAINT_BOTH);
	//renderManager->DrawSphere(3.f,colRED,10,PAINT_BOTH);

	//renderManager->DrawCapsule(3.f,5.f, 10, colRED, PAINT_SOLID);
	
	//renderManager->DrawCube(6.f);
	//renderManager->DrawPlane(20.f,Vect3f(1.0f,0.,0.f), 1.f, colBLUE,20,20, PAINT_BOTH);

	//-----Dibujamos un cubo a base de quads---
	//Vamos a dibujar un cubo 6 quads, con una textura de madera (de 4x4x4):
	//La normal del quad va en el sentido del reloj respecto el vector right que se ha pasado como argumento->
	
	//Quad top->
	/*
	renderManager->DrawQuad3D(v3fY*2,		-v3fX, v3fZ, 4.f, 4.f, m_pQuadTextureFragile1);
	//Quad bottom->
	renderManager->DrawQuad3D(v3fY*-2,	v3fX, v3fZ, 4.f, 4.f, m_pQuadTextureFragile1);
	//Quad front->(miramos por el eje X)
	renderManager->DrawQuad3D(v3fX*2,		v3fY, v3fZ, 4.f, 4.f, m_pQuadTextureFragile1);
	//Quad back->(miramos por el eje X)
	renderManager->DrawQuad3D(v3fX*-2,	-v3fY, v3fZ, 4.f, 4.f, m_pQuadTextureFragile1);
	//Quad left->(miramos por el eje X)
	renderManager->DrawQuad3D(v3fZ*2,		-v3fY, v3fX, 4.f, 4.f, m_pQuadTextureFragile1);
	//Quad right->(miramos por el eje X)
	renderManager->DrawQuad3D(v3fZ*-2,	v3fY, v3fX, 4.f, 4.f, m_pQuadTextureFragile1);
	*/
	//renderManager->DrawCube(5.f,colRED, PAINT_SOLID);
	//renderManager->DrawBox(5.f,6.f,7.f,colRED, PAINT_SOLID);

	//Render avi3D
	//renderManager->DrawQuad3D(v3fX*2.05f,		v3fY, v3fZ, 3.f, 3.f, m_pAviTexture1);
	//renderManager->DrawQuad3D(v3fZ*-2.05f,	v3fY, v3fX, 3.f, 3.f, m_pAviTexture2);

	//renderManager->DrawQuad2D(Vect2i(750,500),250,200,UPPER_LEFT, m_pAviTexture1);	

	
	//renderManager->EnableAlphaBlend();
	//renderManager->EnableZBuffering();
	////CTexture* texture = CORE->GetTextureManager()->GetTexture("Data/Textures/columna.png");
	//CTexture* texture = CORE->GetTextureManager()->GetTexture("Data/Textures/f_helpa.png");
	//renderManager->DrawQuad2D(Vect2i(0,0), 500, 500, UPPER_LEFT, texture, FLIP_X );
	//renderManager->DisbaleZBuffering();
	//renderManager->DisbaleAlphaBlend();
	
	float value;
	m_LerpAnimator1D.Update(0.f,value);
	/*
	renderManager->DrawText((uint32)value,200,colBLACK,renderManager->GetTTF_Id("xfiles"),"El murci corrrre!");
	renderManager->DrawText(100,400,colRED,renderManager->GetTTF_Id("futurama"),"El murcielago corre!");
	renderManager->DrawText(100,600,colWHITE,renderManager->GetTTF_Id("Annabel"),"El murcielago corre!");
	*/

	SLiteral l_literal;
	
	fm->DrawLiteral(100, 100, "HiWorld");
	fm->DrawLiteral(100, 200, "Exit");

	fm->DrawDefaultText(100,300,colRED,"HÒLAÄÑ");


}

uint32 CAVITestProcess::RenderDebugInfo(CRenderManager* renderManager, CFontManager* fm, float fps)
{
	uint32 posY = 0;
	posY = CProcess::RenderDebugInfo(renderManager, fm, fps);
	if (m_bRenderDebugInfo)
	{
		uint32 posX = m_PosRenderDebugInfo.x;
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"____________________________________________________" );
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"" );
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"AviTestProcess: per probar la classe CAvi" );
		CInputManager * inputManager = CORE->GetInputManager();
		float deltaTirggerRight,deltaTirggerLeft;
		inputManager->GetGamePadDeltaTriggers(&deltaTirggerLeft, &deltaTirggerRight);
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"Delta Trigger Left,Right= (%f, %f" , deltaTirggerLeft, deltaTirggerRight);

		float l_fX, l_fY;
		inputManager->GetGamePadLeftThumbDeflection(&l_fX, &l_fY);
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"Left Thumb Deflection (x,y)=(%f, %f)" , l_fX, l_fY);

		inputManager->GetGamePadRightThumbDeflection(&l_fX, &l_fY);
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"Right Thumb Deflection (x,y)=(%f, %f)" , l_fX, l_fY);

		std::string padbuttons = "Pad Buttons isdown: ";
		if( inputManager->IsDown(IDV_GAMEPAD1, PAD_DPAD_UP))
			padbuttons += " DPAD_UP ";
		if( inputManager->IsDown(IDV_GAMEPAD1, PAD_DPAD_DOWN))
			padbuttons += " DPAD_DOWN ";
		if( inputManager->IsDown(IDV_GAMEPAD1, PAD_DPAD_LEFT))
			padbuttons += " DPAD_LEFT ";
		if( inputManager->IsDown(IDV_GAMEPAD1, PAD_DPAD_RIGHT))
			padbuttons += " DPAD_RIGHT ";
		if( inputManager->IsDown(IDV_GAMEPAD1, PAD_DPAD_START))
			padbuttons += " DPAD_START ";
		if( inputManager->IsDown(IDV_GAMEPAD1, PAD_DPAD_BACK))
			padbuttons += " DPAD_BACK ";
		if( inputManager->IsDown(IDV_GAMEPAD1, PAD_BUTTON_LEFT_THUMB))
			padbuttons += " LEFT_THUMB ";
		if( inputManager->IsDown(IDV_GAMEPAD1, PAD_BUTTON_RIGHT_THUMB))
			padbuttons += " RIGHT_THUMB ";
		if( inputManager->IsDown(IDV_GAMEPAD1, PAD_BUTTON_LEFT_SHOULDER))
			padbuttons += " LEFT_SHOULDER ";
		if( inputManager->IsDown(IDV_GAMEPAD1, PAD_BUTTON_RIGHT_SHOULDER))
			padbuttons += " RIGHT_SHOULDER ";
		if( inputManager->IsDown(IDV_GAMEPAD1, PAD_BUTTON_A))
			padbuttons += " A ";
		if( inputManager->IsDown(IDV_GAMEPAD1, PAD_BUTTON_B))
			padbuttons += " B ";
		if( inputManager->IsDown(IDV_GAMEPAD1, PAD_BUTTON_X))
			padbuttons += " X ";
		if( inputManager->IsDown(IDV_GAMEPAD1, PAD_BUTTON_Y))
			padbuttons += " Y ";

		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"%s" , padbuttons.c_str());

		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"____________________________________________________" );
		
	}
	return posY;
}

void CAVITestProcess::Update (float elapsedTime)
{
	CProcess::Update(elapsedTime);
	CCore * core = CCore::GetSingletonPtr();
	CInputManager * inputManager = core->GetInputManager();
	UpdateInputActions(inputManager);

	float value;
	if (m_LerpAnimator1D.Update(elapsedTime,value))
	{
		if (value == 0.f)
		{
			m_LerpAnimator1D.SetValues(0.f,700.f, 2.f,FUNC_INCREMENT);
		}
		else
		{
			m_LerpAnimator1D.SetValues(700.f,0.f, 2.f,FUNC_INCREMENT);
		}
	}

	float deltaTirggerRight,deltaTirggerLeft;
	inputManager->GetGamePadDeltaTriggers(&deltaTirggerLeft, &deltaTirggerRight);
	uint32 left = uint32((deltaTirggerLeft/255)*65535);
	uint32 right = uint32((deltaTirggerRight/255)*65535);
	inputManager->SetGamePadLeftMotorSpeed(left);
	inputManager->SetGamePadRightMotorSpeed(right);
}


void CAVITestProcess::UpdateInputActions	(CInputManager* inputManager)
{
	//----------------------------------------------------------
	//-----ACTUALIZAMOS LA CAMARA ESFERICA AL ESTILO DEL MAX----
	Vect3i deltaMouse;
	deltaMouse = inputManager->GetMouseDelta();
	CActionToInput* input2Action = CORE->GetActionToInput();

	float delta;
	if (input2Action->DoAction("YawViewerCam", delta))
	{
			delta = delta * 0.01f;
			float yaw = m_pObject3D->GetYaw();
			m_pObject3D->SetYaw(yaw+delta);
	}
	if (input2Action->DoAction("PitchViewerCam", delta))
	{
			delta = delta * 0.01f;
			float pitch = m_pObject3D->GetPitch();
			m_pObject3D->SetPitch(pitch+delta);
	}

	if (input2Action->DoAction("SlowZoomViewerCam", delta))
	{
		delta = delta * 0.1f;
		float increment = 0.1f;
		static_cast<CThPSCamera*>(m_pCamera)->AddZoom(-delta*increment);
	}
	else if (input2Action->DoAction("ZoomViewerCam", delta))
	{
		delta = delta * 0.1f;
		float increment = 1.0f;
		static_cast<CThPSCamera*>(m_pCamera)->AddZoom(-delta*increment);
	}


	if (input2Action->DoAction("MoveXViewerCam", delta))
	{
		//Perpendicularmente a su yaw. Realizamos un strafe
		delta = -delta * 0.1f;
		float yaw = m_pObject3D->GetYaw()+ePI2f;
		Vect3f directionXZ(	cos(yaw), 0, sin(yaw) );
		//nos desplazamos a una velocidad de 1unidad x segundo
		m_pObject3D->SetPosition(m_pObject3D->GetPosition()+directionXZ*delta);
	}

	if (input2Action->DoAction("MoveZViewerCam", delta))
	{
		//Segun su yaw directamente
		float delat = -delta * 0.1f;
		float yaw = m_pObject3D->GetYaw();
		Vect3f directionXZ(	cos(yaw), 0, sin(yaw) );
		m_pObject3D->SetPosition(m_pObject3D->GetPosition()+directionXZ*delat);
	}
	//----------------------------------------------------------
	//----------------------------------------------------------

	if( inputManager->IsDownUp(IDV_KEYBOARD,KEY_0) )
	{
		CORE->GetLanguageManager()->SetCurrentLanguage("spanish");
	}
	if( inputManager->IsDownUp(IDV_KEYBOARD,KEY_9) )
	{
		CORE->GetLanguageManager()->SetCurrentLanguage("english");
	}
	if( inputManager->IsDownUp(IDV_KEYBOARD,KEY_8) )
	{
		CORE->GetLanguageManager()->SetCurrentLanguage("catalan");
	}

	if( inputManager->IsDownUp(IDV_KEYBOARD,KEY_7) )
	{
		CORE->GetSoundManager()->FadeOutSource(m_uSoundSource,4.f);
	}
	if( inputManager->IsDownUp(IDV_KEYBOARD,KEY_6) )
	{
		CORE->GetSoundManager()->FadeInSource(m_uSoundSource,4.f);
	}

	if( inputManager->IsDownUp(IDV_KEYBOARD,KEY_H ))
	{
		CORE->GetActionToInput()->SaveXML("./kaka.xml");
	}
	
}


//-----------------ScriptManager------------------------------
void CAVITestProcess::RegisterFunctions (CScriptManager* scriptManager)
{
	lua_State* l_pLUAState = scriptManager->GetLuaState();
	using namespace luabind;
	
	// ahora registramos lo que querramos
	module(l_pLUAState)
    [
		def("getAVITest", GetAVITest),
		
		// registramos la clase CAVITestProcess
		class_<CAVITestProcess>(CScriptRegister::SetClassName("CAVITestProcess"))
		

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