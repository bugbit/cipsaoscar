#include "__PCH_Tests.h"

#include "GUITestProcess.h"

//----Engine Includes----
#include "Core/Core.h"
#include "Input/InputManager.h"
#include "Script/ScriptManager.h"
#include "Graphics/RenderManager.h"
#include "Graphics/Object3D.h"
#include "Graphics/ThPSCamera.h"
#include "Graphics/Textures/TextureManager.h"
#include "luabind/luabind.hpp"
//Include GuiElements
#include "gui/Button.h"
#include "gui/CheckButton.h"
#include "gui/StaticText.h"
#include "gui/Slider.h"
#include "gui/Image.h"
#include "gui/EditableTextBox.h"
#include "gui/ProgressBar.h"
#include "gui/DialogBox.h"
#include "gui/RadioBox.h"
//--------------------------




//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
bool CGUITestProcess::Init ()
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
		uint32 windows_w, windows_h;
		CORE->GetRenderManager()->GetWidthAndHeight(windows_w,windows_h);

		//---Añadimos un Boton CON texturas----
		CTexture* normal = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_exit_normal.png");
		CTexture* over = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_exit_over.png");
		CTexture* clicked = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_exit_press.png");
		CTexture* deactivated = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_fraglimit.png");
		m_pButton = new CButton(windows_w,windows_h, 10.f,15.f, Vect2f(15.f,15.f));
		m_pButton->SetName("Test_Button");
		m_pButton->SetTextures(normal,over,clicked,deactivated);

		//---Añadimos un Boton SIN texturas----
		m_pButton2 = new CButton(windows_w,windows_h, 10.f,15.f, Vect2f(50.f,50.f));
		m_pButton2->SetName("Test_Button_withoutTextures");

		m_pStaticText = new CStaticText(windows_w,windows_h, 10.f,15.f, Vect2f(50.f,50.f));
		//m_pStaticText->SetText("Boton sin textura");
		m_pButton2->SetName("Test_StaticText");
		m_pButton2->AddChild(m_pStaticText);
		
		//--Añadimos un Slider
		m_pSlider = new CSlider(windows_h, windows_w, 2.f,25.f,Vect2f(50.f,80.f),3.f,3.f,0.5f);
		m_pSlider->SetName("Test_Slider");
		normal = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_sliderbutton_normal.png");
		over = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_sliderbutton_over.png");
		clicked = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_sliderbutton_press.png");
		deactivated = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_fraglimit.png");
		CTexture* background = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_sliderback.png");
		m_pSlider->SetBackGroundTexture(background);
		m_pSlider->SetButtonTextures(normal, over, clicked, deactivated);
		
		//---Añadimos una imagen dinamica
		m_pImage = new CImage(windows_h, windows_w, 10.f,25.f,Vect2f(70.f,80.f));
		CTexture* r1 = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/rlboom_1.tga");
		CTexture* r2 = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/rlboom_2.tga");
		CTexture* r3 = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/rlboom_3.tga");
		CTexture* r4 = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/rlboom_4.tga");
		CTexture* r5 = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/rlboom_5.tga");
		CTexture* r6 = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/rlboom_6.tga");
		CTexture* r7 = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/rlboom_7.tga");
		CTexture* r8 = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/rlboom_8.tga");
		
		m_pImage->SetTexture(r1,"r1");
		m_pImage->SetTexture(r2,"r2");
		m_pImage->SetTexture(r3,"r3");
		m_pImage->SetTexture(r4,"r4");
		m_pImage->SetTexture(r5,"r5");
		m_pImage->SetTexture(r6,"r6");
		m_pImage->SetTexture(r7,"r7");
		m_pImage->SetTexture(r8,"r8");
		
		m_pImage->PlayAnimation(0.1f,true);
		
		//---Añadimos una imagen estatica
		m_pImage2 = new CImage(windows_h, windows_w, 10.f,25.f,Vect2f(20.f,80.f));
		r1 = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/rlboom_1.tga");
		m_pImage2->SetTexture(r1,"r1");
		m_pImage2->SetActiveTexture("r1");


		//---Añadimos un CheckButton---
		CTexture* on = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_checkbuttonon_on.png");
		CTexture* off= CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_checkbutton_off.png");
		CTexture* deactivated2 = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_fraglimit.png");
		m_pCheckButton = new CCheckButton(windows_w,windows_h, 5.f,10.f, Vect2f(35.f,15.f));
		m_pCheckButton->SetName("Test_CHeckButton");
		m_pCheckButton->SetTextures(on,off,deactivated2);

		//---Añadimos un EditableTextBox
		m_pEditableTextBox = new CEditableTextBox(windows_h, windows_w,10.f,60.f,Vect2f(30.f,5.f));
		CTexture* progressBack = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/progressBack.jpg");
		m_pEditableTextBox->SetBackGroundTexture(progressBack);


		//---Añadimos un Progress Bar
		m_pProgressBar = new CProgressBar(windows_h, windows_w,5.f,50.f,Vect2f(30.f,35.f));
		CTexture* progress = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/ProgressBar.jpg");
		m_pProgressBar->SetTextures(progressBack,progress);

		//---Añadimos un DialogBox
		m_pDialogBox = new CDialogBox(windows_h, windows_w, 30.f,35.f,Vect2f(5.f,5.f),5.f,5.f);
		normal = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/ButtonDialegBoxN.jpg");
		over = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/ButtonDialegBoxO.jpg");
		clicked = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/ButtonDialegBoxC.jpg");
		deactivated = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/ButtonDialegBoxC.jpg");
		m_pDialogBox->SetMoveButtonTextures(normal, over, clicked, deactivated);
		normal = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/CloseDialegBox.jpg");
		over = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/CloseDialegBoxO.jpg");
		clicked = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/CloseDialegBoxC.jpg");
		deactivated = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/CloseDialegBoxC.jpg");
		m_pDialogBox->SetCloseButtonTextures(normal, over, clicked, deactivated);
		background = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/BaseDialegBox.jpg");
		m_pDialogBox->SetDialogTexture(background);

		//---Añadimos un RadioBox
		m_pRadioBox = new CRadioBox(windows_h, windows_w, 20.f,20.f,Vect2f(5.f,75.f),4,2,"red");
		CTexture* blue = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_colour_blue_off.png");
		CTexture* blue_on = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_colour_blue_on.png");

		CTexture* brown = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_colour_brown_off.png");
		CTexture* brown_on = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_colour_brown_on.png");

		CTexture* green = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_colour_green_off.png");
		CTexture* green_on = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_colour_green_on.png");

		CTexture* grey = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_colour_grey_off.png");
		CTexture* grey_on = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_colour_grey_on.png");

		CTexture* orange = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_colour_orange_off.png");
		CTexture* orange_on = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_colour_orange_on.png");

		CTexture* pink = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_colour_pink_off.png");
		CTexture* pink_on = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_colour_pink_on.png");

		CTexture* red = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_colour_red_off.png");
		CTexture* red_on = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_colour_red_on.png");

		CTexture* yellow = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_colour_yellow_off.png");
		CTexture* yellow_on = CORE->GetTextureManager()->GetTexture("./Data/GUI/Textures_Test/gui_colour_yellow_on.png");

		m_pRadioBox->SetCheckButton("blue", blue_on, blue, blue);
		m_pRadioBox->SetCheckButton("brown", brown_on, brown, brown);
		m_pRadioBox->SetCheckButton("green", green_on, green, green);
		m_pRadioBox->SetCheckButton("grey", grey_on, grey, grey);
		m_pRadioBox->SetCheckButton("orange", orange_on, orange, orange);
		m_pRadioBox->SetCheckButton("pink", pink_on, pink, pink);
		m_pRadioBox->SetCheckButton("red", red_on, red, red);
		m_pRadioBox->SetCheckButton("yellow", yellow_on, yellow, yellow);

	}

	if (!CProcess::m_bIsOk)
	{
		Release();
	}
	return CProcess::m_bIsOk;
}

void CGUITestProcess::Release ()
{
	CHECKED_DELETE(m_pObject3D);
	CHECKED_DELETE(m_pCamera);

	CHECKED_DELETE(m_pButton);
	CHECKED_DELETE(m_pButton2);
	CHECKED_DELETE(m_pStaticText);
	CHECKED_DELETE(m_pCheckButton);
	CHECKED_DELETE(m_pSlider);
	CHECKED_DELETE(m_pImage);
	CHECKED_DELETE(m_pImage2);
	CHECKED_DELETE(m_pEditableTextBox);
	CHECKED_DELETE(m_pProgressBar);
	CHECKED_DELETE(m_pDialogBox);
	CHECKED_DELETE(m_pRadioBox);
}

void CGUITestProcess::RenderScene (CRenderManager* renderManager, CFontManager* fm)
{
	renderManager->DrawAxis(20.f);
	renderManager->DrawGrid(50.f,colWHITE,20,20);
	renderManager->DrawCamera(m_pCamera);

	
	if (m_pButton)
		m_pButton->Render(renderManager, fm);
	if (m_pButton2)
		m_pButton2->Render(renderManager, fm);
	if (m_pCheckButton)
		m_pCheckButton->Render(renderManager, fm);
	if (m_pSlider)
		m_pSlider->Render(renderManager, fm);
	if (m_pImage)
		m_pImage->Render(renderManager, fm);
	if (m_pImage2)
		m_pImage2->Render(renderManager, fm);
	if (m_pEditableTextBox)
		m_pEditableTextBox->Render(renderManager, fm);
	if (m_pProgressBar)
		m_pProgressBar->Render(renderManager, fm);
	if (m_pDialogBox)
		m_pDialogBox->Render(renderManager, fm);
	if (m_pRadioBox)
		m_pRadioBox->Render(renderManager, fm);
	
	
}

uint32 CGUITestProcess::RenderDebugInfo(CRenderManager* renderManager, CFontManager* fm, float fps)
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

void CGUITestProcess::Update (float elapsedTime)
{
	CProcess::Update(elapsedTime);

	m_yaw += elapsedTime;

	CCore * core = CCore::GetSingletonPtr();
	CInputManager*inputManager = core->GetInputManager();
	UpdateInputActions(inputManager);

	if (m_pButton)
		m_pButton->Update(inputManager,elapsedTime);
	if (m_pButton2)
		m_pButton2->Update(inputManager, elapsedTime);
	if (m_pCheckButton)
		m_pCheckButton->Update(inputManager, elapsedTime);
	if (m_pSlider)
		m_pSlider->Update(inputManager, elapsedTime);
	if (m_pImage)
		m_pImage->Update(inputManager, elapsedTime);
	if (m_pImage2)
		m_pImage2->Update(inputManager, elapsedTime);
	if (m_pEditableTextBox)
		m_pEditableTextBox->Update(inputManager,elapsedTime);
	if (m_pProgressBar)
		m_pProgressBar->Update(inputManager,elapsedTime);
	if (m_pDialogBox)
		m_pDialogBox->Update(inputManager,elapsedTime);
	if (m_pRadioBox)
		m_pRadioBox->Update(inputManager,elapsedTime);
	
	

	//a saco hacemos progresar la barra
	m_pProgressBar->SetProgress(m_pProgressBar->GetProgress()+0.01f);
	
}


void CGUITestProcess::UpdateInputActions	(CInputManager* inputManager)
{
	
}
//-----------------ScriptManager------------------------------
void CGUITestProcess::RegisterFunctions (CScriptManager* scriptManager)
{
	lua_State* l_pLUAState = scriptManager->GetLuaState();
	using namespace luabind;
	
	// ahora registramos lo que querramos
	module(l_pLUAState)
    [
		def("getGuiTest", GetGuiTest),
		
		// registramos la clase CGUITestProcess
		class_<CGUITestProcess>(CScriptRegister::SetClassName("CGUITestProcess"))
		

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

