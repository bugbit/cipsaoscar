//----------------------------------------------------------------------------------
// CPhysXTestProcess class
// Author: Enric Vergara
//
// Description:
// Test para probar el funcionamiento del modulo de fisicas.
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_PHYSX_TEST_PROCESS_H_
#define INC_PHYSX_TEST_PROCESS_H_

//---Engine Includes----
#include "Core/Core.h"
#include "Core/Process.h"
#include "PhysX/PhysicTriggerReport.h"
#include "PhysX/PhysicsManager.h"
//-----------------------


//--Forward Declaration--
class CRenderManager;
class CObject3D;
class CPhysicActor;
class CInputManager;
//-----------------------


//---Declaracion de nuevos tipos----
class CGameEntity:public CPhysicUserData
{
public:
	CGameEntity(): m_sName("Default"){}
	~CGameEntity(){/*Nothing*/}

	const std::string&	GetName() const {return m_sName;}
	void								SetName(const std::string& name) {m_sName = name;}

private:
	std::string	m_sName;

};



enum CollisioneGroup
{
	GROUP_BASIC_PRIMITIVES,
	GROUP_GRENADES,
};
#define IMPACT_MASK_1	 (1<<GROUP_BASIC_PRIMITIVES)
#define IMPACT_MASK_2 (1<<GROUP_BASIC_PRIMITIVES | 1<< GROUP_GRENADES)
#define COLLIDABLE_MASK (1<<GROUP_BASIC_PRIMITIVES | 1<< GROUP_GRENADES)
//----------------------------------

class CPhysXTestProcess: public CProcess, public CPhysicTriggerReport
{
public:
	//---Init and End protocols
	CPhysXTestProcess(const std::string& processName);
	virtual ~CPhysXTestProcess(void) {Done();}

	//----CScriptRegister Interface---------------------------------------
	virtual void				RegisterFunctions			(CScriptManager* scriptManager);

	//----CProcess Interface---------------------------------------
	virtual bool	      Init	              ();
	virtual bool				Start								()  {m_bStart = true; return true;}

	//----CPhysicTriggerReport Interface------------------------------------------------------
	virtual void				OnEnter							(CPhysicUserData* trigger1, CPhysicUserData* trigger2);
	virtual void				OnLeave							(CPhysicUserData* trigger1, CPhysicUserData* trigger2);

	//---CFPSGameProcess Interface
	virtual void				Update	            (float elapsedTime);
	virtual void				RenderScene					(CRenderManager* renderManager, CFontManager* fm);
	virtual uint32			RenderDebugInfo			(CRenderManager* renderManager, CFontManager* fm, float fps);
	virtual CCamera*		GetCamera						() const {return m_pCurrentCamera;}
	//--------------------------------------------------------------


private:
	//----CProcess Interface-------------------
	virtual void				Release             ();
	

	//---CFPSGameProcess Interface----------------
	void								UpdateInputActions	(CInputManager* inputManager, float elapsedTime);
	void								UpdateMaxCamera			(CInputManager* inputManager, float elapsedTime);
	void								UpdateInputPlayer		(CInputManager* inputManager, float elapsedTime);

private:
	CCamera*										m_pCurrentCamera; 
	CCamera*										m_pPlayerCamera;
	CObject3D*									m_pObject3D;

	CGameEntity*								m_pLastPickedObject;
	std::string									m_sNameImpactObject;
	std::string									m_sNameTrigger1;
	std::string									m_sNameTrigger2;

	CPhysicSphericalJoint*			m_pPhysicSphericalJoint_Light1;
	CPhysicActor*								m_pPhysicActor_BoxLight1;
	CGameEntity*								m_pGameEntity_BoxLight1;

	CPhysicSphericalJoint*			m_pPhysicSphericalJoint_Light20;
	CPhysicSphericalJoint*			m_pPhysicSphericalJoint_Light21;
	CPhysicSphericalJoint*			m_pPhysicSphericalJoint_Light22;
	CPhysicActor*								m_pPhysicActor_BoxLight20;
	CPhysicActor*								m_pPhysicActor_BoxLight21;
	CPhysicActor*								m_pPhysicActor_BoxLight22;
	CGameEntity*								m_pGameEntity_BoxLight2;

	CPhysicActor*								m_pPhysicActor_Box;
	CGameEntity*								m_pGameEntity_Box;

	CPhysicActor*								m_pPhysicActor_Trigger;
	CGameEntity*								m_pGameEntity_Trigger;

	CPhysicActor*								m_pPhysicActor_Sphere;
	CGameEntity*								m_pGameEntity_Sphere;

	CPhysicActor*								m_pPhysicActor_Capsule;
	CGameEntity*								m_pGameEntity_Capsule;

	CPhysicActor*								m_pPhysicActor_Plane;
	CGameEntity*								m_pGameEntity_Plane;

	CPhysicActor*								m_pPhysicActor_Mesh;
	CGameEntity*								m_pGameEntity_Mesh;

	CPhysicActor*								m_pPhysicActor_Mesh_2;
	CGameEntity*								m_pGameEntity_Mesh_2;

	CPhysicActor*								m_pPhysicActor_Composite;
	CGameEntity*								m_pGameEntity_Composite;

	CPhysicRevoluteJoint*				m_pPhysicRevolute_Bridge;
	CGameEntity*								m_pGameEntity_BoxBridge;
	CPhysicActor*								m_pPhysicActor_BoxBridge;
	CPhysicActor*								m_pPhysicActor_BoxPath;

	CPhysicController*					m_pPhysicController1;
	CGameEntity*								m_pGameEntity_Controller1;

	CPhysicController*					m_pPhysicController2;
	CGameEntity*								m_pGameEntity_Controller2;
	
	std::vector< std::pair<CPhysicActor*,CGameEntity*> >	m_vPhysicActor_Grenades;
	std::vector< std::pair<CPhysicActor*,CGameEntity*> >	m_vPhysicActor_CCDGrenades;
};

static CPhysXTestProcess* GetPhysXTest() {return static_cast<CPhysXTestProcess*>(CORE->GetProcess());}

#endif //INC_PHYSX_TEST_PROCESS_H_