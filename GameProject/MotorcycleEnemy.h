#pragma once
#include "Objects.h"
class MotorcycleEnemy : public Object
{
public:
	MotorcycleEnemy(const Texture* texture, Point2f pos, const Rectf* playerShape);
	~MotorcycleEnemy();
	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const World& world) override;

private:
	virtual bool UpdateVelocity(float elapsedSec, const World& world) override;
	const Rectf* m_PlayerShape;
	bool m_Grounded;
	int m_Passes;
	const int m_MaxPasses;
};

