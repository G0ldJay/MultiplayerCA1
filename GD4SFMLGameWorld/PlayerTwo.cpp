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
	mKeyBinding[sf::Keyboard::J] = ActionID::TurnLeft;
	mKeyBinding[sf::Keyboard::L] = ActionID::TurnRight;
	mKeyBinding[sf::Keyboard::I] = ActionID::MoveUp;
	mKeyBinding[sf::Keyboard::K] = ActionID::MoveDown;
	mKeyBinding[sf::Keyboard::Slash] = ActionID::Fire;
	mKeyBinding[sf::Keyboard::Comma] = ActionID::LaunchMissile;

	mControllerBinding[1] = ActionID::MoveDown;
	mControllerBinding[3] = ActionID::MoveUp;
	mControllerBinding[0] = ActionID::TurnLeft;
	mControllerBinding[2] = ActionID::TurnRight;
	mControllerBinding[5] = ActionID::Fire;

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

	for (auto pair : mControllerBinding) {
		if (sf::Joystick::isButtonPressed(1, pair.first) && isRealtimeAction(pair.second)) {
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

void PlayerTwo::assignJoystickButton(ActionID action, int buttonNumber)
{
	// Remove all keys that already map to action
	for (auto itr = mControllerBinding.begin(); itr != mControllerBinding.end(); )
	{
		if (itr->second == action)
			mControllerBinding.erase(itr++);
		else
			++itr;
	}

	// Insert new binding
	mControllerBinding[buttonNumber] = action;
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

int PlayerTwo::getAssignedJoypadButton(ActionID action) const
{
	for (auto pair : mControllerBinding) {
		if (pair.second == action) {
			return pair.first;
		}
	}

	return -1;
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
	mActionBinding[ActionID::TurnLeft].action = derivedAction<Tank>([](Tank& a, sf::Time) { a.rotate(-5.f); });
	mActionBinding[ActionID::TurnRight].action = derivedAction<Tank>([](Tank& a, sf::Time) { a.rotate(5.f); });
	mActionBinding[ActionID::MoveUp].action = derivedAction<Tank>([](Tank& a, sf::Time) { a.move(1.5f * -sin(toRadian(a.getRotation())), 1.5f * cos(toRadian(a.getRotation()))); });
	mActionBinding[ActionID::MoveDown].action = derivedAction<Tank>([](Tank& a, sf::Time) { a.move(1.5f * sin(toRadian(a.getRotation())), 1.5f * -cos(toRadian(a.getRotation()))); });
	mActionBinding[ActionID::Fire].action = derivedAction<Tank>([](Tank& a, sf::Time) { a.fire(); });
	mActionBinding[ActionID::LaunchMissile].action = derivedAction<Tank>([](Tank& a, sf::Time) { a.launchMissile(); });
}

bool PlayerTwo::isRealtimeAction(ActionID action)
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
