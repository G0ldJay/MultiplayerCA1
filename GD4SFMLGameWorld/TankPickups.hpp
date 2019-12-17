//Jason Lynch - D00137655
//Dylan Reilly D00194504
#pragma once
#include "Entity.hpp"
#include "Command.hpp"
#include "ResourceIdentifiers.hpp"
#include "TankPickupID.hpp"

#include <SFML/Graphics/Sprite.hpp>


class Tank;

class TankPickups : public Entity //Created by me to spawn pickups for tanks. Based off of old pickup class - Jason Lynch
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
