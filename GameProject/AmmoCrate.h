#pragma once
#include "InteractObjects.h"
#include "Crate.h"
#include "SoundEffect.h";
class AmmoCrate final: public Crate
{
public:
	AmmoCrate(const Texture* texture, Point2f position, Player* player);
	~AmmoCrate();
	void Draw() const override;
	void Update(float elapsedSec, const World& world) override;
private:
	bool  m_On;
	float m_TimePassed;
	const float m_OnOffTime;
};

