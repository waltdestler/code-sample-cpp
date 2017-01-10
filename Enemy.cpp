#include "Enemy.h"
#include "rules.h"
#include "Game.h"
#include "Player.h"
#include "IntColor.h"
#include "CircleEffect.h"
#include <float.h>

/**
 * Constructs a new Enemy to be placed in the game.
 * @param game The game object that will own this Enemy.
 * @param rules The static rules governing the behavior of this Enemy.
 * @param loc The initial location of this Enemy.
 */
Enemy::Enemy(Game* game, EnemyRules* rules, ofPoint loc)
	: GameObject(game)
{
	_rules = rules;
	_loc = loc;
}

/**
 * Called by the game to update this Enemy's logic.
 */
void Enemy::update()
{
	// Find closest player.
	Player* closestPlayer = NULL;
	float closestDistSquared = FLT_MAX;
	vector<Player*>::iterator iter;
	for(iter = _game->playersBegin(); iter != _game->playersEnd(); iter++)
	{
		Player* player = *iter;
		ofPoint diff = player->loc() - _loc;
		float distSquared = diff.x*diff.x + diff.y*diff.y;
		if(distSquared < closestDistSquared)
		{
			closestPlayer = player;
			closestDistSquared = distSquared;
		}
	}
	
	// If we found a closest player, move towards that player.
	if(closestPlayer != NULL && !_game->isMarkedForRemoval(closestPlayer))
	{
		// Move towards player.
		ofPoint diff = closestPlayer->loc() - _loc;
		float length = sqrt(diff.x*diff.x + diff.y*diff.y);
		if(length > 0)
		{
			ofPoint normalized = diff / length;
			ofPoint vel = normalized * _rules->speed;
			_loc += vel;
		}
		
		// Check for collision with player.
		diff = _loc - closestPlayer->loc();
		length = sqrt(diff.x*diff.x + diff.y*diff.y);
		if(length < _rules->radius + PLAYER_COLLISION_RAD)
			closestPlayer->hit();
	}
}

/**
 * Called by the game to render this Enemy to the screen.
 */
void Enemy::draw()
{
	ofPushStyle();
	ofSetColor(ENEMY_R, ENEMY_G, ENEMY_B, ENEMY_A);
	ofCircle(_loc.x, _loc.y, _rules->radius);
	ofPopStyle();
}

/**
 * Called when this Enemy is hit by a player's bullet.
 * For now the enemy is instantly killed, but in the future
 * enemies may take multiple hits to kill.
 */
void Enemy::hit()
{
	kill();
}

/**
 * Called to kill this enemy, removing it from the game
 * and creating a death effect.
 */
void Enemy::kill()
{
	_game->delayRemoveGameObject(this);
	
	// Create death effect.
	IntColor startColor(ENEMY_DEATH_R, ENEMY_DEATH_G, ENEMY_DEATH_B, ENEMY_DEATH_A);
	IntColor endColor(ENEMY_DEATH_R, ENEMY_DEATH_G, ENEMY_DEATH_B, 0);
	CircleEffect* effect = new CircleEffect(
		_game,
		ENEMY_DEATH_DURATION,
		_loc,
		_loc,
		startColor,
		endColor,
		_rules->radius,
		_rules->radius * ENEMY_DEATH_RAD_FACTOR);
	_game->delayAddGameObject(shared_ptr<CircleEffect>(effect));
}

/**
 * Returns whether this enemy is currently on the screen.
 * Enemies off the screen cannot be hit by the player's bullets.
 */
bool Enemy::isOnScreen()
{
	return _loc.x + _rules->radius > 0 &&
	       _loc.x - _rules->radius < SCREEN_WIDTH &&
	       _loc.y + _rules->radius > 0 &&
	       _loc.y - _rules->radius < SCREEN_HEIGHT;
}

/**
 * Returns the static rules for this Enemy.
 */
EnemyRules* Enemy::rules()
{
	return _rules;
}

/**
 * Returns the current location of this Enemy.
 */
ofPoint Enemy::loc()
{
	return _loc;
}