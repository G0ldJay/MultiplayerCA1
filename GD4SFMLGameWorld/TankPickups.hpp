#pragma once
#include "Entity.hpp"
#include "Command.hpp"
#include "ResourceIdentifiers.hpp"
#include "TankPickupID.hpp"

#include <SFML/Graphics/Sprite.hpp>


class Tank;

class TankPickups : public Entity
{
public:
	TankPickups(TankPickupID type, const TextureHolder& textures);

	virtual unsigned int	getCategory() const;
	virtual sf::FloatRect	getBoundingRect() const;

	void 					apply(Tank& player) const;


protected:
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	TankPickupID 			mType;
	sf::Sprite				mSprite;
};
