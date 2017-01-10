#include "App.h"
#include "AppState.h"
#include "Game.h"
#include "GenericLevel.h"
#include "rules.h"

/**
 * Called by Open Frameworks when the application should be initialized.
 * Initialization should happen here instead of in the constructor.
 */
void App::setup()
{
	ofxAccelerometer.setup();
	ofxMultiTouch.addListener(this);
	ofEnableAlphaBlending();
	ttfontBig.loadFont(ofToDataPath("verdana.ttf"), 50);
	ttfontSmall.loadFont(ofToDataPath("verdana.ttf"), 12);
	
	nextLevel();
}

/**
 * Called by Open Frameworks when the game logic should be updated.
 */
void App::update()
{
	_curState = _nextState; // Switch to next state here.
	if(_curState)
		_curState->update();
}

/**
 * Called by Open Frameworks when the game should be rendered to the display.
 */
void App::draw()
{
	if(_curState)
		_curState->draw();
}

/**
 * Called by Open Frameworks when the application is about to exit.
 */
void App::exit()
{
	
}

/**
 * Restarts the current level.
 */
void App::resetLevel()
{
	if(_curLevel - 1 < LEVEL_COUNT)
	{
		LevelBase* level = new GenericLevel(levels[_curLevel - 1]);
		switchState(shared_ptr<AppState>(new Game(this, shared_ptr<LevelBase>(level))));
	}
	else
	{
		switchState(shared_ptr<AppState>());
	}
}

/**
 * Advances the game to the next level.
 */
void App::nextLevel()
{
	_curLevel++;
	resetLevel();
}

/**
 * Called by Open Frameworks when a touch event has started.
 */
void App::touchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{
	if(_curState)
		_curState->touchDown(x, y, touchId, data);
}

/**
 * Called by Open Frameworks when an existing touch has moved to a new location.
 */
void App::touchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{
	if(_curState)
		_curState->touchMoved(x, y, touchId, data);
}

/**
 * Called by Open Frameworks when a previous touch has been released.
 */
void App::touchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{
	if(_curState)
		_curState->touchUp(x, y, touchId, data);
}

/**
 * Called by Open Frameworks when the user has performed a special OS-defined double-tap.
 */
void App::touchDoubleTap(float x, float y, int touchId, ofxMultiTouchCustomData *data)
{
	if(_curState)
		_curState->touchDoubleTap(x, y, touchId, data);
}

/**
 * Switches the current application state to the specified state,
 * calling the activate() and deactivate() state methods as appropriate.
 */
void App::switchState(shared_ptr<AppState> state)
{
	if(state != _nextState)
	{
		if(_nextState != NULL)
			_nextState->deactivate();
		_nextState = state;
		if(_nextState != NULL)
			_nextState->activate();
	}
}

/**
 * Returns the current 1-based level number.
 */
int App::levelNum()
{
	return _curLevel;
}

/**
 * Returns the Open Frameworks font object to use for large text.
 */
ofTrueTypeFont* App::fontBig()
{
	return &ttfontBig;
}

/**
 * Returns the Open Frameworks font object to use for small text.
 */
ofTrueTypeFont* App::fontSmall()
{
	return &ttfontSmall;
}
