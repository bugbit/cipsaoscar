#pragma once

//--Forward Declaration--
//Engine:
class CRenderManager;
class CFontManager;
class CQuakePhysicsData;
class CPhysicActor;

class IWorld
{
public:

	IWorld(uint32 group=0);
	virtual ~IWorld(void);

	bool								Init								();
	void	              Done	              ();
	inline bool	        IsOk	              () const { return m_bIsOk; }

	virtual bool				LoadXML							(std::string filexml)=0;
	virtual void				LoadModels					()=0;

	//---Update and Render function
	virtual void				Update	            (float elapsedTime)=0;
	virtual void				RenderScene					(CRenderManager* renderManager, CFontManager* fontManager)=0;

	std::string GetDirWorld() const;

	inline std::string	GetWorldXML				() const { return m_sWorldXML; }
	inline void					SetWorldXML				(std::string file) { m_sWorldXML=file; }
	inline std::string	GetPathTextures		() const { return m_sPathTextures; }
	inline void					SetPathTextures		(std::string path) { m_sPathTextures=path; }
	inline std::string	GetPathPhysx			() const { return m_sPathPhysx; }
	inline void					SetPathPhysx(std::string path) { m_sPathPhysx=path; }
	inline uint32				GetPhysxGroup			() const { return m_uPhysxGroup; }
	inline void					SetPhysxGroup			(uint32 group) { m_uPhysxGroup=group; }

	bool								ReloadXML					();
	void								AddActorInPhysxManager();

protected:
	bool				        m_bIsOk;			      // Initialization boolean control
	std::string					m_sWorldXML;
	std::string					m_sPathTextures;
	std::string					m_sPathPhysx;
	uint32							m_uPhysxGroup;
	CQuakePhysicsData		*m_pPhysxData;
	CPhysicActor				*m_pActor;

	virtual void				Release();
	virtual void				ClearWorld()=0;
	virtual void				ClearModels()=0;
	void								ClearPhysx();
};
