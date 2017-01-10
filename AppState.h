#pragma once

/**
 * The base class for states usable in the App object's FSM.
 * Contains activate, deactivate, update, draw, and input events.
 */
class AppState
{
public:
	
	virtual void activate(){};
	virtual void deactivate(){};
	virtual void update(){};
	virtual void draw(){};
	
	virtual void touchDown(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL){};
	virtual void touchMoved(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL){};
	virtual void touchUp(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL){};
	virtual void touchDoubleTap(float x, float y, int touchId, ofxMultiTouchCustomData *data = NULL){};
};
