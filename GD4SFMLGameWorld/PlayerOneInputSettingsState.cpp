//Jason Lynch - D00137655
//Dylan Reilly D00194504
#include "PlayerOneInputSettingsState.hpp"
#include<iostream>
#include"Utility.hpp"

PlayerOneInputSettingsState::PlayerOneInputSettingsState(StateStack& stack, Context context)//Created by me to handle player one input setup for keyboard and controller - Jason Lynch
	:State(stack, context)
	, mGUIContainer()
{
	mBackgroundSprite.setTexture(context.textures->get(TextureID::PlayerOneInputSetupBackground));
	//Build key bindings and button labels
	addButtonLabel(ActionID::TurnLeft, 300.f, "Move Left", context);
	addButtonLabel(ActionID::TurnRight, 350.f, "Move Right", context);
	addButtonLabel(ActionID::MoveUp, 400.f, "Move Up", context);
	addButtonLabel(ActionID::MoveDown, 450.f, "Move Down", context);
	addButtonLabel(ActionID::Fire, 500.f, "Fire", context);
	addButtonLabel(ActionID::LaunchMissile, 550.f, "Missile", context);

	//Adds buttons for controller - Jason Lynch 
	addButtonLabelController(ActionID::TurnLeft, 300.f, "Move Left", context);
	addButtonLabelController(ActionID::TurnRight, 350.f, "Move Right", context);
	addButtonLabelController(ActionID::MoveUp, 400.f, "Move Up", context);
	addButtonLabelController(ActionID::MoveDown, 450.f, "Move Down", context);
	addButtonLabelController(ActionID::Fire, 500.f, "Fire", context);
	addButtonLabelController(ActionID::LaunchMissile, 550.f, "Missile", context);

	updateLabels();
	updateControllerLabels();

	auto backButton = std::make_shared<GUI::Button>(context);
	backButton->setPosition(426.f, 620.f);
	backButton->setText("Back");
	backButton->setCallback(std::bind(&PlayerOneInputSettingsState::requestStackPop, this));

	mGUIContainer.pack(backButton);

}

void PlayerOneInputSettingsState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool PlayerOneInputSettingsState::update(sf::Time dt)
{
	return true;
}

//Modified to allow controller inoput to be assigned - Jason Lynch 
bool PlayerOneInputSettingsState::handleEvent(const sf::Event& event)
{
	bool isKeyBinding = false;

	//Iterate through all key binding buttons to see they are being pressed, waiting for the user to enter a key
	for (std::size_t action = 0; action < static_cast<int>(ActionID::ActionCount); ++action)
	{
		if (mBindingButtons[action]->isActive())
		{
			isKeyBinding = true;
			if (event.type == sf::Event::KeyReleased)
			{
				getContext().player->assignKey(static_cast<ActionID>(action), event.key.code);
				mBindingButtons[action]->deactivate();
			}
			break;
		}
		else if (mBindingButtonsController[action]->isActive())
		{
			isKeyBinding = true;
			if (event.type == sf::Event::JoystickButtonPressed)
			{
				getContext().player->assignJoystickButton(static_cast<ActionID>(action), event.joystickButton.button);
				mBindingButtonsController[action]->deactivate();
			}
			break;
		}
	}

	//If the keybindings have been updated we need to update the labels
	if (isKeyBinding)
	{
		updateLabels();
		updateControllerLabels();
	}
	else
	{
		mGUIContainer.handleEvent(event);
	}
	return false;
}

void PlayerOneInputSettingsState::updateLabels()
{
	Player& player = *getContext().player;
	for (std::size_t i = 0; i < static_cast<int>(ActionID::ActionCount); ++i)
	{
		sf::Keyboard::Key key = player.getAssignedKey(static_cast<ActionID>(i));
		mBindingLabels[i]->setText(toString(getKeyName(key)));
	}
}

//Updates labels associated with button actions - Jason Lynch 
void PlayerOneInputSettingsState::updateControllerLabels()
{
	Player& player = *getContext().player;
	for (std::size_t i = 0; i < static_cast<int>(ActionID::ActionCount); ++i)
	{
		int buttonNumber = player.getAssignedJoypadButton(static_cast<ActionID>(i));
		mBindingLabelsController[i]->setText(toString(getJoyStickButtonNamePS4(buttonNumber)));
	}
}

void PlayerOneInputSettingsState::addButtonLabel(ActionID action, float y, const std::string& text, Context context)
{
	mBindingButtons[static_cast<int>(action)] = std::make_shared<GUI::Button>(context);
	mBindingButtons[static_cast<int>(action)]->setPosition(210.f, y);
	mBindingButtons[static_cast<int>(action)]->setText(text);
	mBindingButtons[static_cast<int>(action)]->setToggle(true);

	mBindingLabels[static_cast<int>(action)] = std::make_shared<GUI::Label>("", *context.fonts);
	mBindingLabels[static_cast<int>(action)]->setPosition(430.f, y + 15.f);

	mGUIContainer.pack(mBindingButtons[static_cast<int>(action)]);
	mGUIContainer.pack(mBindingLabels[static_cast<int>(action)]);
}

//Adds buttons and labels for controller to GUI - Jason Lynch 
void PlayerOneInputSettingsState::addButtonLabelController(ActionID action, float y, const std::string& text, Context context)
{
	mBindingButtonsController[static_cast<int>(action)] = std::make_shared<GUI::Button>(context);
	mBindingButtonsController[static_cast<int>(action)]->setPosition(630.f, y);
	mBindingButtonsController[static_cast<int>(action)]->setText(text);
	mBindingButtonsController[static_cast<int>(action)]->setToggle(true);

	mBindingLabelsController[static_cast<int>(action)] = std::make_shared<GUI::Label>("", *context.fonts);
	mBindingLabelsController[static_cast<int>(action)]->setPosition(850.f, y + 15.f);

	mGUIContainer.pack(mBindingButtonsController[static_cast<int>(action)]);
	mGUIContainer.pack(mBindingLabelsController[static_cast<int>(action)]);
}

//Returns the name of a key associated with the enum - Jason Lynch (Sourced code)
const char* PlayerOneInputSettingsState::getKeyName(const sf::Keyboard::Key key) { 
	switch (key) {
	default:
	case sf::Keyboard::Unknown:
		return "Unknown";
	case sf::Keyboard::A:
		return "A";
	case sf::Keyboard::B:
		return "B";
	case sf::Keyboard::C:
		return "C";
	case sf::Keyboard::D:
		return "D";
	case sf::Keyboard::E:
		return "E";
	case sf::Keyboard::F:
		return "F";
	case sf::Keyboard::G:
		return "G";
	case sf::Keyboard::H:
		return "H";
	case sf::Keyboard::I:
		return "I";
	case sf::Keyboard::J:
		return "J";
	case sf::Keyboard::K:
		return "K";
	case sf::Keyboard::L:
		return "L";
	case sf::Keyboard::M:
		return "M";
	case sf::Keyboard::N:
		return "N";
	case sf::Keyboard::O:
		return "O";
	case sf::Keyboard::P:
		return "P";
	case sf::Keyboard::Q:
		return "Q";
	case sf::Keyboard::R:
		return "R";
	case sf::Keyboard::S:
		return "S";
	case sf::Keyboard::T:
		return "T";
	case sf::Keyboard::U:
		return "U";
	case sf::Keyboard::V:
		return "V";
	case sf::Keyboard::W:
		return "W";
	case sf::Keyboard::X:
		return "X";
	case sf::Keyboard::Y:
		return "Y";
	case sf::Keyboard::Z:
		return "Z";
	case sf::Keyboard::Num0:
		return "Num0";
	case sf::Keyboard::Num1:
		return "Num1";
	case sf::Keyboard::Num2:
		return "Num2";
	case sf::Keyboard::Num3:
		return "Num3";
	case sf::Keyboard::Num4:
		return "Num4";
	case sf::Keyboard::Num5:
		return "Num5";
	case sf::Keyboard::Num6:
		return "Num6";
	case sf::Keyboard::Num7:
		return "Num7";
	case sf::Keyboard::Num8:
		return "Num8";
	case sf::Keyboard::Num9:
		return "Num9";
	case sf::Keyboard::Escape:
		return "Escape";
	case sf::Keyboard::LControl:
		return "LControl";
	case sf::Keyboard::LShift:
		return "LShift";
	case sf::Keyboard::LAlt:
		return "LAlt";
	case sf::Keyboard::LSystem:
		return "LSystem";
	case sf::Keyboard::RControl:
		return "RControl";
	case sf::Keyboard::RShift:
		return "RShift";
	case sf::Keyboard::RAlt:
		return "RAlt";
	case sf::Keyboard::RSystem:
		return "RSystem";
	case sf::Keyboard::Menu:
		return "Menu";
	case sf::Keyboard::LBracket:
		return "LBracket";
	case sf::Keyboard::RBracket:
		return "RBracket";
	case sf::Keyboard::SemiColon:
		return "SemiColon";
	case sf::Keyboard::Comma:
		return "Comma";
	case sf::Keyboard::Period:
		return "Period";
	case sf::Keyboard::Quote:
		return "Quote";
	case sf::Keyboard::Slash:
		return "Slash";
	case sf::Keyboard::BackSlash:
		return "BackSlash";
	case sf::Keyboard::Tilde:
		return "Tilde";
	case sf::Keyboard::Equal:
		return "Equal";
	case sf::Keyboard::Dash:
		return "Dash";
	case sf::Keyboard::Space:
		return "Space";
	case sf::Keyboard::Return:
		return "Return";
	case sf::Keyboard::BackSpace:
		return "BackSpace";
	case sf::Keyboard::Tab:
		return "Tab";
	case sf::Keyboard::PageUp:
		return "PageUp";
	case sf::Keyboard::PageDown:
		return "PageDown";
	case sf::Keyboard::End:
		return "End";
	case sf::Keyboard::Home:
		return "Home";
	case sf::Keyboard::Insert:
		return "Insert";
	case sf::Keyboard::Delete:
		return "Delete";
	case sf::Keyboard::Add:
		return "Add";
	case sf::Keyboard::Subtract:
		return "Subtract";
	case sf::Keyboard::Multiply:
		return "Multiply";
	case sf::Keyboard::Divide:
		return "Divide";
	case sf::Keyboard::Left:
		return "Left";
	case sf::Keyboard::Right:
		return "Right";
	case sf::Keyboard::Up:
		return "Up";
	case sf::Keyboard::Down:
		return "Down";
	case sf::Keyboard::Numpad0:
		return "Numpad0";
	case sf::Keyboard::Numpad1:
		return "Numpad1";
	case sf::Keyboard::Numpad2:
		return "Numpad2";
	case sf::Keyboard::Numpad3:
		return "Numpad3";
	case sf::Keyboard::Numpad4:
		return "Numpad4";
	case sf::Keyboard::Numpad5:
		return "Numpad5";
	case sf::Keyboard::Numpad6:
		return "Numpad6";
	case sf::Keyboard::Numpad7:
		return "Numpad7";
	case sf::Keyboard::Numpad8:
		return "Numpad8";
	case sf::Keyboard::Numpad9:
		return "Numpad9";
	case sf::Keyboard::F1:
		return "F1";
	case sf::Keyboard::F2:
		return "F2";
	case sf::Keyboard::F3:
		return "F3";
	case sf::Keyboard::F4:
		return "F4";
	case sf::Keyboard::F5:
		return "F5";
	case sf::Keyboard::F6:
		return "F6";
	case sf::Keyboard::F7:
		return "F7";
	case sf::Keyboard::F8:
		return "F8";
	case sf::Keyboard::F9:
		return "F9";
	case sf::Keyboard::F10:
		return "F10";
	case sf::Keyboard::F11:
		return "F11";
	case sf::Keyboard::F12:
		return "F12";
	case sf::Keyboard::F13:
		return "F13";
	case sf::Keyboard::F14:
		return "F14";
	case sf::Keyboard::F15:
		return "F15";
	case sf::Keyboard::Pause:
		return "Pause";
	}
}

//Returns the name of a button associated with the int - Jason Lynch 
const char* PlayerOneInputSettingsState::getJoyStickButtonNamePS4(const int buttonVal) {
	switch (buttonVal) {
	case 0:
		return "Square";
	case 1:
		return "X";
	case 2:
		return "O";
	case 3:
		return "Triangle";
	case 4:
		return "L1";
	case 5:
		return "R1";
	case 6:
		return "L2";
	case 7:
		return "R2";
	case 8:
		return "Share Btn";
	case 9:
		return "Options Btn";
	case 10:
		return "R Stick Click";
	case 11:
		return "L Stick Click";
	case 12:
		return "Home Btn";
	case 13:
		return "Touch Pad";
	default:
		return "Unknown";
	}
}
