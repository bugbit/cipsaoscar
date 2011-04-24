#include "__PCH_Base.h"
#include "LerpAnimator1D.h"
#include <cmath>

void CLerpAnimator1D::SetValues (float initValue, float endValue, float totalTime, ETypeFunction type)
{
	assert( totalTime > 0);

	m_fInitValue = initValue;
	m_fEndValue = endValue;
	m_CDTimer.SetTime(totalTime);
	m_eFunction = type;
}


bool CLerpAnimator1D::Update (float deltaTime, float &value)
{
	bool finish = m_CDTimer.Update(deltaTime);
	
	//mu [0-1]
	float mu = m_CDTimer.GetElapsedTimeInPercent()*0.01f;
	
	//En funcion del tiempo la siguiente funcion nos retorna un valor entre 0-1.
	switch(m_eFunction)
	{
	case FUNC_CONSTANT:
		{
			//Linear
			//nothing to do
		}
		break;
	case FUNC_INCREMENT:
		{
			mu = mathUtils::PowN(mu,m_uDegree);			
		}
		break;
	case FUNC_DECREMENT:
		{
			mu = sqrt(mu);//pow(mu,(float)(1/m_uDegree));
		}
		break;
	}
	
	value = mathUtils::Lerp(m_fInitValue, m_fEndValue, mu);
	
	return finish;
}