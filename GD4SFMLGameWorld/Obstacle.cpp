#include "Obstacle.hpp"
#include "DataTables.hpp"
#include "CategoryID.hpp"
#include "CommandQueue.hpp"
#include "Utility.hpp"
#include "ResourceHolder.hpp"
#include "SoundNode.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

#include<iostream>

//Created by Jason Lynch

namespace
{
	const std::vector<ObstacleData> Table = initializeObstacleData(); //Kinda global but not really only to this class. Obstacle data - Jason Lynch
}

Obstacle::Obstacle(ObstacleID type, const TextureHolder& textures, const FontHolder& fonts) //Constructor taking in obstacle type, texture, font - Jason Lynch
	: Entity(Table[static_cast<int>(type)].hitpoints) //Pass hitpoints to Entity Parent - Jason Lynch
	, mType(type) //Set type - Jason Lynch
	, mSprite(textures.get(Table[static_cast<int>(type)].texture)) //Get texture of obstacle - Jason Lynch
	, mExplosion(textures.get(TextureID::Explosion)) //Explosion animation (not working yet) - Jason Lynch 
	, mShowExplosion(true) //Show explosion or not - Jason Lynch 
	, mPlayedExplosionSound(false) //Was explosion sound played - Jason Lynch
	, mHealthDisplay(nullptr) //Displays health of obstacle - Jason Lynch
{
	std::cout << getCategory() << std::endl; //Test output for setup - Jason Lynch 

	mExplosion.setFrameSize(sf::Vector2i(256, 256)); 
	mExplosion.setNumFrames(16);
	mExplosion.setDuration(sf::seconds(1));

	centreOrigin(mSprite);
	centreOrigin(mExplosion);

	//Set up health display text area and attack it to this object - Jason Lynch 
	std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts, "", sf::Color::Black));
	mHealthDisplay = healthDisplay.get();
	attachChild(std::move(healthDisplay));
}

unsigned int Obstacle::getCategory() const //Returns type of obstacle - Jason Lynch 
{
	return static_cast<int>(mType);
}

unsigned int Obstacle::getDamage() const //Returns damage obstacle dooes on collision - Jason Lynch
{
	return Table[static_cast<int>(mType)].damage;
}

sf::FloatRect Obstacle::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Obstacle::updateCurrent(sf::Time dt, CommandQueue& commands) 
{
	if (isDestroyed()) //Needs looking at, doesnt currently trigger properly - Jason Lynch 
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

	updateTexts();
}

void Obstacle::updateTexts() //Updates attached text area with current health, sets position, rotation, and scale of text - Jason Lynch
{
	mHealthDisplay->setString(toString(getHitpoints()) + " HP");
	mHealthDisplay->setPosition(0.f, 0.0f);
	mHealthDisplay->setRotation(-getRotation());
	mHealthDisplay->setScale(1.8f, 1.8f);
}

void Obstacle::playerLocalSound(CommandQueue& commands, SoundEffectID effect) //Plays sound when called, needs looking at also - Jason Lynch
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

void Obstacle::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const //Draws sprite or explosion if it is destroyed - Jason Lynch 
{
	if (isDestroyed() && mShowExplosion)
		target.draw(mExplosion, states);
	else
		target.draw(mSprite, states);
}