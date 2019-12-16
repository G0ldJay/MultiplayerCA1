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
{
	centreOrigin(mSprite);

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

	Entity::updateCurrent(dt, commands);
}

void Projectile::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mSprite, states);
}

//Returns whether a projectile is allied or enemy(green or red) based on bullet type - Dyllan Reilly
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