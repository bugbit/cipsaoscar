//----------------------------------------------------------------------------------
// CBoard class
// Author: Enric Vergara
//
// Description:
// ...
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_LABERYTNH_BOARD_H_
#define INC_LABERYTNH_BOARD_H_

//---Engine Includes---
#include "PhysX/PhysicsManager.h"
#include "Graphics/ASEObject/ASEObject.h"
//---------------------

//--Forward Declaration--
class CRenderManager;
class CFontManager;
//---------------------


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
#define Y_SETTINGS 500
//----------------------------------

class CLaberynthBoard
{
public:
	CLaberynthBoard(	CASEObject* baseBoardMesh, CASEObject* ballMesh, float ballRadius,
										CASEObject* boardMesh, const std::string& physXMeshName, 
										const Vect3f& goalPosition, const Vect3f& startPosition, bool inSettings = false);

	virtual ~CLaberynthBoard();

	void									RenderScene				(CRenderManager* renderManager, CFontManager* fontManager);
	void									UpdateScene				(float elapsedTime);

	CPhysicRevoluteJoint*	GetRevoluteJoint1	() {return m_pPhysicRevolute_1;}
	CPhysicRevoluteJoint*	GetRevoluteJoint2	() {return m_pPhysicRevolute_2;}


private:

	//---PhysX Elements:
	CPhysicActor*								m_pPhysicActor_Plane;
	CGameEntity*								m_pGameEntity_Plane;

	CPhysicActor*								m_pPhysicActor_Board;
	CGameEntity*								m_pGameEntity_Board;

	CPhysicActor*								m_pPhysicActor_Ball;
	CGameEntity*								m_pGameEntity_Ball;

	CPhysicActor*								m_pPhysicActor_Box;
	CGameEntity*								m_pGameEntity_Box;

	CPhysicRevoluteJoint*				m_pPhysicRevolute_1;
	CPhysicRevoluteJoint*				m_pPhysicRevolute_2;

	CPhysicFixedJoint*					m_pPhysicFixedJoint;
	CPhysicActor*								m_pPhysicActor_GoalBox;
	CGameEntity*								m_pGameEntity_GoalBox;

	CPhysicActor*								m_pPhysicActor_GoalTrigger;
	CGameEntity*								m_pGameEntity_GoalTrigger;

	CPhysicActor*								m_pPhysicActor_FlatTrigger;
	CGameEntity*								m_pGameEntity_FlatTrigger;

	CPhysicActor*								m_pPhysicActor_BaseBoard;
	CGameEntity*								m_pGameEntity_BaseBoard;

	std::vector<CGameEntity*>		m_GameEntities;
	std::vector<CPhysicActor*>	m_PhyscActors;

	float												m_fBoardHeight;

	//---Mesh Elements:
	CASEObject*									m_pASE_Board;
	CASEObject*									m_pASE_Ball;
	CASEObject*									m_pASE_BaseBoard;

};
#endif //INC_LABERYTNH_BOARD_H_