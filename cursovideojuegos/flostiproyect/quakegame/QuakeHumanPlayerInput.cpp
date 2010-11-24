#include "__PCH_Tests.h"

#include "QuakeHumanPlayerInput.h"
#include "QuakePlayer.h"

//---Engine Includes--------
#include "Core/Core.h"
#include "Input/InputManager.h"

CQuakeHumanPlayerInput::CQuakeHumanPlayerInput(void)
{
}

CQuakeHumanPlayerInput::~CQuakeHumanPlayerInput(void)
{
}

void CQuakeHumanPlayerInput::UpdateInputAction(float elapsedTime)
{
	CCore * core = CCore::GetSingletonPtr();
	CInputManager*inputManager = core->GetInputManager();
	Vect3i deltaMouse = inputManager->GetMouseDelta();
	float yaw = m_Player->GetYaw();
	float pitch = m_Player->GetPitch();
	Vect3f directionXZ(0.f,0.f,0.f);

	if (deltaMouse.x != 0)
	{
		float deltaX = deltaMouse.x * 0.01f;
		m_Player->SetYaw(yaw+deltaX);
	}
	if (deltaMouse.y != 0)
	{
		if (pitch<-ePIf/4.f)
			m_Player->SetPitch(-ePIf/4.f);
		else if(pitch>ePIf/4.f)
			m_Player->SetPitch(ePIf/4.f);
		else
		{
			float deltaY = deltaMouse.y * 0.01f;
			m_Player->SetPitch(pitch+deltaY);
		}
	}
	
	//- Si se presiona el boton del medio y se mueve el mouse, la camara se desplaza por el plano (X,Z) segun su yaw
	if( inputManager->IsDown(IDV_MOUSE,2) )
	{
		if (deltaMouse.y != 0)
		{
			//Segun su yaw directamente
			float delatY = deltaMouse.y * 0.1f;
			(	cos(yaw), 0, sin(yaw) );
			//m_Player->SetPosition(m_Player->GetPosition()+directionXZ*delatY);
		}
		if (deltaMouse.x != 0)
		{
			//Perpendicularmente a su yaw. Realizamos un strafe
			float deltaX = deltaMouse.x * 0.1f;
			directionXZ=Vect3f(	sin(yaw), 0, cos(yaw) );
			//nos desplazamos a una velocidad de 1unidad x segundo
			//m_Player->SetPosition(m_Player->GetPosition()+directionXZ*deltaX);
		}
	}
	if (inputManager->IsDown(IDV_KEYBOARD, ZVK_UP))
	{
		directionXZ=Vect3f(	cos(yaw), 0, sin(yaw) );
		//m_Player->SetPosition(m_Player->GetPosition()+directionXZ*m_SpeedPlayer*elapsedTime);
	}
	if (inputManager->IsDown(IDV_KEYBOARD, ZVK_DOWN))
	{
		directionXZ=Vect3f(	-cos(yaw), 0, -sin(yaw) );
		//m_Player->SetPosition(m_Player->GetPosition()-directionXZ*m_SpeedPlayer*elapsedTime);
	}
	if (inputManager->IsDown(IDV_KEYBOARD, ZVK_RIGHT))
	{
		directionXZ=Vect3f(	sin(yaw), 0, cos(yaw) );
		//m_Player->SetPosition(m_Player->GetPosition()+directionXZ*m_SpeedPlayer*elapsedTime);
	}
	if (inputManager->IsDown(IDV_KEYBOARD, ZVK_LEFT))
	{
		directionXZ=Vect3f(	-sin(yaw), 0, -cos(yaw) );
		//m_Player->SetPosition(m_Player->GetPosition()-directionXZ*m_SpeedPlayer*elapsedTime);
	}
	m_Player->Move(.05f*directionXZ,elapsedTime);
}
