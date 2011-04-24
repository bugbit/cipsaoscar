//----------------------------------------------------------------------------------
// CVictoryEndGameCondition class
// Author: Enric Vergara
//
// Description:
// Clase encargada de comprobar la finalizacion de la partida en el caso de que la pelota caiga en el agujero de meta o en el suelo
//----------------------------------------------------------------------------------
#pragma once
#ifndef INC_VICTORY_OR_LOSS_END_GAME_CONDITION_H_
#define INC_VICTORY_OR_LOSS_END_GAME_CONDITION_H_

//---Engine Includes---
#include "PhysX/PhysicTriggerReport.h"
//---------------------

//---Game Includes---
#include "EndGameCondition.h"
//-------------------

//---Forward Declarations---
//Game:
class CLaberynthBoard;
//Engine:
class CPhysicUserData;
//--------------------------

class CVictoryOrLossEndGameCondition : public CEndGameCondition, public CPhysicTriggerReport 
{
public:
	CVictoryOrLossEndGameCondition ();
	virtual ~CVictoryOrLossEndGameCondition () {/*Nothing*/;}

	//-----CEndGameCondition Interface--------
	bool								Check								(CLaberynthBoard *board, float elapsedTime);	

	//----CPhysicTriggerReport Interface------------------------------------------------------
	virtual void				OnEnter							(CPhysicUserData* trigger1, CPhysicUserData* other_shape);
	virtual void				OnLeave							(CPhysicUserData* trigger1, CPhysicUserData* other_shape);


private:
	bool	m_bBallEnterInTrigger;
};

#endif //INC_VICTORY_OR_LOSS_END_GAME_CONDITION_H_