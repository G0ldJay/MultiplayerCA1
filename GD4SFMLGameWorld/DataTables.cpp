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
	//Starter Tank - Light Machine Gun
	data[static_cast<int>(TankID::LMG1)].hitpoints = 50;
	data[static_cast<int>(TankID::LMG1)].speed = 200.f;
	data[static_cast<int>(TankID::LMG1)].fireInterval = sf::seconds(1);
	data[static_cast<int>(TankID::LMG1)].textureRect = sf::IntRect(0, 0, 95, 128);
	data[static_cast<int>(TankID::LMG1)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::LMG1)].bulletType = ProjectileID::LMGBullet;

	//Starter Tank - Double Light Machine Gun
	data[static_cast<int>(TankID::LMG2)].hitpoints = 50;
	data[static_cast<int>(TankID::LMG2)].speed = 200.f;
	data[static_cast<int>(TankID::LMG2)].fireInterval = sf::seconds(0.6f);
	data[static_cast<int>(TankID::LMG2)].textureRect = sf::IntRect(95, 0, 95, 128);
	data[static_cast<int>(TankID::LMG2)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::LMG1)].bulletType = ProjectileID::LMGBullet;

	//Starter Tank - Faster movement
	data[static_cast<int>(TankID::LMG3)].hitpoints = 50;
	data[static_cast<int>(TankID::LMG3)].speed = 250.f;
	data[static_cast<int>(TankID::LMG3)].fireInterval = sf::seconds(1);
	data[static_cast<int>(TankID::LMG3)].textureRect = sf::IntRect(190, 0, 95, 128);
	data[static_cast<int>(TankID::LMG3)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::LMG1)].bulletType = ProjectileID::LMGBullet;

	//Heavy Tank - Heavy Machine Gun
	data[static_cast<int>(TankID::HMG1)].hitpoints = 100;
	data[static_cast<int>(TankID::HMG1)].speed = 200.f;
	data[static_cast<int>(TankID::HMG1)].fireInterval = sf::seconds(1);
	data[static_cast<int>(TankID::HMG1)].textureRect = sf::IntRect(285, 0, 95, 128);
	data[static_cast<int>(TankID::HMG1)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::LMG1)].bulletType = ProjectileID::HMGBullet;

	//Heavy Tank - Double Heavy Machine Gun
	data[static_cast<int>(TankID::HMG2)].hitpoints = 100;
	data[static_cast<int>(TankID::HMG2)].speed = 200.f;
	data[static_cast<int>(TankID::HMG2)].fireInterval = sf::seconds(1);
	data[static_cast<int>(TankID::HMG2)].textureRect = sf::IntRect(380, 0, 95, 128);
	data[static_cast<int>(TankID::HMG2)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::LMG1)].bulletType = ProjectileID::HMGBullet;

	//Heavy Tank - Faster Movement
	data[static_cast<int>(TankID::HMG3)].hitpoints = 100;
	data[static_cast<int>(TankID::HMG3)].speed = 250.f;
	data[static_cast<int>(TankID::HMG3)].fireInterval = sf::seconds(1);
	data[static_cast<int>(TankID::HMG3)].textureRect = sf::IntRect(475, 0, 95, 128);
	data[static_cast<int>(TankID::HMG3)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::LMG1)].bulletType = ProjectileID::HMGBullet;

	//Gatling Tank - Gatling Gun
	data[static_cast<int>(TankID::Gatling1)].hitpoints = 150;
	data[static_cast<int>(TankID::Gatling1)].speed = 200.f;
	data[static_cast<int>(TankID::Gatling1)].fireInterval = sf::seconds(0.6f);
	data[static_cast<int>(TankID::Gatling1)].textureRect = sf::IntRect(570, 0, 95, 128);
	data[static_cast<int>(TankID::Gatling1)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::LMG1)].bulletType = ProjectileID::GatlingBullet;

	//Gatling Tank - Double Gatling Gun
	data[static_cast<int>(TankID::Gatling2)].hitpoints = 150;
	data[static_cast<int>(TankID::Gatling2)].speed = 200.f;
	data[static_cast<int>(TankID::Gatling2)].fireInterval = sf::seconds(0.4f);
	data[static_cast<int>(TankID::Gatling2)].textureRect = sf::IntRect(665, 0, 95, 128);
	data[static_cast<int>(TankID::Gatling2)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::LMG1)].bulletType = ProjectileID::GatlingBullet;

	//Gatling Tank - Faster Movement
	data[static_cast<int>(TankID::Gatling3)].hitpoints = 150;
	data[static_cast<int>(TankID::Gatling3)].speed = 300.f;
	data[static_cast<int>(TankID::Gatling3)].fireInterval = sf::seconds(0.6f);
	data[static_cast<int>(TankID::Gatling3)].textureRect = sf::IntRect(760, 0, 95, 128);
	data[static_cast<int>(TankID::Gatling3)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::LMG1)].bulletType = ProjectileID::GatlingBullet;

	//Tesla Tank - Lightening Gun
	data[static_cast<int>(TankID::Tesla1)].hitpoints = 200;
	data[static_cast<int>(TankID::Tesla1)].speed = 200.f;
	data[static_cast<int>(TankID::Tesla1)].fireInterval = sf::seconds(1);
	data[static_cast<int>(TankID::Tesla1)].textureRect = sf::IntRect(855, 0, 95, 128);
	data[static_cast<int>(TankID::Tesla1)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::LMG1)].bulletType = ProjectileID::TeslaBullet;

	//Tesla Tank - Double Lightening Gun
	data[static_cast<int>(TankID::Tesla2)].hitpoints = 200;
	data[static_cast<int>(TankID::Tesla2)].speed = 200.f;
	data[static_cast<int>(TankID::Tesla2)].fireInterval = sf::seconds(1);
	data[static_cast<int>(TankID::Tesla2)].textureRect = sf::IntRect(950, 0, 95, 128);
	data[static_cast<int>(TankID::Tesla2)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::LMG1)].bulletType = ProjectileID::TeslaBullet;

	//Tesla Tank - Faster Movement
	data[static_cast<int>(TankID::Tesla3)].hitpoints = 200;
	data[static_cast<int>(TankID::Tesla3)].speed = 250.f;
	data[static_cast<int>(TankID::Tesla3)].fireInterval = sf::seconds(1);
	data[static_cast<int>(TankID::Tesla3)].textureRect = sf::IntRect(1045, 0, 95, 128);
	data[static_cast<int>(TankID::Tesla3)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::LMG1)].bulletType = ProjectileID::TeslaBullet;


	return data;
}

std::vector<ProjectileData> initializeProjectileData()
{
	std::vector<ProjectileData> data(static_cast<int>(ProjectileID::TypeCount));

	data[static_cast<int>(ProjectileID::LMGBullet)].damage = 5;
	data[static_cast<int>(ProjectileID::LMGBullet)].speed = 300.f;
	data[static_cast<int>(ProjectileID::LMGBullet)].texture = TextureID::Tanks;
	data[static_cast<int>(ProjectileID::LMGBullet)].textureRect = sf::IntRect(175, 64, 3, 14);

	data[static_cast<int>(ProjectileID::HMGBullet)].damage = 8;
	data[static_cast<int>(ProjectileID::HMGBullet)].speed = 260.f;
	data[static_cast<int>(ProjectileID::HMGBullet)].texture = TextureID::Entities;
	data[static_cast<int>(ProjectileID::HMGBullet)].textureRect = sf::IntRect(184, 66, 30, 38);


	data[static_cast<int>(ProjectileID::GatlingBullet)].damage = 3;
	data[static_cast<int>(ProjectileID::GatlingBullet)].speed = 300.f;
	data[static_cast<int>(ProjectileID::GatlingBullet)].texture = TextureID::Entities;
	data[static_cast<int>(ProjectileID::GatlingBullet)].textureRect = sf::IntRect(175, 64, 3, 14);

	data[static_cast<int>(ProjectileID::TeslaBullet)].damage = 12;
	data[static_cast<int>(ProjectileID::TeslaBullet)].speed = 200.f;
	data[static_cast<int>(ProjectileID::TeslaBullet)].texture = TextureID::Entities;
	data[static_cast<int>(ProjectileID::TeslaBullet)].textureRect = sf::IntRect(160, 54, 15, 32);

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
