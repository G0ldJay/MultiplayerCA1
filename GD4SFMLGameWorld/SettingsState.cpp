#include "SettingsState.hpp"

SettingsState::SettingsState(StateStack& stack, Context context):State(stack, context), mGUIContainer()
{
	mBackgroundSprite.setTexture(context.textures->get(TextureID::TitleScreen));

	addButtonLabel(ActionID::MoveLeft, 150.0f, "Move Left", context);
	addButtonLabel(ActionID::MoveRight, 200.0f, "Move Right", context);
	addButtonLabel(ActionID::MoveUp, 250.0f, "Move Up", context);
	addButtonLabel(ActionID::MoveDown, 300.0f, "Move Down", context);

	updateLabels();
	
	auto backButton = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	backButton->setPosition(80.0f, 375.0f);
	backButton->setText("Back");
	backButton->setCallback(std::bind(&SettingsState::requestStackPop, this));

	mGUIContainer.pack(backButton);
}

void SettingsState::draw()
{
	sf::RenderWindow& window = *getContext().window;
	window.draw(mBackgroundSprite);
	window.draw(mGUIContainer);
}

bool SettingsState::update(sf::Time dt)
{
	return true;
}

bool SettingsState::handleEvent(const sf::Event& event)
{
	bool isKeyBinding = false;

	//Iterate through all keybinding buttons to see if they are being pressed, waiting for the user to enter a key
	for (std::size_t action = 0; action < static_cast<int>(ActionID::ActionCount); ++action) {
		if (mBindingButtons[action]->isActive()) {
			isKeyBinding = true;

			if (event.type == sf::Event::KeyReleased) {
				getContext().player->assignKey(static_cast<ActionID>(action), event.key.code);
				mBindingButtons[action]->deactivate();
			}
			break;
		}
	}

	if (isKeyBinding) {
		updateLabels();
	}
	else {
		mGUIContainer.handleEvent(event);
	}

	return false;
}

void SettingsState::updateLabels()
{
	Player& player = *getContext().player;
	
	for (std::size_t i = 0; i < static_cast<int>(ActionID::ActionCount); ++i) {
		sf::Keyboard::Key key = player.getAssignedKey(static_cast<ActionID>(i));
		mBindingLabels[i]->setText(toString(key));
	}
}

void SettingsState::addButtonLabel(ActionID action, float y, const std::string& text, Context context)
{
	mBindingButtons[static_cast<int>(action)] = std::make_shared<GUI::Button>(*context.fonts, *context.textures);
	mBindingButtons[static_cast<int>(action)]->setPosition(80.0f, y);
	mBindingButtons[static_cast<int>(action)]->setText(text);
	mBindingButtons[static_cast<int>(action)]->setToggle(true);

	mBindingLabels[static_cast<int>(action)] = std::make_shared < GUI::Label>("", *context.fonts);
	mBindingLabels[static_cast<int>(action)]->setPosition(300.0f, y+15.0f);

	mGUIContainer.pack(mBindingButtons[static_cast<int>(action)]);
	mGUIContainer.pack(mBindingLabels[static_cast<int>(action)]);
}
