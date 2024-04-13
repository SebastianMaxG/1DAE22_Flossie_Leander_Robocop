#pragma once
#include "Objects.h"


class Projectile : public Object
{
public:
	Projectile(float angle, float speed, Point2f pos,const Texture* bulletTexture);
	~Projectile();
	void Draw() const override;
	void Update(float elapsedSec, const World& world) override;
	Point2f GetPos();
private:
	float m_Angle;
	bool UpdateVelocity(float elapsedSec, const World& world) override;
	
	float cosDeg(float angle);
	float sinDeg(float angle);
};


