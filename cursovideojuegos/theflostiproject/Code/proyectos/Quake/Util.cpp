#include "__PCH_Quake.h"

#include "Util.h"

//---Engine Includes--------
#include "Core/Core.h"
#include "PhysX/PhysicsManager.h"
#include "PhysX/PhysicASELoader.h"
#include "PhysX/PhysicCookingMesh.h"


CUtil::CUtil(void)
{
}

CUtil::~CUtil(void)
{
}

#pragma warning( push )	
#pragma warning(disable : 4996)
void CUtil::LoadOrCreatePhysXMesh(	const std::string& aseFileName, const std::string& binFileName, const std::string& physXMeshName )
{
	CPhysicsManager* physicManager = CORE->GetPhysicManager();
	assert(physicManager);

	FILE * pFile;
	pFile = fopen (binFileName.c_str(),"r");
	if (pFile==NULL)
	{
		//Si no existe el fichero BIN lo creamos
		std::vector<uint32> faces;
		std::vector<Vect3f> vertices;
		CPhysicASELoader::ReadMeshFromASE(aseFileName,vertices, faces);
		physicManager->GetCookingMesh()->SavePhysicMesh(vertices, faces, binFileName);
	}
	else
	{
		fclose (pFile);
	}
	physicManager->GetCookingMesh()->CreatePhysicMesh(binFileName, physXMeshName);
}
#pragma warning( pop )
