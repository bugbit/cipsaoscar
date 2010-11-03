#include "__PCH_Tests.h"
#include <windows.h>
#include <vector>

//---Engine Includes---------
#include "Core/FlostiEngine.h"
#include "Base/Exceptions/Exception.h"
//----------------------------

//---Include TestsProcess-----
#include "GUITestProcess.h"
#include "GraphicsTestProcess.h"
#include "PhysXTestProcess.h"
#include "AviTestProcess.h"
#include "SpriteTestProcess.h"
#include "ParticleSystemTestProcess.h"
#include "snakegame/SnakeGameTestProcess.h"
#include "jetpacgame/JetPacGameTestProcess.h"
#include "quakegame/QuakeGameProcess.h"
//----------------------------

//---Always the last:
#include "Memory/MemLeaks.h"

#define APPLICATION_NAME	"TESTS"

// ----------------------------------------
// -- Windows Message Handlers
// ----------------------------------------
#define WM_SERVER (WM_USER + 1)
#define WM_CLIENT (WM_USER + 2)

CFlostiEngine * g_FlostEngine = new CFlostiEngine();


//-----------------------------------------------------------------------------
// Name: MsgProc()
// Desc: The window's message handler
//-----------------------------------------------------------------------------
LRESULT WINAPI MsgProc( HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam )
{

	switch( msg )
	{
	case WM_DESTROY:
		{
			PostQuitMessage( 0 );
			return 0;
		}
		break;
	case WM_KEYDOWN:
		{
			switch( wParam )
			{
			case VK_ESCAPE:
				//Cleanup();
				PostQuitMessage( 0 );
				return 0;
				break;
			}
		}
		break;
	case WM_SERVER: case WM_CLIENT: 
		{
			g_FlostEngine->MsgProc(wParam, lParam);
		}
		break;

	}//end switch( msg )

	return DefWindowProc( hWnd, msg, wParam, lParam );
}

void ShowErrorMessage (const std::string& message)
{
		bool logSaved = false;
		logSaved = CLogger::GetSingletonPtr()->SaveLogsInFile();
		std::string end_message = "";
		if (logSaved)
		{
			end_message += "Sorry, Application failed. Logs saved\n";
		}
		else
		{
			end_message += "Sorry, Application failed. Logs could not be saved\n";
		}
		end_message += message;
		MessageBox(0, end_message.c_str(), "FlostiProject Report", MB_OK | MB_ICONERROR);
}


//-----------------------------------------------------------------------
// WinMain
//-----------------------------------------------------------------------
int APIENTRY WinMain(HINSTANCE _hInstance, HINSTANCE _hPrevInstance, LPSTR _lpCmdLine, int _nCmdShow)
{

#if defined( _DEBUG )
	MemLeaks::MemoryBegin();
#endif //defined(_DEBUG)

	WNDCLASSEX wc = {	sizeof(WNDCLASSEX), CS_CLASSDC, MsgProc, 0L, 0L, GetModuleHandle(NULL), 
										NULL, NULL, NULL, NULL, APPLICATION_NAME, NULL };

	// Register the window class
	RegisterClassEx( &wc );

	try
	{		
		CGUITestProcess*						guiTest							= new CGUITestProcess("GuiTest");
		CPhysXTestProcess*					physXTest						= new CPhysXTestProcess("PhysXTest");
		CGraphicsTestProcess* 			graphicsTest				= new CGraphicsTestProcess("GraphicTest");
		CAVITestProcess*						aviTest							= new CAVITestProcess("AVITest");
		CSpriteTestProcess*					spriteTest					= new CSpriteTestProcess("SpriteTest");
		CParticleSystemTestProcess*	particleSystemTest	= new CParticleSystemTestProcess("ParticleSystemTest");
		CSnakeGameTestProcess* snakeGameTest=new CSnakeGameTestProcess("SnakeGameTestProcess");
		CJetPacGameTestProcess *jetpacGameTest=new CJetPacGameTestProcess("JetPacGameTestProcess");
		CQuakeGameProcess *quakeGameTest=new CQuakeGameProcess("QuakeGameProcess");
		
		std::vector<CProcess*> l_ProcessVector;
		l_ProcessVector.push_back(graphicsTest);
		l_ProcessVector.push_back(physXTest);
		l_ProcessVector.push_back(guiTest);
		l_ProcessVector.push_back(aviTest);
		l_ProcessVector.push_back(spriteTest);
		l_ProcessVector.push_back(particleSystemTest);
		l_ProcessVector.push_back(snakeGameTest);
		l_ProcessVector.push_back(jetpacGameTest);
		l_ProcessVector.push_back(quakeGameTest);
		
		g_FlostEngine->LoadInitParams("Data/Config/init_test.xml");
		uint32 width	= g_FlostEngine->GetInitParams().m_ScreenResolution.x;
		uint32 height	= g_FlostEngine->GetInitParams().m_ScreenResolution.y;

		uint32 posX	= g_FlostEngine->GetInitParams().m_WindowsPosition.x;
		uint32 posY	= g_FlostEngine->GetInitParams().m_WindowsPosition.y;


		// Create the application's window
		HWND hWnd = CreateWindow(	APPLICATION_NAME, APPLICATION_NAME, WS_OVERLAPPEDWINDOW, 
															posX, posY, width, height,
															NULL, NULL, wc.hInstance, NULL );

		g_FlostEngine->Init(l_ProcessVector, hWnd);

		ShowWindow( hWnd, SW_SHOWDEFAULT );
		UpdateWindow( hWnd );
		MSG msg;
		ZeroMemory( &msg, sizeof(msg) );
		while( msg.message!=WM_QUIT && !g_FlostEngine->Exit())
		{
			if( PeekMessage( &msg, NULL, 0U, 0U, PM_REMOVE ) )
			{
				TranslateMessage( &msg );
				DispatchMessage( &msg );
			}
			else
			{
				// main loop
				g_FlostEngine->Update();
				g_FlostEngine->Render();
			}
		}

	}
	catch(CException& e)
	{
		ShowErrorMessage(e.GetDescription());
	}
	catch (...)	
	{
		ShowErrorMessage("Exception Occured");
	}

	UnregisterClass( APPLICATION_NAME, wc.hInstance );
	CHECKED_DELETE(g_FlostEngine);

#if defined( _DEBUG )
	MemLeaks::MemoryEnd();
#endif //defined(_DEBUG)

	return 0;
}