#include "Animation.h"
#include "tinyxml2.h"
#include <sstream>
#include "Clip.h"
#include <map>
#include "AnimationEnums.h"
using namespace tinyxml2;



Animation::Animation(std::string daePath)
{
	this->name = daePath;
	std::stringstream lineFromFile;
	tinyxml2::XMLDocument daeFile;
	XMLError loadFailed = daeFile.LoadFile(daePath.c_str());
	if (loadFailed)
		assert(false); //Just crash so we can see

	XMLElement* animationHeader = daeFile.FirstChildElement("COLLADA")->FirstChildElement("library_animations"); //get the library_animations header as our top
	XMLElement* currentAnimation = animationHeader->FirstChildElement(); //Get first <animation>
	Clip* currentClip = new Clip(); //Prepare clip to be re-made for every animation

	//For every <animation, clip in our head>
	do {
		for (int i = 0; i < 4; i++) //Always translation x y z per clip
		{
			std::string animID = (currentAnimation->Attribute("id")); //id = name.FrameType
			currentClip->SetName(animID.substr(0, animID.find_first_of('.'))); //Set robot part for this clip (e.g root.translate = root) //Done only once per anim
			std::string animTypeString = animID.substr(animID.find_first_of('.')+1, std::string::npos);
			AnimationTarget animTarget;
			if (animTypeString == "translate")
				animTarget = Translation;
			else if (animTypeString == "rotateX")
				animTarget = RotateX;
			else if (animTypeString == "rotateY")
				animTarget = RotateY;
			else if (animTypeString == "rotateZ")
				animTarget = RotateZ;
			else
				animTarget = BAD;
			//We now have our animation's robot part, and it's target (trans/rotation)


			XMLElement* inputSource = currentAnimation->FirstChildElement();
			XMLElement* outputSource = currentAnimation->FirstChildElement()->NextSiblingElement();

			std::vector<float> inputData = SplitFileFloatString(inputSource->FirstChild()->FirstChild()->Value()); //Get all numbers as float vector
			std::vector<float> outputData = SplitFileFloatString(outputSource->FirstChild()->FirstChild()->Value()); //Get all numbers as float vector

			std::vector<Keyframe> keyFrames;
			//Get output date first

			int outputDataCount = atoi(outputSource->FirstChildElement()->Attribute("count"));
			int z = 0; //Only used for translation types
			//Add 3 starting frames for translations
			for (int i = 0; i < atoi(inputSource->FirstChildElement()->Attribute("count")); i++) //For however many key times there are
			{
				Keyframe frame = Keyframe();
				



				//Translation output sources have 3 floats per keyframe (x,y,z)
				if (animTarget == Translation) {

					Keyframe frameY = Keyframe();
					Keyframe frameZ = Keyframe();
					frame.SetTime(inputData[i]);
					frameY.SetTime(inputData[i]);
					frameZ.SetTime(inputData[i]);

					frame.SetPosition(outputData[z]/10);
					frameY.SetPosition(outputData[z + 1]/10);
					frameZ.SetPosition(outputData[z + 2]/10);

					currentClip->TranslationsX.push_back(frame);
					currentClip->TranslationsY.push_back(frameY);
					currentClip->TranslationsZ.push_back(frameZ);

					//All our Translation code is in this one if, so add keyframes to clip here

					z += 3; //Increment by three for translations
				}
				else if(animTarget == BAD) {}
				else {
					frame.SetTime(inputData[i]);
					frame.SetPosition(outputData[i]);
					//Add keyframe based on which rotation type
					switch (animTarget) {
					case RotateX:
						currentClip->RotationsX.push_back(frame);
						break;
					case RotateY:
						currentClip->RotationsY.push_back(frame);
						break;
					case RotateZ:
						currentClip->RotationsZ.push_back(frame);
						break;
					}
				}



			}




			//We just need input-array and output-array for each

			currentAnimation = currentAnimation->NextSiblingElement(); //Step to next sibling
		} //We have gotten 4 animations in a row (Translation, X Y Z), clip is complete, insert and move to next 4 animations
		this->Clips.insert(std::pair<std::string, Clip*>(currentClip->GetName(), currentClip)); //Put clip in part name -> clip MAP
		currentClip = new Clip(); //Reset clip
	} while (currentAnimation != nullptr);


}

Animation::~Animation()
{
}

void Animation::UpdateTime(float delta)
{
	for (auto it = Clips.begin(); it != Clips.end(); it++)
	{
		Clip* currentClip = it->second;
		currentClip->UpdateTime(delta);
	}
}

std::vector<float> Animation::SplitFileFloatString(std::string s)
{
	std::vector<float> returnFloats;

	std::istringstream iss(s);
	std::string token;
	while (std::getline(iss, token, ' ')) {
		returnFloats.push_back(atof(token.c_str()));
	}
	return returnFloats;
}

