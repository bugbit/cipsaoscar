#include "__PCH_PhysX.h"

#include "Assert.h"
#include "PhysicsManager.h"
#include "Logger/Logger.h"
#include "Script/ScriptManager.h"
#include "Base/Math/Matrix44.h"

#include "luabind/luabind.hpp"

////----PhysX Includes-------------
#undef min
#undef max
#include "NxPhysics.h"
#include "NxControllerManager.h"
#include "NxCapsuleController.h"
////--------------------------------

#include "PhysicUserAllocator.h"
#include "PhysicCookingMesh.h"
#include "PhysicController.h"
#include "PhysicActor.h"
#include "PhysicSphericalJoint.h"
#include "PhysicRevoluteJoint.h"
#include "PhysicTriggerReport.h"
#include "PhysicFixedJoint.h"
#include "Graphics/RenderManager.h"
#include "Exceptions/Exception.h"

//----------------------------------------------------------------------------
// Init data
//----------------------------------------------------------------------------
bool CPhysicsManager::Init ()
{
	m_pMyAllocator = new CPhysicUserAllocator;
	assert(m_pMyAllocator);
	m_bIsOk = (m_pMyAllocator != NULL);
	if (m_bIsOk)
	{
		LOGGER->AddNewLog(ELL_INFORMATION, "PhysicsManager:: Inicializando la libreria PhysX");
		// Initialize PhysicsSDK
		NxPhysicsSDKDesc desc;
		NxSDKCreateError errorCode = NXCE_NO_ERROR;
		m_pPhysicsSDK = NxCreatePhysicsSDK(NX_PHYSICS_SDK_VERSION, m_pMyAllocator, NULL, desc, &errorCode);
		m_bIsOk = (m_pPhysicsSDK != NULL);
		if (m_bIsOk)
		{
			LOGGER->AddNewLog(ELL_INFORMATION, "PhysicsManager:: Creado el PhysXSDK");
			LOGGER->AddNewLog(ELL_INFORMATION, "PhysicsManager:: -------PhsX Settings---");
			LOGGER->AddNewLog(ELL_INFORMATION, "PhysicsManager:: El valor del SkinWidth es: %f", 
				m_InitParams.m_fSkinWidth);

			m_pPhysicsSDK->setParameter(NX_SKIN_WIDTH, m_InitParams.m_fSkinWidth);
			// Create a scene
			LOGGER->AddNewLog(ELL_INFORMATION, "PhysicsManager:: El valor de la gravedad es: %f",
				m_InitParams.m_fGravity);
			NxSceneDesc sceneDesc;
			sceneDesc.gravity = NxVec3(0.0f, m_InitParams.m_fGravity, 0.0f);
			m_pScene = m_pPhysicsSDK->createScene(sceneDesc);
			m_bIsOk = (m_pScene != NULL);
			if (m_bIsOk)
			{
				LOGGER->AddNewLog(ELL_INFORMATION, "PhysicsManager:: Solo hay un material, con los siguientes params");
				LOGGER->AddNewLog(ELL_INFORMATION, "PhysicsManager:: DefaultMaterial->Restitution %f:",
					m_InitParams.m_Restitution_DefMat);
				LOGGER->AddNewLog(ELL_INFORMATION, "PhysicsManager:: DefaultMaterial->StaticFriction %f:",
					m_InitParams.m_StaticFriction_DefMat);
				LOGGER->AddNewLog(ELL_INFORMATION, "PhysicsManager:: DefaultMaterial->DynamicFriction %f:",
					m_InitParams.m_DynamicFriction_DefMat);
				LOGGER->AddNewLog(ELL_INFORMATION, "PhysicsManager:: ----END PhsX Settings---");
				// Set default material
				NxMaterial* defaultMaterial = m_pScene->getMaterialFromIndex(0);
				defaultMaterial->setRestitution(m_InitParams.m_Restitution_DefMat);
				defaultMaterial->setStaticFriction(m_InitParams.m_StaticFriction_DefMat);
				defaultMaterial->setDynamicFriction(m_InitParams.m_DynamicFriction_DefMat);

				// Create a controllerManager
				m_pControllerManager = NxCreateControllerManager(m_pMyAllocator);
				m_bIsOk = (m_pControllerManager != NULL);
				if (m_bIsOk)
				{
					LOGGER->AddNewLog(ELL_INFORMATION, "PhysicsManager:: Creado el controlador de caracteres");
					m_pCookingMesh = new CPhysicCookingMesh();
					assert(m_pCookingMesh);
					m_bIsOk = m_pCookingMesh->Init(m_pPhysicsSDK, m_pMyAllocator);
					if (m_bIsOk) {
						LOGGER->AddNewLog(ELL_INFORMATION, "PhysicsManager:: Creado el CookingMesh");
					}

				}// isOk m_pControllerManager?

			}//isOk m_pScene?

		}//isOk m_pPhysicsSDK ?

	}//isOk m_pMyAllocator ?

	if (!m_bIsOk)
	{
		std::string msg_error = "PhysicsManager::Init-> Error en la inicializacion de PhysX";
		LOGGER->AddNewLog(ELL_ERROR, msg_error.c_str());
		Release();
		throw CException(__FILE__, __LINE__, msg_error);
	}

	return m_bIsOk;
}

//----------------------------------------------------------------------------
// Finalize data
//----------------------------------------------------------------------------
void CPhysicsManager::Done ()
{
	if (IsOk())
	{
		Release();
		m_bIsOk = false;
	}
}

//----------------------------------------------------------------------------
// Free memory
//----------------------------------------------------------------------------
void CPhysicsManager::Release ()
{
	CHECKED_DELETE(m_pCookingMesh);
	if( m_pControllerManager != NULL )
	{
		m_pControllerManager->purgeControllers();
		NxReleaseControllerManager(m_pControllerManager);
	}
	if(m_pScene != NULL)
	{
		m_pPhysicsSDK->releaseScene(*m_pScene);
		m_pScene = NULL;
	}

	if(m_pPhysicsSDK != NULL)
	{				
		NxReleasePhysicsSDK(m_pPhysicsSDK);
		m_pPhysicsSDK = NULL;
	}
	CHECKED_DELETE(m_pMyAllocator);
}


//----------------------------------------------------------------------------
// Debug Render
//----------------------------------------------------------------------------
void CPhysicsManager::DebugRender (CRenderManager* render)
{
	assert( m_pScene != NULL );

	if( !m_bDebugRenderMode ) return; 

	// Render all actors in the scene
	int nbActors = m_pScene->getNbActors();
	NxActor** actors = m_pScene->getActors();
	while(nbActors--)
	{
		NxActor* actor = *actors++;
		DrawActor(actor, render);
	}
}

void CPhysicsManager::DrawActor (NxActor* actor, CRenderManager* render)
{

	CPhysicUserData* physicUserData = NULL;
	physicUserData =(CPhysicUserData*)actor->userData;
	//Si est� petando aqu� quiere decir que se ha registrado un objeto f�sico sin proporcionarle UserData
	assert(physicUserData);
	if( !physicUserData->GetPaint())
	{
		return;
	}

	NxShape*const* shapes = actor->getShapes();
	NxU32 nShapes = actor->getNbShapes();

	nShapes = actor->getNbShapes();
	while (nShapes--)
	{
		switch(shapes[nShapes]->getType())
		{
		case NX_SHAPE_PLANE:
			{
				CColor color = physicUserData->GetColor();
				float distance = shapes[nShapes]->isPlane()->getPlane().d;
				NxVec3 normal =  shapes[nShapes]->isPlane()->getPlane().normal;
				Vect3f n(normal.x,normal.y,normal.z);
				render->DrawPlane(100.f, n, distance,color,40,40);
			}
			break;
		case NX_SHAPE_BOX:
			{
				NxF32 m_aux[16];
				shapes[nShapes]->getGlobalPose().getColumnMajor44(m_aux);
				Mat44f m(	m_aux[0], m_aux[4], m_aux[8], m_aux[12], 
									m_aux[1], m_aux[5], m_aux[9], m_aux[13], 
									m_aux[2], m_aux[6], m_aux[10], m_aux[14], 
									m_aux[3], m_aux[7], m_aux[11], m_aux[15]);
				
				render->SetTransform(m);
				NxVec3 boxDim = shapes[nShapes]->isBox()->getDimensions();
				CColor color = physicUserData->GetColor();
				render->DrawBox(boxDim.x*2, boxDim.y*2, boxDim.z*2, color);
			}
			break;
		case NX_SHAPE_SPHERE:
			{
				NxF32 m_aux[16];
				shapes[nShapes]->getGlobalPose().getColumnMajor44(m_aux);
				Mat44f m(	m_aux[0], m_aux[4], m_aux[8], m_aux[12], 
									m_aux[1], m_aux[5], m_aux[9], m_aux[13], 
									m_aux[2], m_aux[6], m_aux[10], m_aux[14], 
									m_aux[3], m_aux[7], m_aux[11], m_aux[15]);
				
				render->SetTransform(m);
				NxReal radius = shapes[nShapes]->isSphere()->getRadius();
				CColor color = physicUserData->GetColor();
				render->DrawSphere(radius,color);
			}
			break;
		case NX_SHAPE_CAPSULE:
			{
				NxF32 m_aux[16];
				shapes[nShapes]->getGlobalPose().getColumnMajor44(m_aux);
				Mat44f m(	m_aux[0], m_aux[4], m_aux[8], m_aux[12], 
									m_aux[1], m_aux[5], m_aux[9], m_aux[13], 
									m_aux[2], m_aux[6], m_aux[10], m_aux[14], 
									m_aux[3], m_aux[7], m_aux[11], m_aux[15]);
				
				Mat44f translation, total;
				translation.SetIdentity();
				render->SetTransform(m);

				const NxReal & radius = shapes[nShapes]->isCapsule()->getRadius();
				const NxReal & height = shapes[nShapes]->isCapsule()->getHeight();
				CColor color = physicUserData->GetColor();
				translation.Translate(Vect3f(0.f, (height*0.5f), 0.f));
				
				total = m * translation;
				render->SetTransform(total);
				render->DrawSphere(radius, color);

				translation.Translate( Vect3f(0.f, -(height*0.5f), 0.f ));
				total = m * translation;
				render->SetTransform(total);
				render->DrawSphere(radius, color);
			}
			break;
		case NX_SHAPE_CONVEX:

			break;
		case NX_SHAPE_MESH:
			{
				NxShape* mesh = shapes[nShapes];

				NxTriangleMeshDesc meshDesc;
				mesh->isTriangleMesh()->getTriangleMesh().saveToDesc(meshDesc);

				typedef NxVec3 Point;
				typedef struct _Triangle { NxU32 p0; NxU32 p1; NxU32 p2; } Triangle;

				NxU32 nbVerts = meshDesc.numVertices;
				NxU32 nbTriangles = meshDesc.numTriangles;

				Point* points = (Point *)meshDesc.points;
				Triangle* triangles = (Triangle *)meshDesc.triangles;

				CColor color = physicUserData->GetColor();
				NxF32 m_aux[16];
				mesh->getGlobalPose().getColumnMajor44(m_aux);
				Mat44f m(	m_aux[0], m_aux[4], m_aux[8], m_aux[12], 
									m_aux[1], m_aux[5], m_aux[9], m_aux[13], 
									m_aux[2], m_aux[6], m_aux[10], m_aux[14], 
									m_aux[3], m_aux[7], m_aux[11], m_aux[15]);
				
				render->SetTransform(m);
				
				Vect3f a,b,c;
				while(nbTriangles--)
				{
					a = Vect3f(points[triangles->p0].x, points[triangles->p0].y,points[triangles->p0].z);
					b = Vect3f(points[triangles->p1].x, points[triangles->p1].y,points[triangles->p1].z);
					c = Vect3f(points[triangles->p2].x, points[triangles->p2].y,points[triangles->p2].z);

					render->DrawLine(a, b, color);
					render->DrawLine(b, c, color);
					render->DrawLine(c, a, color);
					triangles++;

				}
			}
			break;
		case NX_SHAPE_WHEEL:
			{
				//TODO...
			}
			break;
		default:
			{
				//TODO...
			}
			break;
		}
	}
}


//----------------------------------------------------------------------------
// Run Physics
//----------------------------------------------------------------------------
void CPhysicsManager::Update (float elapsedTime)
{
	assert( m_pScene != NULL );
	assert( m_pControllerManager != NULL );

	// Start simulation (non blocking)
	m_pScene->simulate(elapsedTime); 

	// Fetch simulation results
	m_pScene->flushStream();
	m_pScene->fetchResults(NX_RIGID_BODY_FINISHED,  true);

	m_pControllerManager->updateControllers();	
}


void CPhysicsManager::SetTriggerReport (CPhysicTriggerReport* report)
{
	assert(m_pScene);
	assert(report);
	NxUserTriggerReport* nxUserTriggerReport = (NxUserTriggerReport*) report;
	m_pScene->setUserTriggerReport(report);
}

bool CPhysicsManager::AddPhysicActor (CPhysicActor* actor)
{
	assert(actor != NULL);
	assert( m_pScene != NULL );

	bool isOk = false;
	NxActor* nxActor;
	NxActorDesc* actorDesc = actor->GetActorDesc();

	assert(actorDesc!=NULL);
	nxActor = m_pScene->createActor( *actorDesc );
	if (nxActor!=NULL)
	{
		nxActor->userData = actor->GetUserData();
		actor->CreateActor(nxActor);
		isOk = true;
	}

	return isOk;
}

bool CPhysicsManager::ReleasePhysicActor (CPhysicActor* actor)
{
	assert(actor != NULL);
	assert(m_pScene != NULL);
	assert(m_pPhysicsSDK != NULL);

	bool isOk = false;
	NxActor* nxActor = actor->GetPhXActor();

	if( nxActor != 0)
	{
		NxArray<NxCCDSkeleton*> skeletons;
		for (NxU32 i = 0; i < nxActor->getNbShapes(); i++) 
		{
			NxShape* shape = nxActor->getShapes()[i];
			if (shape->getCCDSkeleton() != NULL) {
				skeletons.pushBack(shape->getCCDSkeleton());
			}
		}

		for (NxU32 i = 0; i < skeletons.size(); i++) 
		{
			m_pPhysicsSDK->releaseCCDSkeleton(*skeletons[i]);
		}
		m_pScene->releaseActor(*nxActor);
		nxActor = 0;
	}
	//TODO!!!!
	return true;
}



bool CPhysicsManager::AddPhysicSphericalJoint (CPhysicSphericalJoint* joint)
{
	assert(joint != NULL);
	assert( m_pScene != NULL );

	bool isOk = false;
	NxJoint* nxJoint;
	NxSphericalJointDesc* jointDesc = joint->GetPhXDescJoint();

	assert(jointDesc!=NULL);
	nxJoint = m_pScene->createJoint(*jointDesc);
	if (nxJoint!=NULL)
	{
		joint->CreateJoint(nxJoint);
		isOk = true;
	}

	return isOk;
}

bool CPhysicsManager::RelasePhysicSphericalJoint (CPhysicSphericalJoint* joint)
{
	assert(joint);
	assert(m_pScene);
	NxJoint* phXJoint = joint->GetPhXJoint();
	m_pScene->releaseJoint(*phXJoint);
	phXJoint = 0;
	return true;
}


bool CPhysicsManager::AddPhysicRevoluteJoint (CPhysicRevoluteJoint* joint)
{
	assert(joint != NULL);
	assert( m_pScene != NULL );

	bool isOk = false;
	NxJoint* nxJoint;
	NxRevoluteJointDesc* jointDesc = joint->GetPhXDescJoint();

	assert(jointDesc!=NULL);
	nxJoint = m_pScene->createJoint(*jointDesc);
	if (nxJoint!=NULL)
	{
		joint->CreateJoint(nxJoint);
		isOk = true;
	}

	return isOk;
}

bool CPhysicsManager::RelasePhysicRevoluteJoint (CPhysicRevoluteJoint* joint)
{
	assert(joint);
	assert(m_pScene);
	NxJoint* phXJoint = joint->GetPhXJoint();
	m_pScene->releaseJoint(*phXJoint);
	phXJoint = 0;
	return true;
}

bool CPhysicsManager::AddPhysicFixedJoint (CPhysicFixedJoint* joint)
{
	assert(joint != NULL);
	assert( m_pScene != NULL );

	bool isOk = false;
	NxJoint* nxJoint;
	NxFixedJointDesc* jointDesc = joint->GetPhXDescJoint();

	assert(jointDesc!=NULL);
	nxJoint = m_pScene->createJoint(*jointDesc);
	if (nxJoint!=NULL)
	{
		joint->CreateJoint(nxJoint);
		isOk = true;
	}

	return isOk;
}

bool CPhysicsManager::RelasePhysicFixedJoint (CPhysicFixedJoint* joint)
{
	assert(joint);
	assert(m_pScene);
	NxJoint* phXJoint = joint->GetPhXJoint();
	m_pScene->releaseJoint(*phXJoint);
	phXJoint = 0;
	return true;
}

bool CPhysicsManager::AddPhysicController (CPhysicController* controller)
{
	assert(controller != NULL);
	assert(m_pScene != NULL);
	assert(m_pControllerManager != NULL);

	bool isOK = false;
	NxController* nxController = controller->GetPhXController();
	NxCapsuleControllerDesc*	nxControllerDesc = controller->GetPhXControllerDesc();
	assert(nxControllerDesc!=NULL);
	assert(nxController==NULL); //Nos aseguramos que no hayan registrado ya un actor en la escena
	nxController = m_pControllerManager->createController(m_pScene, *nxControllerDesc );
	if (m_pControllerManager!= NULL)
	{
		controller->CreateController(nxController, m_pScene);
		nxController->getActor()->userData = controller->GetUserData();
		isOK = true;
	}

	return isOK;

}

bool CPhysicsManager::ReleasePhysicController	(CPhysicController* controller)
{
	assert(controller != NULL);
	assert(m_pControllerManager != NULL);

	bool isOk = false;
	NxController* nxController = controller->GetPhXController();
	if (nxController != NULL)
	{
		m_pControllerManager->releaseController( *nxController );
	}

	//TODO!!!!
	return true;
}


NxCCDSkeleton* CPhysicsManager::CreateCCDSkeleton (float size)
{
	assert( m_pPhysicsSDK != NULL );

	NxU32 triangles[3 * 12] = { 0,1,3, 	0,3,2, 3,7,6, 3,6,2, 1,5,7, 1,7,3, 4,6,7, 4,7,5, 1,0,4,
		5,1,4,	4,0,2,	4,2,6	};

	NxVec3 points[8];

	//static mesh
	points[0].set( size, -size, -size);
	points[1].set( size, -size,  size);
	points[2].set( size,  size, -size);
	points[3].set( size,  size,  size);

	points[4].set(-size, -size, -size);
	points[5].set(-size, -size,  size);
	points[6].set(-size,  size, -size);
	points[7].set(-size,  size,  size);

	NxSimpleTriangleMesh stm;
	stm.numVertices = 8;
	stm.numTriangles = 6*2;
	stm.pointStrideBytes = sizeof(NxVec3);
	stm.triangleStrideBytes = sizeof(NxU32)*3;

	stm.points = points;
	stm.triangles = triangles;
	stm.flags |= NX_MF_FLIPNORMALS;

	return m_pPhysicsSDK->createCCDSkeleton(stm);
}


CPhysicUserData* CPhysicsManager::RaycastClosestActor (const Vect3f posRay, const Vect3f& dirRay, uint32 impactMask, CPhysicUserData* impactObject, SCollisionInfo& info )
{
	assert(m_pScene != NULL);

	NxRay ray; 
	ray.dir =  NxVec3(dirRay.x, dirRay.y, dirRay.z);
	ray.orig = NxVec3(posRay.x, posRay.y, posRay.z);

	NxRaycastHit hit;
	NxShape* closestShape = NULL;

	closestShape = m_pScene->raycastClosestShape(ray, NX_ALL_SHAPES, hit, impactMask);
	if (!closestShape) 
	{
		//No hemos tokado a ning�n objeto f�sico de la escena.
		return NULL;
	}
	NxActor* actor = &closestShape->getActor();
	impactObject =(CPhysicUserData*)actor->userData;
	//Si est� petando aqu� quiere decir que se ha registrado un objeto f�sico sin proporcionarle UserData
	assert(impactObject);

	info.m_fDistance	= hit.distance;
	info.m_Normal				= Vect3f(hit.worldNormal.x, hit.worldNormal.y, hit.worldNormal.z ); 
	info.m_CollisionPoint	= Vect3f(hit.worldImpact.x, hit.worldImpact.y, hit.worldImpact.z ); 

	return impactObject;
}

void CPhysicsManager::OverlapSphereActor (float radiusSphere, const Vect3f& posSphere, std::vector<CPhysicUserData*> impactObjects)
{
	assert(m_pScene);

	NxSphere worldSphere(NxVec3(posSphere.x,posSphere.y,posSphere.z), radiusSphere);
	NxU32 nbShapes = m_pScene->getNbDynamicShapes();
	NxShape** shapes = new NxShape* [nbShapes];
	for (NxU32 i = 0; i < nbShapes; i++)
	{
		shapes[i] = NULL;
	}

	m_pScene->overlapSphereShapes(worldSphere, NX_ALL_SHAPES, nbShapes, shapes, NULL);

	for (NxU32 i = 0; i < nbShapes; i++) 
	{
		if( shapes[i] != NULL )
		{
			NxActor* actor = &shapes[i]->getActor();
			CPhysicUserData* physicObject = (CPhysicUserData*)actor->userData;
			//Si est� petando aqu� quiere decir que se ha registrado un objeto f�sico sin proporcionarle ID
			assert(physicObject);	
			//Antes de meterlo comprobamos que no exista ya (un objeto fisico puede estar compuesto por varias shapes)
			std::vector<CPhysicUserData*>::iterator it(impactObjects.begin());
			std::vector<CPhysicUserData*>::iterator itEnd(impactObjects.end());
			bool find = false; 
			while (it!=itEnd)
			{
				CPhysicUserData* id = *it;
				if( id == physicObject)
					find = true;
				++it;
			}

			if(!find)
				impactObjects.push_back(physicObject);
		}
	}
}

void CPhysicsManager::RegisterFunctions (CScriptManager* scriptManager)
{
	lua_State* l_pLUAState = scriptManager->GetLuaState();

	using namespace luabind;
	
	// ahora registramos lo que querramos
  module(l_pLUAState)
    [
      // registramos la clase CPhysicsManager
			class_<CPhysicsManager>(CScriptRegister::SetClassName("CPhysicsManager"))


      // registramos su constructor
      .def(constructor<>())

      // registramos sus funciones publicas
			.def(	CScriptRegister::PushFunctionName(AUTO_COMPLETE), &CScriptRegister::AutoComplete)

			.def(	CScriptRegister::PushFunctionName(HELP,"void","void",
						"Muestra todas las funciones de esta clase"),
						&CScriptRegister::Help)

      .def(	CScriptRegister::PushFunctionName("setDebugRender", "void", "bool flag",
						"Setea a true/false la visibilidad de todos los objetos f�sicos de la escena"),
						&CPhysicsManager::SetDebugRenderMode)

			.def(	CScriptRegister::PushFunctionName("getDebugRender", "bool", "void", 
						"Obtiene si se visualiza o no los objetos f�sicos de la escena"),	
						&CPhysicsManager::GetDebugRenderMode)
    ];
}