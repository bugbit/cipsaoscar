#include "__PCH_Tests.h"

#include "GraphicsTestProcess.h"

//---Engine Includes--------
#include "Core/Core.h"
#include "Logger/Logger.h"
#include "Input/InputManager.h"
#include "Graphics/RenderManager.h"
#include "Graphics/FontManager.h"
#include "Graphics/Object3D.h"
#include "Graphics/ThPSCamera.h"
#include "Base/Math/Matrix44.h"
#include "Script/ScriptManager.h"
#include "luabind/luabind.hpp"
//--------------------------

//Para lo del _getcwd i _chdir...
#include <direct.h>

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
bool CGraphicsTestProcess::Init ()
{
	CProcess::m_bIsOk = false;
	m_pObject3D = new CObject3D(Vect3f(0.f,0.f,0.f), 0.f, 0.f);
	uint32 w,h;
	CRenderManager* rm = CORE->GetRenderManager();
	CORE->GetRenderManager()->GetWidthAndHeight(w,h);
	float aspect_ratio = (float)w/h;
	m_pCamera = new CThPSCamera(1.f,500.f,mathUtils::Deg2Rad(60.f),aspect_ratio,m_pObject3D,50.f);

	if (m_pObject3D && m_pCamera) 
	{
		CTextureManager* textureM = CORE->GetTextureManager();
		m_pQuadTexture = textureM->GetTexture("./Data//Textures/gui_exit_normal.png");
		m_pQuadTextureFragile1 = textureM->GetTexture("./Data/Textures/fragile2.jpg");
		m_pQuadTextureFragile2 = textureM->GetTexture("./Data/Textures/fragile1.jpg");
		
		CProcess::m_bIsOk = true;
	}

	if (!CProcess::m_bIsOk)
	{
		Release();
	}
	return CProcess::m_bIsOk;
}

void CGraphicsTestProcess::Release ()
{
	CHECKED_DELETE(m_pMesh);
	CHECKED_DELETE(m_pObject3D);
	CHECKED_DELETE(m_pCamera);
}

void CGraphicsTestProcess::RenderScene (CRenderManager* renderManager, CFontManager* fm)
{
	renderManager->DrawAxis(20.f);
	renderManager->DrawGrid(50.f,colWHITE,20,20);
	renderManager->DrawCamera(m_pCamera);

	if(m_pMesh)
	{
		m_pMesh->Render(renderManager);
	}
	
	Mat44f m, m1;

	//-----Dibujamos una esfera que rota a través del eje X---
	m.SetIdentity();
	m.Translate(Vect3f(0.f,5.f,0.f));
	m1.SetIdentity();
	m1.RotByAngleX(m_fTranslate);
	renderManager->SetTransform(m1*m);
	renderManager->DrawSphere(1.f,colRED);
	
	//-----Dibujamos una caja que hace un translate---
	m.SetIdentity();
	float aux = cos(m_fTranslate)*10.f;
	m.Translate(Vect3f(aux,  aux, aux));
	renderManager->SetTransform(m);
	renderManager->DrawBox(2.f, 2.f, 2.f, colYELLOW);


	//-----Dibujamos una caja que hace un scale---
	m.SetIdentity();
	float aux2 = cos(m_fTranslate)*10.f;
	m.Scale(aux2,  aux2, aux2);
	renderManager->SetTransform(m);
	renderManager->DrawBox(2.f, 2.f, 2.f, colGREEN);

	//-----Dibujamos un quad3d con una textura cualquiera para probar
	m.SetIdentity();
	renderManager->SetTransform(m);


	//-----Dibujamos un cubo a base de quads---
	//Vamos a dibujar un cubo 6 quads, con una textura de madera (de 4x4x4):
	//La normal del quad va en el sentido del reloj respecto el vector right que se ha pasado como argumento->
	
	//Quad top->
	renderManager->DrawQuad3D(v3fY*2,		-v3fX, v3fZ, 4.f, 4.f, m_pQuadTextureFragile2);
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


	//-----Dibujamos cuatro quads con transparencia---
	//Dibujamos un quad con transparencia, SIEMPRE AL FINAL de todo las transparencias!!
	m1.SetIdentity();
	m1.RotByAngleY(m_fTranslate);
	renderManager->SetTransform(m1);
	renderManager->DrawQuad3D(v3fX*4  + v3fY*10,	v3fY, v3fZ,		7.f, 4.f, m_pQuadTexture);
	renderManager->DrawQuad3D(v3fX*-4 + v3fY*10,	v3fY, -v3fZ,	7.f, 4.f, m_pQuadTexture);
	renderManager->DrawQuad3D(v3fZ*4  + v3fY*10,	v3fY, -v3fX,	7.f, 4.f, m_pQuadTexture);
	renderManager->DrawQuad3D(v3fZ*-4 + v3fY*10,	v3fY, v3fX,		7.f, 4.f, m_pQuadTexture);
	

}

uint32 CGraphicsTestProcess::RenderDebugInfo(CRenderManager* renderManager, CFontManager* fm, float fps)
{
	uint32 posY = 0;
	posY = CProcess::RenderDebugInfo(renderManager, fm, fps);
	if (m_bRenderDebugInfo)
	{
		uint32 posX = m_PosRenderDebugInfo.x;
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"____________Controlador de la cámara esferica___________" );
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- Presionado botón izquierdo->la cámara gira" );
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- Presionado botón derecho->la cámara se desplaza" );
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- Con la rueda del botón-> zoomIn/zoomOut" );
		fm->DrawDefaultText(posX,posY,colWHITE,"- Con la rueda del botón+control-> zoomIn/zoomOut suave" );
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"_____________________________________________________" );
	}
	return posY;
}

void CGraphicsTestProcess::Update (float elapsedTime)
{
	CProcess::Update(elapsedTime);

	CCore * core = CCore::GetSingletonPtr();
	CInputManager*inputManager = core->GetInputManager();
	UpdateInputActions(inputManager);

	m_fTranslate += elapsedTime;	
}


void CGraphicsTestProcess::UpdateInputActions	(CInputManager* inputManager)
{

	if((inputManager->IsDown(IDV_KEYBOARD, KEY_RCTRL) || inputManager->IsDown(IDV_KEYBOARD, KEY_LCTRL)) && inputManager->IsUpDown(IDV_KEYBOARD, KEY_E))
	{
		//Reload Effect Manager
		CCore::GetSingleton().GetEffectManager()->Reload();
	}

	if((inputManager->IsDown(IDV_KEYBOARD, KEY_RCTRL) || inputManager->IsDown(IDV_KEYBOARD, KEY_LCTRL)) && inputManager->IsUpDown(IDV_KEYBOARD, KEY_O))
	{
		//Load a mesh
		CHECKED_DELETE(m_pMesh);
		HardCodedLoadMesh();
	}

	if((inputManager->IsDown(IDV_KEYBOARD, KEY_RCTRL) || inputManager->IsDown(IDV_KEYBOARD, KEY_LCTRL)) && inputManager->IsUpDown(IDV_KEYBOARD, KEY_T))
	{
		//Reload textures
		CCore::GetSingleton().GetTextureManager()->Reload();
	}

	//----------------------------------------------------------
	//-----ACTUALIZAMOS LA CAMARA ESFERICA AL ESTILO DEL MAX----
	Vect3i deltaMouse;
	deltaMouse = inputManager->GetMouseDelta();
	if( inputManager->IsDown(IDV_MOUSE, MOUSE_BUTTON_LEFT) )
	{
		if (deltaMouse.x != 0)
		{
			float deltaX = deltaMouse.x * 0.01f;
			float yaw = m_pObject3D->GetYaw();
			m_pObject3D->SetYaw(yaw+deltaX);
		}
		if (deltaMouse.y != 0)
		{
			float deltaY = deltaMouse.y * 0.01f;
			float pitch = m_pObject3D->GetPitch();
			m_pObject3D->SetPitch(pitch+deltaY);
		}
	}

	if (deltaMouse.z != 0)
	{
		float deltaZ = deltaMouse.z * 0.1f;
		float increment = 1.f;
		if (inputManager->IsDown(IDV_KEYBOARD, KEY_RCTRL) || inputManager->IsDown(IDV_KEYBOARD, KEY_LCTRL))
		{
			increment = 0.1f;
		}
		static_cast<CThPSCamera*>(m_pCamera)->AddZoom(-deltaZ*increment);
	}
	//- Si se presiona el boton del medio y se mueve el mouse, la camara se desplaza por el plano (X,Z) segun su yaw
	if( inputManager->IsDown(IDV_MOUSE, MOUSE_BUTTON_MIDDLE) )
	{
		if (deltaMouse.y != 0)
		{
			//Segun su yaw directamente
			float delatY = deltaMouse.y * 0.1f;
			float yaw = m_pObject3D->GetYaw();
			Vect3f directionXZ(	cos(yaw), 0, sin(yaw) );
			m_pObject3D->SetPosition(m_pObject3D->GetPosition()+directionXZ*delatY);
		}
		if (deltaMouse.x != 0)
		{
			//Perpendicularmente a su yaw. Realizamos un strafe
			float deltaX = deltaMouse.x * 0.1f;
			float yaw = m_pObject3D->GetYaw()+ePI2f;
			Vect3f directionXZ(	cos(yaw), 0, sin(yaw) );
			//nos desplazamos a una velocidad de 1unidad x segundo
			m_pObject3D->SetPosition(m_pObject3D->GetPosition()+directionXZ*deltaX);
		}
	}
	//----------------------------------------------------------
	//----------------------------------------------------------
	
}



void CGraphicsTestProcess::HardCodedLoadMesh ()
{
	char l_szFile[MAX_PATH];
	char l_CWD[MAX_PATH];
	char *pnt=_getcwd(l_CWD,MAX_PATH);
	OPENFILENAME l_Ofn;
	ZeroMemory(&l_Ofn, sizeof(l_Ofn));
	l_Ofn.lStructSize = sizeof(l_Ofn);
	l_Ofn.hwndOwner = CORE->GetRenderManager()->GetHWND();
	l_Ofn.lpstrFile = l_szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	l_Ofn.lpstrFile[0] = '\0';
	l_Ofn.nMaxFile = MAX_PATH;
	l_Ofn.lpstrFilter = "Mesh files (*.mesh)|*.mesh||";
	l_Ofn.nFilterIndex = 1;
	l_Ofn.lpstrFileTitle = NULL;
	l_Ofn.nMaxFileTitle = 0;
	l_Ofn.lpstrInitialDir = NULL;
	l_Ofn.Flags = OFN_HIDEREADONLY|OFN_ENABLESIZING|OFN_FILEMUSTEXIST|OFN_PATHMUSTEXIST;

	if (GetOpenFileName(&l_Ofn)==TRUE) 
	{
		_chdir(l_CWD);
		m_pMesh=new CStaticMesh;
		m_pMesh->Load(l_Ofn.lpstrFile);
	}
}


//-----------------ScriptManager------------------------------
void CGraphicsTestProcess::RegisterFunctions (CScriptManager* scriptManager)
{
	lua_State* l_pLUAState = scriptManager->GetLuaState();
	using namespace luabind;
	
	// ahora registramos lo que querramos
	module(l_pLUAState)
    [
		def("getGrpahicTest", GetGrpahicTest),
		
		// registramos la clase CGraphicsTestProcess
		class_<CGraphicsTestProcess>(CScriptRegister::SetClassName("CGraphicsTestProcess"))
		

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