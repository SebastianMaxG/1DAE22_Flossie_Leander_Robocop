#pragma once
#include "InteractObjects.h"

class Crate : public InteractObject
{
public:
	Crate(const Texture* texture, Point2f position, Player* player);
	virtual ~Crate();
	virtual void Draw() const override;
	virtual void Interact(Object* object) override;
private:

};



