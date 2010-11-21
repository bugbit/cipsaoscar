#include "__PCH_Tests.h"

#include <stdio.h>

#include "QuakeGameProcess.h"
#include "QuakePhysicsData.h"
#include "QuakePlayerInput.h"
#include "QuakePlayer.h"
#include "QuakeHumanPlayerInput.h"

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
	CPhysicsManager* physicManager = CCore::GetSingletonPtr()->GetPhysicManager();
	CQuakePhysicsData *playerdata=new CQuakePhysicsData("player",CQuakePhysicsData::TYPE3D_PLAYER);
	CQuakePlayer *player=new CQuakePlayer(.5f,3.f,45.f,0.1f,.5f,IMPACT_MASK_1,playerdata,Vect3f(5.f,6.f,3.f));
	playerdata->SetPaint(true);
	playerdata->SetObject3D(player);
	physicManager->AddPhysicController(player);
	CQuakeHumanPlayerInput *inputplayer=new CQuakeHumanPlayerInput();
	inputplayer->SetPlayer(player);
	m_PlayerInputs.push_back(inputplayer);
	uint32 w,h;
	CCore::GetSingletonPtr()->GetRenderManager()->GetWidthAndHeight(w,h);
	float aspect_ratio = (float)w/h;
	m_pCamera = new CFPSCamera(0.2f,500.f,mathUtils::Deg2Rad(60.f),aspect_ratio,player);
	if (player && m_pCamera) 
	{
		CProcess::m_bIsOk = true;
	}

	if (m_bIsOk)
	{
		CRenderManager* rm = CCore::GetSingletonPtr()->GetRenderManager();
	
		// Quake
		
		m_Arena.LoadWorld("./Data/Textures/quake/","./Data/quake/room%i.ASE",5);
		
		m_PruebaItemASE.Load("./Data/quake/ShotGun_Player.ASE",rm);
		/*SPRUEBAITEM *item=new SPRUEBAITEM;
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

		m_PelotaData=new CQuakePhysicsData("pelota");
		m_PelotaData->SetPaint(true);
		/*CPhysicUserData *data=new CPhysicUserData();
		data->SetPaint(true);
		CPhysicActor plano(data);
		plano.AddPlaneShape(Vect3f(0.f,1.f,0.f),0.f);
		CCore::GetSingletonPtr()->GetPhysicManager()->AddPhysicActor(&plano);
		m_PelotaData=new CQuakePhysicsData("pelota");
		m_PelotaData->SetPaint(true);
		m_Pelota=new CPhysicActor(m_PelotaData);
		m_Pelota->CreateBody(100.9f,.9f);	
		m_Pelota->AddSphereShape(1.f,Vect3f(5.f,6.f,0.f));
		CCore::GetSingletonPtr()->GetPhysicManager()->AddPhysicActor(m_Pelota);	*/

		m_EnemyData=new CQuakePhysicsData("enemy",CQuakePhysicsData::TYPE3D_PLAYER);
		m_EnemyData->SetPaint(true);
		m_Enemy=new CPhysicController(1.5f,5.f,0.4f,0.1f,3.f,IMPACT_MASK_1,m_EnemyData,Vect3f(5.f,6.f,1.f));
		physicManager->AddPhysicController(m_Enemy);
		m_EnemyData->SetObject3D(m_Enemy);
		m_TriggerData=new CQuakePhysicsData("trigger");
		m_TriggerData->SetPaint(false);
		m_Trigger=new CPhysicActor(m_TriggerData);
		m_Trigger->CreateBoxTrigger(Vect3f(7.f,2.f,7.f),GROUP_BASIC_PRIMITIVES);
		m_Trigger->SetGlobalPosition(Vect3f(5.f,2.f,1.f));
		physicManager->AddPhysicActor(m_Trigger);
		physicManager->SetTriggerReport(this);

		m_ActorPruebaShutData=new CQuakePhysicsData("shut");
		m_ActorPruebaShutData->SetPaint(true);
		m_ActorPruebaShut=new CPhysicActor(m_ActorPruebaShutData);
		m_ActorPruebaShut->AddBoxSphape(Vect3f(.5f,.3f,.25f));
		m_ActorPruebaShut->SetGlobalPosition(Vect3f(5.f,6.f,0.f));
		m_ActorPruebaShut->CreateBody(0.1f,.8f);
		physicManager->AddPhysicActor(m_ActorPruebaShut);

		m_ActorPruebaJointData=new CQuakePhysicsData("joint");
		m_ActorPruebaJointData->SetPaint(true);
		m_ActorPruebaJoint=new CPhysicActor(m_ActorPruebaJointData);
		m_ActorPruebaJoint->AddBoxSphape(Vect3f(1.f,1.f,1.f));
		m_ActorPruebaJoint->SetGlobalPosition(Vect3f(0,5.f,0));
		m_ActorPruebaJoint->CreateBody(2.f,3.f);
		physicManager->AddPhysicActor(m_ActorPruebaJoint);
		m_PruebaJoint=new CPhysicSphericalJoint();
		m_PruebaJoint->SetInfo(Vect3f(0,3.f,0),m_ActorPruebaJoint);
		physicManager->AddPhysicSphericalJoint(m_PruebaJoint);
	}
	if (!CProcess::m_bIsOk)
	{
		Release();
	}
	return CProcess::m_bIsOk;
}

void CQuakeGameProcess::ReleasePlayerInputs()
{
	std::vector<CQuakePlayerInput *>::iterator it=m_PlayerInputs.begin(),
		itend=m_PlayerInputs.end();
	for(;it!=itend;it++)
	{
		CQuakePlayerInput *playerinput=*it;
		delete playerinput;
	}
}

void CQuakeGameProcess::Release ()
{
	CHECKED_DELETE(m_PelotaData);
	CHECKED_DELETE(m_EnemyData);
	CHECKED_DELETE(m_pCamera);
	m_PruebaItemASE.CleanUp();
	CHECKED_DELETE(m_Pelota);
	CHECKED_DELETE(m_Trigger);
	CHECKED_DELETE(m_TriggerData);
	CHECKED_DELETE(m_ActorPruebaShut);
	CHECKED_DELETE(m_ActorPruebaShutData);
	CHECKED_DELETE(m_ActorPruebaJoint);
	CHECKED_DELETE(m_ActorPruebaJointData);
	CHECKED_DELETE(m_PruebaJoint);
	ReleasePlayerInputs();
}

void CQuakeGameProcess::OnEnter (CPhysicUserData* trigger, CPhysicUserData* other_shape)
{
	char str[100];

	sprintf_s(str,sizeof(str),"El %s ha entrado en %s",((CQuakePhysicsData *) other_shape)->GetName().c_str(),((CQuakePhysicsData *) trigger)->GetName().c_str());
	mStrTrigger.assign(str);
}

void CQuakeGameProcess::OnLeave (CPhysicUserData* trigger1, CPhysicUserData* other_shape)
{
	char str[100];

	sprintf_s(str,sizeof(str),"El %s ha salido en %s",((CQuakePhysicsData *) other_shape)->GetName().c_str(),((CQuakePhysicsData *) trigger1)->GetName().c_str());
	mStrTrigger.assign(str);
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

	Mat44f mat;
	//mat.SetIdentity();
	//mat.Translate(Vect3f(5.f,6.f,0.f));
	m_ActorPruebaShut->GetMat44(mat);
	renderManager->SetTransform(mat);
	m_PruebaItemASE.Render(renderManager);
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
		SPRUEBASHUT *shut=*it;
		Mat44f mat;
		mat.SetIdentity();
		mat.Translate(shut->pos);
		renderManager->SetTransform(mat);
		renderManager->DrawSphere(.3f,colRED);
	}
	/*Mat44f mat;
	mat.SetIdentity();
	mat.Translate(Vect3f(5.f,6.f,1.f));
	renderManager->SetTransform(mat);
	renderManager->DrawBox(7.f,2.f,7.f,colGREEN);*/
}

uint32 CQuakeGameProcess::RenderDebugInfo(CRenderManager* renderManager, float fps)
{
	uint32 posY = 0;
	posY = CProcess::RenderDebugInfo(renderManager, fps);
	if (m_bRenderDebugInfo)
	{
		uint32 posX = m_PosRenderDebugInfo.x;
		std::vector<SPRUEBASHUT *>::iterator itend=m_PruebaShut.end();
		for (std::vector<SPRUEBASHUT *>::iterator it=m_PruebaShut.begin();it!=itend;it++)
		{
			SPRUEBASHUT *shut=*it;
			posY += renderManager->DrawDefaultText(posX,posY,colWHITE,shut->msg.c_str());
		}
		//...
		posY += renderManager->DrawDefaultText(posX,posY,colWHITE,mStrTrigger.c_str());
	}
	return posY;
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

void  CQuakeGameProcess::UpdatePlayerInputs(float elapsedTime)
{
	std::vector<CQuakePlayerInput *>::iterator it=m_PlayerInputs.begin(),
		itend=m_PlayerInputs.end();
	for(;it!=itend;it++)
	{
		CQuakePlayerInput *playerinput=*it;
		playerinput->UpdateInputAction(elapsedTime);
	}
}

void CQuakeGameProcess::Update (float elapsedTime)
{
	CProcess::Update(elapsedTime);
	UpdatePlayerInputs(elapsedTime);
	m_Arena.Update(elapsedTime);
	//UpdatePruebaItems(elapsedTime);

	// Prueba PhysX

	CInputManager*inputManager = CCore::GetSingletonPtr()->GetInputManager();
	
	if (inputManager->IsDownUp(IDV_KEYBOARD, ZVK_B))
	{				
			CPhysicActor *pelota=new CPhysicActor(m_PelotaData);
			pelota->CreateBody(100.9f,.9f);		
			pelota->AddSphereShape(1.f);
			pelota->SetGlobalPosition(m_pCamera->GetObject3D()->GetPosition());
			CCore::GetSingletonPtr()->GetPhysicManager()->AddPhysicActor(pelota);
			pelota->SetLinearVelocity(m_pCamera->GetDirection());		
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

	if (inputManager->IsDownUp(IDV_MOUSE, 0))
	{
		m_PruebaShut.clear();
		SCollisionInfo info;
		Vect3f dir=m_pCamera->GetDirection();
		Vect3f pos=m_pCamera->GetObject3D()->GetPosition()+.6f*dir;
		CQuakePhysicsData *data=(CQuakePhysicsData *) CCore::GetSingletonPtr()->GetPhysicManager()->RaycastClosestActor(pos,dir,COLLIDABLE_MASK,NULL,info);
		if (data!=NULL)
		{
			SPRUEBASHUT *shut=new SPRUEBASHUT;
			shut->msg += std::string("RaycastClosestActor a colisionado con ").append(data->GetName());			
			shut->pos=info.m_CollisionPoint;
			m_PruebaShut.push_back(shut);
		}
	}

	// Granada

	if (inputManager->IsDownUp(IDV_MOUSE, 1))
	{
		m_PruebaShut.clear();
		Vect3f dir=m_pCamera->GetDirection();
		Vect3f pos=m_pCamera->GetObject3D()->GetPosition()+.6f*dir;
		std::vector<CPhysicUserData *> mdatas;
		CCore::GetSingletonPtr()->GetPhysicManager()->OverlapSphereActor(10.f,pos,mdatas);
		if (!mdatas.empty())
		{
			std::vector<CPhysicUserData *>::iterator itend=mdatas.end();
			for(std::vector<CPhysicUserData *>::iterator it=mdatas.begin();it!=itend;it++)
			{
				CQuakePhysicsData *data=(CQuakePhysicsData *) *it;
				SPRUEBASHUT *shut=new SPRUEBASHUT;
				shut->msg += std::string("OverlapSphereActor a colisionado con ").append(data->GetName());
				const CObject3D *obj3d= data->GetObject3D();
				if (obj3d==NULL)
					shut->pos=pos;
				else
					shut->pos=obj3d->GetPosition();
				m_PruebaShut.push_back(shut);
			}
		}
	}

	// Prueba Joint

	if (inputManager->IsDown(IDV_KEYBOARD, ZVK_C))
	{
		m_ActorPruebaJoint->SetLinearVelocity(Vect3f(0.f,10.f,0.f));
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