#pragma once

class AppState;

/**
 * The class that controls high-level application logic.
 * Contains the main state system to which update, draw, and input events are sent.
 */
class App : public ofSimpleApp, public ofxMultiTouchListener
{
private:
	
	shared_ptr<AppState> _curState;
	shared_ptr<AppState> _nextState;
	int _curLevel;
	ofTrueTypeFont ttfontBig;
	ofTrueTypeFont ttfontSmall;
	
public:
	
	void setup();
	void update();
	void draw();
	void exit();
	
	void resetLevel();
	void nextLevel();
	
	void touchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL);
	void touchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL);
	void touchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL);
	void touchDoubleTap(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL);
	
	void switchState(shared_ptr<AppState> state);
	
	int levelNum();
	ofTrueTypeFont* fontBig();
	ofTrueTypeFont* fontSmall();
};

