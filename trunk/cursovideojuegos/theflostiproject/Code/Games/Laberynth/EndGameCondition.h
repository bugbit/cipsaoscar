//----------------------------------------------------------------------------------
// CBoard class
// Author: Enric Vergara
//
// Description:
// Clase abstracta con la interfaz para detectar si se ha de finalizar una partida segun su logica a implementar.
//----------------------------------------------------------------------------------
#pragma once

#ifndef INC_END_GAME_CONDITION_H_
#define INC_END_GAME_CONDITION_H_


//---Forward Declarations--
class CLaberynthBoard;
//-------------------------

//---Definicion de nuevos tipos------------------------------------
typedef enum ETypeEndGameCondition {TC_NONE = 0, TC_TIME_OUT , TC_VICTORY, TC_LOSS};
//-----------------------------------------------------------------


class CEndGameCondition
{
public:
	CEndGameCondition(ETypeEndGameCondition type = TC_NONE): m_eTypeEndGameCondition(type){/*Nothing*/;}
	virtual ~CEndGameCondition()	{/*Nothing*/;}

	virtual bool					Check											(CLaberynthBoard *board, float elapsedTime ) = 0;
	ETypeEndGameCondition	GetTypeOfEndGameCondition	() const {return m_eTypeEndGameCondition;}

protected:
	ETypeEndGameCondition	m_eTypeEndGameCondition;
};

#endif //INC_END_GAME_CONDITION_H_