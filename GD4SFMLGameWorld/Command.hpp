//Jason Lynch - D00137655
//Dylan Reilly D00194504
#pragma once

#include "CategoryID.hpp"
#include "SFML/System/Time.hpp"

#include <functional>
#include <cassert>

class SceneNode;

struct Command
{
	Command();
	std::function<void(SceneNode&, sf::Time)> action;
	unsigned int category;
};

template <typename GameObject, typename Function>
std::function<void(SceneNode&, sf::Time)> derivedAction(Function fn)
{
	return [=](SceneNode& node, sf::Time dt)
	{
		//Check if the cast is safe
		assert(dynamic_cast<GameObject*>(&node) != nullptr);

		//Downcast node and invoke the function on it
		fn(static_cast<GameObject&>(node), dt);

	};
}

