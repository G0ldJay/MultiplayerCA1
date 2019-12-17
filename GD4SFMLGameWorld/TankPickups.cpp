//Dylan Reilly D00194504 
//Jason Lynch D00137655
#include"TankPickups.hpp"
#include "DataTables.hpp"
#include "CategoryID.hpp"
#include "CommandQueue.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"

#include <SFML/Graphics/RenderTarget.hpp>


namespace
{
	const std::vector<TankPickupData> Table = initializeTankPickupData();
}

TankPickups::TankPickups(TankPickupID type, const TextureHolder& textures)//Created by me for handling tank pickups. Based off pickup.cpp - Jason Lynch
	: Entity(1)
	, mType(type)
	, mSprite(textures.get(Table[static_cast<int>(type)].texture))
{
	centreOrigin(mSprite);
}

unsigned int TankPickups::getCategory() const //Return category as pickup - Jason Lynch
{
	return static_cast<int>(CategoryID::Pickup);
}

sf::FloatRect TankPickups::getBoundingRect() const //REturn bounding rectangle for collision - Jason Lynch 
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void TankPickups::apply(Tank& player) const //Apply to tank - Jason Lynch 
{
	Table[static_cast<int>(mType)].action(player);
}

void TankPickups::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const //Draw sprite
{
	target.draw(mSprite, states);
}