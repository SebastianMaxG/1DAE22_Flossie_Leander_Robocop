#pragma once
#include "Objects.h"
class Game;
class WindowEnemy : public Object
{
public:
	WindowEnemy(const Texture* texture, Point2f pos, const Rectf* playerShape, Game* game);
	~WindowEnemy();
	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const World& world) override;

private:
	virtual bool UpdateVelocity(float elapsedSec, const World& world) override;
	const Rectf* m_PlayerShape;
	Game* m_Game;
	bool m_Angled;
	int m_NumberBullets;
	int m_BulletsShot;
	float m_TimeSinceLastShot;
	const float m_TimeBetweenShots;
	const float m_ReloadTime;
	bool m_Reloading;
};

