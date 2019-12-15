#include "Obstacle.hpp"
#include "DataTables.hpp"
#include "CategoryID.hpp"
#include "CommandQueue.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"
#include "SoundNode.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include<iostream>


namespace
{
	const std::vector<ObstacleData> Table = initializeObstacleData();
}

Obstacle::Obstacle(ObstacleID type, const TextureHolder& textures)
	: Entity(Table[static_cast<int>(type)].hitpoints)
	, mType(type)
	, mSprite(textures.get(Table[static_cast<int>(type)].texture))
	, mExplosion(textures.get(TextureID::Explosion))
	, mShowExplosion(true)
	, mPlayedExplosionSound(false)
{
	std::cout << getCategory() << std::endl;
	mExplosion.setFrameSize(sf::Vector2i(256, 256));
	mExplosion.setNumFrames(16);
	mExplosion.setDuration(sf::seconds(1));

	centreOrigin(mSprite);
	centreOrigin(mExplosion);
}

unsigned int Obstacle::getCategory() const
{
	return static_cast<int>(mType);
}

sf::FloatRect Obstacle::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Obstacle::updateCurrent(sf::Time dt, CommandQueue& commands) 
{
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
}

void Obstacle::playerLocalSound(CommandQueue& commands, SoundEffectID effect)
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

void Obstacle::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isDestroyed() && mShowExplosion)
		target.draw(mExplosion, states);
	else
		target.draw(mSprite, states);
}