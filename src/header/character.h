#ifndef CHARACTER_H
#define CHARACTER_H

#include "entity.h"
#include <vector>

class Character : public RenderableEntity {
	public:
		// Constructor
		Character();
		// Build character entity with position and collision layer
		Character(Vector2, int);

	public:
		void update(Game *);
};

#endif
