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

TankPickups::TankPickups(TankPickupID type, const TextureHolder& textures)
	: Entity(1)
	, mType(type)
	, mSprite(textures.get(Table[static_cast<int>(type)].texture))
{
	centreOrigin(mSprite);
}

unsigned int TankPickups::getCategory() const
{
	return static_cast<int>(CategoryID::Pickup);
}

sf::FloatRect TankPickups::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void TankPickups::apply(Tank& player) const
{
	Table[static_cast<int>(mType)].action(player);
}

void TankPickups::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}