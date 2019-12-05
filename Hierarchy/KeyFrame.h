#pragma once
class Keyframe
{
public:
	Keyframe();
	~Keyframe();
	void SetTime(float f) { time = f; }
	void SetPosition(float p) { position = p; }
	float GetTime() { return time; }
	float GetPosition() { return position; }
private:
	float time;
	float position;
};

