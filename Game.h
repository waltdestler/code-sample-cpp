#pragma once

#include "AppState.h"
#include "GameObject.h"

class App;
class LevelBase;
class GameObject;
class Player;
class Bullet;
class Enemy;

/**
 * An application state that implements the high-level game logic.
 * A single Game object exists for the duration of a level.
 */
class Game : public AppState
{
private:
	
	App* _app;
	shared_ptr<LevelBase> _level;
	vector<shared_ptr<GameObject> > _gobjects;
	vector<shared_ptr<GameObject> > _gobjectsToAdd;
	vector<GameObject*> _gobjectsToRemove;
	vector<Player*> _players;
	vector<Bullet*> _bullets;
	vector<Enemy*> _enemies;
	int _frames;
	int _resetAtFrame;
	int _nextAtFrame;
	int _winAtFrame;
	int _loseAtFrame;
	
public:
	
	Game(App* app, shared_ptr<LevelBase> level);
	
	void addGameObject(shared_ptr<GameObject> gobject);
	void delayAddGameObject(shared_ptr<GameObject> gobject);
	void removeGameObject(GameObject* gobject);
	void delayRemoveGameObject(GameObject* gobject);
	bool isMarkedForRemoval(GameObject* gobject);
	GameObjectIter gameObjectsBegin();
	GameObjectIter gameObjectsEnd();
	vector<Player*>::iterator playersBegin();
	vector<Player*>::iterator playersEnd();
	vector<Bullet*>::iterator bulletsBegin();
	vector<Bullet*>::iterator bulletsEnd();
	vector<Enemy*>::iterator enemiesBegin();
	vector<Enemy*>::iterator enemiesEnd();
	
	void activate();
	void draw();
	void update();
	
	void win();
	void lose();
	void resetAtFrame(int frame);
	void nextAtFrame(int frame);
	void winAtFrame(int frame);
	void loseAtFrame(int frame);
	
	void drawArrow(ofPoint start, float deg, float length, const char* text=NULL);
	
	void touchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data);
	void touchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data);
	void touchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data);
	void touchDoubleTap(float x, float y, int touchId, ofxMultiTouchCustomData *data);
	
	App* app();
	LevelBase* level();
	int frames();
};