#include "DataTables.hpp"
#include "Tank.hpp"
#include "Projectile.hpp"
#include "Pickup.hpp"
#include "TankID.hpp"
#include "ProjectileID.hpp"
#include "PickupID.hpp"
#include "ParticleID.hpp"
#include"ObstacleID.hpp"

std::vector<TankData> initializeTankData()
{
	std::vector<TankData> data(static_cast<int>(TankID::TypeCount));
	//Starter Tank - Light Machine Gun
	data[static_cast<int>(TankID::GreenLMG1)].hitpoints = 50;
	data[static_cast<int>(TankID::GreenLMG1)].speed = 200.f;
	data[static_cast<int>(TankID::GreenLMG1)].fireInterval = sf::seconds(1);
	data[static_cast<int>(TankID::GreenLMG1)].textureRect = sf::IntRect(0, 0, 95, 128);
	data[static_cast<int>(TankID::GreenLMG1)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::GreenLMG1)].bulletType = ProjectileID::LMGBullet;

	//Starter Tank - Double Light Machine Gun
	data[static_cast<int>(TankID::GreenLMG2)].hitpoints = 50;
	data[static_cast<int>(TankID::GreenLMG2)].speed = 200.f;
	data[static_cast<int>(TankID::GreenLMG2)].fireInterval = sf::seconds(0.6f);
	data[static_cast<int>(TankID::GreenLMG2)].textureRect = sf::IntRect(95, 0, 95, 128);
	data[static_cast<int>(TankID::GreenLMG2)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::GreenLMG2)].bulletType = ProjectileID::LMGBullet;

	//Starter Tank - Faster movement
	data[static_cast<int>(TankID::GreenLMG3)].hitpoints = 50;
	data[static_cast<int>(TankID::GreenLMG3)].speed = 250.f;
	data[static_cast<int>(TankID::GreenLMG3)].fireInterval = sf::seconds(1);
	data[static_cast<int>(TankID::GreenLMG3)].textureRect = sf::IntRect(190, 0, 95, 128);
	data[static_cast<int>(TankID::GreenLMG3)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::GreenLMG3)].bulletType = ProjectileID::LMGBullet;

	//Heavy Tank - Heavy Machine Gun
	data[static_cast<int>(TankID::GreenHMG1)].hitpoints = 100;
	data[static_cast<int>(TankID::GreenHMG1)].speed = 200.f;
	data[static_cast<int>(TankID::GreenHMG1)].fireInterval = sf::seconds(1);
	data[static_cast<int>(TankID::GreenHMG1)].textureRect = sf::IntRect(285, 0, 95, 128);
	data[static_cast<int>(TankID::GreenHMG1)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::GreenHMG1)].bulletType = ProjectileID::HMGBullet;

	//Heavy Tank - Double Heavy Machine Gun
	data[static_cast<int>(TankID::GreenHMG2)].hitpoints = 100;
	data[static_cast<int>(TankID::GreenHMG2)].speed = 200.f;
	data[static_cast<int>(TankID::GreenHMG2)].fireInterval = sf::seconds(1);
	data[static_cast<int>(TankID::GreenHMG2)].textureRect = sf::IntRect(380, 0, 95, 128);
	data[static_cast<int>(TankID::GreenHMG2)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::GreenHMG2)].bulletType = ProjectileID::HMGBullet;

	//Heavy Tank - Faster Movement
	data[static_cast<int>(TankID::GreenHMG3)].hitpoints = 100;
	data[static_cast<int>(TankID::GreenHMG3)].speed = 250.f;
	data[static_cast<int>(TankID::GreenHMG3)].fireInterval = sf::seconds(1);
	data[static_cast<int>(TankID::GreenHMG3)].textureRect = sf::IntRect(475, 0, 95, 128);
	data[static_cast<int>(TankID::GreenHMG3)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::GreenHMG3)].bulletType = ProjectileID::HMGBullet;

	//Gatling Tank - Gatling Gun
	data[static_cast<int>(TankID::GreenGatling1)].hitpoints = 150;
	data[static_cast<int>(TankID::GreenGatling1)].speed = 200.f;
	data[static_cast<int>(TankID::GreenGatling1)].fireInterval = sf::seconds(0.6f);
	data[static_cast<int>(TankID::GreenGatling1)].textureRect = sf::IntRect(570, 0, 95, 128);
	data[static_cast<int>(TankID::GreenGatling1)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::GreenGatling1)].bulletType = ProjectileID::GatlingBullet;

	//Gatling Tank - Double Gatling Gun
	data[static_cast<int>(TankID::GreenGatling2)].hitpoints = 150;
	data[static_cast<int>(TankID::GreenGatling2)].speed = 200.f;
	data[static_cast<int>(TankID::GreenGatling2)].fireInterval = sf::seconds(0.4f);
	data[static_cast<int>(TankID::GreenGatling2)].textureRect = sf::IntRect(665, 0, 95, 128);
	data[static_cast<int>(TankID::GreenGatling2)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::GreenGatling2)].bulletType = ProjectileID::GatlingBullet;

	//Gatling Tank - Faster Movement
	data[static_cast<int>(TankID::GreenGatling3)].hitpoints = 150;
	data[static_cast<int>(TankID::GreenGatling3)].speed = 300.f;
	data[static_cast<int>(TankID::GreenGatling3)].fireInterval = sf::seconds(0.6f);
	data[static_cast<int>(TankID::GreenGatling3)].textureRect = sf::IntRect(760, 0, 95, 128);
	data[static_cast<int>(TankID::GreenGatling3)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::GreenGatling3)].bulletType = ProjectileID::GatlingBullet;

	//Tesla Tank - Lightening Gun
	data[static_cast<int>(TankID::GreenTesla1)].hitpoints = 200;
	data[static_cast<int>(TankID::GreenTesla1)].speed = 200.f;
	data[static_cast<int>(TankID::GreenTesla1)].fireInterval = sf::seconds(1);
	data[static_cast<int>(TankID::GreenTesla1)].textureRect = sf::IntRect(855, 0, 95, 128);
	data[static_cast<int>(TankID::GreenTesla1)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::GreenTesla1)].bulletType = ProjectileID::TeslaBullet;

	//Tesla Tank - Double Lightening Gun
	data[static_cast<int>(TankID::GreenTesla2)].hitpoints = 200;
	data[static_cast<int>(TankID::GreenTesla2)].speed = 200.f;
	data[static_cast<int>(TankID::GreenTesla2)].fireInterval = sf::seconds(1);
	data[static_cast<int>(TankID::GreenTesla2)].textureRect = sf::IntRect(950, 0, 95, 128);
	data[static_cast<int>(TankID::GreenTesla2)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::GreenTesla2)].bulletType = ProjectileID::TeslaBullet;

	//Tesla Tank - Faster Movement
	data[static_cast<int>(TankID::GreenTesla3)].hitpoints = 200;
	data[static_cast<int>(TankID::GreenTesla3)].speed = 250.f;
	data[static_cast<int>(TankID::GreenTesla3)].fireInterval = sf::seconds(1);
	data[static_cast<int>(TankID::GreenTesla3)].textureRect = sf::IntRect(1045, 0, 95, 128);
	data[static_cast<int>(TankID::GreenTesla3)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::GreenTesla3)].bulletType = ProjectileID::TeslaBullet;

	//-----------------------------------------------------------------------------------------
	// RED TANK
	//-----------------------------------------------------------------------------------------

	data[static_cast<int>(TankID::RedLMG1)].hitpoints = 50;
	data[static_cast<int>(TankID::RedLMG1)].speed = 200.f;
	data[static_cast<int>(TankID::RedLMG1)].fireInterval = sf::seconds(1);
	data[static_cast<int>(TankID::RedLMG1)].textureRect = sf::IntRect(0, 128, 95, 128);
	data[static_cast<int>(TankID::RedLMG1)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::RedLMG1)].bulletType = ProjectileID::LMGBullet;
								  
	//Starter Tank - Double Light Redne Gun
	data[static_cast<int>(TankID::RedLMG2)].hitpoints = 50;
	data[static_cast<int>(TankID::RedLMG2)].speed = 200.f;
	data[static_cast<int>(TankID::RedLMG2)].fireInterval = sf::seconds(0.6f);
	data[static_cast<int>(TankID::RedLMG2)].textureRect = sf::IntRect(95, 128, 95, 128);
	data[static_cast<int>(TankID::RedLMG2)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::RedLMG2)].bulletType = ProjectileID::LMGBullet;
								  
	//Starter Tank - Faster movemeRed
	data[static_cast<int>(TankID::RedLMG3)].hitpoints = 50;
	data[static_cast<int>(TankID::RedLMG3)].speed = 250.f;
	data[static_cast<int>(TankID::RedLMG3)].fireInterval = sf::seconds(1);
	data[static_cast<int>(TankID::RedLMG3)].textureRect = sf::IntRect(190, 128, 95, 128);
	data[static_cast<int>(TankID::RedLMG3)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::RedLMG3)].bulletType = ProjectileID::LMGBullet;
								  
	//Heavy Tank - Heavy Machine GRed
	data[static_cast<int>(TankID::RedHMG1)].hitpoints = 100;
	data[static_cast<int>(TankID::RedHMG1)].speed = 200.f;
	data[static_cast<int>(TankID::RedHMG1)].fireInterval = sf::seconds(1);
	data[static_cast<int>(TankID::RedHMG1)].textureRect = sf::IntRect(285, 128, 95, 128);
	data[static_cast<int>(TankID::RedHMG1)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::RedHMG1)].bulletType = ProjectileID::HMGBullet;
								  
	//Heavy Tank - Double Heavy MaRed Gun
	data[static_cast<int>(TankID::RedHMG2)].hitpoints = 100;
	data[static_cast<int>(TankID::RedHMG2)].speed = 200.f;
	data[static_cast<int>(TankID::RedHMG2)].fireInterval = sf::seconds(1);
	data[static_cast<int>(TankID::RedHMG2)].textureRect = sf::IntRect(380, 128, 95, 128);
	data[static_cast<int>(TankID::RedHMG2)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::RedHMG2)].bulletType = ProjectileID::HMGBullet;
								  
	//Heavy Tank - Faster MovementRed
	data[static_cast<int>(TankID::RedHMG3)].hitpoints = 100;
	data[static_cast<int>(TankID::RedHMG3)].speed = 250.f;
	data[static_cast<int>(TankID::RedHMG3)].fireInterval = sf::seconds(1);
	data[static_cast<int>(TankID::RedHMG3)].textureRect = sf::IntRect(475, 128, 95, 128);
	data[static_cast<int>(TankID::RedHMG3)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::RedHMG3)].bulletType = ProjectileID::HMGBullet;
								  
	//Gatling Tank - Gatling Gun  Red
	data[static_cast<int>(TankID::RedGatling1)].hitpoints = 150;
	data[static_cast<int>(TankID::RedGatling1)].speed = 200.f;
	data[static_cast<int>(TankID::RedGatling1)].fireInterval = sf::seconds(0.6f);
	data[static_cast<int>(TankID::RedGatling1)].textureRect = sf::IntRect(570, 128, 95, 128);
	data[static_cast<int>(TankID::RedGatling1)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::RedGatling1)].bulletType = ProjectileID::GatlingBullet;
								  
	//Gatling Tank - Double GatlinRed
	data[static_cast<int>(TankID::RedGatling2)].hitpoints = 150;
	data[static_cast<int>(TankID::RedGatling2)].speed = 200.f;
	data[static_cast<int>(TankID::RedGatling2)].fireInterval = sf::seconds(0.4f);
	data[static_cast<int>(TankID::RedGatling2)].textureRect = sf::IntRect(665, 128, 95, 128);
	data[static_cast<int>(TankID::RedGatling2)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::RedGatling2)].bulletType = ProjectileID::GatlingBullet;
								  
	//Gatling Tank - Faster MovemeRed
	data[static_cast<int>(TankID::RedGatling3)].hitpoints = 150;
	data[static_cast<int>(TankID::RedGatling3)].speed = 300.f;
	data[static_cast<int>(TankID::RedGatling3)].fireInterval = sf::seconds(0.6f);
	data[static_cast<int>(TankID::RedGatling3)].textureRect = sf::IntRect(760, 128, 95, 128);
	data[static_cast<int>(TankID::RedGatling3)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::RedGatling3)].bulletType = ProjectileID::GatlingBullet;
								  
	//Tesla Tank - Lightening Gun Red
	data[static_cast<int>(TankID::RedTesla1)].hitpoints = 200;
	data[static_cast<int>(TankID::RedTesla1)].speed = 200.f;
	data[static_cast<int>(TankID::RedTesla1)].fireInterval = sf::seconds(1);
	data[static_cast<int>(TankID::RedTesla1)].textureRect = sf::IntRect(855, 128, 95, 128);
	data[static_cast<int>(TankID::RedTesla1)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::RedTesla1)].bulletType = ProjectileID::TeslaBullet;
								  
	//Tesla Tank - Double LighteniRedn
	data[static_cast<int>(TankID::RedTesla2)].hitpoints = 200;
	data[static_cast<int>(TankID::RedTesla2)].speed = 200.f;
	data[static_cast<int>(TankID::RedTesla2)].fireInterval = sf::seconds(1);
	data[static_cast<int>(TankID::RedTesla2)].textureRect = sf::IntRect(950, 128, 95, 128);
	data[static_cast<int>(TankID::RedTesla2)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::RedTesla2)].bulletType = ProjectileID::TeslaBullet;
								  
	//Tesla Tank - Faster MovementRed
	data[static_cast<int>(TankID::RedTesla3)].hitpoints = 200;
	data[static_cast<int>(TankID::RedTesla3)].speed = 250.f;
	data[static_cast<int>(TankID::RedTesla3)].fireInterval = sf::seconds(1);
	data[static_cast<int>(TankID::RedTesla3)].textureRect = sf::IntRect(1045, 128, 95, 128);
	data[static_cast<int>(TankID::RedTesla3)].texture = TextureID::Tanks;
	data[static_cast<int>(TankID::RedTesla3)].bulletType = ProjectileID::TeslaBullet;


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

std::vector<ObstacleData> initializeObstacleData()
{
	std::vector<ObstacleData> data(static_cast<int>(ObstacleID::TypeCount));

	data[static_cast<int>(ObstacleID::Wall)].damage = 2;
	data[static_cast<int>(ObstacleID::Wall)].hitpoints = 10000;
	data[static_cast<int>(ObstacleID::Wall)].texture = TextureID::Wall;

	data[static_cast<int>(ObstacleID::Barrel)].damage = 40;
	data[static_cast<int>(ObstacleID::Barrel)].hitpoints = 10;
	data[static_cast<int>(ObstacleID::Barrel)].texture = TextureID::Barrel;

	data[static_cast<int>(ObstacleID::DestructableWall)].damage = 2;
	data[static_cast<int>(ObstacleID::DestructableWall)].hitpoints = 20;
	data[static_cast<int>(ObstacleID::DestructableWall)].texture = TextureID::DestructableWall;

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
