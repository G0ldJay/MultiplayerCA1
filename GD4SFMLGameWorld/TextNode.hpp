//Jason Lynch - D00137655
//Dylan Reilly D00194504
#pragma once
#include "SceneNode.hpp"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/Text.hpp>

class TextNode : public SceneNode //Modified by me to accept a color as a parameter - Jason Lynch
{
public:
	explicit TextNode(const FontHolder& fonts, const std::string& text, sf::Color colour);
	void setString(const std::string& text);
	void setColor(const sf::Color colour);

private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;

private:
	sf::Text mText;
};