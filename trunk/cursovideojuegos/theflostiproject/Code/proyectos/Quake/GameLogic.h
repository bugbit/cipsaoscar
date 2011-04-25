#pragma once

#include "Arena.h"
#include "Item.h"

//---Engine Includes---
#include "PhysX/PhysicTriggerReport.h"

//--Forward Declaration--
//Engine:
class CRenderManager;
class CFontManager;
class CPlayer;

class CGameLogic : public CPhysicTriggerReport
{
public:
	CGameLogic																														(void);
	~CGameLogic																														(void);

	bool																							Init	              ();
	void																							Done	              ();
	inline bool																				IsOk	              () const { return m_bIsOk; }

	void																							Shot								(CPlayer &player);

	//---Update and Render function
	void																							Update	            (float elapsedTime);
	void																							RenderScene					(CRenderManager* renderManager, CFontManager* fontManager);

	//----CPhysicTriggerReport Interface------------------------------------------------------
	virtual void																			OnEnter							(CPhysicUserData* trigger1, CPhysicUserData* other_shape);
	virtual void																			OnLeave							(CPhysicUserData* trigger1, CPhysicUserData* other_shape);

	inline CArena &																		GetArena						() { return m_Arena; }
private:
	bool																							m_bIsOk;			      // Initialization boolean control	
	CArena																						m_Arena;

	void																							Release             ();
	void																							ShotRay							(CItem::ETYTE argType,CPlayer &player);
	void																							ShotOverlapSphere		(CItem::ETYTE argType,CPlayer &player);
};
