#include "Projectile.hpp"
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
	const std::vector<ProjectileData> Table = initializeProjectileData();
}

Projectile::Projectile(ProjectileID type, const TextureHolder& textures)
	: Entity(1)
	, mType(type)
	, mSprite(textures.get(Table[static_cast<int>(type)].texture), Table[static_cast<int>(type)].textureRect)
	, mTargetDirection()
	, mFiringAnimation(textures.get(TextureID::TeslaBullet))
{
	//Initializes animation for tesla bullet - Dylan Reilly
	mFiringAnimation.setFrameSize(sf::Vector2i(128, 128));
	mFiringAnimation.setScale(0.5f, 0.5f);
	mFiringAnimation.setNumFrames(12);
	mFiringAnimation.setDuration(sf::seconds(2));

	centreOrigin(mSprite);
	centreOrigin(mFiringAnimation);

	//Add particle system for system
	if (mType == ProjectileID::GreenHMGBullet || mType == ProjectileID::RedHMGBullet)
	{
		std::unique_ptr<EmitterNode> smoke(new EmitterNode(ParticleID::BulletSmoke));
		smoke->setPosition(0.f, getBoundingRect().height / 2.f);
		attachChild(std::move(smoke));
	}

	if (mType == ProjectileID::GreenTeslaBullet || mType == ProjectileID::RedTeslaBullet)
	{
		std::unique_ptr<EmitterNode> smoke(new EmitterNode(ParticleID::TeslaSmoke));
		smoke->setPosition(0.f, getBoundingRect().height / 2.f);
		attachChild(std::move(smoke));
	}
}

void Projectile::guideTowards(sf::Vector2f position)
{
	assert(isGuided());
	mTargetDirection = unitVector(position - getWorldPosition());
}

bool Projectile::isGuided() const
{
	return mType == ProjectileID::Missile;
}

void Projectile::updateCurrent(sf::Time dt, CommandQueue& commands)
{
	if (isGuided())
	{
		const float approachRate = 200.f;

		sf::Vector2f newVelocity = unitVector(approachRate * dt.asSeconds() * mTargetDirection + getVelocity());
		newVelocity *= getMaxSpeed();
		float angle = std::atan2(newVelocity.y, newVelocity.x);

		setRotation(toDegree(angle) + 90.f);
		setVelocity(newVelocity);
	}
	//Updates the firing animation if it shoots a tesla bullet - Dylan Reilly
	if (mType == ProjectileID::GreenTeslaBullet || mType == ProjectileID::RedTeslaBullet)
	{
		mFiringAnimation.update(dt);
	}
	Entity::updateCurrent(dt, commands);
}

void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	//Draws tesla animation if it shoots a tesla bullet, elses draws plain sprite
	if(mType == ProjectileID::GreenTeslaBullet || mType == ProjectileID::RedTeslaBullet)
	{
		target.draw(mFiringAnimation, states);
	}
	else
	{
		target.draw(mSprite, states);
	}
}

//Returns whether a projectile is allied or enemy(green or red) based on bullet type - Dylan Reilly
unsigned int Projectile::getCategory() const
{
	if (mType == ProjectileID::RedLMGBullet || mType == ProjectileID::RedHMGBullet || mType == ProjectileID::RedGatlingBullet || mType == ProjectileID::RedTeslaBullet)
		return static_cast<int>(CategoryID::EnemyProjectile);
	else
		return static_cast<int>(CategoryID::AlliedProjectile);
}

sf::FloatRect Projectile::getBoundingRect() const
{
	return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

float Projectile::getMaxSpeed() const
{
	return Table[static_cast<int>(mType)].speed;
}

int Projectile::getDamage() const
{
	return Table[static_cast<int>(mType)].damage;
}