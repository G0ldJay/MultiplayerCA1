#include"DataTables.hpp"
#include"Aircraft.hpp"
#include"Projectile.hpp"
#include"Pickup.hpp"
#include"AircraftID.hpp"
#include"ProjectileID.hpp"
#include"PickupID.hpp"

std::vector<AircraftData> initializeAircraftData() {
	std::vector<AircraftData> data(static_cast<int>(AircraftID::TypeCount));

	data[static_cast<int>(AircraftID::Eagle)].hitpoints = 100;
	data[static_cast<int>(AircraftID::Eagle)].speed = 200;
	data[static_cast<int>(AircraftID::Eagle)].fireInterval = sf::seconds(1);
	data[static_cast<int>(AircraftID::Eagle)].texture = TextureID::Eagle;

	data[static_cast<int>(AircraftID::Raptor)].hitpoints = 20;
	data[static_cast<int>(AircraftID::Raptor)].speed = 80.f;
	data[static_cast<int>(AircraftID::Raptor)].fireInterval = sf::Time::Zero;
	data[static_cast<int>(AircraftID::Raptor)].texture = TextureID::Raptor;
	data[static_cast<int>(AircraftID::Raptor)].directions.push_back(Direction(+45.f, 80.f));
	data[static_cast<int>(AircraftID::Raptor)].directions.push_back(Direction(-45.f, 160.f));
	data[static_cast<int>(AircraftID::Raptor)].directions.push_back(Direction(+45.f, 80.f));

	data[static_cast<int>(AircraftID::Avenger)].hitpoints = 40;
	data[static_cast<int>(AircraftID::Avenger)].speed = 50;
	data[static_cast<int>(AircraftID::Avenger)].fireInterval = sf::seconds(2);
	data[static_cast<int>(AircraftID::Avenger)].texture = TextureID::Avenger;
	data[static_cast<int>(AircraftID::Avenger)].directions.push_back(Direction(+45.f, 80.f));
	data[static_cast<int>(AircraftID::Avenger)].directions.push_back(Direction(0.f, 50.f));
	data[static_cast<int>(AircraftID::Avenger)].directions.push_back(Direction(-45.f, 100.f));
	data[static_cast<int>(AircraftID::Avenger)].directions.push_back(Direction(0.f, 50.f));
	data[static_cast<int>(AircraftID::Avenger)].directions.push_back(Direction(+45.f, 50.f));

	return data;
};

std::vector<ProjectileData> initializeProjectileData() {
	std::vector<ProjectileData> data(static_cast<int>(ProjectileID::TypeCount));

	data[static_cast<int>(ProjectileID::AlliedBullet)].damage = 10;
	data[static_cast<int>(ProjectileID::AlliedBullet)].speed = 300.f;
	data[static_cast<int>(ProjectileID::AlliedBullet)].texture = TextureID::Bullet;

	data[static_cast<int>(ProjectileID::EnemyBullet)].damage = 10;
	data[static_cast<int>(ProjectileID::EnemyBullet)].speed = 300.f;
	data[static_cast<int>(ProjectileID::EnemyBullet)].texture = TextureID::Bullet;

	data[static_cast<int>(ProjectileID::Missile)].damage = 200;
	data[static_cast<int>(ProjectileID::Missile)].speed = 250.f;
	data[static_cast<int>(ProjectileID::Missile)].texture = TextureID::Missile;

	return data;
};

std::vector<PickupData> initializePickupData() {
	std::vector<PickupData> data(static_cast<int>(PickupID::TypeCount));

	data[static_cast<int>(PickupID::HealthRefill)].texture = TextureID::HealthRefill;
	data[static_cast<int>(PickupID::HealthRefill)].action = [](Aircraft& a) {a.repair(25); };

	data[static_cast<int>(PickupID::MissileRefill)].texture = TextureID::MissileRefill;
	data[static_cast<int>(PickupID::MissileRefill)].action = std::bind(&Aircraft::collectMissiles, std::placeholders::_1, 3);

	data[static_cast<int>(PickupID::FireSpread)].texture = TextureID::FireSpread;
	data[static_cast<int>(PickupID::FireSpread)].action = std::bind(&Aircraft::increaseFireSpread, std::placeholders::_1);

	data[static_cast<int>(PickupID::FireRate)].texture = TextureID::FireRate;
	data[static_cast<int>(PickupID::FireRate)].action = std::bind(&Aircraft::increaseFireRate, std::placeholders::_1);

	return data;
};