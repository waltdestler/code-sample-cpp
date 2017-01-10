#pragma once

class Game;

/**
 * The base class for level objects. Level objects
 * describe to the Game object the particular rules
 * and initial state of the level.
 */
class LevelBase
{
public:
	
	virtual void populateGame(Game* game) = 0;
	virtual float levelTextRot(Game* game){return 0;}
	virtual const char* instructions(Game* game){return NULL;}
	virtual int gravityArrowAlpha(Game* game){return 0;}
	virtual int pathProjectionAlpha(Game* game){return 0;}
	virtual int pathProjectionCount(Game* game){return 0;}
};