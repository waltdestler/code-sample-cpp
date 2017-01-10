#include "Game.h"
#include "rules.h"
#include "GameObject.h"
#include "App.h"
#include "LevelBase.h"
#include "IntColor.h"
#include "CircleEffect.h"
#include "Player.h"
#include "Bullet.h"
#include "Enemy.h"
#include "Dust.h"

/**
 * Constructs a new Game object, initializing it to the specified level object.
 * The level's populateGame() method will be called to populate the
 * actual GameObjects in this Game.
 * @param app The main application object.
 * @param level The object used to initialize the contents of this Game object.
 */
Game::Game(App* app, shared_ptr<LevelBase> level)
{
	_app = app;	
	_level = level;
	_frames = 0;
	_resetAtFrame = -1;
	_nextAtFrame = -1;
	_winAtFrame = -1;
	_loseAtFrame = -1;
	
	// Create dust.
	int i;
	for(i = 0; i < DUST_COUNT; i++)
	{
		int x = rand() % SCREEN_WIDTH;
		int y = rand() % SCREEN_HEIGHT;
		Dust* dust = new Dust(this, ofPoint(x, y), ofPoint(0, 0));
		addGameObject(shared_ptr<Dust>(dust));
	}
	
	// Populate game. This will callback to this Game object's various methods.
	_level->populateGame(this);
	
	// Create intro effect.
	IntColor color(LEVEL_INTRO_R, LEVEL_INTRO_G, LEVEL_INTRO_B, LEVEL_INTRO_A);
	CircleEffect* effect = new CircleEffect(
		this,
		LEVEL_INTRO_DURATION,
		SCREEN_CENTER,
		SCREEN_CENTER,
		color,
		color,
		LEVEL_INTRO_RAD,
		0);
	delayAddGameObject(shared_ptr<CircleEffect>(effect));
}

/**
 * Immediately adds the specified GameObject to the game.
 */
void Game::addGameObject(shared_ptr<GameObject> gobject)
{
	_gobjects.push_back(gobject);
	
	// Use RTTI to determine type and add to appropriate lists.
	
	Player* asPlayer = dynamic_cast<Player*>(gobject.get());
	if(asPlayer != NULL)
		_players.push_back(asPlayer);
	
	Bullet* asBullet = dynamic_cast<Bullet*>(gobject.get());
	if(asBullet != NULL)
		_bullets.push_back(asBullet);
	
	Enemy* asEnemy = dynamic_cast<Enemy*>(gobject.get());
	if(asEnemy != NULL)
		_enemies.push_back(asEnemy);
}

/**
 * Adds the specified GameObject at the end of the current frame.
 */
void Game::delayAddGameObject(shared_ptr<GameObject> gobject)
{
	_gobjectsToAdd.push_back(gobject);
}

/**
 * Immediately removes the specified GameObject from the game.
 */
void Game::removeGameObject(GameObject* gobject)
{
	// Use RTTI to determine type and remove from appropriate lists.
	
	Player* asPlayer = dynamic_cast<Player*>(gobject);
	if(asPlayer != NULL)
	{
		vector<Player*>::iterator found = find(_players.begin(), _players.end(), asPlayer);
		_players.erase(found);
	}
	
	Bullet* asBullet = dynamic_cast<Bullet*>(gobject);
	if(asBullet != NULL)
	{
		vector<Bullet*>::iterator found = find(_bullets.begin(), _bullets.end(), asBullet);
		_bullets.erase(found);
	}
	
	Enemy* asEnemy = dynamic_cast<Enemy*>(gobject);
	if(asEnemy != NULL)
	{
		vector<Enemy*>::iterator found = find(_enemies.begin(), _enemies.end(), asEnemy);
		_enemies.erase(found);
	}
	
	GameObjectIter iter;
	for(iter = _gobjects.begin(); iter != _gobjects.end(); ++iter)
	{
		if(iter->get() == gobject)
		{
			_gobjects.erase(iter);
			break;
		}
	}
}

/**
 * Removes the specified GameObject at the end of the current frame.
 */
void Game::delayRemoveGameObject(GameObject* gobject)
{
	_gobjectsToRemove.push_back(gobject);
}

/**
 * Returns whether the specified GameObject is currently marked for removal from the game.
 */
bool Game::isMarkedForRemoval(GameObject* gobject)
{
	vector<GameObject*>::iterator iter;
	for(iter = _gobjectsToRemove.begin(); iter != _gobjectsToRemove.end(); ++iter)
	{
		if(*iter == gobject)
			return true;
	}
	return false;
}

/**
 * Returns an iterator pointing to the beginning of the GameObjects list.
 */
GameObjectIter Game::gameObjectsBegin()
{
	return _gobjects.begin();
}

/**
 * Returns an iterator pointing to the end of the GameObjects list.
 */
GameObjectIter Game::gameObjectsEnd()
{
	return _gobjects.end();
}

/**
 * Returns an iterator pointing to the beginning of the Players list.
 */
vector<Player*>::iterator Game::playersBegin()
{
	return _players.begin();
}

/**
 * Returns an iterator pointing to the end of the Players list.
 */
vector<Player*>::iterator Game::playersEnd()
{
	return _players.end();
}

/**
 * Returns an iterator pointing to the beginning of the Bullets list.
 */
vector<Bullet*>::iterator Game::bulletsBegin()
{
	return _bullets.begin();
}

/**
 * Returns an iterator pointing to the end of the Bullets list.
 */
vector<Bullet*>::iterator Game::bulletsEnd()
{
	return _bullets.end();
}

/**
 * Returns an iterator pointing to the beginning of the Enemies list.
 */
vector<Enemy*>::iterator Game::enemiesBegin()
{
	return _enemies.begin();
}

/**
 * Returns an iterator pointing to the beginning of the Enemies list.
 */
vector<Enemy*>::iterator Game::enemiesEnd()
{
	return _enemies.end();
}

/**
 * Called when this Game object becomes the active application state.
 */
void Game::activate()
{
	// Setup Open Frameworks for this state.
	ofBackground(0, 0, 0);
	ofSetBackgroundAuto(true);
	ofSetFrameRate(60);
}

/**
 * Called by the application when the game logic should be updated.
 */
void Game::update()
{
	_frames++;
	
	// Update all game objects.
	GameObjectIter iter;
	for(iter = _gobjects.begin(); iter != _gobjects.end(); ++iter)
		(*iter)->update();
	
	// Add any game objects delayed for addition.
	for(iter = _gobjectsToAdd.begin(); iter != _gobjectsToAdd.end(); ++iter)
		addGameObject(*iter);
	_gobjectsToAdd.clear();
	
	// Remove any game objects delayed for removal.
	GameObjectPtrIter iter2;
	for(iter2 = _gobjectsToRemove.begin(); iter2 != _gobjectsToRemove.end(); ++iter2)
		removeGameObject(*iter2);
	_gobjectsToRemove.clear();
	
	// Winning, losing, resetting, and moving to the next
	// level are detected and then scheduled to happen at
	// a certain frame in the future.
	// Check here to see if that future frame is now.
	if(_frames == _winAtFrame)
		win();
	if(_frames == _loseAtFrame)
		lose();
	if(_frames == _resetAtFrame)
		_app->resetLevel();
	if(_frames == _nextAtFrame)
		_app->nextLevel();
	
	// Detect win condition (no enemies left) and if so schedule a win.
	if(_enemies.size() == 0 && _winAtFrame < 0 && _loseAtFrame < 0)
		winAtFrame(_frames + LEVEL_WIN_DELAY);
	
	// Detect lose condition (no player ships left) and if so schedule a loss.
	if(_players.size() == 0 && _loseAtFrame < 0 && _winAtFrame < 0)
		loseAtFrame(_frames + LEVEL_LOSE_DELAY);
}

/**
 * Called by the application to render the game to the screen.
 */
void Game::draw()
{
	// Draw instructions.
	const char* instr = _level->instructions(this);
	if(instr != NULL)
	{
		int frames = min(max(0, _frames-LEVEL_TEXT_DURATION), LEVEL_TEXT_DURATION);
		float floatAlpha = (float)frames / LEVEL_TEXT_DURATION;
		float scaledFloatAlpha = floatAlpha * LEVEL_INSTRUCTIONS_A / 255;
		int intAlpha = scaledFloatAlpha * 255;
		
		float width = _app->fontSmall()->stringWidth(instr);
		float height = _app->fontSmall()->stringHeight(instr);
		float xOffset = -width/2;
		float yOffset = -height/2 + _app->fontSmall()->getLineHeight()/2;
		
		ofPushStyle();
		ofPushMatrix();
		ofTranslate(LEVEL_INSTRUCTIONS_X, LEVEL_INSTRUCTIONS_Y);
		ofRotateZ(_level->levelTextRot(this));
		ofTranslate(xOffset, yOffset);
		ofSetColor(LEVEL_INSTRUCTIONS_R, LEVEL_INSTRUCTIONS_G, LEVEL_INSTRUCTIONS_B, intAlpha);
		_app->fontSmall()->drawString(instr, 0, 0);
		ofPopMatrix();
		ofPopStyle();
	}
	
	// Draw gravity vector.
	int gravAlpha = _level->gravityArrowAlpha(this);
	if(gravAlpha > 0)
	{
		ofPoint rawAccel = ofxAccelerometer.getAccelOrientation();
		ofPoint fixedAccel(rawAccel.x, -rawAccel.y);
		float rad = atan2(fixedAccel.y, fixedAccel.x);
		float length = sqrt(fixedAccel.x*fixedAccel.x + fixedAccel.y*fixedAccel.y);
		ofPushStyle();
		ofSetColor(GRAVITY_ARROW_R, GRAVITY_ARROW_G, GRAVITY_ARROW_B, GRAVITY_ARROW_A * gravAlpha / 255);
		drawArrow(SCREEN_CENTER, ofRadToDeg(rad), length * GRAVITY_ARROW_LENGTH, GRAVITY_ARROW_TEXT);
		ofPopStyle();
	}
	
	// Draw game objects.
	GameObjectIter iter;
	for(iter = _gobjects.begin(); iter != _gobjects.end(); ++iter)
		(*iter)->draw();
	
	// Draw level text.
	if(_frames < LEVEL_TEXT_DURATION)
	{
		const int LEVEL_STR_LEN = 20;
		char str[20];
		snprintf(str, LEVEL_STR_LEN, LEVEL_TEXT_FORMAT, _app->levelNum(), LEVEL_COUNT);
		
		int invFrames = LEVEL_TEXT_DURATION - _frames;
		float floatAlpha = (float)invFrames / LEVEL_TEXT_DURATION;
		float scaledFloatAlpha = floatAlpha * LEVEL_TEXT_A / 255;
		int intAlpha = scaledFloatAlpha * 255;
		
		float width = _app->fontBig()->stringWidth(str);
		float height = _app->fontBig()->stringHeight(str);
		float xOffset = -width/2;
		float yOffset = -height/2 + _app->fontBig()->getLineHeight()/2;
		
		ofPushStyle();
		ofPushMatrix();
		ofTranslate(LEVEL_TEXT_X, LEVEL_TEXT_Y);
		ofRotateZ(_level->levelTextRot(this));
		ofTranslate(xOffset, yOffset);
		ofSetColor(LEVEL_TEXT_R, LEVEL_TEXT_G, LEVEL_TEXT_B, intAlpha);
		_app->fontBig()->drawString(str, 0, 0);
		ofPopMatrix();
		ofPopStyle();
	}
}

/**
 * Called to trigger the win effect and then advance to the next level
 * once the effect is complete.
 */
void Game::win()
{
	nextAtFrame(_frames + WIN_DURATION);
	
	// Create win effect.
	IntColor color(WIN_R, WIN_G, WIN_B, WIN_A);
	CircleEffect* effect = new CircleEffect(
		this,
		WIN_DURATION,
		SCREEN_CENTER,
		SCREEN_CENTER,
		color,
		color,
		0,
		WIN_RAD);
	delayAddGameObject(shared_ptr<CircleEffect>(effect));
}

/**
 * Called to trigger the lose effect and then reset the level once
 * the effect is complete.
 */
void Game::lose()
{
	resetAtFrame(_frames + LOSE_DURATION);
	
	// Create lose effect.
	IntColor color(LOSE_R, LOSE_G, LOSE_B, LOSE_A);
	CircleEffect* effect = new CircleEffect(
		this,
		LOSE_DURATION,
		SCREEN_CENTER,
		SCREEN_CENTER,
		color,
		color,
		0,
		LOSE_RAD);
	delayAddGameObject(shared_ptr<CircleEffect>(effect));
}

/**
 * Resets the level at the end of the specified frame.
 */
void Game::resetAtFrame(int frame)
{
	_resetAtFrame = frame;
}

/**
 * Moves to the next level at the end of the specified frame.
 */
void Game::nextAtFrame(int frame)
{
	_nextAtFrame = frame;
}

/**
 * Wins the game at the end of the specified frame.
 */
void Game::winAtFrame(int frame)
{
	_winAtFrame = frame;
}

/**
 * Loses the game at the end of the specified frame.
 */
void Game::loseAtFrame(int frame)
{
	_loseAtFrame = frame;
}

/**
 * Draws the gravity arrow.
 * @param start The starting point of the arrow.
 * @param deg The direction (in degrees) in which the arrow should point.
 * @param length The length of the arrow.
 * @param text The text to display at the end of the arrow, or NULL to display no text.
 */
void Game::drawArrow(ofPoint start, float deg, float length, const char* text)
{
	ofPushMatrix();
	ofTranslate(start.x, start.y);
	ofRotateZ(deg);
	
	ofLine(0, 0, length, 0);
	ofLine(length-10, -10, length, 0);
	ofLine(length-10, 10, length, 0);
	
	if(text != NULL)
	{
		float width = _app->fontSmall()->stringWidth(text);
		float height = _app->fontSmall()->stringHeight(text);
		float xOffset = -width/2;
		float yOffset = -height/2 + _app->fontSmall()->getLineHeight();
		
		ofPushMatrix();
		ofTranslate(length, 0);
		ofRotateZ(-90);
		ofTranslate(xOffset, yOffset);
		_app->fontSmall()->drawString(text, 0, 0);
		ofPopMatrix();
	}
	
	ofPopMatrix();
}

/**
 * Called by the application when a touch event has started.
 */
void Game::touchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{
	
}

/**
 * Called by the application when an existing touch has moved.
 */
void Game::touchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{
	
}

/**
 * Called by the application when a previous touch has ended.
 */
void Game::touchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{
	
}

/**
 * Called by the application when an OS-defined double-tap has been detected.
 */
void Game::touchDoubleTap(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{
	
}

/**
 * Returns the App object associated with this game.
 */
App* Game::app()
{
	return _app;
}

/**
 * Returns the number of frames that have elapsed during the duration of this game.
 */
int Game::frames()
{
	return _frames;
}

/**
 * Returns the number of the current level.
 */
LevelBase* Game::level()
{
	return _level.get();
}