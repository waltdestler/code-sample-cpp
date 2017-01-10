#pragma once

#include "GameObject.h"

/**
 * A background particle that has no effect on the gameplay but is
 * effected by gravity and so gives a subtle indication of the direction of
 * gravity.
 */
class Dust : public GameObject
{
private:
	
	ofPoint _loc;
	ofPoint _vel;
	
public:
	
	Dust(Game* game, ofPoint loc, ofPoint vel);
	
	virtual void update();
	virtual void draw();
	
	ofPoint loc();
	ofPoint vel();
};