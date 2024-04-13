#include "pch.h"
#include "AmmoCrate.h"
#include "Texture.h"
#include "utils.h"

AmmoCrate::AmmoCrate(const Texture* texture, Point2f position, Player* player)
	:Crate(texture, position, player)
	, m_On{}
	, m_TimePassed{}
	, m_OnOffTime{1.f}
{
}

AmmoCrate::~AmmoCrate()
{
	m_Player->ActivatePowerUp();
}

void AmmoCrate::Draw() const
{

	Rectf srcRect{ 0, GetShape().height * 2, GetShape().width * 2, GetShape().height * 2 };
	if (m_On)
	{
		srcRect.left = GetShape().width * 2 + 12.f;
	}

	Rectf destRect{ 0, 0, GetShape().width, GetShape().height };

	glPushMatrix();
	{
		glTranslatef(GetShape().left, GetShape().bottom, 0.f);
		if (!m_Direction)
		{
			glScalef(-1, 1, 1);
			glTranslatef(-GetShape().width, 0, 0);
		}
		m_Texture->Draw(destRect, srcRect);
	}
	glPopMatrix();
	//utils::FillRect(GetShape());
}

void AmmoCrate::Update(float elapsedSec, const World& world)
{
	m_TimePassed += elapsedSec;
	if (m_TimePassed >= m_OnOffTime)
	{
		m_TimePassed = 0;
		m_On = !m_On;
	}
	Move(elapsedSec, UpdateVelocity(elapsedSec, world));
	world.HandleCollision(GetShape(), m_Velocity);
}
