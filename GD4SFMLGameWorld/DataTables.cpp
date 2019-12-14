#include "DataTables.hpp"
#include "Tank.hpp"
#include "Projectile.hpp"
#include "Pickup.hpp"
#include "TankID.hpp"
#include "TankID.hpp"
#include "ProjectileID.hpp"
#include "PickupID.hpp"
#include "ParticleID.hpp"

std::vector<TankData> initializeTankData()
{
	std::vector<TankData> data(static_cast<int>(TankID::TypeCount));
	data[static_cast<int>(TankID::LMG1)].hitpoints = 100;
	data[static_cast<int>(TankID::LMG1)].speed = 200.f;
	data[static_cast<int>(TankID::LMG1)].fireInterval = sf::seconds(1);
	data[static_cast<int>(TankID::LMG1)].textureRect = sf::IntRect(0, 0, 95, 128);
	data[static_cast<int>(TankID::LMG1)].texture = TextureID::Tanks;

	data[static_cast<int>(TankID::LMG2)].hitpoints = 20;
	data[static_cast<int>(TankID::LMG2)].speed = 80.f;
	data[static_cast<int>(TankID::LMG2)].fireInterval = sf::Time::Zero;
	data[static_cast<int>(TankID::LMG2)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::LMG2)].textureRect = sf::IntRect(144, 0, 84, 64);

	data[static_cast<int>(TankID::LMG3)].directions.push_back(Direction(+45.f, 80.f));
	data[static_cast<int>(TankID::LMG3)].directions.push_back(Direction(-45.f, 160.f));
	data[static_cast<int>(TankID::LMG3)].directions.push_back(Direction(+45.f, 80.f));

	return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
	std::vector<ProjectileData> data(static_cast<int>(ProjectileID::TypeCount));

	data[static_cast<int>(ProjectileID::AlliedBullet)].damage = 10;
	data[static_cast<int>(ProjectileID::AlliedBullet)].speed = 300.f;
	data[static_cast<int>(ProjectileID::AlliedBullet)].texture = TextureID::Entities;
	data[static_cast<int>(ProjectileID::AlliedBullet)].textureRect = sf::IntRect(175, 64, 3, 14);

	data[static_cast<int>(ProjectileID::EnemyBullet)].damage = 10;
	data[static_cast<int>(ProjectileID::EnemyBullet)].speed = 300.f;
	data[static_cast<int>(ProjectileID::EnemyBullet)].texture = TextureID::Entities;
	data[static_cast<int>(ProjectileID::EnemyBullet)].textureRect = sf::IntRect(175, 64, 3, 14);


	data[static_cast<int>(ProjectileID::Missile)].damage = 200;
	data[static_cast<int>(ProjectileID::Missile)].speed = 250.f;
	data[static_cast<int>(ProjectileID::Missile)].texture = TextureID::Entities;
	data[static_cast<int>(ProjectileID::Missile)].textureRect = sf::IntRect(160, 64, 15, 32);

	return data;
}

std::vector<PickupData> initializePickupData()
{
	std::vector<PickupData> data(static_cast<int>(PickupID::TypeCount));
	data[static_cast<int>(PickupID::HealthRefill)].texture = TextureID::Entities;
	data[static_cast<int>(PickupID::HealthRefill)].textureRect = sf::IntRect(0, 64, 40, 40);
	data[static_cast<int>(PickupID::HealthRefill)].action = [](Tank& a) {a.repair(25); };

	data[static_cast<int>(PickupID::MissileRefill)].texture = TextureID::Entities;
	data[static_cast<int>(PickupID::MissileRefill)].textureRect = sf::IntRect(40, 64, 40, 40);
	data[static_cast<int>(PickupID::MissileRefill)].action = std::bind(&Tank::collectMissiles, std::placeholders::_1, 3);

	data[static_cast<int>(PickupID::FireSpread)].texture = TextureID::Entities;
	data[static_cast<int>(PickupID::FireSpread)].textureRect = sf::IntRect(80, 64, 40, 40);
	data[static_cast<int>(PickupID::FireSpread)].action = std::bind(&Tank::increaseSpread, std::placeholders::_1);

	data[static_cast<int>(PickupID::FireRate)].texture = TextureID::Entities;
	data[static_cast<int>(PickupID::FireRate)].textureRect = sf::IntRect(120, 64, 40, 40);
	data[static_cast<int>(PickupID::FireRate)].action = std::bind(&Tank::increaseFireRate, std::placeholders::_1);

	return data;
}

std::vector<ParticleData> initializeParticleData()
{
	std::vector<ParticleData> data(static_cast<int>(ParticleID::ParticleCount));

	data[static_cast<int>(ParticleID::Propellant)].color = sf::Color(255, 255, 50);
	data[static_cast<int>(ParticleID::Propellant)].lifetime = sf::seconds(0.6f);

	data[static_cast<int>(ParticleID::Smoke)].color = sf::Color(50, 50, 50);
	data[static_cast<int>(ParticleID::Smoke)].lifetime = sf::seconds(4.f);

	return data;
}
