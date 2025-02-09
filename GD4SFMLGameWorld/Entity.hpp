//Jason Lynch - D00137655
//Dylan Reilly D00194504
#pragma once
#include "SFML/Graphics.hpp"
#include "SceneNode.hpp"
#include "CommandQueue.hpp"

class Entity : public SceneNode
{
public:
	Entity(int hitpoints);
	Entity(int hitpoints, CategoryID category); //Overloaded Enity to pass down category to sceneNode - Jason Lynch
	void setVelocity(sf::Vector2f velocity);
	void setVelocity(float vx, float vy);
	void accelerate(sf::Vector2f velocity);
	void accelerate(float vx, float vy);
	void turn(float x);
	sf::Vector2f getVelocity() const;

	int getHitpoints() const;
	void repair(int points);
	void damage(int points);
	void destroy();
	virtual bool isDestroyed() const;

protected:
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);

private:
	sf::Vector2f mVelocity;
	float direction;
	int mHitpoints;
};