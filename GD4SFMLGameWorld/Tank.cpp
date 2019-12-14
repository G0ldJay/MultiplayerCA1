#include "Tank.hpp"
#include "ResourceHolder.hpp"
#include "DataTables.hpp"
#include "Utility.hpp"
#include "Pickup.hpp"
#include "CommandQueue.hpp"
#include "SoundNode.hpp"

#include <SFML/Graphics/RenderTarget.hpp>
#include "SFML/Graphics/RenderStates.hpp"
#include "TextureID.hpp"
#include "TankID.hpp"
#include "ProjectileID.hpp"
#include "PickupID.hpp"

#include "TankID.hpp"

#include <cmath>

namespace
{
	const std::vector<TankData> Table = initializeTankData();
}

//TextureID toTextureID(TankID type)
//{
//	switch (type)
//	{
//	case TankID::Eagle:
//		return TextureID::Eagle;
//
//	case TankID::Raptor:
//		return TextureID::Raptor;
//	}
//	return TextureID::Eagle;
//}

Tank::Tank(TankID type, const TextureHolder& textures, const FontHolder& fonts)
	: Entity(Table[static_cast<int>(type)].hitpoints)
	, mType(type)
	, mSprite(textures.get(Table[static_cast<int>(type)].texture), Table[static_cast<int>(type)].textureRect)
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

	std::unique_ptr<TextNode> healthDisplay(new TextNode(fonts, ""));
	mHealthDisplay = healthDisplay.get();
	attachChild(std::move(healthDisplay));

	if (getCategory() == (static_cast<int>(CategoryID::PlayerTank)))
	{
		std::unique_ptr<TextNode> missileDisplay(new TextNode(fonts, ""));
		std::unique_ptr<TextNode> playerDisplay(new TextNode(fonts, "Player 1"));
		missileDisplay->setPosition(0, 70);
		playerDisplay->setPosition(0, 90);
		mMissileDisplay = missileDisplay.get();
		attachChild(std::move(missileDisplay));
		attachChild(std::move(playerDisplay));
	}

	if (getCategory() == (static_cast<int>(CategoryID::PlayerTwoTank)))
	{
		std::unique_ptr<TextNode> missileDisplay(new TextNode(fonts, ""));
		std::unique_ptr<TextNode> playerDisplay(new TextNode(fonts, "Player 2"));
		missileDisplay->setPosition(0, 70);
		playerDisplay->setPosition(0, 90);
		mMissileDisplay = missileDisplay.get();
		attachChild(std::move(missileDisplay));
		attachChild(std::move(playerDisplay));
	}

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
		checkPickupDrop(commands);
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

	// Check if bullets or missiles are fired
	checkProjectileLaunch(dt, commands);

	// Update enemy movement pattern; apply velocity
	//updateMovementPattern(dt);
	Entity::updateCurrent(dt, commands);

	// Update texts
	updateTexts();
}


unsigned int Tank::getCategory() const
{
	if (isAllied())
		return static_cast<int>(CategoryID::PlayerTank);
	else
		return static_cast<int>(CategoryID::EnemyTank);
}

sf::FloatRect Tank::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

bool Tank::isMarkedForRemoval() const
{
	return isDestroyed() && (mExplosion.isFinished() || !mShowExplosion);
}

bool Tank::isAllied() const
{
	return mType != TankID::Tesla3;
}

ProjectileID Tank::getProjectile() const
{
	if (mType == TankID::LMG1 || mType == TankID::LMG2 || mType == TankID::LMG3)
		return ProjectileID::LMGBullet;
	else if (mType == TankID::HMG1 || mType == TankID::HMG2 || mType == TankID::HMG3)
		return ProjectileID::HMGBullet;
	else if (mType == TankID::Gatling1 || mType == TankID::Gatling2 || mType == TankID::Gatling3)
		return ProjectileID::GatlingBullet;
	else if (mType == TankID::Tesla1 || mType == TankID::Tesla2 || mType == TankID::Tesla3)
		return ProjectileID::TeslaBullet;
}


float Tank::getMaxSpeed() const
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
	if (Table[static_cast<int>(mType)].fireInterval != sf::Time::Zero)
		mIsFiring = true;
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
		playerLocalSound(commands, SoundEffectID::EnemyGunfire);
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
		createProjectile(node, type, -0.33f, 0.33f, textures);
		createProjectile(node, type, +0.33f, 0.33f, textures);
		break;

	case 3:
		createProjectile(node, type, -0.5f, 0.33f, textures);
		createProjectile(node, type, 0.0f, 0.5f, textures);
		createProjectile(node, type, +0.5f, 0.33f, textures);
		break;
	}
}

void Tank::createProjectile(SceneNode& node, ProjectileID type, float xOffset, float yOffset, const TextureHolder& textures) const
{
	std::unique_ptr<Projectile> projectile(new Projectile(type, textures));

	sf::Vector2f offset(xOffset * mSprite.getGlobalBounds().width, yOffset * mSprite.getGlobalBounds().height);
	sf::Vector2f velocity(0, projectile->getMaxSpeed());

	float sign = isAllied() ? -1.f : +1.f;
	projectile->setPosition(getWorldPosition() + offset * sign);
	projectile->setVelocity(velocity * sign);
	node.attachChild(std::move(projectile));
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
	mHealthDisplay->setPosition(0.f, 50.f);
	mHealthDisplay->setRotation(-getRotation());

	if (mMissileDisplay)
	{
		if (mMissileAmmo == 0)
			mMissileDisplay->setString("");
		else
			mMissileDisplay->setString("M: " + toString(mMissileAmmo));
	}
}