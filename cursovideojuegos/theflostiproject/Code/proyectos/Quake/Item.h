#pragma once

#include "Graphics\Object3D.h"
#include "Base\Singleton.h"

//---Forward Declarations---
class CPhysicActor;
class CQuakePhysicsData;
class CItemModel;

class CItem :	public CObject3D
{
public:

	enum ETYTE { NONE,SHOTGUN, LIFE, ROCKETL, MACHINEGUN, AMMOMACHINEGUN, AMMOROCKETL, AMMOSHOTGUN };

	static const std::string	NameLife;
	static const std::string	NameShotGun;
	static const std::string	NameRocketl;
	static const std::string	NameMachineGun;
	static const std::string	NameAmmoShotGun;
	static const std::string	NameAmmoRocketl;
	static const std::string	NameAmmoMachineGun;
	CItem																		(void);
	virtual	~CItem													(void);
	inline CItemModel &				GetModel			() { return *m_pModel; }
	inline void								SetModel			(CItemModel *model) { m_pModel=model; }
	inline CPhysicActor &			GetActor			() { return *m_pActor; }
	void											CreateActor		(const Vect3f &pos);
	void											ReCreateActor	();
	const Vect3f&							GetPosition		();
	void											SetPosition		( const Vect3f& pos );
	inline bool								GetSelected		() const { return m_bSelected; }
	void											SetSelected		(bool selected);
	inline float							GetTimer			() const { return m_fTimer; }
	inline void								SetTimer			(float timer) { m_fTimer=timer; }
	void											Update	      (float elapsedTime);
protected:
	CQuakePhysicsData					*m_pPhysxData;
	CPhysicActor							*m_pActor;
	CItemModel								*m_pModel;
	float											m_fTimer;
	bool											m_bSelected;
	float											m_fCounterTimer;

	void											ReleaseActor	();
};

struct GUN
{
	int gunState;
	bool selected;
	float timeShot;
	CItem::ETYTE type;
};

class CItemTypeManager
{
public:
	CItem::ETYTE																			GetTypeForName	(std::string name);
	std::string																				GetNameForType	(CItem::ETYTE type);
	static CItemTypeManager &													GetInstance			();
	static inline const GUN &													GetMachineGun		() { return m_MachineGun; }
	CItem::ETYTE																			GetAmmo					(CItem::ETYTE gun);
private:
	std::map<std::string,CItem::ETYTE>								m_MapTypes;
	std::map<CItem::ETYTE,std::string>								m_MapTypesNameEtype;
	std::map<CItem::ETYTE,CItem::ETYTE>								m_MapGunAmmo;
	static const GUN																	m_MachineGun;

	CItemTypeManager																									();
	inline ~CItemTypeManager																					() {}
};
