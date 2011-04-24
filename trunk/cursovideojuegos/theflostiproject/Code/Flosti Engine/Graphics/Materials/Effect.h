//----------------------------------------------------------------------------------
// CEffect class
// Author: Enric Vergara
//
// Description:
// Clase...
//----------------------------------------------------------------------------------
#pragma once
#ifndef EFFECT_H
#define EFFECT_H

#include <d3dx9.h>
#include <string>

class CEffect
{

public:
	CEffect();
	~CEffect();
	inline LPD3DXEFFECT	GetD3DEffect		() const {return m_Effect;}
	bool								Load						(const std::string &FileName);
	bool 								Reload					();
	void 								BeginRender			(const D3DXMATRIX &ViewMatrix, const D3DXMATRIX &ProjectionMatrix);
	void 								SetWorldMatrix	(const D3DXMATRIX &WorldMatrix);
	void 								SetTechnique		(const std::string &TechniqueName);

private:
	bool								LoadEffect			();
	void								UnLoad					();

private:
	LPD3DXEFFECT			m_Effect;
	std::string				m_FileName;
	D3DXMATRIX				m_ViewMatrix, m_ProjectionMatrix, m_ViewProjectionMatrix;
	D3DXHANDLE				m_hViewInverse, m_hWorld, m_hWorldViewProjection, m_hWorldInverseTranspose;

};

#endif //EFFECT_H