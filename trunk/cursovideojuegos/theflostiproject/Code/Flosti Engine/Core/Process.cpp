#include "__PCH_Core.h"

#include "Process.h"
#include "Utils/BaseUtils.h"
#include "Graphics/RenderManager.h"
#include "Graphics/FontManager.h"
#include "Core/Core.h"
#include "Input/InputManager.h"
#include "Input/ActionToInput.h"

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CProcess::Done ()
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

uint32 CProcess::RenderDebugInfo (CRenderManager* renderManager, CFontManager* fm, float fps)
{
	uint32 posY = 0;
	if (m_bRenderDebugInfo)
	{
		CInputManager* inputManager = CORE->GetInputManager();
		Vect2i mousePos;
		inputManager->GetPosition(IDV_MOUSE,mousePos);
		uint32 posX = m_PosRenderDebugInfo.x;
		float value;
		m_LerpAnimator1D.Update(0.f,value);
		posY = 	(uint32)value;
		std::string info, shortInfo;
		CActionToInput* action2Input = CORE->GetActionToInput();

		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"_________________PROCESS INFORMATIONl______________" );
		action2Input->GetActionInfo(ACTION_PROCESS_INFO, info, shortInfo);
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- Press %s to show/hide this information", shortInfo.c_str() );
		action2Input->GetActionInfo(ACTION_SCREENSHOT, info, shortInfo);
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- Press %s to save in a file the current screenshot", shortInfo.c_str() );
		action2Input->GetActionInfo(ACTION_LOGGER, info, shortInfo);
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- Press %s to save in a file the current logger", shortInfo.c_str() );
		action2Input->GetActionInfo(ACTION_RELOAD_TTFS, info, shortInfo);
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- Press %s to reload the font", shortInfo.c_str() );
		action2Input->GetActionInfo(ACTION_RELOAD_LANGUAGES, info, shortInfo);
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- Press %s to reload all xml language", shortInfo.c_str() );
		action2Input->GetActionInfo(ACTION_RELOAD_ACTIONS, info, shortInfo);
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- Press %s to reload action xml", shortInfo.c_str() );
		if (inputManager->HasGamePad(IDV_GAMEPAD1) )
		{
			posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- GamePad1 is connected" );
		}
		else
		{
			posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- GamePad1 is not connected" );
		}

		if (inputManager->HasGamePad(IDV_GAMEPAD2) )
		{
			posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- GamePad2 is connected" );
		}
		else
		{
			posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- GamePad2 is not connected" );
		}

		if (inputManager->HasGamePad(IDV_GAMEPAD3) )
		{
			posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- GamePad3 is connected" );
		}
		else
		{
			posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- GamePad3 is not connected" );
		}

		if (inputManager->HasGamePad(IDV_GAMEPAD4) )
		{
			posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- GamePad4 is connected" );
		}
		else
		{
			posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- GamePad4 is not connected" );
		}
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- FPS: %f", fps );
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- Process name: %s", m_sProcessName.c_str() );
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- Pointer mouse position: (%d,%d)", mousePos.x,mousePos.y);
	}
	else
	{
		//Draw background quad2D
		CColor quad2dColor(0.f,0.f,0.5f,0.7f);

		std::string info, shortInfo;
		CActionToInput* action2Input = CORE->GetActionToInput();
		action2Input->GetActionInfo(ACTION_PROCESS_INFO, info, shortInfo);
		std::string l_sInfo;
		baseUtils::FormatSrting(l_sInfo, "Press %s to view the Process Info (FPS: %f)", shortInfo.c_str(), fps);

		uint32 w = fm->SizeX(l_sInfo.c_str());
		uint32 h = fm->SizeY(l_sInfo.c_str());
		
		CColor edgeColor = colBLACK;
		edgeColor.SetAlpha(0.7f);
		renderManager->DrawRectangle2D(m_PosRenderDebugInfo, w, h, quad2dColor, 1, 1, edgeColor);

		edgeColor = colWHITE;
		fm->DrawDefaultText(m_PosRenderDebugInfo.x,m_PosRenderDebugInfo.y, edgeColor, l_sInfo.c_str());
	}
	return posY;
}

void CProcess::EndRenderDebugInfo (CRenderManager* renderManager, uint32 finalPosY)
{
	if (m_bRenderDebugInfo)
	{
		CColor quad2dColor(0.f,0.f,0.5f,0.7f);
		CColor edgeColor = colBLACK;
		edgeColor.SetAlpha(0.7f);
		uint32 height = finalPosY- m_PosRenderDebugInfo.y;
		renderManager->DrawRectangle2D(m_PosRenderDebugInfo, 500, height,quad2dColor,1,1,edgeColor);
	}
}

void CProcess::Update (float elapsedTime)
{
	CCore * core = CCore::GetSingletonPtr();
	CActionToInput* action2Input = core->GetActionToInput();
	float value;
	bool finish = m_LerpAnimator1D.Update(elapsedTime,value);
	if (value == -180.f)
	{
		m_bRenderDebugInfo = false;
	}

	if (action2Input->DoAction(ACTION_PROCESS_INFO) && finish)
	{
		if (!m_bRenderDebugInfo)
		{
			m_bRenderDebugInfo = true;
			m_LerpAnimator1D.SetValues(-180.f,5.f,1.f,FUNC_DECREMENT);
		}
		else
		{
			m_LerpAnimator1D.SetValues(5.f,-180.f,1.f,FUNC_DECREMENT);
		}
	}
}