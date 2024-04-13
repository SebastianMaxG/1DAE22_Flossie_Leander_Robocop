#pragma once
#include "Objects.h"
class Game;
class MechEnemy : public Object
{
public:
	MechEnemy(const Texture* texture, Point2f pos, int health, Game* game);
	~MechEnemy();
	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const World& world) override;

private:
	virtual bool UpdateVelocity(float elapsedSec, const World& world) override;
	//const Rectf* m_PlayerShape;
	Game* m_Game;

	const int	m_NrOfFrames;
	const int	m_NrOfRows;
	const int	m_NrFramesPerSec;
	float		m_AnimTime;
	float		m_timeSinceFrame;
	int			m_AnimFrame;
	int			m_FramesMoved;
	const float	m_MovementAmountArr[8];

	int m_NumberBullets;
	int m_BulletsShot;
	float m_TimeSinceLastShot;
	const float m_TimeBetweenShots;
	bool m_Reloading;
	// 25 25 50 25 25 25 50 25
};