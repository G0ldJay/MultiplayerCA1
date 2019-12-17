//Jason Lynch - D00137655
//Dylan Reilly D00194504
#pragma once
#include "State.hpp"
#include "Container.hpp"
#include "Button.hpp"
#include "Player.hpp"
#include "Label.hpp"
#include "ActionID.hpp"

#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Text.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include <array>

class PlayerOneInputSettingsState : public State //Created by me to set up player 1s input for keyboard and controller. Built from SettingsState.hpp - Jason Lynch 
{
public:
	PlayerOneInputSettingsState(StateStack& stack, Context context);//Constructor - Jason Lynch 

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	void updateLabels();
	void updateControllerLabels(); //Updates labels beside controller buttons for input assignment - Jason Lynch
	void addButtonLabel(ActionID action, float y, const std::string& text, Context context);
	void addButtonLabelController(ActionID action, float y, const std::string& text, Context context); //Adds Labels to controller buttons fopr input assignment -Jason Lynch
	const char* getKeyName(const sf::Keyboard::Key key);
	const char* getJoyStickButtonNamePS4(const int buttonVal); //Returns a button on the joystick

private:
	sf::Sprite mBackgroundSprite;
	GUI::Container mGUIContainer;
	std::array<GUI::Button::Ptr, static_cast<int>(ActionID::ActionCount)> mBindingButtons;
	std::array<GUI::Label::Ptr, static_cast<int>(ActionID::ActionCount)> mBindingLabels;

	std::array<GUI::Button::Ptr, static_cast<int>(ActionID::ActionCount)> mBindingButtonsController; //Holds buttons and actions associated - Jason Lynch 
	std::array<GUI::Label::Ptr, static_cast<int>(ActionID::ActionCount)> mBindingLabelsController; //Holds labels and actions associated - Jason Lynch
};