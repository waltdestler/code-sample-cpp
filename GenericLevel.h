#pragma once

#include "LevelBase.h"

class Game;
struct PlayerRules;
struct EnemyRules;

/**
 * Contains the rules for a particular level.
 */
struct GenericLevelRules
{
	PlayerRules* playerRules[4]; // The rules for each player ship.
	int playerCount; // The number of player ships.
	int enemyCount; // The number of initial enemies.
	EnemyRules* enemyRules; // The rules for the enemies.
	const char* instructions; // The instruction text to show to the player, or NULL to display no instructions.
	int gravityArrowAlpha; // The alpha translucency with which to draw the gravity arrow.
	int pathProjectionAlpha; // The alpha translucency with which to draw the bullet path projection.
	int pathProjectionCount; // The number of steps to iterate the bullet path projection into the future.
};

/**
 * Implements the LevelBase class using a GenericLevelRules structure.
 */
class GenericLevel : public LevelBase
{
private:
	
	GenericLevelRules* _rules;
	
public:
	
	GenericLevel(GenericLevelRules* rules);
	
	virtual void populateGame(Game* game);
	virtual float levelTextRot(Game* game);
	virtual const char* instructions(Game* game);
	virtual int gravityArrowAlpha(Game* game);
	virtual int pathProjectionAlpha(Game* game);
	virtual int pathProjectionCount(Game* game);
};