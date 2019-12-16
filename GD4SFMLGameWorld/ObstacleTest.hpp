#pragma once
#include "Entity.hpp"
#include "ObstacleID.hpp"
#include "ResourceIdentifiers.hpp"
#include "CommandQueue.hpp"
#include "TextNode.hpp"
#include "Projectile.hpp"
#include "Animation.hpp"

class ObstacleTest : public Entity
{
public:
	ObstacleTest(ObstacleID type, const TextureHolder& textures, const FontHolder& fonts);
	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;
	virtual bool isMarkedForRemoval() const;
	virtual unsigned int getDamage() const;

	void playerLocalSound(CommandQueue& command, SoundEffectID effect);

private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	void updateTexts();

private:
	ObstacleID mType;
	sf::Sprite mSprite;
	Animation mExplosion;
	TextNode* mHealthDisplay;

	bool mIsMarkedForRemoval;
	bool mShowExplosion;
	bool mPlayedExplosionSound;
};