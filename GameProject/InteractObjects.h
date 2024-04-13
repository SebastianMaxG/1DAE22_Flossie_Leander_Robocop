#pragma once
#include "Objects.h"
#include "player.h"

class InteractObject : public Object
{
public:
	InteractObject(const Texture* texture, size_t health, Rectf shape, Player* player, bool direction = true);
	virtual ~InteractObject();

	virtual void Draw() const = 0;
	virtual void Update(float elapsedSec, const World& world) override;
	virtual bool UpdateVelocity(float elapsedSec, const World& world) override;
	virtual void Interact(Object* object) = 0;
protected:
	Player* m_Player;
};

