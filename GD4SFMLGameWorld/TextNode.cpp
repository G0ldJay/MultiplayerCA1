#include "TextNode.hpp"

#include <SFML/Graphics/RenderTarget.hpp>

TextNode::TextNode(const FontHolder& fonts, const std::string& text, sf::Color colour)
{
	mText.setFont(fonts.get(FontID::Main));
	mText.setCharacterSize(20);
	setString(text);
	setColor(colour);
}

void TextNode::setString(const std::string& text)
{
	mText.setString(text);
	centreOrigin(mText);
}

void TextNode::setColor(const sf::Color colour)
{
	mText.setFillColor(colour);
}

void TextNode::drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(mText, states);
}
