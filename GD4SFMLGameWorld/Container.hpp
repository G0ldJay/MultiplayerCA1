//Jason Lynch - D00137655
//Dylan Reilly D00194504
#pragma once
#include "Component.hpp"
#include <memory>
#include <vector>

namespace GUI
{
	class Container : public Component
	{
	public:
		typedef std::shared_ptr<Container> Ptr;

	public:
		Container();
		void pack(Component::Ptr component);
		virtual bool isSelectable() const;
		virtual void handleEvent(const sf::Event& event);

	private:
		virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

		bool hasSelection() const;
		void select(std::size_t index);
		void selectNext();
		void selectPrevious();

	private:
		std::vector<Component::Ptr> mChildren;
		int mSelectedChild;
	};
}