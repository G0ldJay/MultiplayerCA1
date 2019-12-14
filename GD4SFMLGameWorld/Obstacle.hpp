#pragma once
#include "Entity.hpp"
#include "Command.hpp"
#include "ResourceIdentifiers.hpp"
#include "ObstacleID.hpp"

#include <SFML/Graphics/Sprite.hpp>

class Obstacle : public Entity
{
public:
	Obstacle(ObstacleID type, const TextureHolder& textures);

	virtual unsigned int	getCategory() const;
	virtual sf::FloatRect	getBoundingRect() const;

protected:
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	ObstacleID mType;
	sf::Sprite mSprite;
};
