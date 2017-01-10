#include "Dust.h"
#include "rules.h"
#include "Game.h"

/**
 * Creates a new dust particle to be placed in the game.
 * @param game The game object in which this Dust will be placed.
 * @param loc The initial location of this dust particle.
 * @param vel The initial velocity of this dust particle.
 */
Dust::Dust(Game* game, ofPoint loc, ofPoint vel)
	: GameObject(game)
{
	_loc = loc;
	_vel = vel;
}

/**
 * Called by the game to update the physics and logic of this dust particle.
 */
void Dust::update()
{
	// Apply velocity to location and acceleration to velocity.
	_loc += _vel;
	ofPoint rawAccel = ofxAccelerometer.getRawAcceleration();
	ofPoint fixedAccel = ofPoint(rawAccel.x, -rawAccel.y);
	_vel += fixedAccel * DUST_GRAVITY_FACTOR;
	_vel *= DUST_FRICTION;
	
	// Loop if off screen.
	if(_loc.x + DUST_RAD < 0)
		_loc.x = SCREEN_WIDTH + DUST_RAD;
	else if(_loc.x - DUST_RAD > SCREEN_WIDTH)
		_loc.x = -DUST_RAD;
	if(_loc.y + DUST_RAD < 0)
		_loc.y = SCREEN_HEIGHT + DUST_RAD;
	else if(_loc.y - DUST_RAD > SCREEN_HEIGHT)
		_loc.y = -DUST_RAD;
}

/**
 * Called by the game to draw this dust particle to the screen.
 */
void Dust::draw()
{
	ofPushStyle();
	ofSetColor(DUST_R, DUST_G, DUST_B, DUST_A);
	ofCircle(_loc.x, _loc.y, DUST_RAD);
	ofPopStyle();
}

/**
 * Returns the current location of this dust particle.
 */
ofPoint Dust::loc()
{
	return _loc;
}

/**
 * Returns the current velocity of this dust particle.
 */
ofPoint Dust::vel()
{
	return _vel;
}