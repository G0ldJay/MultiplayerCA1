//Jason Lynch - D00137655
//Dylan Reilly D00194504
#pragma once
#include <SFML/System/Vector2.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/Color.hpp>

struct Particle
{
	sf::Vector2f position;
	sf::Color color;
	sf::Time lifetime;
};