#include "__PCH_Tests.h"

#include "PhysXtestProcess.h"
//---PhysX Includes---//
#undef min
#undef max
#include "NxPhysics.h"
#include "NxController.h"
#include "NxCapsuleController.h"
//---------------------//
//---Engine Includes--------
#include "Core/Core.h"
#include "Input/InputManager.h"
#include "Graphics/RenderManager.h"
#include "Graphics/FontManager.h"
#include "Graphics/Object3D.h"
#include "Graphics/ThPSCamera.h"
#include "Graphics/FPSCamera.h"
#include "Base/Math/Matrix34.h"
#include "Script/ScriptManager.h"
#include "luabind/luabind.hpp"
//Includes para el test de físicas:
#include "PhysX/PhysicActor.h"
#include "PhysX/PhysicsManager.h"
#include "PhysX/PhysicASELoader.h"
#include "PhysX/PhysicCookingMesh.h"
#include "PhysX/PhysicSphericalJoint.h"
#include "PhysX/PhysicRevoluteJoint.h"
#include "PhysX/PhysicController.h"
//--------------------------------------

CPhysXTestProcess::CPhysXTestProcess(const std::string& processName)
: CProcess(processName)
, m_pObject3D(NULL)
, m_sNameImpactObject("None")
, m_sNameTrigger1("None") 
, m_sNameTrigger2("None")
, m_pPhysicSphericalJoint_Light20(NULL)
, m_pPhysicSphericalJoint_Light21(NULL)
, m_pPhysicSphericalJoint_Light22(NULL)
, m_pPhysicActor_BoxLight20(NULL)
, m_pPhysicActor_BoxLight21(NULL)
, m_pPhysicActor_BoxLight22(NULL)
, m_pGameEntity_BoxLight2(NULL)
, m_pPhysicSphericalJoint_Light1(NULL)
, m_pPhysicActor_BoxLight1(NULL)
, m_pGameEntity_BoxLight1(NULL)
, m_pPhysicActor_Box(NULL)
, m_pGameEntity_Box(NULL)
, m_pPhysicActor_Sphere(NULL)
, m_pGameEntity_Sphere(NULL)
, m_pPhysicActor_Capsule(NULL)
, m_pGameEntity_Capsule(NULL)
, m_pPhysicActor_Plane(NULL)
, m_pGameEntity_Plane(NULL)
, m_pPhysicActor_Mesh(NULL)
, m_pGameEntity_Mesh_2(NULL)
, m_pPhysicActor_Mesh_2(NULL)
, m_pGameEntity_Mesh(NULL)
, m_pPhysicActor_Composite(NULL)
, m_pGameEntity_Composite(NULL)
, m_pGameEntity_BoxBridge(NULL)
, m_pPhysicActor_BoxBridge(NULL)
, m_pPhysicActor_BoxPath(NULL)
, m_pLastPickedObject(NULL)
, m_pPhysicController1(NULL)
, m_pPhysicController2(NULL)
, m_pGameEntity_Controller1(NULL)
, m_pGameEntity_Controller2(NULL)
{
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
bool CPhysXTestProcess::Init ()
{
	CProcess::m_bIsOk = false;
	uint32 w,h;
	CORE->GetRenderManager()->GetWidthAndHeight(w,h);
	m_pObject3D = new CObject3D(Vect3f(0.f,0.f,0.f), 0.f, 0.f);

	float aspect_ratio = (float)w/h;
	m_pCamera = new CThPSCamera(1.f,500.f,mathUtils::Deg2Rad(60.f),aspect_ratio,m_pObject3D,50.f);
	if (m_pObject3D && m_pCamera) 
	{
		CProcess::m_bIsOk = true;
		m_pCurrentCamera = m_pCamera;
	}
	//---Vamos a probar el tema de físicas:--------------------------------------------------
	CPhysicsManager* physicManager = CORE->GetPhysicManager();
	assert(physicManager);


	//-----------------------------------------------------------------
	// Añadimos un actor con solo una BOX como shape
	//-----------------------------------------------------------------
	m_pGameEntity_Box = new CGameEntity();
	assert(m_pGameEntity_Box);
	m_pGameEntity_Box->SetName("Box");
	m_pGameEntity_Box->SetPaint(true);
	m_pPhysicActor_Box = new CPhysicActor(m_pGameEntity_Box);
	assert(m_pPhysicActor_Box);
	m_pPhysicActor_Box->AddBoxSphape(Vect3f(1.f,1.f,1.f),v3fZERO,0,GROUP_BASIC_PRIMITIVES);
	m_pPhysicActor_Box->SetGlobalPosition(Vect3f(0.f,10.f,0.f));
	m_pPhysicActor_Box->CreateBody(0.5f);
	CProcess::m_bIsOk = physicManager->AddPhysicActor(m_pPhysicActor_Box);

	//-----------------------------------------------------------------
	// Añadimos un actor con solo una SHPERE como shape
	//-----------------------------------------------------------------
	if (CProcess::m_bIsOk)
	{
		m_pGameEntity_Sphere = new CGameEntity();
		assert(m_pGameEntity_Sphere);
		m_pGameEntity_Sphere->SetName("Sphere");
		m_pGameEntity_Sphere->SetPaint(true);
		m_pPhysicActor_Sphere = new CPhysicActor(m_pGameEntity_Sphere);
		assert(m_pPhysicActor_Sphere); 
		m_pPhysicActor_Sphere->AddSphereShape(0.5f,v3fZERO,0,GROUP_BASIC_PRIMITIVES);
		m_pPhysicActor_Sphere->SetGlobalPosition(Vect3f(5.f,10.f,0.f));
		m_pPhysicActor_Sphere->CreateBody(0.5f);
		CProcess::m_bIsOk = physicManager->AddPhysicActor(m_pPhysicActor_Sphere);
	}
	//-----------------------------------------------------------------
	// Añadimos un actor con solo un PLANE como shape
	//-----------------------------------------------------------------
	if (CProcess::m_bIsOk)
	{
		m_pGameEntity_Plane = new CGameEntity();
		assert(m_pGameEntity_Plane);
		m_pGameEntity_Plane->SetName("Plane");
		m_pGameEntity_Plane->SetPaint(true);
		m_pPhysicActor_Plane = new CPhysicActor(m_pGameEntity_Plane);
		assert(m_pPhysicActor_Plane); 
		m_pPhysicActor_Plane->AddPlaneShape(Vect3f(0.f,1.f,0.f),0.f,GROUP_BASIC_PRIMITIVES);
		CProcess::m_bIsOk = physicManager->AddPhysicActor(m_pPhysicActor_Plane);
	}
	//-----------------------------------------------------------------
	// Añadimos un actor con solo una CAPSULE como shape
	//-----------------------------------------------------------------
	if (CProcess::m_bIsOk)
	{
		m_pGameEntity_Capsule = new CGameEntity();
		assert(m_pGameEntity_Capsule);
		m_pGameEntity_Capsule->SetName("Capsule");
		m_pGameEntity_Capsule->SetPaint(true);
		m_pPhysicActor_Capsule = new CPhysicActor(m_pGameEntity_Capsule);
		assert(m_pPhysicActor_Capsule); 
		m_pPhysicActor_Capsule->AddCapsuleShape(0.5f,1.f,v3fZERO,0,GROUP_BASIC_PRIMITIVES);
		m_pPhysicActor_Capsule->SetGlobalPosition(Vect3f(5.f,50.f,5.f));
		m_pPhysicActor_Capsule->CreateBody(0.5f);
		CProcess::m_bIsOk = physicManager->AddPhysicActor(m_pPhysicActor_Capsule);
	}
	//-----------------------------------------------------------------
	// Añadimos un actor con más de una forma 2boxes y 4 shperes representando un "COCHE"
	//-----------------------------------------------------------------
	if (CProcess::m_bIsOk)
	{
		m_pGameEntity_Composite = new CGameEntity();
		assert(m_pGameEntity_Composite);
		m_pGameEntity_Composite->SetName("Composite-Car");
		m_pGameEntity_Composite->SetPaint(true);
		m_pPhysicActor_Composite = new CPhysicActor(m_pGameEntity_Composite);
		assert(m_pPhysicActor_Composite);
		m_pPhysicActor_Composite->AddSphereShape(0.2f,Vect3f(1.f,0.f,-2.f),0,GROUP_BASIC_PRIMITIVES);
		m_pPhysicActor_Composite->AddSphereShape(0.2f,Vect3f(-1.f,0.f,-2.f),0,GROUP_BASIC_PRIMITIVES);
		m_pPhysicActor_Composite->AddSphereShape(0.2f,Vect3f(1.f,0.f,2.f),0,GROUP_BASIC_PRIMITIVES);
		m_pPhysicActor_Composite->AddSphereShape(0.2f,Vect3f(-1.f,0.f,2.f),0,GROUP_BASIC_PRIMITIVES);
		m_pPhysicActor_Composite->AddBoxSphape(Vect3f(1.f,0.4f,2.f),Vect3f(0.f,0.4f,0.f),0,GROUP_BASIC_PRIMITIVES);
		m_pPhysicActor_Composite->AddBoxSphape(Vect3f(0.5f,0.4f,0.5f),Vect3f(0.f,0.8f+0.4f,0.f),0,GROUP_BASIC_PRIMITIVES);
		m_pPhysicActor_Composite->SetGlobalPosition(Vect3f(-10.f,50.f,5.f));
		m_pPhysicActor_Composite->CreateBody(0.5f);
		CProcess::m_bIsOk = physicManager->AddPhysicActor(m_pPhysicActor_Composite);
	}
	//-----------------------------------------------------------------
	// Añadimos un actor con más de una MESH como shape.. la mesh de un personaje del quake (biker)
	//-----------------------------------------------------------------
	if (CProcess::m_bIsOk)
	{
		std::vector<uint32> faces;
		std::vector<Vect3f> vertices;

		CPhysicASELoader::ReadMeshFromASE("./Data/quake_biker.ASE",vertices, faces);
		physicManager->GetCookingMesh()->CreatePhysicMesh(vertices, faces,"biker");
		NxTriangleMesh* mesh = physicManager->GetCookingMesh()->GetPhysicMesh("biker");

		m_pGameEntity_Mesh = new CGameEntity();
		assert(m_pGameEntity_Mesh);
		m_pGameEntity_Mesh->SetName("Mesh_biker");
		m_pGameEntity_Mesh->SetPaint(true);

		m_pPhysicActor_Mesh = new CPhysicActor(m_pGameEntity_Mesh);
		assert(m_pPhysicActor_Mesh); 
		m_pPhysicActor_Mesh->AddMeshShape(mesh,v3fZERO,GROUP_BASIC_PRIMITIVES);
		m_pPhysicActor_Mesh->SetGlobalPosition(Vect3f(0.f,50.f,0.f));
		m_pPhysicActor_Mesh->CreateBody(0.5f);
		CProcess::m_bIsOk = physicManager->AddPhysicActor(m_pPhysicActor_Mesh);
	}
	//-----------------------------------------------------------------
	// Añadimos un actor BOX y un Joint tipo PhysicSphericalJoint para represntar un luz 
	//-----------------------------------------------------------------
	if (CProcess::m_bIsOk)
	{
		CPhysicsManager* physicManager = CORE->GetPhysicManager();
		assert(physicManager);
		//- Afegim un actor a l'escena amb només una forma-->BOX per fer una llum amb nomes un node
		m_pGameEntity_BoxLight1 = new CGameEntity();
		assert(m_pGameEntity_BoxLight1);
		m_pGameEntity_BoxLight1->SetName("BoxLight1");
		m_pGameEntity_BoxLight1->SetPaint(true);
		m_pPhysicActor_BoxLight1 = new CPhysicActor(m_pGameEntity_BoxLight1);
		assert(m_pPhysicActor_BoxLight1);
		m_pPhysicActor_BoxLight1->AddBoxSphape(Vect3f(1.f,1.f,1.f),v3fZERO,0,GROUP_BASIC_PRIMITIVES);
		m_pPhysicActor_BoxLight1->SetGlobalPosition(Vect3f(-20.f,5.f,-20.f));
		m_pPhysicActor_BoxLight1->CreateBody(0.5f);
		CProcess::m_bIsOk = physicManager->AddPhysicActor(m_pPhysicActor_BoxLight1);
		if(CProcess::m_bIsOk)
		{	
			//- Afegim un joint de tipus esfèric per simular la llum.
			m_pPhysicSphericalJoint_Light1 = new CPhysicSphericalJoint();
			assert(m_pPhysicSphericalJoint_Light1);
			m_pPhysicSphericalJoint_Light1->SetInfo(Vect3f(-20.f,10.f,-20.f),m_pPhysicActor_BoxLight1,NULL);
			CProcess::m_bIsOk  = physicManager->AddPhysicSphericalJoint(m_pPhysicSphericalJoint_Light1);
		}
	}
	//-----------------------------------------------------------------
	// Añadimos tres actores BOX y tres Joint tipo PhysicSphericalJoint para represntar un luz con mas de un nodo
	//-----------------------------------------------------------------
	if (CProcess::m_bIsOk)
	{
		CPhysicsManager* physicManager = CORE->GetPhysicManager();
		assert(physicManager);
		m_pGameEntity_BoxLight2 = new CGameEntity();
		assert(m_pGameEntity_BoxLight2);
		m_pGameEntity_BoxLight2->SetName("BoxLight2");
		m_pGameEntity_BoxLight2->SetPaint(true);
		m_pPhysicActor_BoxLight20 = new CPhysicActor(m_pGameEntity_BoxLight2);
		assert(m_pPhysicActor_BoxLight20);
		m_pPhysicActor_BoxLight20->AddBoxSphape(Vect3f(1.f,1.f,1.f),v3fZERO,0,GROUP_BASIC_PRIMITIVES);
		m_pPhysicActor_BoxLight20->SetGlobalPosition(Vect3f(-30.f,5.f,-30.f));
		m_pPhysicActor_BoxLight20->CreateBody(0.5f);
		CProcess::m_bIsOk = physicManager->AddPhysicActor(m_pPhysicActor_BoxLight20);
	}
	if (CProcess::m_bIsOk)
	{
		m_pPhysicActor_BoxLight21 = new CPhysicActor(m_pGameEntity_BoxLight2);
		assert(m_pPhysicActor_BoxLight21);
		m_pPhysicActor_BoxLight21->AddBoxSphape(Vect3f(1.f,1.f,1.f),v3fZERO,0,GROUP_BASIC_PRIMITIVES);
		m_pPhysicActor_BoxLight21->SetGlobalPosition(Vect3f(-30.f,10.f,-30.f));
		m_pPhysicActor_BoxLight21->CreateBody(0.5f);
		CProcess::m_bIsOk = physicManager->AddPhysicActor(m_pPhysicActor_BoxLight21);
	}
	if (CProcess::m_bIsOk)
	{
		m_pPhysicActor_BoxLight22 = new CPhysicActor(m_pGameEntity_BoxLight2);
		assert(m_pPhysicActor_BoxLight22);
		m_pPhysicActor_BoxLight22->AddBoxSphape(Vect3f(1.f,1.f,1.f),v3fZERO,0,GROUP_BASIC_PRIMITIVES);
		m_pPhysicActor_BoxLight22->SetGlobalPosition(Vect3f(-30.f,15.f,-30.f));
		m_pPhysicActor_BoxLight22->CreateBody(0.5f);
		CProcess::m_bIsOk = physicManager->AddPhysicActor(m_pPhysicActor_BoxLight22);
	}
	if(CProcess::m_bIsOk)
	{	
		//- Afegim un joint de tipus esfèric per simular la llum.
		m_pPhysicSphericalJoint_Light20 = new CPhysicSphericalJoint();
		assert(m_pPhysicSphericalJoint_Light20);
		m_pPhysicSphericalJoint_Light20->SetInfo(Vect3f(-30.f,20.f,-30.f),m_pPhysicActor_BoxLight22,NULL);
		CProcess::m_bIsOk  = physicManager->AddPhysicSphericalJoint(m_pPhysicSphericalJoint_Light20);
	}
	if (CProcess::m_bIsOk)
	{
			m_pPhysicSphericalJoint_Light21 = new CPhysicSphericalJoint();
			assert(m_pPhysicSphericalJoint_Light21);
			m_pPhysicSphericalJoint_Light21->SetInfo(Vect3f(-30.f,15.f,-30.f),m_pPhysicActor_BoxLight22,m_pPhysicActor_BoxLight21);
			CProcess::m_bIsOk  = physicManager->AddPhysicSphericalJoint(m_pPhysicSphericalJoint_Light21);
	}
	if (CProcess::m_bIsOk)
	{
		m_pPhysicSphericalJoint_Light22 = new CPhysicSphericalJoint();
		assert(m_pPhysicSphericalJoint_Light22);
		m_pPhysicSphericalJoint_Light22->SetInfo(Vect3f(-30.f,10.f,-30.f),m_pPhysicActor_BoxLight21,m_pPhysicActor_BoxLight20);
		CProcess::m_bIsOk  = physicManager->AddPhysicSphericalJoint(m_pPhysicSphericalJoint_Light22);
	}

	//-----------------------------------------------------------------
	// Añadimos un actor BOX y un joint tipo CPhysicRevoluteJoint representado un puente elevadizo
	//-----------------------------------------------------------------
	if (CProcess::m_bIsOk)
	{
		//El puente
		m_pGameEntity_BoxBridge = new CGameEntity();
		assert(m_pGameEntity_BoxBridge);
		m_pGameEntity_BoxBridge->SetName("BoxBridge");
		m_pGameEntity_BoxBridge->SetPaint(true);
		m_pPhysicActor_BoxBridge = new CPhysicActor(m_pGameEntity_BoxBridge);
		assert(m_pPhysicActor_BoxBridge);
		m_pPhysicActor_BoxBridge->AddBoxSphape(Vect3f(2.f,5.f,0.2f),v3fZERO,0,GROUP_BASIC_PRIMITIVES);
		m_pPhysicActor_BoxBridge->SetGlobalPosition(Vect3f(10.f,10.2f,10.f));
		m_pPhysicActor_BoxBridge->CreateBody(0.5f);
		CProcess::m_bIsOk = physicManager->AddPhysicActor(m_pPhysicActor_BoxBridge);
	}
	if (CProcess::m_bIsOk)
	{
		//Camino
		m_pPhysicActor_BoxPath = new CPhysicActor(m_pGameEntity_BoxBridge);
		assert(m_pPhysicActor_BoxPath);
		m_pPhysicActor_BoxPath->AddBoxSphape(Vect3f(2.f,2.5f,5.0f),v3fZERO,0,GROUP_BASIC_PRIMITIVES);
		m_pPhysicActor_BoxPath->AddBoxSphape(Vect3f(2.f,2.5f,5.0f),Vect3f(0.f,0.f,-18.0f),0,GROUP_BASIC_PRIMITIVES);
		m_pPhysicActor_BoxPath->SetGlobalPosition(Vect3f(10.f,2.5f,15.0f));
		m_pPhysicActor_BoxPath->CreateBody(0.5f);
		CProcess::m_bIsOk = physicManager->AddPhysicActor(m_pPhysicActor_BoxPath);
	}
	if (CProcess::m_bIsOk)
	{
		m_pPhysicRevolute_Bridge = new CPhysicRevoluteJoint();
		assert(m_pPhysicRevolute_Bridge);
		m_pPhysicRevolute_Bridge->SetInfo(Vect3f(1.f,0.f,0.f),Vect3f(10.f,5.2f,10.f),m_pPhysicActor_BoxBridge,NULL);
		m_pPhysicRevolute_Bridge->SetMotor(10000.f,1.0f,true,0.f,-1.57f);
		CProcess::m_bIsOk  = physicManager->AddPhysicRevoluteJoint(m_pPhysicRevolute_Bridge);
	}
	//-----------------------------------------------------------------
	// Añadimos un actor mesh de una habitación del quake (room3)
	//-----------------------------------------------------------------
	if (CProcess::m_bIsOk)
	{
		std::vector<uint32> faces;
		std::vector<Vect3f> vertices;

		CPhysicASELoader::ReadMeshFromASE("./Data/room3.ASE",vertices, faces);
		physicManager->GetCookingMesh()->CreatePhysicMesh(vertices, faces,"room3");
		NxTriangleMesh* mesh = physicManager->GetCookingMesh()->GetPhysicMesh("room3");

		m_pGameEntity_Mesh_2 = new CGameEntity();
		assert(m_pGameEntity_Mesh_2);
		m_pGameEntity_Mesh_2->SetName("Mesh_Room3");
		m_pGameEntity_Mesh_2->SetPaint(true);

		m_pPhysicActor_Mesh_2 = new CPhysicActor(m_pGameEntity_Mesh_2);
		assert(m_pPhysicActor_Mesh_2); 
		m_pPhysicActor_Mesh_2->AddMeshShape(mesh,v3fZERO,GROUP_BASIC_PRIMITIVES);
		m_pPhysicActor_Mesh_2->SetGlobalPosition(Vect3f(50.f,0.f,-20.f));
		CProcess::m_bIsOk = physicManager->AddPhysicActor(m_pPhysicActor_Mesh_2);
	}
	//-----------------------------------------------------------------
	// Añadimos dos Controller, un player y un enemigo
	//-----------------------------------------------------------------
	if (CProcess::m_bIsOk)
	{
		//Actor1 el player por ejemplo
		float slopeLimit_Capsule = 60.f;
		float skinWidth_Capsule = 0.01f;
		float stepOffset_Capsule = 0.6f;
		Vect3f pos(50.f,10.f,-20.f);

		m_pGameEntity_Controller1 = new CGameEntity();
		assert(m_pGameEntity_Controller1);
		m_pGameEntity_Controller1->SetName("Controller1 Player");
		m_pGameEntity_Controller1->SetPaint(true);
		m_pPhysicController1 = new CPhysicController(	0.5f,1.f,slopeLimit_Capsule,skinWidth_Capsule,stepOffset_Capsule,
																									COLLIDABLE_MASK,m_pGameEntity_Controller1,pos);

		m_pPhysicController1->SetYaw(0.f);
		m_pPhysicController1->SetPitch(0.f);

		CProcess::m_bIsOk  = physicManager->AddPhysicController(m_pPhysicController1);
	}

	if (CProcess::m_bIsOk)
	{
		m_pPlayerCamera = new CFPSCamera(1.f,500.f,mathUtils::Deg2Rad(60.f),aspect_ratio,m_pPhysicController1);
		CProcess::m_bIsOk  = (m_pPlayerCamera!=NULL);
	}

	if (CProcess::m_bIsOk)
	{
		//Actor2 el enemigo por ejemplo
		float slopeLimit_Capsule = 60.f;
		float skinWidth_Capsule = 0.01f;
		float stepOffset_Capsule = 0.6f;
		Vect3f pos(50.f,10.f,-25.f);

		m_pGameEntity_Controller2 = new CGameEntity();
		assert(m_pGameEntity_Controller2);
		m_pGameEntity_Controller2->SetName("Controller2 Enemy");
		m_pGameEntity_Controller2->SetPaint(true);
		m_pPhysicController2 = new CPhysicController(	0.5f,1.f,slopeLimit_Capsule,skinWidth_Capsule,stepOffset_Capsule,
																									COLLIDABLE_MASK,m_pGameEntity_Controller2,pos);

		CProcess::m_bIsOk  = physicManager->AddPhysicController(m_pPhysicController2);
	}
	//----------------------------------------------------------------------------------------
	
	//-----------------------------------------------------------------
	// Añadimos un actor trigger en forma de box
	//-----------------------------------------------------------------
	m_pGameEntity_Trigger = new CGameEntity();
	assert(m_pGameEntity_Trigger);
	m_pGameEntity_Trigger->SetName("trigger_test");
	m_pGameEntity_Trigger->SetPaint(true);
	m_pGameEntity_Trigger->SetColor(colBLUE);
	m_pPhysicActor_Trigger = new CPhysicActor(m_pGameEntity_Trigger);
	assert(m_pPhysicActor_Trigger);
	m_pPhysicActor_Trigger->CreateBoxTrigger(Vect3f(1.f,1.f,1.f),GROUP_BASIC_PRIMITIVES);
	m_pPhysicActor_Trigger->SetGlobalPosition(Vect3f(50.f,1.f,-28.f));
	CProcess::m_bIsOk = physicManager->AddPhysicActor(m_pPhysicActor_Trigger);

	physicManager->SetTriggerReport(this);
	

	if (!CProcess::m_bIsOk)
	{
		Release();
	}
	return CProcess::m_bIsOk;
}

void CPhysXTestProcess::Release ()
{
	CHECKED_DELETE(m_pObject3D);
	CHECKED_DELETE(m_pCamera);
	CHECKED_DELETE(m_pPlayerCamera);
	
	//---Delete physics objects-------
	CPhysicsManager* physicManager = CORE->GetPhysicManager();
	assert(physicManager);
	//-----------------Light1---------------
	if (m_pPhysicActor_BoxLight1)
	{
		physicManager->ReleasePhysicActor(m_pPhysicActor_BoxLight1);
		delete m_pPhysicActor_BoxLight1;
		m_pPhysicActor_BoxLight1 = NULL;
	}
	CHECKED_DELETE(m_pGameEntity_BoxLight1);
	
	if (m_pPhysicSphericalJoint_Light1)
	{
		physicManager->RelasePhysicSphericalJoint(m_pPhysicSphericalJoint_Light1);
		delete m_pPhysicSphericalJoint_Light1;
		m_pPhysicSphericalJoint_Light1 = NULL;
	}

	//---------------Light2-------------------------
	if (m_pPhysicActor_BoxLight20)
	{
		physicManager->ReleasePhysicActor(m_pPhysicActor_BoxLight20);
		delete m_pPhysicActor_BoxLight20;
		m_pPhysicActor_BoxLight20 = NULL;
	}
	if (m_pPhysicActor_BoxLight21)
	{
		physicManager->ReleasePhysicActor(m_pPhysicActor_BoxLight21);
		delete m_pPhysicActor_BoxLight21;
		m_pPhysicActor_BoxLight21 = NULL;
	}
	if (m_pPhysicActor_BoxLight22)
	{
		physicManager->ReleasePhysicActor(m_pPhysicActor_BoxLight22);
		delete m_pPhysicActor_BoxLight22;
		m_pPhysicActor_BoxLight22 = NULL;
	}
	CHECKED_DELETE(m_pGameEntity_BoxLight2);
	
	if (m_pPhysicSphericalJoint_Light20)
	{
		physicManager->RelasePhysicSphericalJoint(m_pPhysicSphericalJoint_Light20);
		delete m_pPhysicSphericalJoint_Light20;
		m_pPhysicSphericalJoint_Light20 = NULL;
	}
	if (m_pPhysicSphericalJoint_Light21)
	{
		physicManager->RelasePhysicSphericalJoint(m_pPhysicSphericalJoint_Light21);
		delete m_pPhysicSphericalJoint_Light21;
		m_pPhysicSphericalJoint_Light21 = NULL;
	}
	if (m_pPhysicSphericalJoint_Light22)
	{
		physicManager->RelasePhysicSphericalJoint(m_pPhysicSphericalJoint_Light22);
		delete m_pPhysicSphericalJoint_Light22;
		m_pPhysicSphericalJoint_Light22 = NULL;
	}


	//-------------------Primitive Box
	if (m_pPhysicActor_Box)
	{
		physicManager->ReleasePhysicActor(m_pPhysicActor_Box);
		delete m_pPhysicActor_Box;
		m_pPhysicActor_Box = NULL;
	}
	CHECKED_DELETE(m_pGameEntity_Box);
	
	if (m_pPhysicActor_Sphere)
	{
		physicManager->ReleasePhysicActor(m_pPhysicActor_Sphere);
		delete m_pPhysicActor_Sphere;
		m_pPhysicActor_Sphere = NULL;
	}
	CHECKED_DELETE(m_pGameEntity_Sphere);
	
	if (m_pPhysicActor_Plane)
	{
		physicManager->ReleasePhysicActor(m_pPhysicActor_Plane);
		delete m_pPhysicActor_Plane;
		m_pPhysicActor_Plane = NULL;
	}
	CHECKED_DELETE(m_pGameEntity_Plane);
	
	if (m_pPhysicActor_Capsule)
	{
		physicManager->ReleasePhysicActor(m_pPhysicActor_Capsule);
		delete m_pPhysicActor_Capsule;
		m_pPhysicActor_Capsule = NULL;
	}
	CHECKED_DELETE(m_pGameEntity_Capsule);
	
	if (m_pPhysicActor_Composite)
	{
		physicManager->ReleasePhysicActor(m_pPhysicActor_Composite);
		delete m_pPhysicActor_Composite;
		m_pPhysicActor_Composite = NULL;
	}
	CHECKED_DELETE(m_pGameEntity_Composite);
	
	if (m_pPhysicActor_Mesh)
	{
		physicManager->ReleasePhysicActor(m_pPhysicActor_Mesh);
		delete m_pPhysicActor_Mesh;
		m_pPhysicActor_Mesh = NULL;
	}
	CHECKED_DELETE(m_pGameEntity_Mesh);
	
	if (m_pPhysicActor_Mesh_2)
	{
		physicManager->ReleasePhysicActor(m_pPhysicActor_Mesh_2);
		delete m_pPhysicActor_Mesh_2;
		m_pPhysicActor_Mesh_2 = NULL;
	}
	CHECKED_DELETE(m_pGameEntity_Mesh_2);
	
	if (m_vPhysicActor_Grenades.size())
	{
		std::vector< std::pair<CPhysicActor*,CGameEntity*> >::iterator it(m_vPhysicActor_Grenades.begin());
		std::vector< std::pair<CPhysicActor*,CGameEntity*> >::iterator itEnd(m_vPhysicActor_Grenades.end());
		while (it!=itEnd)
		{
			CPhysicActor* actor = it->first;
			if (actor)
			{
				physicManager->ReleasePhysicActor(actor);
				delete actor;
				actor = NULL;
			}
			CGameEntity* entity = it->second;
			CHECKED_DELETE(entity);
			it++;
		}
		m_vPhysicActor_Grenades.clear();
	}

	if (m_vPhysicActor_CCDGrenades.size())
	{
		std::vector< std::pair<CPhysicActor*,CGameEntity*> >::iterator it(m_vPhysicActor_CCDGrenades.begin());
		std::vector< std::pair<CPhysicActor*,CGameEntity*> >::iterator itEnd(m_vPhysicActor_CCDGrenades.end());
		while (it!=itEnd)
		{
			CPhysicActor* actor = it->first;
			if (actor)
			{
				physicManager->ReleasePhysicActor(actor);
				delete actor;
				actor = NULL;
			}
			CGameEntity* entity = it->second;
			CHECKED_DELETE(entity);
			it++;
		}
		m_vPhysicActor_CCDGrenades.clear();
	}
	CHECKED_DELETE(m_pGameEntity_BoxBridge);
	if (m_pPhysicRevolute_Bridge)
	{
		physicManager->RelasePhysicRevoluteJoint(m_pPhysicRevolute_Bridge);
		delete m_pPhysicRevolute_Bridge;
		m_pPhysicRevolute_Bridge = NULL;
	}
	if (m_pPhysicActor_BoxBridge)
	{
		physicManager->ReleasePhysicActor(m_pPhysicActor_BoxBridge);
		delete m_pPhysicActor_BoxBridge;
		m_pPhysicActor_BoxBridge = NULL;
	}
	if (m_pPhysicActor_BoxPath)
	{
		physicManager->ReleasePhysicActor(m_pPhysicActor_BoxPath);
		delete m_pPhysicActor_BoxPath;
		m_pPhysicActor_BoxPath = NULL;
	}
	CHECKED_DELETE(m_pGameEntity_Controller1);
	if (m_pPhysicController1)
	{
		physicManager->ReleasePhysicController(m_pPhysicController1);
		delete m_pPhysicController1;
		m_pPhysicController1 = NULL;
	}
	CHECKED_DELETE(m_pGameEntity_Controller2);
	if (m_pPhysicController2)
	{
		physicManager->ReleasePhysicController(m_pPhysicController2);
		delete m_pPhysicController2;
		m_pPhysicController2 = NULL;
	}
		
	if (m_pPhysicActor_Trigger)
	{
		physicManager->ReleasePhysicActor(m_pPhysicActor_Trigger);
		delete m_pPhysicActor_Trigger;
		m_pPhysicActor_Trigger = NULL;
	}
	CHECKED_DELETE(m_pGameEntity_Trigger);
	//--------------------------------

}

void CPhysXTestProcess::RenderScene (CRenderManager* renderManager, CFontManager* fm)
{
	renderManager->DrawAxis(20.f);
	//renderManager->DrawGrid(50.f,colWHITE,20,20);
	renderManager->DrawCamera(m_pCamera);

	//Draw Light1:
	Vect3f posa = m_pPhysicActor_BoxLight1->GetPosition();
	Vect3f posb(-20.f,10.f,-20.f);
	renderManager->DrawLine(posa,posb);

	//Draw Light2:
	posa = m_pPhysicActor_BoxLight22->GetPosition();
	posb = Vect3f(-30.f,20.f,-30.f);
	renderManager->DrawLine(posa,posb);
	posa = m_pPhysicActor_BoxLight22->GetPosition();
	posb = m_pPhysicActor_BoxLight21->GetPosition();
	renderManager->DrawLine(posa,posb);
	posa = m_pPhysicActor_BoxLight21->GetPosition();
	posb = m_pPhysicActor_BoxLight20->GetPosition();
	renderManager->DrawLine(posa,posb);
}

uint32 CPhysXTestProcess::RenderDebugInfo(CRenderManager* renderManager, CFontManager* fm, float fps)
{
	CCore * core = CCore::GetSingletonPtr();
	CInputManager*inputManager = core->GetInputManager();
	Vect2i posMouse;
	inputManager->GetPosition(IDV_MOUSE,posMouse);

	uint32 posY = 0;
	posY = CProcess::RenderDebugInfo(renderManager, fm,  fps);
	if (m_bRenderDebugInfo)
	{
		uint32 posX = m_PosRenderDebugInfo.x;
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"____________Controlador de la cámara esferica___________" );
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- Presionado botón izquierdo->la cámara gira" );
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- Presionado botón derecho->la cámara se desplaza" );
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- Con la rueda del botón-> zoomIn/zoomOut" );
		fm->DrawDefaultText(posX,posY,colWHITE,"- Con la rueda del botón+control-> zoomIn/zoomOut suave" );
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"_____________________________________________________" );
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- Colision con objeto fisico:%s", m_sNameImpactObject.c_str());
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- Posicion del mouse: (x,y)=(%d,%d)",posMouse.x,posMouse.y);
		posY+=fm->DrawDefaultText(posX,posY,colWHITE,"- Entidad %s ha entrado en el trigger %s",m_sNameTrigger2.c_str(),m_sNameTrigger1.c_str());
	}
	return posY;
}

void CPhysXTestProcess::Update (float elapsedTime)
{
	CProcess::Update(elapsedTime);

	m_pPhysicController1->Move(Vect3f(0.f,0.f,0.f),elapsedTime);
	m_pPhysicController2->Move(Vect3f(0.f,0.f,0.f),elapsedTime);

	CCore * core = CCore::GetSingletonPtr();
	CInputManager*inputManager = core->GetInputManager();
	UpdateInputActions(inputManager, elapsedTime);
}


void CPhysXTestProcess::UpdateInputActions	(CInputManager* inputManager, float elapsedTime)
{


	//-----Input para cambiar de camara esferica a camara Player(fps)
	if( inputManager->IsUpDown(IDV_KEYBOARD,DIK_C) )
	{
		if (m_pCurrentCamera == m_pCamera)
		{
			m_pCurrentCamera = m_pPlayerCamera;
		}
		else
		{
			m_pCurrentCamera = m_pCamera;
		}
	}

	//-----Input para manejar el motor del puente
	if( inputManager->IsDown(IDV_KEYBOARD,DIK_M) )
	{
		float velTarget=-1.f;
		m_pPhysicRevolute_Bridge->ActiveMotor(velTarget);
	}
	if( inputManager->IsDown(IDV_KEYBOARD,DIK_N) )
	{
		float velTarget=1.f;
		m_pPhysicRevolute_Bridge->ActiveMotor(velTarget);
	}
	
	//---Lanzamos un rayo en la dirección del puntero del mouse para saber con que objeto físico ha impactado---
	CGameEntity* entity = NULL;
	if (inputManager->IsDown(IDV_MOUSE, MOUSE_BUTTON_RIGHT) && inputManager->IsDown(IDV_KEYBOARD, DIK_LCONTROL) )
	{
		CPhysicsManager* physicManager = CORE->GetPhysicManager();
		Vect2i mousePos;
		inputManager->GetPosition(IDV_MOUSE,mousePos);
		Vect3f posRay, dirRay;
		CORE->GetRenderManager()->GetRay(mousePos,posRay, dirRay);
		dirRay.Normalize();
		
		SCollisionInfo colInfo;
		uint32 impactMask = 0;
		CPhysicUserData* userData = NULL;
		uint32 mask = IMPACT_MASK_2;
		userData = physicManager->RaycastClosestActor(posRay,dirRay,mask,userData,colInfo);
		if (userData!=NULL)
		{
			//Ha habido colision con algun objeto fisico de la escena.
			entity = (CGameEntity*) userData;
			m_sNameImpactObject = entity->GetName();
		}
		else
		{
			m_sNameImpactObject = "None";
		}
	}

	//---Lanzamos un rayo en la dirección de la camara esferica para saber con que objeto físico ha impactado---
	entity = NULL;
	if (inputManager->IsDown(IDV_MOUSE, MOUSE_BUTTON_RIGHT) && !inputManager->IsDown(IDV_KEYBOARD,DIK_LCONTROL))
	{
		CPhysicsManager* physicManager = CORE->GetPhysicManager();
		Vect3f posRay = m_pCurrentCamera->GetEye();
		Vect3f dirRay = m_pCurrentCamera->GetDirection();
		dirRay.Normalize();
		if (m_pCurrentCamera == m_pPlayerCamera)
		{
			//Lanzamos el rayo un poco lejos para que no colisione con el mismo player
			posRay += dirRay*2;
		}
		SCollisionInfo colInfo;
		uint32 impactMask = 0;
		CPhysicUserData* userData = NULL;
		uint32 mask = IMPACT_MASK_2;
		if (inputManager->IsDown(IDV_KEYBOARD, KEY_LCTRL))
		{
			mask = IMPACT_MASK_1;	
		}
		userData = physicManager->RaycastClosestActor(posRay,dirRay,mask,userData,colInfo);
		if (userData!=NULL)
		{
			//Ha habido colision con algun objeto fisico de la escena.
			entity = (CGameEntity*) userData;
			m_sNameImpactObject = entity->GetName();
		}
		else
		{
			m_sNameImpactObject = "None";
		}
	}

	if (entity!=NULL)
	{
		if (entity!=m_pLastPickedObject)
		{
			if (m_pLastPickedObject)
				m_pLastPickedObject->SetColor(colWHITE);
			entity->SetColor(colYELLOW);
			m_pLastPickedObject = entity;
		}
	}
	

	//---Lanzamos esferas físicas sin CCD---
	if (inputManager->IsUpDown(IDV_KEYBOARD, KEY_T))
	{
		CPhysicsManager* physicManager = CORE->GetPhysicManager();
		CGameEntity* pGameEntity_Grenade = new CGameEntity();
		assert(pGameEntity_Grenade);
		pGameEntity_Grenade->SetName("Grenade");
		pGameEntity_Grenade->SetPaint(true);
		CPhysicActor* pPhysicActor_Grenade = new CPhysicActor(pGameEntity_Grenade);
		assert(pPhysicActor_Grenade); 
		pPhysicActor_Grenade->AddSphereShape(0.5f,v3fZERO,0,GROUP_GRENADES);
		pPhysicActor_Grenade->SetGlobalPosition(m_pCurrentCamera->GetEye());
		pPhysicActor_Grenade->CreateBody(0.5f);
		physicManager->AddPhysicActor(pPhysicActor_Grenade);
		float velocity = 100.f;
		if (inputManager->IsDown(IDV_KEYBOARD, KEY_LCTRL))
		{
			velocity = 200.f;
		}
		Vect3f direction = m_pCurrentCamera->GetDirection();
		direction.Normalize();
		pPhysicActor_Grenade->SetLinearVelocity(direction*velocity);
		m_vPhysicActor_Grenades.push_back(std::pair<CPhysicActor*,CGameEntity*>(pPhysicActor_Grenade, pGameEntity_Grenade));
	}

	//---Lanzamos esfera físicas con CCD
	if (inputManager->IsUpDown(IDV_KEYBOARD, KEY_Y))
	{
		CPhysicsManager* physicManager = CORE->GetPhysicManager();
		CGameEntity* pGameEntity_CDDGrenade = new CGameEntity();
		assert(pGameEntity_CDDGrenade);
		pGameEntity_CDDGrenade->SetName("GrenadeCCD");
		pGameEntity_CDDGrenade->SetPaint(true);
		CPhysicActor* pPhysicActor_CCDGrenade = new CPhysicActor(pGameEntity_CDDGrenade);
		assert(pPhysicActor_CCDGrenade); 
		pPhysicActor_CCDGrenade->AddSphereShape(0.5f,v3fZERO,physicManager->CreateCCDSkeleton(20.0f), GROUP_GRENADES);
		pPhysicActor_CCDGrenade->SetGlobalPosition(m_pCurrentCamera->GetEye());
		pPhysicActor_CCDGrenade->CreateBody(2.5f);
		physicManager->AddPhysicActor(pPhysicActor_CCDGrenade);
		float velocity = 400.f;
		Vect3f direction = m_pCurrentCamera->GetDirection();
		direction.Normalize();
		pPhysicActor_CCDGrenade->SetLinearVelocity(direction*velocity);
		m_vPhysicActor_CCDGrenades.push_back(std::pair<CPhysicActor*,CGameEntity*>(pPhysicActor_CCDGrenade, pGameEntity_CDDGrenade));
	}

	if (m_pCurrentCamera == m_pCamera)
	{
		UpdateMaxCamera(inputManager, elapsedTime);
	}
	else
	{
		UpdateInputPlayer(inputManager, elapsedTime);
	}

}

void CPhysXTestProcess::UpdateMaxCamera (CInputManager* inputManager, float elapsedTime)
{
	//----------------------------------------------------------
	//-----ACTUALIZAMOS LA CAMARA ESFERICA AL ESTILO DEL MAX----
	Vect3i deltaMouse;
	deltaMouse = inputManager->GetMouseDelta();
	if (inputManager->IsDown(IDV_MOUSE, MOUSE_BUTTON_LEFT) )
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


void CPhysXTestProcess::UpdateInputPlayer (CInputManager* inputManager, float elapsedTime)
{
	Vect3i deltaMouse;
	deltaMouse = inputManager->GetMouseDelta();
	if (deltaMouse.x!=0.f)
	{
		float yaw = m_pPhysicController1->GetYaw();
		yaw -= deltaMouse.x * 0.01f;
		m_pPhysicController1->SetYaw(yaw);
	}
	if (deltaMouse.y!=0.f)
	{
		float pitch = m_pPhysicController1->GetPitch();
		pitch -= deltaMouse.y * 0.01f;
		m_pPhysicController1->SetPitch(pitch);
	}
	
	float forward = 0.f;
	if (inputManager->IsDown(IDV_KEYBOARD, KEY_W) )
	{
		forward++;
	}
	if (inputManager->IsDown(IDV_KEYBOARD, KEY_S) )
	{
		forward--;
	}
	float yaw = m_pPhysicController1->GetYaw();
	Vect3f direction_forward;
	direction_forward = Vect3f(cos(yaw),0.f,sin(yaw));
	direction_forward *= forward;

	

	float strafe = 0.f;
	if (inputManager->IsDown(IDV_KEYBOARD, KEY_A) )
	{
		strafe++;
	}
	if (inputManager->IsDown(IDV_KEYBOARD, KEY_D) )
	{
		strafe--;
	}
	Vect3f direction_strafe;
	direction_strafe = Vect3f(cos(yaw+ePI2f),0.f,sin(yaw+ePI2f));
	direction_strafe *= strafe;
	Vect3f direction = direction_forward+direction_strafe;
	direction *= 0.1f;
	
	m_pPhysicController1->Move(direction,elapsedTime);

	if (inputManager->IsDown(IDV_KEYBOARD, KEY_SPACE) )
	{
		m_pPhysicController1->Jump(100.f);
	}


	if (inputManager->IsDown(IDV_KEYBOARD, KEY_8) )
	{
		m_pPhysicController2->GetPhXController()->setCollision(false);
		m_pPhysicController1->GetPhXController()->setCollision(false);
	}
	if (inputManager->IsDown(IDV_KEYBOARD, KEY_9) )
	{
		m_pPhysicController2->GetPhXController()->setCollision(true);
	}
}


void CPhysXTestProcess::OnEnter (CPhysicUserData* trigger1, CPhysicUserData* trigger2)
{
	CGameEntity * entity = (CGameEntity*) trigger1;
	assert(entity);
	m_sNameTrigger1 = entity->GetName();

	entity = (CGameEntity*) trigger2;
	assert(entity);
	m_sNameTrigger2 = entity->GetName();
}

void CPhysXTestProcess::OnLeave (CPhysicUserData* trigger1, CPhysicUserData* trigger2)
{
	//Nothing...
}


//-----------------ScriptManager------------------------------
void CPhysXTestProcess::RegisterFunctions (CScriptManager* scriptManager)
{
	lua_State* l_pLUAState = scriptManager->GetLuaState();
	using namespace luabind;
	
	// ahora registramos lo que querramos
	module(l_pLUAState)
    [
		def("getPhysXTest", GetPhysXTest),
		
		// registramos la clase CPhysXTestProcess
		class_<CPhysXTestProcess>(CScriptRegister::SetClassName("CPhysXTestProcess"))
		
		
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