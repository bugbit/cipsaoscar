#include "__PCH_Quake.h"

#include "GUIPlayerFaceRender.h"

CGUIPlayerFaceRender::CGUIPlayerFaceRender(void)
{
	D3DXMatrixTranslation( &mTranslation, 0.6f, -0.28f , 0.25f);
}

CGUIPlayerFaceRender::~CGUIPlayerFaceRender(void)
{
}