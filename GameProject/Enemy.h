#pragma once
#include "Objects.h"
class Enemy : public Object
{
public:
	Enemy(const Texture* texture, Point2f pos, const Rectf* playerShape);
	~Enemy();
	virtual void Draw() const override;
	virtual void Update(float elapsedSec, const World& world) override;

private:
	virtual bool UpdateVelocity(float elapsedSec, const World& world) override;
	const Rectf*	m_PlayerShape;

	const int m_NrOfFrames;
	const int m_NrFramesPerSec;
	float	m_AnimTime;
	int		m_AnimFrame;

};


