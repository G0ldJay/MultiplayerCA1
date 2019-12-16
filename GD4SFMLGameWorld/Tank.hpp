#pragma once
#include "Entity.hpp"
#include "TankID.hpp"
#include "ResourceIdentifiers.hpp"
#include "CommandQueue.hpp"
#include "TextNode.hpp"
#include "Projectile.hpp"
#include "Animation.hpp"

class Tank : public Entity
{
public:
	Tank(CategoryID entity,TankID type, const TextureHolder& textures, const FontHolder& fonts);
	virtual unsigned int getCategory() const;
	virtual sf::FloatRect getBoundingRect() const;
	virtual bool isMarkedForRemoval() const;

	float getMaxSpeed() const;
	void fire();
	void launchMissile();
	bool isAllied() const;
	ProjectileID getProjectile() const;
	void increaseFireRate();
	void increaseSpread();
	void setTankTexture(unsigned int val); //Will allow texture swap for picking up weapon power ups - Jason Lynch
	void collectMissiles(unsigned int count);

	void playerLocalSound(CommandQueue& command, SoundEffectID effect);
	void CorrectFiringSound(CommandQueue& commands);

private:
	virtual void drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;
	virtual void updateCurrent(sf::Time dt, CommandQueue& commands);
	//void updateMovementPattern(sf::Time dt);
	void updateTexts();

	void checkProjectileLaunch(sf::Time dt, CommandQueue& commands);

	void createBullets(SceneNode& node, const TextureHolder& textures) const;
	void createProjectile(SceneNode& node, ProjectileID type, float xOffset, float yOffset, const TextureHolder& textures) const;

	void createPickup(SceneNode& node, const TextureHolder& textures) const;
	void checkPickupDrop(CommandQueue& commands);

private:
	CategoryID mEntity;
	TankID mType;
	sf::Sprite mSprite;
	Animation mExplosion;
	TextNode* mHealthDisplay;
	TextNode* mMissileDisplay;
	const TextureHolder& mTextures; //Hold texture for tank changes - Jason Lynch

	bool mIsFiring;
	bool mIsLaunchingMissile;
	int	mFireRateLevel;
	sf::Time mFireCountdown;

	bool mIsMarkedForRemoval;

	Command mFireCommand;
	Command	mMissileCommand;
	Command mDropPickupCommand;
	bool mShowExplosion;
	bool mPlayedExplosionSound;
	bool mSpawnedPickup;
	int mSpreadLevel;

	int mMissileAmmo;
	float mTravelledDistance;
	std::size_t mDirectionIndex;
};