#include "Player.hpp"
#include "CommandQueue.hpp"
#include "Tank.hpp"
#include "ActionID.hpp"
#include "Utility.hpp"

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

Player::Player() : mCurrentMissionStatus(MissionStatusID::MissionRunning)
{
	// Set initial key bindings
	mKeyBinding[sf::Keyboard::A] = ActionID::TurnLeft;
	mKeyBinding[sf::Keyboard::D] = ActionID::TurnRight;
	mKeyBinding[sf::Keyboard::W] = ActionID::MoveUp;
	mKeyBinding[sf::Keyboard::S] = ActionID::MoveDown;
	mKeyBinding[sf::Keyboard::Space] = ActionID::Fire;
	mKeyBinding[sf::Keyboard::M] = ActionID::LaunchMissile;

	// Set initial action bindings
	initializeActions();

	// Assign all categories to player's Tank
	for (auto& pair : mActionBinding)
		pair.second.category = static_cast<int>(CategoryID::PlayerTank);
}

void Player::handleEvent(const sf::Event& event, CommandQueue& commands)
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

void Player::handleRealtimeInput(CommandQueue& commands)
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

void Player::assignKey(ActionID action, sf::Keyboard::Key key)
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

sf::Keyboard::Key Player::getAssignedKey(ActionID action) const
{
	for (auto pair : mKeyBinding)
	{
		if (pair.second == action)
			return pair.first;
	}

	return sf::Keyboard::Unknown;
}

void Player::setMissionStatus(MissionStatusID status)
{
	mCurrentMissionStatus = status;
}

MissionStatusID Player::getMissionStatus() const
{
	return mCurrentMissionStatus;
}

//Updated to allow the tank to turn rather than strafe, tank now moves foward in the direction it is facing - Dylan Reilly
void Player::initializeActions()
{
	//Turns tank while key is held
	mActionBinding[ActionID::TurnLeft].action = derivedAction<Tank>([](Tank& a, sf::Time) { a.rotate(-5.f); });
	mActionBinding[ActionID::TurnRight].action = derivedAction<Tank>([](Tank& a, sf::Time) { a.rotate(5.f); });
	//Moves tank fowards/backwards based on its direction
	mActionBinding[ActionID::MoveUp].action = derivedAction<Tank>([](Tank& a, sf::Time) { a.move(1.5f * -sin(toRadian( a.getRotation() )), 1.5f * cos(toRadian( a.getRotation() ))); });
	mActionBinding[ActionID::MoveDown].action = derivedAction<Tank>([](Tank& a, sf::Time) { a.move(1.5f * sin(toRadian(a.getRotation())), 1.5f * -cos(toRadian(a.getRotation()))); });
	mActionBinding[ActionID::Fire].action = derivedAction<Tank>([](Tank& a, sf::Time) { a.fire(); });
	mActionBinding[ActionID::LaunchMissile].action = derivedAction<Tank>([](Tank& a, sf::Time) { a.launchMissile(); });
}

bool Player::isRealtimeAction(ActionID action)
{
	switch (action)
	{
	case ActionID::TurnLeft:
	case ActionID::TurnRight:
	case ActionID::MoveDown:
	case ActionID::MoveUp:
	case ActionID::Fire:
		return true;

	default:
		return false;
	}
}