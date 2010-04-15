#include ".\sphere.h"
#include "DebugRender.h"
#include "Defines.h"

CSphere::CSphere(void)
: m_YAW(0.0f)
, m_Pitch(0.0f)
, m_Roll(0.0f)
, m_Radius(2.0f)
{
	m_Position=D3DXVECTOR3(0.0f,0.0f,0.0f);
}

CSphere::~CSphere(void)
{

}

void CSphere::Update(float ElapsedTime)
{
	m_Pitch+=DEG2RAD(180.0f*ElapsedTime);		//el pitch girara a 180 grados por segundo
	m_YAW+=DEG2RAD(45.0f*ElapsedTime);			//el yaw girara a 90 grados por segundo
	m_Roll=DEG2RAD(30.0f*ElapsedTime);			//el roll girara a 30 grados por segundo
	m_Position.y=2.0f*sin(m_YAW);
}

void CSphere::Render(LPDIRECT3DDEVICE9 Device)
{
	//calculamos matrices
	D3DXMATRIX l_Matrix, l_RotationMatrix, l_TranslationMatrix;
	//seteamos matriz
	D3DXMatrixRotationYawPitchRoll(&l_RotationMatrix,m_YAW,m_Pitch,m_Roll);
	D3DXMatrixTranslation(&l_TranslationMatrix,m_Position.x,m_Position.y,m_Position.z);
	l_Matrix=l_RotationMatrix*l_TranslationMatrix;
	Device->SetTransform(D3DTS_WORLD,&l_Matrix);
	//pintamos
	CDebugRender::DrawSphere(Device,m_Radius);

}