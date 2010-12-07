#pragma once

class CUtil
{
public:
	inline CUtil(void);
	inline ~CUtil(void);
	static void LoadOrCreatePhysXMesh(	const std::string& aseFileName, 
																								const std::string& binFileName, 
																								const std::string& physXMeshName );
};
