#include "pch.h"
#include "InteractObjects.h"
#include "utils.h"
#include "Texture.h"
#include "World.h"

InteractObject::InteractObject(const Texture* texture, size_t health, Rectf shape, Player* player, bool direction)
	: Object(texture,health,shape,Vector2f(0,0),direction,0)
	, m_Player{player}
{

}


InteractObject::~InteractObject()
{
}

void InteractObject::Update(float elapsedSec, const World& world)
{
	Move(elapsedSec, UpdateVelocity(elapsedSec, world));
	world.HandleCollision(GetShape(), m_Velocity);

}

bool InteractObject::UpdateVelocity(float elapsedSec, const World& world)
{

	return world.IsOnGround(GetShape());
}
