#pragma once
#include "Entity.hpp"
#include "Command.hpp"
#include "ResourceIdentifiers.hpp"
#include "ObstacleID.hpp"
#include "TextNode.hpp"

#include <SFML/Graphics/Sprite.hpp>

class Obstacle : public Entity
{
public:
	Obstacle(ObstacleID type, const TextureHolder& textures, const FontHolder& fonts);

	virtual unsigned int	getCategory() const;
	virtual unsigned int	getDamage() const;
	virtual sf::FloatRect	getBoundingRect() const;
	void updateTexts();
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	void playerLocalSound(CommandQueue& command, SoundEffectID effect);

protected:
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	ObstacleID mType;
	sf::Sprite mSprite;
	Animation mExplosion;
	bool mShowExplosion;
	bool mPlayedExplosionSound;
	TextNode* mHealthDisplay;
};
