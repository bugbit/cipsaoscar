//----------------------------------------------------------------------------------
// CAssetErrors class
// Author: Enric Vergara
//
// Description:
// Clase encargada de gestionar los errores de assets, basicamente renderizarlos
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_ASSET_ERRORS_H_
#define INC_ASSET_ERRORS_H_


//---- Declaracion de nuevos tipos------------
enum TypeOfAssetError 
{ 
  ASSET_ERROR_SOUNDFILE = 0, 
  ASSET_ERROR_TEXTUREFILE, 
  ASSET_ERROR_MESHFILE, 
  ASSET_ERROR_PHYSICMESHFILE, 
	ASSET_ERROR_SCRIPTFILE,
	ASSET_ERROR_AVIFILE,
	ASSET_ERROR_LANGUAGE_LITERAL,
	ASSET_ERROR_TTF,
  NUM_ASSET_ERRORS
};
//---------------------------------------------


//--Forward Declaration--
class CRenderManager;
class CFontManager;
//-----------------------

class CAssetErrors
{
public:
	//---Init and End protocols
	CAssetErrors();
	virtual ~CAssetErrors(void) {}

	void	Render						(CRenderManager* rm, CFontManager * fm);
	void	Update						(float deltaTime);

	void	SetAssetError			(TypeOfAssetError error);
  void	ResetAssetErrors	();
	

private:
	bool					m_AssetErrors[NUM_ASSET_ERRORS];	// Asset Errors
	std::string		m_vMessage[NUM_ASSET_ERRORS];
	std::string		m_sMessage;
	float					m_fCounterAssetTime;
};

#endif //INC_ASSET_ERRORS_H_