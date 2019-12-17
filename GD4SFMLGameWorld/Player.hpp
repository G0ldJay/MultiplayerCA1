//Jason Lynch - D00137655
//Dylan Reilly D00194504
#pragma once
#include "Command.hpp"
#include "ActionID.hpp"
#include "MissionStatusID.hpp"

#include <SFML/Window/Event.hpp>
#include <map>

class CommandQueue;

class Player //Modified by me for controller input - Jason Lynch
{
public:
	Player();

	void handleEvent(const sf::Event& event, CommandQueue& commands);
	void handleRealtimeInput(CommandQueue& commands);

	void assignKey(ActionID action, sf::Keyboard::Key key);
	void assignJoystickButton(ActionID action, int buttonNumber); //Assigns joystick button to action - Jason Lynch
	sf::Keyboard::Key getAssignedKey(ActionID action) const;
	int getAssignedJoypadButton(ActionID action) const; //Gets button assigned to joystick - Jason Lynch

	void setMissionStatus(MissionStatusID status);
	MissionStatusID getMissionStatus() const;

private:
	void initializeActions();
	static bool isRealtimeAction(ActionID action);

private:
	std::map<sf::Keyboard::Key, ActionID> mKeyBinding;
	std::map<int, ActionID> mControllerBinding; //Holds controller button and its action - Jason Lynch
	std::map<ActionID, Command> mActionBinding;
	MissionStatusID mCurrentMissionStatus;
};