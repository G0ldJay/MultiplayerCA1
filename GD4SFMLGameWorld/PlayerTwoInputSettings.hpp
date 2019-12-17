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

class PlayerTwoInputSettingsState : public State
{
public:
	PlayerTwoInputSettingsState(StateStack& stack, Context context);

	virtual void draw();
	virtual bool update(sf::Time dt);
	virtual bool handleEvent(const sf::Event& event);

private:
	void updateLabels();
	void updateControllerLabels();
	void addButtonLabel(ActionID action, float y, const std::string& text, Context context);
	void addButtonLabelController(ActionID action, float y, const std::string& text, Context context);
	const char* getKeyName(const sf::Keyboard::Key key);
	const char* getJoyStickButtonNamePS4(const int buttonVal);

private:
	sf::Sprite mBackgroundSprite;
	GUI::Container mGUIContainer;
	std::array<GUI::Button::Ptr, static_cast<int>(ActionID::ActionCount)> mBindingButtons;
	std::array<GUI::Label::Ptr, static_cast<int>(ActionID::ActionCount)> mBindingLabels;

	std::array<GUI::Button::Ptr, static_cast<int>(ActionID::ActionCount)> mBindingButtonsController;
	std::array<GUI::Label::Ptr, static_cast<int>(ActionID::ActionCount)> mBindingLabelsController;
};