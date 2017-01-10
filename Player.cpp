#include "Player.h"
#include "rules.h"
#include "Game.h"
#include "IntColor.h"
#include "CircleEffect.h"
#include "Bullet.h"
#include "LevelBase.h"

/**
 * Creates a new Player object to be placed in the game.
 * @param game The game object that will own this Player.
 * @param rules The static rules for this player.
 */
Player::Player(Game* game, PlayerRules* rules)
	: GameObject(game)
{
	_rules = rules;
	_loc = rules->locs[0]; // Starting location is initial waypoint.
	_targetLocIndex = 0;
	_rot = rules->initRot;
}

/**
 * Called by the game to update the player's logic.
 */
void Player::update()
{
	// Rotate.
	_rot += _rules->rotVel;
	
	// Move to next target?
	ofPoint diff = targetLoc() - _loc;
	if(fabs(diff.x) < 1 && fabs(diff.y) < 1)
		_targetLocIndex++;
	
	// Move to target location.
	float dist = sqrt(diff.x*diff.x + diff.y*diff.y);
	if(dist > 0)
	{
		ofPoint normalized = diff / dist;
		_loc += normalized * min(dist, _rules->speed);
	}
	
	// Fire if enough time has elapsed.
	if(_game->frames() % _rules->fireInterval == 0)
	{
		float rad = ofDegToRad(_rot);
		float cosRot = cos(rad);
		float sinRot = sin(rad);
		ofPoint vel = ofPoint(
			_rules->fireVel.x * cosRot - _rules->fireVel.y * sinRot,
			_rules->fireVel.x * sinRot + _rules->fireVel.y * cosRot);
		Bullet* bullet = new Bullet(_game, _rules->bulletRules, _loc, vel);
		_game->delayAddGameObject(shared_ptr<Bullet>(bullet));
	}
}

/**
 * Called by the game to draw the bullet to the screen.
 */
void Player::draw()
{
	ofPushStyle();
	
	// Draw outer circle.
	ofSetColor(PLAYER_CIRCLE_R, PLAYER_CIRCLE_G, PLAYER_CIRCLE_B, PLAYER_CIRCLE_A);
	ofNoFill();
	ofCircle(_loc.x, _loc.y, PLAYER_CIRCLE_RAD);
	
	ofPushMatrix();
	ofTranslate(_loc.x, _loc.y);
	ofRotateZ(_rot);
	
	// Draw inner triangle.
	ofSetColor(PLAYER_TRIANGLE_R, PLAYER_TRIANGLE_G, PLAYER_TRIANGLE_B, PLAYER_TRIANGLE_A);
	ofFill();
	float left = -PLAYER_TRIANGLE_WIDTH / 2;
	float right = PLAYER_TRIANGLE_WIDTH / 2;
	float top = -PLAYER_TRIANGLE_HEIGHT / 2;
	float bottom = PLAYER_TRIANGLE_HEIGHT / 2;
	ofTriangle(left, bottom, 0, top, right, bottom);
	
	ofPopMatrix();
	
	// Draw line of path.
	int i;
	for(i = 0; i < _rules->locCount; i++)
	{
		ofPoint loc1 = _rules->locs[i];
		ofPoint loc2 = _rules->locs[(i+1) % _rules->locCount];
		ofLine(loc1.x, loc1.y, loc2.x, loc2.y);
	}
	
	ofPopStyle();
	
	// Draw predicted bullet path projection?
	int ppa = _game->level()->pathProjectionAlpha(_game);
	if(ppa > 0)
		drawPathProjection(PATH_PROJECTION_R, PATH_PROJECTION_G, PATH_PROJECTION_B, PATH_PROJECTION_A * ppa / 255);
}

/**
 * Draws a curved path showing the predicted path of the bullets.
 * The rgba parameters are the color and alpha of the drawn curve.
 * The path will gradually fade out.
 */
void Player::drawPathProjection(int r, int g, int b, int a)
{
	ofPushStyle();
	
	ofPoint lastLoc = _loc;
	float rad = ofDegToRad(_rot);
	float cosRot = cos(rad);
	float sinRot = sin(rad);
	ofPoint vel = ofPoint(
		_rules->fireVel.x * cosRot - _rules->fireVel.y * sinRot,
		_rules->fireVel.x * sinRot + _rules->fireVel.y * cosRot);
	int i;
	int ppCount = _game->level()->pathProjectionCount(_game);
	for(i = 0; i < ppCount; i++)
	{
		ofPoint curLoc = lastLoc + vel;
		ofPoint rawAccel = ofxAccelerometer.getAccelOrientation();
		ofPoint fixedAccel = ofPoint(rawAccel.x, -rawAccel.y);
		vel += fixedAccel * _rules->bulletRules->gravityFactor;
		
		float alphaFactor = (float)(ppCount - i) / ppCount;
		ofSetColor(r, g, b, a * alphaFactor);

		ofLine(lastLoc.x, lastLoc.y, curLoc.x, curLoc.y);
		
		lastLoc = curLoc;
	}
	
	ofPopStyle();
}

/**
 * Should be called when this player is hit by an enemy.
 * For now just kills the player, but maybe in the future
 * it will take multiple hits to kill a player.
 */
void Player::hit()
{
	kill();
}

/**
 * Kills the player, removing it from the game and creating a death effect.
 */
void Player::kill()
{
	_game->delayRemoveGameObject(this);
	
	// If any one player dies, lose the game. Turns out the game is more fun this way.
	_game->loseAtFrame(_game->frames() + LEVEL_LOSE_DELAY);
	
	// Create death effect.
	IntColor startColor(PLAYER_DEATH_R, PLAYER_DEATH_G, PLAYER_DEATH_B, PLAYER_DEATH_A);
	IntColor endColor(PLAYER_DEATH_R, PLAYER_DEATH_G, PLAYER_DEATH_B, 0);
	CircleEffect* effect = new CircleEffect(
		_game,
		PLAYER_DEATH_DURATION,
		_loc,
		_loc,
		startColor,
		endColor,
		0,
		PLAYER_DEATH_RAD);
	_game->delayAddGameObject(shared_ptr<CircleEffect>(effect));
}

/**
 * Returns the static rules governing this player's behavior.
 */
PlayerRules* Player::rules()
{
	return _rules;
}

/**
 * Returns the current location of this player.
 */
ofPoint Player::loc()
{
	return _loc;
}

/**
 * Returns the location of the player's next waypoint target.
 */
ofPoint Player::targetLoc()
{
	return _rules->locs[_targetLocIndex % _rules->locCount];
}

/**
 * Returns the current rotation of the player.
 */
float Player::rot()
{
	return _rot;
}