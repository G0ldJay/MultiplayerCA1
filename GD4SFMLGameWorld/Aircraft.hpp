#pragma once
#include "Entity.hpp"
#include "AircraftID.hpp"
#include "ResourceIdentifiers.hpp"
#include"CommandQueue.hpp"

class Aircraft : public Entity
{
public:
	Aircraft(AircraftID type, const TextureHolder& textures, const FontHolder& fonts);
	virtual unsigned int getCategory() const;

private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	void updateMovementPattern(sf::Time dt);
	void updateTexts();

private:
	AircraftID mType;
	sf::Sprite mSprite;
	TextNode* mHealthDisplay;
	TextNode* mMissileDisplay;
	int mMissileAmmo;
	float mTavelledDistance;
	std::size_t mDirectionIndex;
};