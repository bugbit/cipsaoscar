#include "__PCH_Tests.h"

#include <stdio.h>

#include "QuakeGameProcess.h"
#include "QuakePhysicsData.h"

//---Engine Includes--------
#include "Core/Core.h"
#include "Input/InputManager.h"
#include "Graphics/RenderManager.h"
#include "Logger/Logger.h"
#include "Graphics/Object3D.h"
#include "Graphics/FPSCamera.h"
#include "Graphics/Textures/TextureManager.h"
#include "Graphics/Textures/Texture.h"
#include "Graphics/AviPlayer.h"
#include "Graphics/AviPlayerManager.h"
#include "Script/ScriptManager.h"
#include "luabind/luabind.hpp"
#include "Graphics/ASEObject/ASETextureManager.h"
//Includes para el test de físicas:
#include "PhysX/PhysicActor.h"
#include "PhysX/PhysicsManager.h"
#include "PhysX/PhysicASELoader.h"
#include "PhysX/PhysicCookingMesh.h"
#include "PhysX/PhysicSphericalJoint.h"
#include "PhysX/PhysicRevoluteJoint.h"
#include "PhysX/PhysicController.h"
//--------------------------


//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
bool CQuakeGameProcess::Init ()
{
	CProcess::m_bIsOk = false;	
	CPhysicUserData *m_PlayerData=new CQuakePhysicsData("player");
	m_PlayerData->SetPaint(true);
	CPhysicsManager* physicManager = CCore::GetSingletonPtr()->GetPhysicManager();
	m_Player=new CPhysicController(.5f,3.f,45.f,0.1f,.5f,IMPACT_MASK_1,m_PlayerData,Vect3f(5.f,6.f,3.f));
	physicManager->AddPhysicController(m_Player);
	uint32 w,h;
	CCore::GetSingletonPtr()->GetRenderManager()->GetWidthAndHeight(w,h);
	float aspect_ratio = (float)w/h;
	m_pCamera = new CFPSCamera(0.2f,500.f,mathUtils::Deg2Rad(60.f),aspect_ratio,m_Player);
	if (m_Player && m_pCamera) 
	{
		CProcess::m_bIsOk = true;
	}

	if (m_bIsOk)
	{
		CRenderManager* rm = CCore::GetSingletonPtr()->GetRenderManager();
	
		// Quake
		
		m_Arena.LoadWorld("./Data/Textures/quake/","./Data/quake/room%i.ASE",5);
		
		/*m_PruebaItemASE.Load("./Data/quake/ShotGun_Player.ASE",rm);
		SPRUEBAITEM *item=new SPRUEBAITEM;
		item->Position=Vect3f(5.f,6.f,0.f);
		item->Mat.SetIdentity();
		item->Mat.Translate(item->Position);
		item->Angle=0;
		item=new SPRUEBAITEM;
		m_PruebaItems.push_back(item);
		item->Position=Vect3f(5.f,6.f,5.f);
		item->Mat.SetIdentity();
		item->Mat.Translate(item->Position);
		m_PruebaItems.push_back(item);*/

		// Prueba PhysX

		/*CPhysicUserData *data=new CPhysicUserData();
		data->SetPaint(true);
		CPhysicActor plano(data);
		plano.AddPlaneShape(Vect3f(0.f,1.f,0.f),0.f);
		CCore::GetSingletonPtr()->GetPhysicManager()->AddPhysicActor(&plano);		
		m_Pelota=new CPhysicActor(data);
		m_Pelota->CreateBody(1.f);		
		m_Pelota->AddSphereShape(1.f,Vect3f(5.f,6.f,0.f));
		CCore::GetSingletonPtr()->GetPhysicManager()->AddPhysicActor(m_Pelota);*/

		m_EnemyData=new CQuakePhysicsData("enemy");
		m_EnemyData->SetPaint(true);
		m_Enemy=new CPhysicController(1.5f,5.f,0.4f,0.1f,3.f,1,m_EnemyData,Vect3f(5.f,6.f,1.f));
		physicManager->AddPhysicController(m_Enemy);
	}
	if (!CProcess::m_bIsOk)
	{
		Release();
	}
	return CProcess::m_bIsOk;
}

void CQuakeGameProcess::Release ()
{
	CHECKED_DELETE(m_PelotaData);
	CHECKED_DELETE(m_PlayerData);
	CHECKED_DELETE(m_EnemyData);
	CHECKED_DELETE(m_Player);
	CHECKED_DELETE(m_pCamera);
	//m_PruebaItemASE.CleanUp();
	CHECKED_DELETE(m_Pelota);
	CHECKED_DELETE(m_Player);
}

void CQuakeGameProcess::RenderQuake(CRenderManager* renderManager)
{
	// Render quake
	
	m_Arena.RenderScene(renderManager);
	/*std::vector<SPRUEBAITEM *>::iterator it=m_PruebaItems.begin(),itend=m_PruebaItems.end();
	for(;it!=itend;it++)
	{
		SPRUEBAITEM *item=*it;
		renderManager->SetTransform(item->Mat);
		m_PruebaItemASE.Render(renderManager);
	}*/
}

void CQuakeGameProcess::RenderScene (CRenderManager* renderManager)
{
	/*renderManager->DrawAxis(100.f);
	renderManager->DrawGrid(150.f,colWHITE,20,20);
	renderManager->DrawCamera(m_pCamera);*/

	// Render quake
	
	RenderQuake(renderManager);

	// Prueba PhysX

	CCore::GetSingletonPtr()->GetPhysicManager()->DebugRender(renderManager);

	//Pruebas shut

	std::vector<SPRUEBASHUT *>::iterator itend=m_PruebaShut.end();
	for (std::vector<SPRUEBASHUT *>::iterator it=m_PruebaShut.begin();it!=itend;it++)
	{
		renderManager->DrawSphere(3.f);
	}
}

uint32 CQuakeGameProcess::RenderDebugInfo(CRenderManager* renderManager, float fps)
{
	uint32 posY = 0;
	posY = CProcess::RenderDebugInfo(renderManager, fps);
	if (m_bRenderDebugInfo)
	{
		uint32 posX = m_PosRenderDebugInfo.x;
		//...
	}
	return posY;
}

void CQuakeGameProcess::UpdatePlayer(float elapsedTime)
{
	CProcess::Update(elapsedTime);
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

//void CQuakeGameProcess::UpdatePruebaItems  (float elapsedTime)
//{
//	std::vector<SPRUEBAITEM *>::iterator it=m_PruebaItems.begin(),itend=m_PruebaItems.end();
//	for(;it!=itend;it++)
//	{
//		SPRUEBAITEM *item=*it;
//		Mat44f matTrans;
//		Mat44f matRot;
//		item->Angle = fmod(item->Angle+ePIf*elapsedTime / 180.f,e2PIf);
//		matTrans.SetIdentity();
//		matRot.SetIdentity();
//		matTrans.Translate(item->Position);
//		matRot.RotByAngleY(item->Angle);
//		item->Mat=matTrans*matRot;
//	}
//}

void CQuakeGameProcess::Update (float elapsedTime)
{
	UpdatePlayer(elapsedTime);
	//UpdatePruebaItems(elapsedTime);

	// Prueba PhysX

	CInputManager*inputManager = CCore::GetSingletonPtr()->GetInputManager();
	
	if (inputManager->IsDownUp(IDV_KEYBOARD, ZVK_B))
	{
		if (m_Pelota==NULL)
		{
			CPhysicUserData *data=new CPhysicUserData();
			data->SetPaint(true);
			m_Pelota=new CPhysicActor(data);
			m_Pelota->CreateBody(100.9f,.9f);		
			m_Pelota->AddSphereShape(1.f,m_Player->GetPosition());
			CCore::GetSingletonPtr()->GetPhysicManager()->AddPhysicActor(m_Pelota);
		}
		m_Pelota->SetLinearVelocity(m_pCamera->GetDirection());
		
	}
	if (m_Pelota!=NULL)
	{
		if (inputManager->IsDown(IDV_KEYBOARD, ZVK_A))
		{
			m_Pelota->SetLinearVelocity(Vect3f(0.f,10.f,0.f));
		}
	}

	Vect3f v(0,0,0);
	//if (inputManager->IsDown(IDV_KEYBOARD, ZVK_Z))
	//	v.z=-.3f;
	//if (inputManager->IsDown(IDV_KEYBOARD, ZVK_X))
	//	v.z=.3f;
	//if (inputManager->IsDown(IDV_KEYBOARD, ZVK_W))
	//	v.x=-.3f;
	//if (inputManager->IsDown(IDV_KEYBOARD, ZVK_S))
	//	v.x=.3f;
	//m_Player->Move(v,elapsedTime);
	v.x=(float) mPCDiff;
	v.z=0;
	mPCX += mPCDiff;
	if (mPCDiff>0)
	{
		if(mPCX>200)
			mPCDiff *= -1;
	}
	else
	{
		if(mPCX<-200)
			mPCDiff *= -1;
	}
	m_Enemy->Move(.025f*v,elapsedTime);

	//Pruebas shut

	//Rayo

	if (inputManager->IsDown(IDV_MOUSE, 0))
	{
		m_PruebaShut.clear();
		SCollisionInfo info;
		CQuakePhysicsData *data=(CQuakePhysicsData *) CCore::GetSingletonPtr()->GetPhysicManager()->RaycastClosestActor(m_Player->GetPosition(),m_pCamera->GetDirection(),IMPACT_MASK_1,NULL,info);
		if (data!=NULL)
		{
			SPRUEBASHUT *shut=new SPRUEBASHUT;
			shut->msg += std::string("RaycastClosestActor a colisionado con ").append(data->GetName());
			shut->pos=info.m_CollisionPoint;
		}
	}
}

//-----------------ScriptManager------------------------------
void CQuakeGameProcess::RegisterFunctions (CScriptManager* scriptManager)
{
	lua_State* l_pLUAState = scriptManager->GetLuaState();
	using namespace luabind;
	
	// ahora registramos lo que querramos
	module(l_pLUAState)
    [
		def("getQuakeGame", GetQuakeGame),
		
		// registramos la clase CQuakeGameProcess
		class_<CQuakeGameProcess>(CScriptRegister::SetClassName("CQuakeGameProcess"))
		

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