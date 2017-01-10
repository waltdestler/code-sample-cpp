#pragma once

#include "GameObject.h"

/**
 * Contains static rules for a particular kind of enemy.
 */
struct EnemyRules
{
	float radius;
	float speed;
};

/**
 * An enemy blob that continuously advances towards the player.
 * If it hits the player, the player is killed.
 */
class Enemy : public GameObject
{
private:
	
	EnemyRules* _rules;
	ofPoint _loc;
	
public:
	
	Enemy(Game* game, EnemyRules* rules, ofPoint loc);
	
	void update();
	void draw();
	
	void hit();
	void kill();
	
	bool isOnScreen();
	EnemyRules* rules();
	ofPoint loc();
	
	bool isEnemy(){return true;}
};