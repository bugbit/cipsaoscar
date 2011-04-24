#include "__PCH_Tests.h"

#include "ParticleSystemTestProcess.h"
#include "Core/Core.h"
#include "Input/InputManager.h"
#include "Graphics/RenderManager.h"
#include "Graphics/Object3D.h"
#include "Graphics/ThPSCamera.h"
#include "Graphics/Particle System/ParticleSystem.h"
#include "Script/ScriptManager.h"
#include "luabind/luabind.hpp"




//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
bool CParticleSystemTestProcess::Init ()
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
		InitParticles();
	}

	if (!CProcess::m_bIsOk)
	{
		Release();
	}
	return CProcess::m_bIsOk;
}

void CParticleSystemTestProcess::Release ()
{
	CHECKED_DELETE(m_pObject3D);
	CHECKED_DELETE(m_pCamera);

	for(uint16 cont = 0 ; cont < NUM_PARTICLES; cont++ )
	{
		CHECKED_DELETE( m_pParticleSystems[cont] );
	}
}

void CParticleSystemTestProcess::RenderScene (CRenderManager* renderManager, CFontManager* fm)
{
	renderManager->DrawAxis(20.f);
	renderManager->DrawGrid(50.f,colWHITE,20,20);
	renderManager->DrawCamera(m_pCamera);

	m_pParticleSystems[m_uActiveSystem]->Render( renderManager );
}

uint32 CParticleSystemTestProcess::RenderDebugInfo(CRenderManager* renderManager, CFontManager* fm, float fps)
{
	uint32 posY = 0;
	posY = CProcess::RenderDebugInfo(renderManager, fm,  fps);
	if (m_bRenderDebugInfo)
	{
		uint32 posX = m_PosRenderDebugInfo.x;
		//...
	}
	return posY;
}

void CParticleSystemTestProcess::Update (float elapsedTime)
{
	CProcess::Update(elapsedTime);

	m_pParticleSystems[m_uActiveSystem]->Update( elapsedTime );

	CCore * core = CCore::GetSingletonPtr();
	CInputManager*inputManager = core->GetInputManager();
	UpdateInputActions(inputManager);	
}


void CParticleSystemTestProcess::UpdateInputActions	(CInputManager* inputManager)
{
	if( inputManager->IsUpDown(IDV_KEYBOARD, KEY_1) )
	{
		++m_uActiveSystem;
		if( m_uActiveSystem>= 6 )
		{
			m_uActiveSystem = 0;
		}
		m_pParticleSystems[m_uActiveSystem]->RestartPartSyst();
	}

	if( inputManager->IsUpDown(IDV_KEYBOARD, KEY_2) )
	{
		float fSize = m_pParticleSystems[m_uActiveSystem]->GetSize();
		fSize += 0.1f;
		if( fSize >= m_pParticleSystems[m_uActiveSystem]->GetMaxPointSize() )
		{
			fSize = m_pParticleSystems[m_uActiveSystem]->GetMaxPointSize();
		}

		m_pParticleSystems[m_uActiveSystem]->SetSize( fSize );
	}

	if( inputManager->IsUpDown(IDV_KEYBOARD, KEY_3) )
	{
		float fSize = m_pParticleSystems[m_uActiveSystem]->GetSize();
		fSize -= 0.1f;
		if( fSize <= 0.1f )
		{
			fSize = 0.1f;
		}

		m_pParticleSystems[m_uActiveSystem]->SetSize( fSize );
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


void CParticleSystemTestProcess::InitParticles()
{
	m_uActiveSystem = 0;

	CRenderManager * rm = CORE->GetRenderManager();


	//--- Exploding burst---
	m_pParticleSystems[0] = new CParticleSystem();
	m_pParticleSystems[0]->SetTexture( "./Data/particle.bmp" );
	m_pParticleSystems[0]->SetMaxParticles( 100 );
	m_pParticleSystems[0]->SetNumToRelease( 100 );
	m_pParticleSystems[0]->SetReleaseInterval( 0.05f );
	m_pParticleSystems[0]->SetLifeCycle( 0.5f );
	m_pParticleSystems[0]->SetSize( 1.0f );
	m_pParticleSystems[0]->SetColor( Vect3f( 1.0f, 0.0f, 0.0f), 1.0f );
	m_pParticleSystems[0]->SetPosition( Vect3f( 0.0f, 5.0f, 0.0f) );
	m_pParticleSystems[0]->SetVelocity( Vect3f( 0.0f, 0.0f, 0.0f) );
	m_pParticleSystems[0]->SetGravity( Vect3f( 0.0f, 0.0f, 0.0f) );
	m_pParticleSystems[0]->SetWind( Vect3f( 0.0f, 0.0f, 0.0f) );
	m_pParticleSystems[0]->SetVelocityVar( 10.0f );
	m_pParticleSystems[0]->Init( rm );

	
	//--- Wind blown fountain---
	m_pParticleSystems[1] = new CParticleSystem();
  m_pParticleSystems[1]->SetTexture( "./Data/particle.bmp" );
  m_pParticleSystems[1]->SetMaxParticles( 500 );
  m_pParticleSystems[1]->SetNumToRelease( 5 );
  m_pParticleSystems[1]->SetReleaseInterval( 0.05f );
  m_pParticleSystems[1]->SetLifeCycle( 4.0f );
  m_pParticleSystems[1]->SetSize( 0.5f );
  m_pParticleSystems[1]->SetColor( Vect3f( 1.0f, 1.0f, 1.0f), 1.0f );
  m_pParticleSystems[1]->SetPosition( Vect3f( 0.0f, 0.0f, 0.0f ) );
  m_pParticleSystems[1]->SetVelocity( Vect3f( 0.0f, 5.0f, 0.0f ) );
  m_pParticleSystems[1]->SetGravity( Vect3f( 0.0f, 0.0f, 0.0f ) );
  m_pParticleSystems[1]->SetWind( Vect3f( 2.0f, 0.0f, 0.0f ) );
  m_pParticleSystems[1]->SetVelocityVar( 1.5f );
  m_pParticleSystems[1]->Init( rm );

	//--- Omni-directiional emission expanding into space with no air resistence---
	m_pParticleSystems[2] = new CParticleSystem();
  m_pParticleSystems[2]->SetTexture( "./Data/particle.bmp" );
  m_pParticleSystems[2]->SetMaxParticles( 2048 );
  m_pParticleSystems[2]->SetNumToRelease( 10 );
  m_pParticleSystems[2]->SetReleaseInterval( 0.05f );
  m_pParticleSystems[2]->SetLifeCycle( 5.0f );
  m_pParticleSystems[2]->SetSize( 0.5f );
  m_pParticleSystems[2]->SetColor( Vect3f( 1.0f, 1.0f, 1.0f), 1.0f );
  m_pParticleSystems[2]->SetPosition( Vect3f( 0.0f, 0.0f, 0.0f) );
  m_pParticleSystems[2]->SetVelocity( Vect3f( 0.0f, 0.0f, 0.0f) );
  m_pParticleSystems[2]->SetGravity( Vect3f( 0.0f, 0.0f, 0.0f) );
  m_pParticleSystems[2]->SetWind( Vect3f( 0.0f, 0.0f, 0.0f) );
  m_pParticleSystems[2]->SetAirResistence( false );
  m_pParticleSystems[2]->SetVelocityVar(2.0f);
  m_pParticleSystems[2]->Init( rm );
 
	//--- Fountain particles behave like paint spots when striking a plane as the wind blowing them towards us---
	m_pParticleSystems[3] = new CParticleSystem();
  m_pParticleSystems[3]->SetTexture( "./Data/particle.bmp" );
  m_pParticleSystems[3]->SetMaxParticles( 100 );
  m_pParticleSystems[3]->SetNumToRelease( 10 );
  m_pParticleSystems[3]->SetReleaseInterval( 0.05f );
  m_pParticleSystems[3]->SetLifeCycle( 3.0f );
  m_pParticleSystems[3]->SetSize( 0.5f );
  m_pParticleSystems[3]->SetColor( Vect3f( 1.0f, 1.0f, 1.0f), 1.0f );
  m_pParticleSystems[3]->SetPosition( Vect3f( 0.0f, 0.0f, 0.0f ) );
  m_pParticleSystems[3]->SetVelocity( Vect3f( 0.0f, 5.0f, 0.0f ) );
  m_pParticleSystems[3]->SetGravity( Vect3f( 0.0f, 0.0f, 0.0f ) );
  m_pParticleSystems[3]->SetWind( Vect3f( 0.0f, 0.0f, -20.0f ) );
  m_pParticleSystems[3]->SetVelocityVar( 2.5f );
  m_pParticleSystems[3]->SetCollisionPlane( Vect3f( 0.0f, 0.0f,1.0f ), Vect3f( 0.0f, 0.0f, -5.0f ),1.0f, CR_STICK );// Create a series of planes to collide with
  m_pParticleSystems[3]->Init( rm );

	
	//--- Fountain using a single collision plane acting as a floor---
	m_pParticleSystems[4] = new CParticleSystem();
  m_pParticleSystems[4]->SetTexture( "./Data/particle.bmp" );
  m_pParticleSystems[4]->SetMaxParticles( 200 );
  m_pParticleSystems[4]->SetNumToRelease( 10 );
  m_pParticleSystems[4]->SetReleaseInterval( 0.05f );
  m_pParticleSystems[4]->SetLifeCycle( 5.0f );
  m_pParticleSystems[4]->SetSize( 1.5f );
  m_pParticleSystems[4]->SetColor( Vect3f( 1.0f, 1.0f, 1.0f), 1.0f );
  m_pParticleSystems[4]->SetPosition( Vect3f( 0.0f, 0.0f, 0.0f ) );
  m_pParticleSystems[4]->SetVelocity( Vect3f( 0.0f, 0.0f, 0.0f ) );
  m_pParticleSystems[4]->SetGravity( Vect3f( 0.0f, -9.8f, 0.0f ) );
  m_pParticleSystems[4]->SetWind( Vect3f( 0.0f, 0.0f, 0.0f ) );
  m_pParticleSystems[4]->SetVelocityVar( 20.0f ); 
  m_pParticleSystems[4]->SetCollisionPlane( Vect3f( 0.0f, 1.0f, 0.0f ), Vect3f( 0.0f, 0.0f, 0.0f ) );// Create a series of planes to collide with
  m_pParticleSystems[4]->Init( rm );

	//--- Fountain boxed-in by 6 collision planes---
	m_pParticleSystems[5] = new CParticleSystem();
  m_pParticleSystems[5]->SetTexture( "./Data/particle.bmp" );
  m_pParticleSystems[5]->SetMaxParticles( 100 );
  m_pParticleSystems[5]->SetNumToRelease( 5 );
  m_pParticleSystems[5]->SetReleaseInterval( 0.05f );
  m_pParticleSystems[5]->SetLifeCycle( 5.0f );
  m_pParticleSystems[5]->SetSize( 1.5f );
  m_pParticleSystems[5]->SetColor( Vect3f( 1.0f, 1.0f, 1.0f), 1.0f );
  m_pParticleSystems[5]->SetPosition( Vect3f( 0.0f, 0.0f, 0.0f ) );
  m_pParticleSystems[5]->SetVelocity( Vect3f( 0.0f, 0.0f, 0.0f ) );
  m_pParticleSystems[5]->SetGravity( Vect3f( 0.0f, -9.8f, 0.0f ) );
  m_pParticleSystems[5]->SetWind( Vect3f( 0.0f, 0.0f, 0.0f ) );
  m_pParticleSystems[5]->SetVelocityVar( 20.0f );
  // Create a series of planes to collide with
  m_pParticleSystems[5]->SetCollisionPlane( Vect3f( 0.0f, 1.0f, 0.0f ), Vect3f( 0.0f, 0.0f, 0.0f ) ); // Floor
  m_pParticleSystems[5]->SetCollisionPlane( Vect3f( 1.0f, 0.0f, 0.0f ), Vect3f(-3.0f, 0.0f, 0.0f ) ); // Left Wall
  m_pParticleSystems[5]->SetCollisionPlane( Vect3f(-1.0f, 0.0f, 0.0f ), Vect3f( 3.0f, 0.0f, 0.0f ) ); // Right Wall
  m_pParticleSystems[5]->SetCollisionPlane( Vect3f( 0.0f, 0.0f, 1.0f ), Vect3f( 0.0f, 0.0f,-3.0f ) ); // Front Wall
  m_pParticleSystems[5]->SetCollisionPlane( Vect3f( 0.0f, 0.0f,-1.0f ), Vect3f( 0.0f, 0.0f, 3.0f ) ); // Back Wall
  m_pParticleSystems[5]->SetCollisionPlane( Vect3f( 0.0f,-1.0f, 0.0f ), Vect3f( 0.0f, 5.0f, 0.0f ) ); // Ceiling
  m_pParticleSystems[5]->Init( rm );

}

//-----------------ScriptManager------------------------------
void CParticleSystemTestProcess::RegisterFunctions (CScriptManager* scriptManager)
{
	lua_State* l_pLUAState = scriptManager->GetLuaState();
	using namespace luabind;

	// ahora registramos lo que querramos
	module(l_pLUAState)
		[
			def("getPartSystTest", GetPartSystTest),

			// registramos la clase CParticleSystemTestProcess
			class_<CParticleSystemTestProcess>(CScriptRegister::SetClassName("CParticleSystemTestProcess"))


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