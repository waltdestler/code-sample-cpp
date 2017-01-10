#pragma once

class Game;

/**
 * The base class for objects in the game that are updated and drawn.
 */
class GameObject
{
protected:
	
	Game* _game;
	
public:
	
	GameObject(Game* game){_game = game;}
	
	virtual void update(){}
	virtual void draw(){}
};

typedef vector<shared_ptr<GameObject> >::iterator GameObjectIter;
typedef vector<GameObject*>::iterator GameObjectPtrIter;