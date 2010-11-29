#pragma once

#include <vector>

//---Engine Includes--------

//--Forward Declaration--
class CRenderManager;

class CEnemy
{
public:
	enum States
	{
		NO_STATE,	REBIRTH, PATROL, WATCH, SHOOT, DEATH
	};
	CEnemy(void);
	~CEnemy(void);
	virtual void																			Update(float elapsedTime);
	virtual void																			RenderScene(CRenderManager* renderManager);
private:
	std::vector<Vect3f>																m_WayPoints;
	Vect3f																						m_Position,m_VectDir;
	int																								m_CurrentWayP;
	States																						m_State;
	void																							RenderWayPoints(CRenderManager* renderManager);
	void																							RenderModel(CRenderManager* renderManager);
	void																							Rebirth(float elapsedTime);
	void																							Patrol(float elapsedTime);
	void																							Watch(float elapsedTime);
	void																							Shoot(float elapsedTime);
	void																							Death(float elapsedTime);
};
