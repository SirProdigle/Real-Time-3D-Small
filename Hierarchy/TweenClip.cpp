#include "TweenClip.h"




TweenClip::~TweenClip()
{
}

void TweenClip::UpdateTime(float delta)
{
	currentTransitionTime += delta;
	
	if (currentTransitionTime >= this->transitionTime) {
		currentTransitionTime = this->transitionTime;
	}
	bool breakEarly = false;


	clip2->currentTimeTrans += delta;
	clip2->currentTimeRots += delta;
	if (clip2->currTran >= clip2->TranslationsX.size() - 1 && clip2->currentTimeTrans >= clip2->TranslationsX[clip2->TranslationsX.size() - 1].GetTime()) { // 1 before end
		clip2->currTran = 0;
		clip2->currentTimeTrans = 0;
		breakEarly = true;
	}
	if (clip2->currRot >= clip2->RotationsX.size() - 1 && clip2->currentTimeRots >= clip2->RotationsX[clip2->RotationsX.size() - 1].GetTime()) { // 1 before end
		clip2->currRot = 0;
		clip2->currentTimeRots = 0;
		breakEarly = true;
	}


	if (clip2->currTran < clip2->TranslationsX.size() - 1 && clip2->currentTimeTrans > clip2->TranslationsX[clip2->currTran + 1].GetTime())
		clip2->currTran++;
	if (clip2->currRot < clip2->RotationsX.size() - 1 && clip2->currentTimeRots > clip2->RotationsX[clip2->currRot + 1].GetTime())
		clip2->currRot++;


	clip1->currentTimeTrans += delta;
	clip1->currentTimeRots += delta;
	if (clip1->currTran >= clip1->TranslationsX.size() - 1 && clip1->currentTimeTrans >= clip1->TranslationsX[clip1->TranslationsX.size() - 1].GetTime()) { // 1 before end
		clip1->currTran = 0;
		clip1->currentTimeTrans = 0;
		breakEarly = true;
	}
	if (clip1->currRot >= clip1->RotationsX.size() - 1 && clip1->currentTimeRots >= clip1->RotationsX[clip1->RotationsX.size() - 1].GetTime()) { // 1 before end
		clip1->currRot = 0;
		clip1->currentTimeRots = 0;
		breakEarly = true;
	}


	if (clip1->currTran < clip1->TranslationsX.size() - 1 && clip1->currentTimeTrans > clip1->TranslationsX[clip1->currTran + 1].GetTime())
		clip1->currTran++;
	if (clip1->currRot < clip1->RotationsX.size() - 1 && clip1->currentTimeRots > clip1->RotationsX[clip1->currRot + 1].GetTime())
		clip1->currRot++;

	
}

std::pair<XMFLOAT3, XMFLOAT3> TweenClip::GetOffset()
{
	//get trans
	float transplusOne = clip1->currTran + 1;
	float rotsPlusOne = clip1->currRot + 1;
	if (transplusOne >= clip1->TranslationsX.size())
		transplusOne = 0;
	if (rotsPlusOne >= clip1->RotationsX.size())
		rotsPlusOne = 0;

	float transXLerp = clip1->TranslationsX[clip1->currTran].GetPosition();
	float transYLerp = clip1->TranslationsY[clip1->currTran].GetPosition();
	float transZLerp = clip1->TranslationsZ[clip1->currTran].GetPosition();

	float rotXLerp = clip1->RotationsX[clip1->currRot].GetPosition();
	float rotYLerp = clip1->RotationsY[clip1->currRot].GetPosition();
	float rotZLerp = clip1->RotationsZ[clip1->currRot].GetPosition();

	if (clip1->TranslationsX.size() > 1) {

		float tweenTotalTrans = abs(clip1->TranslationsX[transplusOne].GetTime() - clip1->TranslationsX[clip1->currTran].GetTime());
		float amountInTrans = tweenTotalTrans - clip1->currentTimeTrans;
		float tweenAmountTrans = (amountInTrans / tweenTotalTrans) * 0.1;

		transXLerp = Tween(clip1->TranslationsX[clip1->currTran].GetPosition(), clip1->TranslationsX[transplusOne].GetPosition(), tweenAmountTrans);
		transYLerp = Tween(clip1->TranslationsY[clip1->currTran].GetPosition(), clip1->TranslationsY[transplusOne].GetPosition(), tweenAmountTrans);
		transZLerp = Tween(clip1->TranslationsZ[clip1->currTran].GetPosition(), clip1->TranslationsZ[transplusOne].GetPosition(), tweenAmountTrans);
	}


	float tweenTotalRot = abs(clip1->RotationsX[rotsPlusOne].GetTime() - clip1->RotationsX[clip1->currRot].GetTime());
	float amountInRot = tweenTotalRot - clip1->currentTimeRots;
	float tweenAmountRot = (amountInRot / tweenTotalRot) * 0.1;

	tweenAmountRot = (clip1->currentTimeRots - clip1->RotationsX[clip1->currRot].GetTime()) * 10 / (clip1->RotationsX[rotsPlusOne].GetTime() - clip1->RotationsX[clip1->currRot].GetTime()) * 0.1f;
	tweenAmountRot = max(0, tweenAmountRot);
	tweenAmountRot = min(1, tweenAmountRot);

	rotXLerp = Tween(clip1->RotationsX[clip1->currRot].GetPosition(), clip1->RotationsX[rotsPlusOne].GetPosition(), tweenAmountRot);
	rotYLerp = Tween(clip1->RotationsY[clip1->currRot].GetPosition(), clip1->RotationsY[rotsPlusOne].GetPosition(), tweenAmountRot);
	rotZLerp = Tween(clip1->RotationsZ[clip1->currRot].GetPosition(), clip1->RotationsZ[rotsPlusOne].GetPosition(), tweenAmountRot);

	//Now for clip 2

	 transplusOne = clip2->currTran + 1;
	 rotsPlusOne = clip2->currRot + 1;
	if (transplusOne >= clip2->TranslationsX.size())
		transplusOne = 0;
	if (rotsPlusOne >= clip2->RotationsX.size())
		rotsPlusOne = 0;

	float transplusOne2 = clip2->currTran + 1;
	float rotsPlusOne2 = clip2->currRot + 1;
	if (transplusOne2 >= clip2->TranslationsX.size())
		transplusOne2 = 0;
	if (rotsPlusOne2 >= clip2->RotationsX.size())
		rotsPlusOne2 = 0;

	float transXLerp2 = clip2->TranslationsX[clip2->currTran].GetPosition();
	float transYLerp2 = clip2->TranslationsY[clip2->currTran].GetPosition();
	float transZLerp2 = clip2->TranslationsZ[clip2->currTran].GetPosition();

	float rotXLerp2 = clip2->RotationsX[clip2->currRot].GetPosition();
	float rotYLerp2 = clip2->RotationsY[clip2->currRot].GetPosition();
	float rotZLerp2 = clip2->RotationsZ[clip2->currRot].GetPosition();

	if (clip2->TranslationsX.size() > 1) {

		float tweenTotalTrans = abs(clip2->TranslationsX[transplusOne].GetTime() - clip2->TranslationsX[clip2->currTran].GetTime());
		float amountInTrans = tweenTotalTrans - clip2->currentTimeTrans;
		float tweenAmountTrans = (amountInTrans / tweenTotalTrans) * 0.1;

		transXLerp2 = Tween(clip2->TranslationsX[clip2->currTran].GetPosition(), clip2->TranslationsX[transplusOne].GetPosition(), tweenAmountTrans);
		transYLerp2 = Tween(clip2->TranslationsY[clip2->currTran].GetPosition(), clip2->TranslationsY[transplusOne].GetPosition(), tweenAmountTrans);
		transZLerp2 = Tween(clip2->TranslationsZ[clip2->currTran].GetPosition(), clip2->TranslationsZ[transplusOne].GetPosition(), tweenAmountTrans);
	}


	float tweenTotalRot2 = abs(clip2->RotationsX[rotsPlusOne].GetTime() - clip2->RotationsX[clip2->currRot].GetTime());
	float amountInRot2 = tweenTotalRot2 - clip2->currentTimeRots;
	float tweenAmountRot2 = (amountInRot2 / tweenTotalRot2) * 0.1;

	tweenAmountRot2 = (clip2->currentTimeRots - clip2->RotationsX[clip2->currRot].GetTime()) * 10 / (clip2->RotationsX[rotsPlusOne].GetTime() - clip2->RotationsX[clip2->currRot].GetTime()) * 0.1f;
	tweenAmountRot2 = max(0, tweenAmountRot2);
	tweenAmountRot2 = min(1, tweenAmountRot2);

	rotXLerp2 = Tween(clip2->RotationsX[clip2->currRot].GetPosition(), clip2->RotationsX[rotsPlusOne].GetPosition(), tweenAmountRot2);
	rotYLerp2 = Tween(clip2->RotationsY[clip2->currRot].GetPosition(), clip2->RotationsY[rotsPlusOne].GetPosition(), tweenAmountRot2);
	rotZLerp2 = Tween(clip2->RotationsZ[clip2->currRot].GetPosition(), clip2->RotationsZ[rotsPlusOne].GetPosition(), tweenAmountRot2);




	//Got both clips ready, now to tween between
	float percentageToTween = (currentTransitionTime / transitionTime);

	transXLerp = Tween(transXLerp, transXLerp2, percentageToTween);
	transYLerp = Tween(transYLerp, transYLerp2, percentageToTween);
	transZLerp = Tween(transZLerp, transZLerp2, percentageToTween);
	rotXLerp = Tween(rotXLerp, rotXLerp2, percentageToTween);
	rotYLerp = Tween(rotYLerp, rotYLerp2, percentageToTween);
	rotZLerp = Tween(rotZLerp, rotZLerp2, percentageToTween);





	XMFLOAT3 trans(transXLerp, transYLerp, transZLerp);
	XMFLOAT3 rots(rotXLerp, rotYLerp, rotZLerp);
	return std::pair<XMFLOAT3, XMFLOAT3>(trans, rots);
}
