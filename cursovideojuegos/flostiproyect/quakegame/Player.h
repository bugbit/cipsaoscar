#pragma once

//--Forward Declaration--
class CPhysicActor;
class CPhysicController;

class CPlayer
{
public:
	CPlayer();
	~CPlayer();
private:
	CPhysicUserData																			*m_PhysicData;
	CPhysicController																		*m_PControler;
};