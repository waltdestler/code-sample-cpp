#include "GenericLevel.h"
#include "Game.h"
#include "Player.h"
#include "rules.h"
#include "Enemy.h"

/**
 * Creates a new GenericLevel object that prodided information to
 * the Game object based on the specified rules.
 */
GenericLevel::GenericLevel(GenericLevelRules* rules)
{
	_rules = rules;
}

/**
 * Called by the Game to populate the GameObjects.
 * Calls the Game's addGameObject method to populate players and enemies.
 */
void GenericLevel::populateGame(Game* game)
{
	// Create players.
	int i;
	for(i = 0; i < _rules->playerCount; i++)
	{
		Player* player = new Player(game, _rules->playerRules[i]);
		game->addGameObject(shared_ptr<Player>(player));
	}
	
	// Create enemies.
	for(i = 0; i < _rules->enemyCount; i++)
	{
		int deg = rand() % 360;
		float rad = ofDegToRad(deg);
		int dist = rand() % (ENEMY_MAX_DIST - ENEMY_MIN_DIST) + ENEMY_MIN_DIST;
		ofPoint offset(dist * cos(rad), dist * sin(rad));
		
		Enemy* enemy = new Enemy(game, _rules->enemyRules, offset + SCREEN_CENTER);
		game->addGameObject(shared_ptr<Enemy>(enemy));
	}
}

/**
 * Returns the rotation of the level text to display.
 */
float GenericLevel::levelTextRot(Game* game)
{
	// Always orient the level text with the current rotation of the first player.
	vector<Player*>::iterator iter;
	for(iter = game->playersBegin(); iter != game->playersEnd(); iter++)
		return (*iter)->rot();
	return 0;
}

/**
 * Returns the instructions to display to the player, or NULL
 * to display no instructions.
 */
const char* GenericLevel::instructions(Game* game)
{
	return _rules->instructions;
}

/**
 * Returns the alpha translucency value for the gravity arrow.
 */
int GenericLevel::gravityArrowAlpha(Game* game)
{
	return _rules->gravityArrowAlpha;
}

/**
 * Returns the alpha translucency value for the bullet path projection.
 */
int GenericLevel::pathProjectionAlpha(Game* game)
{
	return _rules->pathProjectionAlpha;
}

/**
 * Returns the number of times to iterate the bullet path projection.
 */
int GenericLevel::pathProjectionCount(Game* game)
{
	return _rules->pathProjectionCount;
}