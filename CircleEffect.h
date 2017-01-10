#pragma once

#include "GameObject.h"
#include "IntColor.h"

/**
 * A special short-lived game object that is an expanding or shrinking circle
 * that fades from one color to another. It can also move from one point to another.
 * Once it has been alive for a particular duration, it removes itself from the game.
 */
class CircleEffect : public GameObject
{
private:
	
	int _startFrame;
	int _duration;
	ofPoint _startLoc;
	ofPoint _endLoc;
	IntColor _startColor;
	IntColor _endColor;
	float _startRadius;
	float _endRadius;
	
public:
	
	CircleEffect(Game* game, int duration, ofPoint startLoc, ofPoint endLoc,
				 IntColor startColor, IntColor endColor, float startRadius, float endRadius);
	
	virtual void update();
	virtual void draw();
};