#include "Clip.h"



Clip::Clip()
{
	currentTimeTrans = 0;
	currentTimeRots = 0;
	currTran = 0;
	currRot = 0;
}


Clip::~Clip()
{
}

void Clip::UpdateTime(float delta)
{
	currentTimeTrans += delta;
	currentTimeRots += delta;
	bool breakEarly = false;

	if (this->currTran >= TranslationsX.size() - 1 && currentTimeTrans >= TranslationsX[TranslationsX.size() - 1].GetTime()) { // 1 before end
		currTran = 0;
		currentTimeTrans = 0;
		breakEarly = true;
	}
	if (currRot >= RotationsX.size() - 1 && currentTimeRots >= RotationsX[RotationsX.size() - 1].GetTime()) { // 1 before end
		currRot = 0;
		currentTimeRots = 0;
		breakEarly = true;
	}
	if (breakEarly)
		return;

	if (currTran < TranslationsX.size() - 1 && currentTimeTrans > TranslationsX[currTran+1].GetTime()  )
		currTran++;
	if (currRot < RotationsX.size() - 1 && currentTimeRots > RotationsX[currRot+1].GetTime()  )
		currRot++;




}

std::pair<XMFLOAT3, XMFLOAT3> Clip::GetOffset()
{
	//get trans
	float transplusOne = currTran + 1;
	float rotsPlusOne = currRot + 1;
	if (transplusOne >= TranslationsX.size())
		transplusOne = 0;
	if (rotsPlusOne >= RotationsX.size())
		rotsPlusOne = 0;

	float transXLerp = TranslationsX[currTran].GetPosition();
	float transYLerp = TranslationsY[currTran].GetPosition();
	float transZLerp = TranslationsZ[currTran].GetPosition();

	float rotXLerp = RotationsX[currRot].GetPosition();
	float rotYLerp = RotationsY[currRot].GetPosition();
	float rotZLerp = RotationsZ[currRot].GetPosition();

	if (TranslationsX.size() > 1) {

		float tweenTotalTrans = abs(TranslationsX[transplusOne].GetTime() - TranslationsX[currTran].GetTime());
		float amountInTrans = tweenTotalTrans - currentTimeTrans;
		float tweenAmountTrans = (amountInTrans / tweenTotalTrans) * 0.1;

		transXLerp = Tween(TranslationsX[currTran].GetPosition(), TranslationsX[transplusOne].GetPosition(), tweenAmountTrans);
		transYLerp = Tween(TranslationsY[currTran].GetPosition(), TranslationsY[transplusOne].GetPosition(), tweenAmountTrans);
		transZLerp = Tween(TranslationsZ[currTran].GetPosition(), TranslationsZ[transplusOne].GetPosition(), tweenAmountTrans);
	}


	float tweenTotalRot = abs(RotationsX[rotsPlusOne].GetTime() - RotationsX[currRot].GetTime());
	float amountInRot = tweenTotalRot - currentTimeRots;
	float tweenAmountRot = (amountInRot / tweenTotalRot) * 0.1;

	tweenAmountRot = (currentTimeRots - RotationsX[currRot].GetTime()) * 10 / (RotationsX[rotsPlusOne].GetTime() - RotationsX[currRot].GetTime()) * 0.1f;
	tweenAmountRot = max(0, tweenAmountRot) ;
	tweenAmountRot = min(1, tweenAmountRot) ;

	rotXLerp = Tween(RotationsX[currRot].GetPosition(), RotationsX[rotsPlusOne].GetPosition(), tweenAmountRot);
	rotYLerp = Tween(RotationsY[currRot].GetPosition(), RotationsY[rotsPlusOne].GetPosition(), tweenAmountRot);
	rotZLerp = Tween(RotationsZ[currRot].GetPosition(), RotationsZ[rotsPlusOne].GetPosition(), tweenAmountRot);
	 //rotYLerp = RotationsY[currRot].GetPosition() * (1.0f - (currentTimeRots - RotationsY[currRot].GetTime()) + (RotationsY[rotsPlusOne].GetPosition() * currentTimeRots - RotationsY[currRot].GetTime()));
	 //rotZLerp = RotationsZ[currRot].GetPosition() * (1.0f - (currentTimeRots - RotationsZ[currRot].GetTime()) + (RotationsZ[rotsPlusOne].GetPosition() * currentTimeRots - RotationsZ[currRot].GetTime()));
	
	XMFLOAT3 trans(transXLerp, transYLerp, transZLerp);
	XMFLOAT3 rots(rotXLerp, rotYLerp, rotZLerp);
	return std::pair<XMFLOAT3, XMFLOAT3>(trans, rots);
}

float Clip::Tween(float pos1, float pos2, float amount)
{
	return ((1 - amount)*pos1) + (amount * pos2);
}
