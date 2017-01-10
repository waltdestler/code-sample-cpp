#pragma once

#include "GameObject.h"

/**
 * Contains static rules for a particular kind of bullet.
 */
struct BulletRules
{
	float radius;
	float gravityFactor;
};

/**
 * A projectile fired from the player's ship(s).
 * Effected by real-world gravity according to the accelerometer.
 */
class Bullet : public GameObject
{
private:
	
	BulletRules* _rules;
	ofPoint _loc;
	ofPoint _vel;
	
public:
	
	Bullet(Game* game, BulletRules* rules, ofPoint loc, ofPoint vel);
	
	virtual void update();
	virtual void draw();
	
	BulletRules* rules();
	ofPoint loc();
	ofPoint vel();
	
	bool isBullet(){return true;}
};