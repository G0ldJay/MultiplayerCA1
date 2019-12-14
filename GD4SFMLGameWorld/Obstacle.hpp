#pragma once

#include "Entity.hpp"
#include "ResourceIdentifiers.hpp"
#include "ObstacleID.hpp"
#include "CommandQueue.hpp"

#include <SFML/Graphics/Sprite.hpp>


class Obstacle : public Entity
{
public:
	Obstacle(ObstacleID type, const TextureHolder& textures);

	virtual unsigned int	getCategory() const;
	virtual sf::FloatRect	getBoundingRect() const;
	int						getDamage() const;


private:
	virtual void			updateCurrent(sf::Time dt, CommandQueue& commands);
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	ObstacleID			mType;
	sf::Sprite				mSprite;
	sf::Vector2f			mTargetDirection;
};