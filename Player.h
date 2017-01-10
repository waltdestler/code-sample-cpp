#pragma once

#include "GameObject.h"

struct BulletRules;

/**
 * Contains static rules for a particular kind of player.
 */
struct PlayerRules
{
	ofPoint locs[12]; // Waypoints that the player will automatically follow.
	int locCount; // Number of waypoints.
	float speed; // The speed at which the player will travel between waypoints.
	float initRot; // The initial rotation of the player.
	float rotVel; // The initial rotational velocity of the player.
	int fireInterval; // The interval between bullet spawns.
	ofPoint fireVel; // The initial velocity of the fired bullets.
	BulletRules* bulletRules; // The static rules for the fired bullets.
};

/**
 * A single player "ship" in the game that moves and rotates along a
 * predefined path and fires bullets at a regular interval.
 */
class Player : public GameObject
{
private:
	
	PlayerRules* _rules;
	ofPoint _loc;
	int _targetLocIndex;
	float _rot;
	
public:
	
	Player(Game* game, PlayerRules* rules);
	
	void update();
	void draw();
	void drawPathProjection(int r, int g, int b, int a);
	
	void hit();
	void kill();
	
	PlayerRules* rules();
	ofPoint loc();
	ofPoint targetLoc();
	float rot();
	
	bool isPlayer(){return true;}
};