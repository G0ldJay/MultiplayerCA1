//Jason Lynch - D00137655
//Dylan Reilly D00194504
#pragma once

#include "ResourceIdentifiers.hpp"
#include "TextureID.hpp"
#include "ProjectileID.hpp"

#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Rect.hpp>

#include <vector>
#include <functional>

class Tank;

struct Direction
{
	Direction(float angle, float distance)
		:angle(angle), distance(distance)
	{}

	float angle;
	float distance;
};

struct TankData //Modified aircraft data for tank - Dylan Reilly
{
	int hitpoints;
	float speed;
	TextureID texture;
	sf::IntRect textureRect;
	sf::Time fireInterval;
	ProjectileID bulletType;
};

struct ProjectileData
{
	int damage;
	float speed;
	TextureID texture;
	sf::IntRect textureRect;
};

struct ObstacleData //Struct for obstacles in world - Jason Lynch
{
	int hitpoints; //Hitpoints of obstacle - Jason Lynch
	int damage; //Damage done to player colliding with obstacle - Jason Lynch
	TextureID texture; //Texture of Obstacle - Jason Lynch
	//sf::IntRect textureRect;
};

struct PickupData
{
	std::function<void(Tank&)> action;
	TextureID texture;
	sf::IntRect textureRect;
};

struct TankPickupData //Pickups for tanks (texture changing ones so far) - Jason Lynch
{
	std::function<void(Tank&)> action; //Function to call - Jason Lynch 
	TextureID texture; ///Texture of pickup - Jason Lynch
	//sf::IntRect textureRect;
};

struct ParticleData
{
	sf::Color color;
	sf::Time lifetime;
};

std::vector<TankData> initializeTankData();
std::vector<ProjectileData> initializeProjectileData();
std::vector<PickupData> initializePickupData();
std::vector<TankPickupData> initializeTankPickupData(); 
std::vector<ParticleData> initializeParticleData();
std::vector<ObstacleData> initializeObstacleData();



