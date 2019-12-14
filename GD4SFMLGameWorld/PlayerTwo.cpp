#include "PlayerTwo.hpp"
#include "CommandQueue.hpp"
#include "Tank.hpp"
#include "ActionID.hpp"

#include <map>
#include <string>
#include <algorithm>
#include <iostream>

struct TankMover
{
	TankMover(float vx, float vy)
		: velocity(vx, vy)
	{
	}

	void operator() (Tank& Tank, sf::Time) const
	{
		Tank.accelerate(velocity * Tank.getMaxSpeed());
	}

	sf::Vector2f velocity;
};

PlayerTwo::PlayerTwo():mCurrentMissionStatus(MissionStatusID::MissionRunning)
{
	// Set initial key bindings
	mKeyBinding[sf::Keyboard::J] = ActionID::MoveLeft;
	mKeyBinding[sf::Keyboard::L] = ActionID::MoveRight;
	mKeyBinding[sf::Keyboard::I] = ActionID::MoveUp;
	mKeyBinding[sf::Keyboard::K] = ActionID::MoveDown;
	mKeyBinding[sf::Keyboard::Slash] = ActionID::Fire;
	mKeyBinding[sf::Keyboard::Comma] = ActionID::LaunchMissile;

	// Set initial action bindings
	initializeActions();

	// Assign all categories to player's Tank
	for (auto& pair : mActionBinding)
		pair.second.category = static_cast<int>(CategoryID::PlayerTwoTank);
}

void PlayerTwo::handleEvent(const sf::Event& event, CommandQueue& commands)
{
	if (event.type == sf::Event::KeyPressed)
	{
		// Check if pressed key appears in key binding, trigger command if so
		auto found = mKeyBinding.find(event.key.code);

		if (found != mKeyBinding.end() && !isRealtimeAction(found->second))
		{
			commands.push(mActionBinding[found->second]);
		}
	}
}

void PlayerTwo::handleRealtimeInput(CommandQueue& commands)
{
	// Traverse all assigned keys and check if they are pressed
	for (auto pair : mKeyBinding)
	{
		// If key is pressed, lookup action and trigger corresponding command
		if (sf::Keyboard::isKeyPressed(pair.first) && isRealtimeAction(pair.second))
		{
			commands.push(mActionBinding[pair.second]);
		}
	}
}

void PlayerTwo::assignKey(ActionID action, sf::Keyboard::Key key)
{
	// Remove all keys that already map to action
	for (auto itr = mKeyBinding.begin(); itr != mKeyBinding.end(); )
	{
		if (itr->second == action)
			mKeyBinding.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	mKeyBinding[key] = action;
}

sf::Keyboard::Key PlayerTwo::getAssignedKey(ActionID action) const
{
	for (auto pair : mKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

void PlayerTwo::setMissionStatus(MissionStatusID status)
{
	mCurrentMissionStatus = status;
}

MissionStatusID PlayerTwo::getMissionStatus() const
{
	return mCurrentMissionStatus;
}

void PlayerTwo::initializeActions()
{
	mActionBinding[ActionID::MoveLeft].action = derivedAction<Tank>(TankMover(-1, 0));
	mActionBinding[ActionID::MoveRight].action = derivedAction<Tank>(TankMover(+1, 0));
	mActionBinding[ActionID::MoveUp].action = derivedAction<Tank>(TankMover(0, -1));
	mActionBinding[ActionID::MoveDown].action = derivedAction<Tank>(TankMover(0, +1));
	mActionBinding[ActionID::Fire].action = derivedAction<Tank>([](Tank& a, sf::Time) { a.fire(); });
	mActionBinding[ActionID::LaunchMissile].action = derivedAction<Tank>([](Tank& a, sf::Time) { a.launchMissile(); });
}

bool PlayerTwo::isRealtimeAction(ActionID action)
{
	switch (action)
	{
	case ActionID::MoveLeft:
	case ActionID::MoveRight:
	case ActionID::MoveDown:
	case ActionID::MoveUp:
	case ActionID::Fire:
		return true;

	default:
		return false;
	}
}
