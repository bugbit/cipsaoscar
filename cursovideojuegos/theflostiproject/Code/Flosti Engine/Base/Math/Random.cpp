//----------------------------------------------------------------------------------------------------------------
// Clase de n�meros Aleatorios
//----------------------------------------------------------------------------------------------------------------
#include "__PCH_Base.h"
#include "Random.h"

CRandom CRandom::s_Random;

// --------------------------------------------------------
//  Uso:            
//  Retorno:        
//  Par�metros:     
// --------------------------------------------------------
unsigned CRandom::getRand () const
{
  m_uSeed = m_uSeed* 214013L + 2531011L;
  return ((m_uSeed>> 16) & Rand_MAX);
}
