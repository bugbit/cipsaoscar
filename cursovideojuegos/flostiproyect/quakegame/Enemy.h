#pragma once

#include <vector>

//---Engine Includes--------

//--Forward Declaration--
class CRenderManager;

class CEnemy
{
public:
	CEnemy(void);
	~CEnemy(void);
	virtual void																			Update(float elapsedTime);
	virtual void																			RenderScene(CRenderManager* renderManager);
private:
	std::vector<Vect3f>																m_WayPoints;
	Vect3f																						m_Position;
	int																								m_CurrentWayP;
};
