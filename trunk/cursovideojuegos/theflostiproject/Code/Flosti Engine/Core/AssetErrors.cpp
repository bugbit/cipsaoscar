#include "__PCH_Core.h"

#include "AssetErrors.h"
#include "Core/Core.h"
#include "Graphics/RenderManager.h"
#include "Graphics/FontManager.h"
#include "Logger/Logger.h"

CAssetErrors::CAssetErrors()
: m_fCounterAssetTime(0.f) 
{
	ResetAssetErrors();

	m_sMessage																= "-----Asset Error-----";
	m_vMessage[ASSET_ERROR_SOUNDFILE]					= "   [Sound File]      ";
	m_vMessage[ASSET_ERROR_TEXTUREFILE]				= "  [Texture File]     ";
	m_vMessage[ASSET_ERROR_MESHFILE]					= "   [Mesh File]       ";
	m_vMessage[ASSET_ERROR_PHYSICMESHFILE]		= " [PhysicMesh File]   ";
	m_vMessage[ASSET_ERROR_SCRIPTFILE]				= "   [Script File]     ";
	m_vMessage[ASSET_ERROR_AVIFILE]						= "     [Avi File]      ";
	m_vMessage[ASSET_ERROR_LANGUAGE_LITERAL]	= " [Language Literal]  ";
	m_vMessage[ASSET_ERROR_TTF]								= "        [TTF]        ";
}

void	CAssetErrors::SetAssetError			(TypeOfAssetError error)
{
	if (error < NUM_ASSET_ERRORS && error >= 0)
  {
    m_AssetErrors[error] = true;
  }
  else
  {
    LOGGER->AddNewLog(ELL_WARNING, "Core::SetAssetError se desconoce el error de asset!.");
  }
}

void	CAssetErrors::ResetAssetErrors	()
{
	for(uint16 index = 0; index < NUM_ASSET_ERRORS; index++)
  {
    m_AssetErrors[index] = false;
  }
}

void CAssetErrors::Update (float deltaTime)
{
	m_fCounterAssetTime += deltaTime;
	if (m_fCounterAssetTime > 2*FLOAT_PI_VALUE)
	{
		m_fCounterAssetTime = 0.f;
	}
}

void CAssetErrors::Render (CRenderManager* rm, CFontManager * fm)
{
	 //Pintamos las advertencias de errores en la carga de los assets:
  uint32 w,h;
  rm->GetWidthAndHeight(w,h);

	Vect2i quad_size(125,50);
  Vect2i quad_position( w-(quad_size.x)-5, 10);
  Vect2i message_position(quad_position.x+2, quad_position.y + uint32(quad_size.y*0.1));
  
	CColor colorBackGround_Quad	=	colRED;
	CColor colorEdge_Quad				=	colBLACK;

	float aux = cos(m_fCounterAssetTime)*0.5f; //aux ->[-0.5,0.5]
	colorBackGround_Quad.SetAlpha(0.5f+aux);
  for(uint16 index = 0; index < NUM_ASSET_ERRORS; index++)
  {
		if (m_AssetErrors[index])
		{
			//Draw backgorund quad:
			rm->DrawRectangle2D(quad_position, quad_size.x, quad_size.y, colorBackGround_Quad, 1, 1, colorEdge_Quad);

			//Draw message:
			uint32 dy = 0;
			dy = fm->DrawDefaultText(message_position.x, message_position.y,colWHITE, m_sMessage.c_str());
			fm->DrawDefaultText(message_position.x, message_position.y+dy+5,colWHITE, m_vMessage[index].c_str());

			quad_position.y			+= 75;
			message_position.y	+= 75;
		}
	}
}