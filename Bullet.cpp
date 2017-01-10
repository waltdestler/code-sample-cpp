#include "Bullet.h"
#include "rules.h"
#include "Game.h"
#include "Enemy.h"

/**
 * Creates a new Bullet to be placed in the game.
 * @param game The game object that will own this Bullet.
 * @param rules The static rules for the particular kind of bullet.
 * @param loc The initial location of this Bullet.
 * @param vel The initial velocity of this bullet.
 */
Bullet::Bullet(Game* game, BulletRules* rules, ofPoint loc, ofPoint vel)
	: GameObject(game)
{
	_rules = rules;
	_loc = loc;
	_vel = vel;
}

/**
 * Called by the game to update this bullet's physics and logic.
 */
void Bullet::update()
{
	// Apply velocity to location and accelerometer acceleration to velocity.
	_loc += _vel;
	ofPoint rawAccel = ofxAccelerometer.getRawAcceleration();
	ofPoint fixedAccel = ofPoint(rawAccel.x, -rawAccel.y);
	_vel += fixedAccel * _rules->gravityFactor;
	
	// Remove if off screen.
	if(_loc.x + _rules->radius + BULLET_DELETE_THRESHOLD < 0 ||
	   _loc.x - _rules->radius - BULLET_DELETE_THRESHOLD > SCREEN_WIDTH ||
	   _loc.y + _rules->radius + BULLET_DELETE_THRESHOLD < 0 ||
	   _loc.y - _rules->radius - BULLET_DELETE_THRESHOLD > SCREEN_HEIGHT)
	{
		_game->delayRemoveGameObject(this);
		return;
	}
	
	// Check for collision with each enemy.
	vector<Enemy*>::iterator iter;
	for(iter = _game->enemiesBegin(); iter != _game->enemiesEnd(); ++iter)
	{
		if(!(*iter)->isOnScreen() || _game->isMarkedForRemoval(*iter))
			continue;
		
		// Check circle intersection between this bullet and the enemy.
		ofPoint diff = _loc - (*iter)->loc();
		float length = sqrt(diff.x*diff.x + diff.y*diff.y);
		if(length < (*iter)->rules()->radius + _rules->radius)
		{
			(*iter)->hit();
			_game->delayRemoveGameObject(this);
			return;
		}
	}
}

/**
 * Called by the game to render this bullet to the screen.
 */
void Bullet::draw()
{
	ofPushStyle();
	ofSetColor(BULLET_R, BULLET_G, BULLET_B, BULLET_A);
	ofCircle(_loc.x, _loc.y, _rules->radius);
	ofPopStyle();
}

/**
 * Returns the static rules for this bullet.
 */
BulletRules* Bullet::rules()
{
	return _rules;
}

/**
 * Returns the current location of this bullet.
 */
ofPoint Bullet::loc()
{
	return _loc;
}

/**
 * Returns the current velocity of this bullet.
 */
ofPoint Bullet::vel()
{
	return _vel;
}