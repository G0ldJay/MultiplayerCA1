#include "Aircraft.hpp"
#include "ResourceHolder.hpp"
#include "CategoryID.hpp"
#include"DataTables.hpp"
#include"TextNode.hpp"
#include"Utility.hpp"

namespace 
{
	const std::vector<AircraftData> Table = initializeAircraftData();
}

TextureID toTextureID(AircraftID type)
{
	switch (type)
	{
	case AircraftID::Eagle:
		return TextureID::Eagle;

	case AircraftID::Raptor:
		return TextureID::Raptor;
	}
	return TextureID::Eagle;
}

Aircraft::Aircraft(AircraftID type, const TextureHolder& textures, const FontHolder& fonts) : Entity(Table[static_cast<int>(type)].hitpoints), mType(type), mSprite(textures.get(toTextureID(type))), mHealthDisplay(nullptr), mMissileDisplay(nullptr)
{
	sf::FloatRect bounds = mSprite.getLocalBounds();
	mSprite.setOrigin(bounds.width / 2.f, bounds.height / 2.f);

	std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts, ""));
	mHealthDisplay = healthDisplay.get();
	attachChild(std::move(healthDisplay));

	if (getCategory() == static_cast<int>(CategoryID::PlayerAircraft)) {
		std::unique_ptr<TextNode> missileDisplay(new TextNode(fonts, ""));
		missileDisplay->setPosition(0, 70);
		mMissileDisplay = missileDisplay.get();
		attachChild(std::move(missileDisplay));
	}
}

unsigned int Aircraft::getCategory() const
{
	switch (mType)
	{
	case AircraftID::Eagle:
		return static_cast<int>(CategoryID::PlayerAircraft);

	default:
		return static_cast<int>(CategoryID::EnemyAircraft);
	}
}

void Aircraft::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

void Aircraft::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	updateTexts();
}

void Aircraft::updateMovementPattern(sf::Time dt)
{
	const std::vector<Direction>& directions = Table[static_cast<int>(mType)].directions;

	if (!directions.empty()) {
		if (mTavelledDistance > directions[mDirectionIndex].distance) {
			mDirectionIndex = (mDirectionIndex + 1) % directions.size();
			mTavelledDistance = 0.f;
		}

		float radians = toRadians(directions[mDirectionIndex].angle + 90.f);
		float xVel = getMaxSpeed() * std::cos(radians);
		float yVel = getMaxSpeed() * std::sin(radians);

		setVelocity(xVel, yVel);

		mTavelledDistance += getMaxSpeed() * dt.asSeconds();
	}
}

void Aircraft::updateTexts() {
	mHealthDisplay->setString(toString(getHitpoints() + " HP"));
	mHealthDisplay->setPosition(0.f, 50.f);
	mHealthDisplay->setRotation(-getRotation());

	if (mMissileDisplay) {
		if (mMissileAmmo == 0) {
			mMissileDisplay->setString("");
		}
		else {
			mMissileDisplay->setString("M: " + toString(mMissileAmmo));
		}
	}
}
