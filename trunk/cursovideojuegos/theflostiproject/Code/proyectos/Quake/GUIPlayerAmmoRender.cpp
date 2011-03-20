#include "__PCH_Quake.h"

#include "GUIPlayerAmmoRender.h"

CGUIPlayerAmmoRender::CGUIPlayerAmmoRender(void)
{
	D3DXMatrixTranslation( &mTranslation, 0.6f, -0.15f , 0.25f);
}

CGUIPlayerAmmoRender::~CGUIPlayerAmmoRender(void)
{
}
