#pragma once

#include "ResourceIdentifiers.hpp"
#include "TextureID.hpp"

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

struct TankData
{
	int hitpoints;
	float speed;
	TextureID texture;
	sf::IntRect textureRect;
	sf::Time fireInterval;
	std::vector<Direction> directions;
};

struct ProjectileData
{
	int damage;
	float speed;
	TextureID texture;
	sf::IntRect textureRect;
};

struct PickupData
{
	std::function<void(Tank&)> action;
	TextureID texture;
	sf::IntRect textureRect;
};

struct ParticleData
{
	sf::Color color;
	sf::Time lifetime;
};

std::vector<TankData> initializeTankData();
std::vector<TankData> initializeTankData();
std::vector<ProjectileData> initializeProjectileData();
std::vector<PickupData> initializePickupData();
std::vector<ParticleData> initializeParticleData();



