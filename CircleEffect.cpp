#include "CircleEffect.h"
#include "Game.h"

/**
 * Creates a new CircleEffect to be placed in the game.
 * @param game The game that will own this effect.
 * @param duration The time in frames that this effect will last.
 * @param startLoc The initial location of this effect.
 * @param endLoc The location that this effect will move to throughout its duration.
 * @param startColor The initial color of the circle.
 * @param endColor The color that the circle will fade to throughout its duration.
 * @param startRadius The initial radius of th circle.
 * @param endRadius The radius that the circle will grow/shrink to throughout its duration.
 */
CircleEffect::CircleEffect(Game* game, int duration, ofPoint startLoc, ofPoint endLoc,
			 IntColor startColor, IntColor endColor, float startRadius, float endRadius)
	: GameObject(game)
{
	_startFrame = game->frames();
	_duration = duration;
	_startLoc = startLoc;
	_endLoc = endLoc;
	_startColor = startColor;
	_endColor = endColor;
	_startRadius = startRadius;
	_endRadius = endRadius;
}

/**
 * Called by the game to update this effect's logic.
 */
void CircleEffect::update()
{
	// Remove from game if enough frames have passed.
	int frame = _game->frames();
	if(frame > _startFrame + _duration)
		_game->delayRemoveGameObject(this);
}

/**
 * Called by the game to draw this CircleEffect to the screen.
 */
void CircleEffect::draw()
{
	// Interpolate location, color, and radius from start to end.
	int frame = _game->frames();
	float f = (frame - _startFrame) / (float)_duration;
	ofPoint loc = _startLoc*(1-f) + _endLoc*f;
	int r = _startColor.r()*(1-f) + _endColor.r()*f;
	int g = _startColor.g()*(1-f) + _endColor.g()*f;
	int b = _startColor.b()*(1-f) + _endColor.b()*f;
	int a = _startColor.a()*(1-f) + _endColor.a()*f;
	float rad = _startRadius*(1-f) + _endRadius*f;
	
	ofPushStyle();
	ofSetColor(r, g, b, a);
	ofCircle(loc.x, loc.y, rad);
	ofPopStyle();
}