#include "ObstacleTest.hpp"
#include "ResourceHolder.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "Pickup.hpp"
#include "CommandQueue.hpp"
#include "SoundNode.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include "SFML/Graphics/RenderStates.hpp"
#include "TextureID.hpp"
#include "ObstacleID.hpp"
#include "ProjectileID.hpp"
#include "PickupID.hpp"

#include <cmath>

namespace
{
	const std::vector<ObstacleData> Table = initializeObstacleData();
}

ObstacleTest::ObstacleTest(ObstacleID type, const TextureHolder& textures, const FontHolder& fonts, const TextureID deathAnimation, sf::Vector2i frameSize, int numberOfFrames, int seconds, sf::Vector2f scale)
	: Entity(Table[static_cast<int>(type)].hitpoints, CategoryID::Collidable)
	, mType(type)
	, mSprite(textures.get(Table[static_cast<int>(type)].texture)/*, Table[static_cast<int>(type)].textureRect*/)
	, mExplosion(textures.get(deathAnimation))
	, mShowExplosion(true)
	, mPlayedExplosionSound(false)
	, mIsMarkedForRemoval(false)
	, mHealthDisplay(nullptr)
{
	mExplosion.setFrameSize(frameSize);
	mExplosion.setNumFrames(numberOfFrames);
	mExplosion.setScale(scale);
	mExplosion.setDuration(sf::seconds(seconds));

	centreOrigin(mSprite);
	centreOrigin(mExplosion);

	std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts, "", sf::Color::Black));
	mHealthDisplay = healthDisplay.get();
	mHealthDisplay->setScale(1.5f, 1.5f);
	attachChild(std::move(healthDisplay));

	updateTexts();
}

unsigned int ObstacleTest::getCategory() const
{
		return static_cast<int>(CategoryID::Collidable);
}

unsigned int ObstacleTest::getType() const {
	return static_cast<int>(mType);
}

sf::FloatRect ObstacleTest::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool ObstacleTest::isMarkedForRemoval() const
{
	return isDestroyed() && (mExplosion.isFinished() || !mShowExplosion);
}

unsigned int ObstacleTest::getDamage() const //Returns damage obstacle dooes on collision - Jason Lynch
{
	return Table[static_cast<int>(mType)].damage;
}

void ObstacleTest::playerLocalSound(CommandQueue& commands, SoundEffectID effect)
{
	sf::Vector2f worldPosition = getWorldPosition();

	Command command;
	command.category = static_cast<int>(CategoryID::SoundEffect);
	command.action = derivedAction<SoundNode>(
		[effect, worldPosition](SoundNode& node, sf::Time)
		{
			node.playSound(effect, worldPosition);
		});
	commands.push(command);
}

void ObstacleTest::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isDestroyed() && mShowExplosion)
		target.draw(mExplosion, states);
	else
		target.draw(mSprite, states);
}

void ObstacleTest::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	// Entity has been destroyed: Possibly drop pickup, mark for removal
	if (isDestroyed())
	{

		mExplosion.update(dt);
		//mIsMarkedForRemoval = true;
		//Play explosion sound
		if (!mPlayedExplosionSound)
		{
			SoundEffectID soundEffect = (randomInt(2) == 0) ? SoundEffectID::Explosion1 : SoundEffectID::Explosion2;
			playerLocalSound(commands, soundEffect);

			mPlayedExplosionSound = true;
		}
		return;
	}

	Entity::updateCurrent(dt, commands);

	// Update texts
	updateTexts();
}

void ObstacleTest::updateTexts()
{
	mHealthDisplay->setString(toString(getHitpoints()) + " HP");
	mHealthDisplay->setPosition(0.f, 0.f);
	mHealthDisplay->setRotation(-getRotation());
}
