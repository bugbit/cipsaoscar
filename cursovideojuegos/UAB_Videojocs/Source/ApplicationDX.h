#ifndef _APPLICATION_DX_H
#define _APPLICATION_DX_H

#include <d3dx9.h>
#include "Sphere.h"
#include "Timer.h"
#include "DebugPrinttext2D.h"
#include "BaseGame.h"
#include <vector>

class CApplicationDX
{

public:

	CApplicationDX	(void);
	~CApplicationDX	(void);

	//--------------------------------------------------------//
	// Diagrama de una aplicacion de graficos en tiempo real
	// 1. InitAndLoad()
	// 2. Mientras (No Salir) hacer
	//		2.1 Update() 
	//		2.2 Render()
	//---------------------------------------------------------//
	
	//--- Iniciliza la aplicacion y carga en memoria los datos
	void		InitAndLoad	( HWND hWnd );
	
	//--- Lee los inputs de entrada (Mouse y Keyboard) y actualiza todos los elementos del juego
	void		Update		( void );

	//--- Dibuja en pantalla todos los elementos que deben ser renderizados
	void		Render		( void );

private:

	LPDIRECT3D9             m_pD3D;
	LPDIRECT3DDEVICE9       m_pD3DDevice;
	
	bool		m_PaintSolid;		//Indica si pintamos los elementos solidos o en wireframe
	bool		m_paintInfoGame;	//Indica si pintamos en texto2d informacion util del juego (fps, posicion del player,tc)
	bool		m_paintInfoInput;	//Indica si pintamos en texto2d las asociaciones [input-accion]
	bool		m_drawAxisGrid;		//Indica si pintamos un eje de referencia y un grid en la posicion (0,0,0)
	
	CTimer		m_timer;			//Objeto con toda la informacion del tiempo (fps, ElapsedTime, etc)
	HWND			m_hWnd;				//Identificador de ventana donde se incrusta el visualizador 

	CBaseGame*	m_CurrentGame;
	std::vector<CBaseGame*>	m_VectorGames;


	CDebugPrintText2D	m_printText2D;	//Objeto para poder pintar texto2D en pantalla
	
	//La funcion Init()  llama a InitDX() y InitData()
	//--- InitDX inicializa todo lo relacionado con DirectX
	HRESULT		InitDX			( HWND hWnd );

	//--- InitData carga en memoria todo los elementos del juego que seran necesarios durante la partida
	HRESULT		InitData		( void );

	//Update llama a esta funcion para ejecutar las acciones asociadas a los inputs que estan activados
	void		UpdateActions	( float l_ElapsedTime );

	//--- RENDER FUNCTIONS ---
	//La funcion Render() llama a estas funciones
	
	//Prepara la visualizacion (obteniendo la matrix View y Projection)
	void		SetupMatrices	( void );
	
	//Prepara las variables de Direct3D para hacer el render de la escena
	void		BeginRenderDX	( void );

	//Finaliza la composicion de la escena con Direct3D
	void		EndRenderDX		( void );

	//Pinta toda la informacion de texto2D en la pantalla
	void		RenderInfoGame	( void );

};

#endif //_APPLICATION_DX_H