#pragma once
#include "SceneNode.hpp"
#include "ResourceHolder.hpp"
#include "ResourceIdentifiers.hpp"
#include "Utility.hpp"

#include <SFML/Graphics/Text.hpp>

class TextNode : public SceneNode
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