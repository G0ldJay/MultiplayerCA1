#include "Obstacle.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"
#include "EmitterNode.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include <SFML/Graphics/RenderStates.hpp>

#include <cmath>
#include <cassert>

#include <iostream>

namespace
{
	const std::vector<ObstacleData> Table = initializeObstacleData();
}

Obstacle::Obstacle(ObstacleID type, const TextureHolder& textures)
	: Entity(1)
	, mType(type)
	, mSprite(textures.get(Table[static_cast<int>(type)].texture))
{

}

unsigned int Obstacle::getCategory() const
{
	return 0;
}

sf::FloatRect Obstacle::getBoundingRect() const
{
	return sf::FloatRect();
}


int Obstacle::getDamage() const
{
	return Table[static_cast<int>(mType)].damage;
}

void Obstacle::updateCurrent(sf::Time dt, CommandQueue& commands)
{
}

void Obstacle::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}
