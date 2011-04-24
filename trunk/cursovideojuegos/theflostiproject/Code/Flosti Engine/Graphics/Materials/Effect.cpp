#include "__PCH_Graphics.h"
#include "effect.h"
#include "Core/Core.h"
#include "RenderManager.h"
#include <assert.h>

#ifndef D3DXSHADER_USE_LEGACY_D3DX9_31_DLL
#define D3DXSHADER_USE_LEGACY_D3DX9_31_DLL 0
#endif

CEffect::CEffect()
: m_Effect(NULL)
, m_FileName("")
, m_hViewInverse(NULL)
, m_hWorld(NULL)
, m_hWorldViewProjection(NULL)
, m_hWorldInverseTranspose(NULL)
{
}

CEffect::~CEffect()
{
	UnLoad();
}

void CEffect::UnLoad()
{
	CHECKED_RELEASE(m_Effect);
}

bool CEffect::LoadEffect()
{
	CRenderManager *l_RM=CCore::GetSingleton().GetRenderManager();
	LPD3DXBUFFER l_ErrorBuffer=NULL;
	HRESULT l_HR=D3DXCreateEffectFromFile(l_RM->GetDevice(),m_FileName.c_str(),NULL,NULL,D3DXSHADER_USE_LEGACY_D3DX9_31_DLL,NULL,&m_Effect,&l_ErrorBuffer);
	if(l_ErrorBuffer)
	{
		char l_Str[MAX_PATH], l_Str2[MAX_PATH];
		_snprintf_s(l_Str,MAX_PATH,MAX_PATH,"Error creating Effect: %s",m_FileName.c_str());
		_snprintf_s(l_Str2,MAX_PATH,MAX_PATH,"%s",l_ErrorBuffer->GetBufferPointer());
		MessageBox(NULL,l_Str2,l_Str,MB_OK);
		CHECKED_RELEASE(l_ErrorBuffer);
		return false;
	}
	m_hViewInverse=m_Effect->GetParameterBySemantic(NULL,"ViewInverse");
	m_hWorld=m_Effect->GetParameterBySemantic(NULL,"World");
	m_hWorldViewProjection=m_Effect->GetParameterBySemantic(NULL,"WorldViewProjection");
	m_hWorldInverseTranspose=m_Effect->GetParameterBySemantic(NULL,"WorldInverseTranspose");
	return true;
}

bool CEffect::Load(const std::string &FileName)
{
	assert(FileName!="");
	m_FileName=FileName;
	return LoadEffect();
}

bool CEffect::Reload()
{
	UnLoad();
	return LoadEffect();
}

void CEffect::BeginRender(const D3DXMATRIX &ViewMatrix, const D3DXMATRIX &ProjectionMatrix)
{
	assert(m_Effect);
	assert(m_hViewInverse);
	D3DXMATRIX l_InverseViewMatrix;

	m_ViewMatrix=ViewMatrix;
	m_ProjectionMatrix=ProjectionMatrix;

	D3DXMatrixMultiply(&m_ViewProjectionMatrix,&ViewMatrix,&ProjectionMatrix);
	D3DXMatrixInverse(&l_InverseViewMatrix, NULL, &ViewMatrix);

	m_Effect->SetMatrix(m_hViewInverse,&l_InverseViewMatrix);
}

void CEffect::SetWorldMatrix(const D3DXMATRIX &WorldMatrix)
{
	assert(m_Effect);
	assert(m_hWorld);
	assert(m_hWorldViewProjection);
	assert(m_hWorldInverseTranspose);

	D3DXMATRIX l_WorldViewProjectionMatrix, l_WorldViewMatrix, l_WorldInverseMatrix, l_WorldInverseTransposeMatrix;

	D3DXMatrixMultiply(&l_WorldViewProjectionMatrix,&WorldMatrix,&m_ViewProjectionMatrix);
	D3DXMatrixMultiply(&l_WorldViewMatrix,&WorldMatrix,&m_ViewMatrix);

	D3DXMatrixInverse(&l_WorldInverseMatrix,NULL,&WorldMatrix);
	D3DXMatrixTranspose(&l_WorldInverseTransposeMatrix,&l_WorldInverseMatrix);

	m_Effect->SetMatrix(m_hWorldViewProjection,&l_WorldViewProjectionMatrix);
	m_Effect->SetMatrix(m_hWorld,&WorldMatrix);
	m_Effect->SetMatrix(m_hWorldInverseTranspose,&l_WorldInverseTransposeMatrix);
}

void CEffect::SetTechnique(const std::string &TechniqueName)
{
	m_Effect->SetTechnique(TechniqueName.c_str());
}
