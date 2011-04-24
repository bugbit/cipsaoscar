#include "__PCH_Laberynth.h"

#include "LaberynthBoard.h"

//---Engine Includes-------
#include "Base/Math/Matrix44.h"
#include "Graphics/RenderManager.h"
#include "Graphics/FontManager.h"
#include "Graphics/ASEObject/ASETextureManager.h"
#include "Core/Core.h"
#include "PhysX/PhysicActor.h"
#include "PhysX/PhysicsManager.h"
#include "PhysX/PhysicASELoader.h"
#include "PhysX/PhysicCookingMesh.h"
#include "PhysX/PhysicSphericalJoint.h"
#include "PhysX/PhysicRevoluteJoint.h"
#include "PhysX/PhysicFixedJoint.h"
#undef min
#undef max
#include "NxPhysics.h"
//---------------------------------


CLaberynthBoard::CLaberynthBoard(	CASEObject* baseBoardMesh, CASEObject* ballMesh, float ballRadius,
																	CASEObject* boardMesh, const std::string& physXMeshName, 
																	const Vect3f& goalPosition, const Vect3f& startPosition, bool inSettings)
:	m_pPhysicActor_Plane(NULL)
,	m_pGameEntity_Plane(NULL)
,	m_pPhysicActor_Board(NULL)
,	m_pGameEntity_Board(NULL)
,	m_pPhysicActor_Ball(NULL)
,	m_pGameEntity_Ball(NULL)
,	m_pPhysicActor_Box(NULL)
, m_pGameEntity_Box(NULL)
,	m_pPhysicRevolute_1(NULL)
,	m_pPhysicRevolute_2(NULL)
,	m_pPhysicFixedJoint(NULL)
,	m_pPhysicActor_GoalBox(NULL)
,	m_pGameEntity_GoalBox(NULL)
,	m_pPhysicActor_GoalTrigger(NULL)
,	m_pGameEntity_GoalTrigger(NULL)
,	m_pPhysicActor_FlatTrigger(NULL)
,	m_pGameEntity_FlatTrigger(NULL)
,	m_pPhysicActor_BaseBoard(NULL)
,	m_pGameEntity_BaseBoard(NULL)
, m_pASE_Board(NULL)
, m_pASE_Ball(NULL)
, m_pASE_BaseBoard(NULL)

{
	m_pASE_Board			= boardMesh;
	m_pASE_Ball				= ballMesh;
	m_pASE_BaseBoard	= baseBoardMesh;
	
	if (inSettings)
	{
		m_fBoardHeight = Y_SETTINGS;
	}
	else
	{
		m_fBoardHeight = 40.f;
	}
	

	CPhysicsManager* physicManager = CORE->GetPhysicManager();
	assert(physicManager);

	//-----------------------------------------------------------------
	// Añadimos un actor con solo una BOX como shape para la CAJA de ROTACIÓN
	//-----------------------------------------------------------------
	m_pGameEntity_Box = new CGameEntity();
	assert(m_pGameEntity_Box);
	m_pGameEntity_Box->SetName("Box");
	m_pGameEntity_Box->SetPaint(true);
	m_pPhysicActor_Box = new CPhysicActor(m_pGameEntity_Box);
	assert(m_pPhysicActor_Box);
	m_pPhysicActor_Box->AddBoxSphape(Vect3f(4.f,4.f,4.f),v3fZERO,0,GROUP_BASIC_PRIMITIVES);
	m_pPhysicActor_Box->SetGlobalPosition(Vect3f(0.f,m_fBoardHeight-5.f,0.f));
	m_pPhysicActor_Box->CreateBody(1000.f);
	physicManager->AddPhysicActor(m_pPhysicActor_Box);

	m_GameEntities.push_back(m_pGameEntity_Box);
	m_PhyscActors.push_back(m_pPhysicActor_Box);

	//-----------------------------------------------------------------
	// Añadimos un actor con solo una SHPERE como shape para la PELOTA
	//-----------------------------------------------------------------
	m_pGameEntity_Ball = new CGameEntity();
	assert(m_pGameEntity_Ball);
	m_pGameEntity_Ball->SetName("Ball");
	m_pGameEntity_Ball->SetPaint(true);
	m_pPhysicActor_Ball = new CPhysicActor(m_pGameEntity_Ball);
	assert(m_pPhysicActor_Ball); 
	m_pPhysicActor_Ball->AddSphereShape(ballRadius, v3fZERO, 0, GROUP_BASIC_PRIMITIVES);
	m_pPhysicActor_Ball->SetGlobalPosition(Vect3f(startPosition.x, m_fBoardHeight + 10.f, startPosition.z));
	m_pPhysicActor_Ball->CreateBody(0.1f);
	physicManager->AddPhysicActor(m_pPhysicActor_Ball);

	m_GameEntities.push_back(m_pGameEntity_Ball);
	m_PhyscActors.push_back(m_pPhysicActor_Ball);


	//-----------------------------------------------------------------
	// Añadimos un actor con más de una MESH como shape, el tablero--> DEPENDE DEL LEVEL.
	//-----------------------------------------------------------------
	NxTriangleMesh* mesh = physicManager->GetCookingMesh()->GetPhysicMesh(physXMeshName);

	m_pGameEntity_Board = new CGameEntity();
	assert(m_pGameEntity_Board);
	m_pGameEntity_Board->SetName("Board");
	m_pGameEntity_Board->SetPaint(true);

	m_pPhysicActor_Board = new CPhysicActor(m_pGameEntity_Board);
	assert(m_pPhysicActor_Board); 
	m_pPhysicActor_Board->AddMeshShape(mesh,v3fZERO,GROUP_BASIC_PRIMITIVES);
	m_pPhysicActor_Board->SetGlobalPosition(Vect3f(0.f,m_fBoardHeight,0.f));
	m_pPhysicActor_Board->CreateBody(0.01f);
	physicManager->AddPhysicActor(m_pPhysicActor_Board);
	
	m_GameEntities.push_back(m_pGameEntity_Board);
	m_PhyscActors.push_back(m_pPhysicActor_Board);

	//-----------------------------------------------------------------
	// Añadimos un actor con más de una MESH como shape, la base del tablero
	//-----------------------------------------------------------------
	mesh = physicManager->GetCookingMesh()->GetPhysicMesh("BoardBase");

	m_pGameEntity_BaseBoard = new CGameEntity();
	assert(m_pGameEntity_BaseBoard);
	m_pGameEntity_BaseBoard->SetName("BaseBoard");
	m_pGameEntity_BaseBoard->SetPaint(true);

	m_pPhysicActor_BaseBoard = new CPhysicActor(m_pGameEntity_BaseBoard);
	assert(m_pPhysicActor_BaseBoard); 
	m_pPhysicActor_BaseBoard->AddMeshShape(mesh,v3fZERO,GROUP_BASIC_PRIMITIVES);
	m_pPhysicActor_BaseBoard->SetGlobalPosition(Vect3f(0.f,m_fBoardHeight - 40.f,0.f));
	//m_pPhysicActor_BaseBoard->CreateBody(100.f); //No hace falta crear body, es estatico
	physicManager->AddPhysicActor(m_pPhysicActor_BaseBoard);

	m_GameEntities.push_back(m_pGameEntity_BaseBoard);
	m_PhyscActors.push_back(m_pPhysicActor_BaseBoard);


	//-----------------------------------------------------------------
	// Añadimos un actor BOX y un joint tipo CPhysicRevoluteJoint
	//-----------------------------------------------------------------
	m_pPhysicRevolute_1 = new CPhysicRevoluteJoint();
	assert(m_pPhysicRevolute_1);
	m_pPhysicRevolute_1->SetInfo(Vect3f(1.f,0.f,0.f),Vect3f(0.f,m_fBoardHeight,0.f),m_pPhysicActor_Board,m_pPhysicActor_Box);
	m_pPhysicRevolute_1->SetMotor(10000.f,5.0f,true,0.05f*FLOAT_PI_VALUE,-0.05f*FLOAT_PI_VALUE);
	physicManager->AddPhysicRevoluteJoint(m_pPhysicRevolute_1);

	m_pPhysicRevolute_2 = new CPhysicRevoluteJoint();
	assert(m_pPhysicRevolute_2);
	m_pPhysicRevolute_2->SetInfo(Vect3f(0.f,0.f,1.f),Vect3f(0.f,m_fBoardHeight-5.f,0.f),m_pPhysicActor_Box,NULL);
	m_pPhysicRevolute_2->SetMotor(100000.f,10000.0f,true,0.05f*FLOAT_PI_VALUE,-0.05f*FLOAT_PI_VALUE);
	physicManager->AddPhysicRevoluteJoint(m_pPhysicRevolute_2);

	
	if (!inSettings)
	{
		//-----------------------------------------------------------------
		// Añadimos un trigger para saber cuando cae la pelota al suelo y ha de reiniciarse la partida
		//-----------------------------------------------------------------
		m_pGameEntity_FlatTrigger = new CGameEntity();
		assert(m_pGameEntity_FlatTrigger);
		m_pGameEntity_FlatTrigger->SetName("trigger_Flat");
		m_pGameEntity_FlatTrigger->SetPaint(true);
		m_pGameEntity_FlatTrigger->SetColor(colBLUE);
		m_pPhysicActor_FlatTrigger = new CPhysicActor(m_pGameEntity_FlatTrigger);
		assert(m_pPhysicActor_FlatTrigger);
		m_pPhysicActor_FlatTrigger->CreateBoxTrigger(Vect3f(70.f,22.f,70.f),GROUP_BASIC_PRIMITIVES);
		m_pPhysicActor_FlatTrigger->SetGlobalPosition(Vect3f(0.f,1.f,0.f));
		physicManager->AddPhysicActor(m_pPhysicActor_FlatTrigger);

		m_GameEntities.push_back(m_pGameEntity_FlatTrigger);
		m_PhyscActors.push_back(m_pPhysicActor_FlatTrigger);

		//-----------------------------------------------------------------
		// Añadimos los triggers para saber cuando cae la pelota en la meta y ha de reiniciarse la partida
		//-----------------------------------------------------------------
		m_pGameEntity_GoalTrigger = new CGameEntity();
		assert(m_pGameEntity_GoalTrigger);
		m_pGameEntity_GoalTrigger->SetName("trigger_Goal");
		m_pGameEntity_GoalTrigger->SetPaint(true);
		m_pGameEntity_GoalTrigger->SetColor(colBLUE);
		m_pPhysicActor_GoalTrigger = new CPhysicActor(m_pGameEntity_GoalTrigger);
		assert(m_pPhysicActor_GoalTrigger);
		m_pPhysicActor_GoalTrigger->CreateBoxTrigger(Vect3f(4.f,4.f,4.f),GROUP_BASIC_PRIMITIVES);
		m_pPhysicActor_GoalTrigger->SetGlobalPosition(Vect3f(0.f,1.f,0.f));
		physicManager->AddPhysicActor(m_pPhysicActor_GoalTrigger);

		m_GameEntities.push_back(m_pGameEntity_GoalTrigger);
		m_PhyscActors.push_back(m_pPhysicActor_GoalTrigger);

		//-----------------------------------------------------------------
		// Añadimos una box muy pequeña situada en la meta, la cual estara atada al biard mediante un fixed joint.
		// Esta box se utilizara para poner el trigger goal en su posicion (ya que no podemos atar directamente un trigger a un objeto dinamico)
		//-----------------------------------------------------------------
		m_pGameEntity_GoalBox = new CGameEntity();
		assert(m_pGameEntity_GoalBox);
		m_pGameEntity_GoalBox->SetName("GoalBox");
		m_pGameEntity_GoalBox->SetPaint(true);
		m_pPhysicActor_GoalBox = new CPhysicActor(m_pGameEntity_GoalBox);
		assert(m_pPhysicActor_GoalBox);
		m_pPhysicActor_GoalBox->AddBoxSphape(Vect3f(0.1f,0.1f,0.1f),v3fZERO,0,GROUP_BASIC_PRIMITIVES);
		m_pPhysicActor_GoalBox->SetGlobalPosition(Vect3f(goalPosition.x,m_fBoardHeight+5.f,goalPosition.z));
		m_pPhysicActor_GoalBox->CreateBody(0.001f);
		physicManager->AddPhysicActor(m_pPhysicActor_GoalBox);

		m_GameEntities.push_back(m_pGameEntity_GoalBox);
		m_PhyscActors.push_back(m_pPhysicActor_GoalBox);

		m_pPhysicFixedJoint = new CPhysicFixedJoint();
		assert(m_pPhysicFixedJoint);
		m_pPhysicFixedJoint->SetInfo(m_pPhysicActor_Board, m_pPhysicActor_GoalBox);
		physicManager->AddPhysicFixedJoint(m_pPhysicFixedJoint);
	}
}

CLaberynthBoard::~CLaberynthBoard()
{
	//---Delete physics objects-------
	CPhysicsManager* physicManager = CORE->GetPhysicManager();
	assert(physicManager);

	//---Delete physics Objects:
	std::vector<CPhysicActor*>::iterator it_PhysicActor			= m_PhyscActors.begin();
	std::vector<CPhysicActor*>::iterator itEnd_PhysicActor	= m_PhyscActors.end();
	for(; it_PhysicActor != itEnd_PhysicActor; ++it_PhysicActor)
	{
		CPhysicActor* physicActor = *it_PhysicActor;
		physicManager->ReleasePhysicActor(physicActor);
		delete physicActor;
		physicActor = NULL;
	}
	m_PhyscActors.clear();

	//---Delete GameEntitites  Objects:
	std::vector<CGameEntity*>::iterator it_GameEntity			= m_GameEntities.begin();
	std::vector<CGameEntity*>::iterator itEnd_GameEntity	= m_GameEntities.end();
	for(; it_GameEntity != itEnd_GameEntity; ++it_GameEntity)
	{
		CGameEntity* gameEntity = *it_GameEntity;
		CHECKED_DELETE(gameEntity);
	}
	m_GameEntities.clear();


	//-----------------Revolute Joints---------------
	if (m_pPhysicRevolute_1)
	{
		physicManager->RelasePhysicRevoluteJoint(m_pPhysicRevolute_1);
		delete m_pPhysicRevolute_1;
		m_pPhysicRevolute_1 = NULL;
	}
	if (m_pPhysicRevolute_2)
	{
		physicManager->RelasePhysicRevoluteJoint(m_pPhysicRevolute_2);
		delete m_pPhysicRevolute_2;
		m_pPhysicRevolute_2 = NULL;
	}

	//-----------------Fixed Joint---------------
	if (m_pPhysicFixedJoint)
	{
		physicManager->RelasePhysicFixedJoint(m_pPhysicFixedJoint);
		delete m_pPhysicFixedJoint;
		m_pPhysicFixedJoint = NULL;
	}
}

void	CLaberynthBoard::UpdateScene	(float elapsedTime)
{
	//Update GoalTrigger position
	if (m_pPhysicActor_GoalTrigger)
	{
		Mat44f l_matrix;
		m_pPhysicActor_GoalBox->GetMat44(l_matrix);
		m_pPhysicActor_GoalTrigger->SetMat44(l_matrix);
	}
}

void	CLaberynthBoard::RenderScene	(CRenderManager* renderManager, CFontManager* fontManager)
{
	//---Render BoardLevel:
	Mat44f l_Matrix;
	m_pPhysicActor_Board->GetMat44(l_Matrix);
	renderManager->SetTransform(l_Matrix);
	m_pASE_Board->Render(renderManager);

	//---Render base Board:
	m_pPhysicActor_BaseBoard->GetMat44(l_Matrix);
	renderManager->SetTransform(l_Matrix);
	m_pASE_BaseBoard->Render(renderManager);

	//---Render Ball:
	m_pPhysicActor_Ball->GetMat44(l_Matrix);
	renderManager->SetTransform(l_Matrix);
	m_pASE_Ball->Render(renderManager);	
}