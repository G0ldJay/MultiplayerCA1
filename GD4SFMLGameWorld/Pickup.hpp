//Jason Lynch - D00137655
//Dylan Reilly D00194504
#pragma once
#include "Entity.hpp"
#include "Command.hpp"
#include "ResourceIdentifiers.hpp"
#include "PickUpID.hpp"

#include <SFML/Graphics/Sprite.hpp>


class Tank;

class Pickup : public Entity
{
public:
	Pickup(PickupID type, const TextureHolder& textures);

	virtual unsigned int	getCategory() const;
	virtual sf::FloatRect	getBoundingRect() const;

	void 					apply(Tank& player) const;


protected:
	virtual void			drawCurrent(sf::RenderTarget& target, sf::RenderStates states) const;


private:
	PickupID 					mType;
	sf::Sprite				mSprite;
};
