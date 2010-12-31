#pragma once

#include <vector>

#include "world.h"

//--Forward Declaration--
//Engine:
class CASEObject;

class CWorldASE :
	public IWorld
{
public:
	CWorldASE(void);
	virtual ~CWorldASE(void);

	virtual bool				LoadXML(std::string filexml);
	virtual void				LoadModels();

	//---Update and Render function
	virtual void				Update	            (float elapsedTime);
	virtual void				RenderScene					(CRenderManager* renderManager, CFontManager* fontManager);
private:
	int																			m_iNumRooms;
	std::vector<std::string>								m_RoomsFilesASE;
	std::vector<CASEObject *>								m_RoomsASE;
	CASEObject *														LoadModelsRoom(std::string filease,int nRoom);
protected:
	virtual void				ClearWorld();
	virtual void				ClearModels();
};
