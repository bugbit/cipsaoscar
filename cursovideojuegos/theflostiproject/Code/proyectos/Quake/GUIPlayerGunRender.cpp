#include "__PCH_Quake.h"

#include "GUIPlayerGunRender.h"

//---Engine Includes----
#include "Core/Core.h"
#include "Graphics/RenderManager.h"
#include "Graphics/FontManager.h"
#include "Graphics/Camera.h"

CGUIPlayerGunRender::CGUIPlayerGunRender(bool bAnimatedShot,bool bAnimatedSetBack)
:m_bAnimatedShot(bAnimatedShot)
,m_bAnimatedSetBack(bAnimatedSetBack)
{
}

CGUIPlayerGunRender::~CGUIPlayerGunRender(void)
{
}

void CGUIPlayerGunRender::RenderScene(CRenderManager* renderManager, CFontManager* fm, CCamera *camera,CGUIPlayer &guiplayer)
{
	D3DXMATRIX translation;
	D3DXMATRIX translation2;
	D3DXMATRIX rotation;
	D3DXMATRIX rotation2;
	D3DXMATRIX rotation_yaw;
	D3DXMATRIX rotation_pitch;
	D3DXMATRIX rotation_machinegun;
	Vect3f position = camera->GetObject3D()->GetPosition();
	float yaw =  camera->GetObject3D()->GetYaw();
	float pitch =  camera->GetObject3D()->GetPitch();

	D3DXMatrixRotationY ( &rotation_yaw,  -0.2f);
	D3DXMatrixRotationZ ( &rotation_pitch,  0.2f);

	if (m_bAnimatedShot && guiplayer.IsShot())
	{
		D3DXMatrixRotationX ( &rotation_machinegun,  -guiplayer.GetYawGun()*10 );
	}
	else
	{
		D3DXMatrixRotationX ( &rotation_machinegun,  0.f );
	}
	// Realizar el movimiento de la shotgun y rocket cuando han disparado
	float amountSetBack = 0.f;
	D3DXMatrixTranslation( &translation2, 0.2f - amountSetBack, -0.2f , -0.1f);
	D3DXMatrixTranslation( &translation, position.x ,position.y + 0.005f*sin(guiplayer.GetPitchGun()),position.z );
	
	D3DXMatrixRotationY ( &rotation,  -yaw + sin( guiplayer.GetYawGun() )*0.01f);
	D3DXMatrixRotationZ ( &rotation2,  pitch);

	m_Matrix = rotation_machinegun * rotation_yaw * rotation_pitch * translation2 * rotation2 * rotation * translation; 

	CGUIPlayerObjectASERender::RenderScene(renderManager,fm,camera,guiplayer);
}
