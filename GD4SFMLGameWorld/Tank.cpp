//Dylan Reilly D00194504 
//Jason Lynch D00137655
//Dylan created I modified as needed 
#include "Tank.hpp"
#include "ResourceHolder.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "Pickup.hpp"
#include "CommandQueue.hpp"
#include "SoundNode.hpp"
#include "EmitterNode.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include "SFML/Graphics/RenderStates.hpp"
#include "TextureID.hpp"
#include "TankID.hpp"
#include "ProjectileID.hpp"
#include "PickupID.hpp"

#include "TankID.hpp"

#include <cmath>
#include<iostream>

namespace
{
	const std::vector<TankData> Table = initializeTankData();
}

Tank::Tank(CategoryID entity, TankID type, const TextureHolder& textures, const FontHolder& fonts)//Created by modifying aircraft.cpp for our needs - Dylan Reilly
	: Entity(Table[static_cast<int>(type)].hitpoints)
	, mEntity(entity)
	, mType(type)
	, mSprite(textures.get(Table[static_cast<int>(type)].texture), Table[static_cast<int>(type)].textureRect)
	, mTextures(textures) //Needed to change tank texture on powerup pickup - Jason Lynch
	, mExplosion(textures.get(TextureID::Explosion))
	, mFireCommand()
	, mMissileCommand()
	, mFireCountdown(sf::Time::Zero)
	, mIsFiring(false)
	, mIsLaunchingMissile(false)
	, mShowExplosion(true)
	, mPlayedExplosionSound(false)
	, mSpawnedPickup(false)
	, mIsMarkedForRemoval(false)
	, mFireRateLevel(1)
	, mSpreadLevel(1)
	, mMissileAmmo(2)
	, mDropPickupCommand()
	, mTravelledDistance(0.f)
	, mDirectionIndex(0)
	, mHealthDisplay(nullptr)
	, mMissileDisplay(nullptr)
{
	mExplosion.setFrameSize(sf::Vector2i(256, 256));
	mExplosion.setNumFrames(16);
	mExplosion.setDuration(sf::seconds(1));

	centreOrigin(mSprite);
	centreOrigin(mExplosion);

	mFireCommand.category = static_cast<int>(CategoryID::SceneAirLayer);
	mFireCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createBullets(node, textures);
	};

	mMissileCommand.category = static_cast<int>(CategoryID::SceneAirLayer);
	mMissileCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createProjectile(node, ProjectileID::Missile, 0.f, 0.5f, textures);
	};

	mDropPickupCommand.category = static_cast<int>(CategoryID::SceneAirLayer);
	mDropPickupCommand.action = [this, &textures](SceneNode& node, sf::Time)
	{
		createPickup(node, textures);
	};

	std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts, "", sf::Color::Cyan));
	mHealthDisplay = healthDisplay.get();
	attachChild(std::move(healthDisplay));

	if (mEntity == CategoryID::PlayerTank)
	{
		//std::unique_ptr<TextNode> missileDisplay(new TextNode(fonts, ""));
		std::unique_ptr<TextNode> playerDisplay(new TextNode(fonts, "Player 1", sf::Color::Green)); //Show player one beside Player tank - Jason Lynch
		//missileDisplay->setPosition(0, 70);
		playerDisplay->setPosition(0, 90);
		//mMissileDisplay = missileDisplay.get();
		//attachChild(std::move(missileDisplay));
		attachChild(std::move(playerDisplay));
	}

	/*if (getCategory() == (static_cast<int>(CategoryID::PlayerTwoTank)))*/
	if (mEntity == CategoryID::PlayerTwoTank)
	{
		//std::unique_ptr<TextNode> missileDisplay(new TextNode(fonts, ""));
		std::unique_ptr<TextNode> playerDisplay(new TextNode(fonts, "Player 2", sf::Color::Red)); //Show Player 2 beside Player 2 Tank - Jason Lynch
		//missileDisplay->setPosition(0, 70);
		playerDisplay->setPosition(0, 90);
		//mMissileDisplay = missileDisplay.get();
		//attachChild(std::move(missileDisplay));
		attachChild(std::move(playerDisplay));
	}

	//Dust trails for tank treads - Dylan
	//Adds trail to the front left & right of the sprite
	std::unique_ptr<EmitterNode> smokeLeft(new EmitterNode(ParticleID::Smoke));
	smokeLeft->setPosition(-40.f, getBoundingRect().height / 2.f);
	attachChild(std::move(smokeLeft));

	std::unique_ptr<EmitterNode> smokeRight(new EmitterNode(ParticleID::Smoke));
	smokeRight->setPosition(40.f, getBoundingRect().height / 2.f);
	attachChild(std::move(smokeRight));

	//Properllant when firing - Dylan Reilly -NOT WORKING
	/*if (mIsFiring)
	{
		std::unique_ptr<EmitterNode> tankShot(new EmitterNode(ParticleID::Propellant));
		tankShot->setPosition(0.f, getBoundingRect().height / 2.f);
		attachChild(std::move(tankShot));
	}*/
	updateTexts();
}


void Tank::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (isDestroyed() && mShowExplosion)
		target.draw(mExplosion, states);
	else
		target.draw(mSprite, states);
}

void Tank::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	// Entity has been destroyed: Possibly drop pickup, mark for removal
	if (isDestroyed())
	{
		//checkPickupDrop(commands);
		mExplosion.update(dt);

		//Play explosion sound
		if (!mPlayedExplosionSound)
		{
			playerLocalSound(commands, SoundEffectID::TankDestroyed);

			mPlayedExplosionSound = true;
		}
		return;
	}
	// Check if bullets or missiles are fired
	checkProjectileLaunch(dt, commands);

	// Update enemy movement pattern; apply velocity
	Entity::updateCurrent(dt, commands);

	// Update texts
	updateTexts();
}


unsigned int Tank::getCategory() const
{
	if (isAllied())
		return static_cast<int>(CategoryID::PlayerTank);
	else
		return static_cast<int>(CategoryID::PlayerTwoTank);
}

sf::FloatRect Tank::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool Tank::isMarkedForRemoval() const
{
	return isDestroyed() && (mExplosion.isFinished() || !mShowExplosion);
}

//Returns whether tank is allied or enemy (green or red respectively) - Dylan Reilly
bool Tank::isAllied() const
{
	switch (mType)
	{
	case TankID::GreenLMG1:
	case TankID::GreenLMG2:
	case TankID::GreenLMG3:
	case TankID::GreenHMG1:
	case TankID::GreenHMG2:
	case TankID::GreenHMG3:
	case TankID::GreenGatling1:
	case TankID::GreenGatling2:
	case TankID::GreenGatling3:
	case TankID::GreenTesla1:
	case TankID::GreenTesla2:
	case TankID::GreenTesla3:
		return true;
	}
	return false;
}

//Returns correct projectile ID based on the tank being used - Dylan Reilly
ProjectileID Tank::getProjectile() const
{
	switch (mType) {
	case TankID::GreenLMG1:
	case TankID::GreenLMG2:
	case TankID::GreenLMG3:
		return ProjectileID::GreenLMGBullet;
	case TankID::RedLMG1:
	case TankID::RedLMG2:
	case TankID::RedLMG3:
		return ProjectileID::RedLMGBullet;
	case TankID::GreenHMG1:
	case TankID::GreenHMG2:
	case TankID::GreenHMG3:
		return ProjectileID::GreenHMGBullet;
	case TankID::RedHMG1:
	case TankID::RedHMG2:
	case TankID::RedHMG3:
		return ProjectileID::RedHMGBullet;
	case TankID::GreenGatling1:
	case TankID::GreenGatling2:
	case TankID::GreenGatling3:
		return ProjectileID::GreenGatlingBullet;
	case TankID::RedGatling1:
	case TankID::RedGatling2:
	case TankID::RedGatling3:
		return ProjectileID::RedGatlingBullet;
	case TankID::GreenTesla1:
	case TankID::GreenTesla2:
	case TankID::GreenTesla3:
		return ProjectileID::GreenTeslaBullet;
	case TankID::RedTesla1:
	case TankID::RedTesla2:
	case TankID::RedTesla3:
		return ProjectileID::RedTeslaBullet;
	default:
		return ProjectileID::None;
	}
}


float Tank::getMaxSpeed()
{
	return Table[static_cast<int>(mType)].speed;
}

void Tank::increaseFireRate()
{
	if (mFireRateLevel < 10)
		++mFireRateLevel;
}

void Tank::increaseSpread()
{
	if (mSpreadLevel < 3)
		++mSpreadLevel;
}

void Tank::setTankTexture(unsigned int val) { //Allows change of tank texture (e.g pickups) - Jason Lynch
	if (val == 1) //Checks for type of tank to change to - Jason Lynch
	{
		int tank = getCategory();
		switch (tank) //2 id player 1, 8 is player 2 - Jason Lynch
		{
		case 2:
			//Assigns new texture to player one tank - Jason Lynch
			mSprite.setTexture(mTextures.get(Table[static_cast<int>(TankID::GreenHMG1)].texture), false);
			mSprite.setTextureRect(Table[static_cast<int>(TankID::GreenHMG1)].textureRect);
			mType = TankID::GreenHMG1;
			break;
		case 8:
			//Assigns new texture to player two tank - Jason Lynch
			mSprite.setTexture(mTextures.get(Table[static_cast<int>(TankID::RedHMG1)].texture), false);
			mSprite.setTextureRect(Table[static_cast<int>(TankID::RedHMG1)].textureRect);
			mType = TankID::RedHMG1;
			break;
		}
	}
	else if (val == 2) //Checks for type of tank to change to - Jason Lynch
	{
		int tank = getCategory();
		switch (tank)
		{
		case 2:
			//Assigns new texture to player one tank - Jason Lynch
			mSprite.setTexture(mTextures.get(Table[static_cast<int>(TankID::GreenGatling2)].texture), false);
			mSprite.setTextureRect(Table[static_cast<int>(TankID::GreenGatling2)].textureRect);
			mType = TankID::GreenGatling2;
			break;
		case 8:
			//Assigns new texture to player two tank - Jason Lynch
			mSprite.setTexture(mTextures.get(Table[static_cast<int>(TankID::RedGatling2)].texture), false);
			mSprite.setTextureRect(Table[static_cast<int>(TankID::RedGatling2)].textureRect);
			mType = TankID::RedGatling2;
			break;
		}
		increaseFireRate();
	}
	else if (val == 3) //Checks for type of tank to change to - Jason Lynch
	{
		int tank = getCategory();
		switch (tank)
		{
		case 2:
			//Assigns new texture to player one tank - Jason Lynch
			mSprite.setTexture(mTextures.get(Table[static_cast<int>(TankID::GreenTesla2)].texture), false);
			mSprite.setTextureRect(Table[static_cast<int>(TankID::GreenTesla2)].textureRect);
			mType = TankID::GreenTesla2;
			break;
		case 8:
			//Assigns new texture to player two tank - Jason Lynch
			mSprite.setTexture(mTextures.get(Table[static_cast<int>(TankID::RedTesla3)].texture), false);
			mSprite.setTextureRect(Table[static_cast<int>(TankID::RedTesla3)].textureRect);
			mType = TankID::RedTesla3;
			break;
		}
	}
}

void Tank::collectMissiles(unsigned int count)
{
	mMissileAmmo += count;
}

void Tank::playerLocalSound(CommandQueue& commands, SoundEffectID effect)
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

void Tank::fire()
{
	// Only ships with fire interval != 0 are able to fire
	if (Table[static_cast<int>(mType)].fireInterval != sf::Time::Zero) {
		mIsFiring = true;
	}

}

void Tank::launchMissile()
{
	if (mMissileAmmo > 0)
	{
		mIsLaunchingMissile = true;
		--mMissileAmmo;
	}
}

void Tank::checkPickupDrop(CommandQueue& commands)
{
	if (randomInt(3) == 0 && !mSpawnedPickup)
		commands.push(mDropPickupCommand);
	mSpawnedPickup = true;
}

void Tank::checkProjectileLaunch(sf::Time dt, CommandQueue& commands)
{
	// Check for automatic gunfire, allow only in intervals
	if (mIsFiring && mFireCountdown <= sf::Time::Zero)
	{
		// Interval expired: We can fire a new bullet
		commands.push(mFireCommand);
		CorrectFiringSound(commands);
		mFireCountdown += Table[static_cast<int>(mType)].fireInterval / (mFireRateLevel + 1.f);
		mIsFiring = false;
	}
	else if (mFireCountdown > sf::Time::Zero)
	{
		// Interval not expired: Decrease it further
		mFireCountdown -= dt;
		mIsFiring = false;
	}

	// Check for missile launch
	if (mIsLaunchingMissile)
	{
		commands.push(mMissileCommand);
		playerLocalSound(commands, SoundEffectID::LaunchMissile);
		mIsLaunchingMissile = false;
	}
}

void Tank::CorrectFiringSound(CommandQueue& commands) { //Determines firing sound based in projectile type - Jason lynch 
	ProjectileID id = getProjectile();

	if (id == ProjectileID::GreenLMGBullet || id == ProjectileID::RedLMGBullet)  //If tank type is LMG - Jason lynch 
	{
		playerLocalSound(commands, SoundEffectID::TankLMG); //Play LMG sound - Jason lynch 
	}
	else if (id == ProjectileID::GreenHMGBullet || id == ProjectileID::RedHMGBullet) //If tank type is HMG - Jason lynch 
	{
		SoundEffectID soundEffect = (randomInt(2) == 0) ? SoundEffectID::TankCannon1 : SoundEffectID::TankCannon2; //Pick one of two sounds - Jason lynch  
		playerLocalSound(commands, soundEffect); //Play that sound - Jason lynch 
	}
	else if (id == ProjectileID::GreenGatlingBullet || id == ProjectileID::RedGatlingBullet) //If tank type is Gatling - Jason lynch  
	{
		playerLocalSound(commands, SoundEffectID::TankGatling);//Play gatling sound - Jason lynch 
	}
	else if (id == ProjectileID::GreenTeslaBullet || id == ProjectileID::RedTeslaBullet)//If tank type is Tesla - Jason lynch 
	{
		playerLocalSound(commands, SoundEffectID::TeslaBullet);
	}
}

void Tank::createBullets(SceneNode& node, const TextureHolder& textures) const
{
	//ProjectileID type = isAllied() ? ProjectileID::HMGBullet : ProjectileID::HMGBullet;
	ProjectileID type = Tank::getProjectile();

	switch (mSpreadLevel)
	{
	case 1:
		createProjectile(node, type, 0.0f, 0.5f, textures);
		break;

	case 2:
		createProjectile(node, type, -0.01f, 0.01f, textures);
		createProjectile(node, type, +0.01f, 0.01f, textures);
		break;

	case 3:
		createProjectile(node, type, -0.5f, 0.33f, textures);
		createProjectile(node, type, 0.0f, 0.5f, textures);
		createProjectile(node, type, +0.5f, 0.33f, textures);
		break;
	}
}

//Creates projectile and sets its direction to the direction the tank is facing - Dylan Reilly
void Tank::createProjectile(SceneNode& node, ProjectileID type, float xOffset, float yOffset, const TextureHolder& textures) const
{
	std::unique_ptr<Projectile> projectile(new Projectile(type, textures));

	//Sets projectile spawn position to origin on the tank
	sf::Vector2f offset(xOffset * Tank::getWorldPosition());
	//Sets velocity respective to the type of bullet and direction based on the direction the tank is facing
	sf::Vector2f velocity(projectile->getMaxSpeed() * 1.5f * -sin(toRadian(Tank::getRotation())), projectile->getMaxSpeed() * 1.5f * cos(toRadian(Tank::getRotation())));

	projectile->setPosition(getWorldPosition() + offset);
	projectile->setVelocity(velocity);
	projectile->setRotation(Tank::getRotation() + 180.f);
	if (type == ProjectileID::GreenTeslaBullet || type == ProjectileID::RedTeslaBullet)
		node.attachChild(std::move(projectile));
	else {
		std::unique_ptr<EmitterNode> tankShot(new EmitterNode(ParticleID::Propellant)); //Fixed your propelant Dylan - Jason Lynch 
		tankShot->setPosition(0.f, getBoundingRect().height / 2.f);
		projectile->attachChild(std::move(tankShot));
		node.attachChild(std::move(projectile));
	}
}

void Tank::createPickup(SceneNode& node, const TextureHolder& textures) const
{
	auto type = static_cast<PickupID>(randomInt(static_cast<int>(PickupID::TypeCount)));

	std::unique_ptr<Pickup> pickup(new Pickup(type, textures));
	pickup->setPosition(getWorldPosition());
	pickup->setVelocity(0.f, 1.f);
	node.attachChild(std::move(pickup));
}

void Tank::updateTexts()
{
	mHealthDisplay->setString(toString(getHitpoints()) + " HP");
	mHealthDisplay->setPosition(0.f, -120.f);
	mHealthDisplay->setRotation(-getRotation());
	mHealthDisplay->setScale(2.f, 2.f);
}