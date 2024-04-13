#include "pch.h"
#include "WindowEnemy.h"
#include "World.h"
#include "Texture.h"
#include "Game.h"
#include "utils.h"

WindowEnemy::WindowEnemy(const Texture* texture, Point2f pos, const Rectf* playerShape, Game* game)
	: Object(texture, 1, Rectf(pos.x - 11.5f, pos.y-9.f, 23.f, 30.f), Vector2f(0, 0), true, 2)
	, m_PlayerShape{ playerShape }
	, m_Game{game}
	, m_Angled{}
	, m_NumberBullets{}
	, m_BulletsShot{}
	, m_TimeSinceLastShot{}
	, m_TimeBetweenShots{0.2f}
	, m_ReloadTime{4.f}
	, m_Reloading{false}
{
	m_Acceleration = Vector2f(0, 0);
	m_NumberBullets = (rand() % 5);
}

WindowEnemy::~WindowEnemy()
{
}

void WindowEnemy::Draw() const
{
	Rectf srcRect{ 0,
					GetShape().height * 2.f + 22.f,
					m_Texture->GetWidth(),
					GetShape().height * 2.f };
	

	Rectf destRect{ 0.f,
					0.f,
					GetShape().width,
					GetShape().height };
	if (!m_Angled)
	{
		srcRect.height += 22.f;
		destRect.height += 11.f;
		destRect.bottom -= 11.f;
	}
	else
	{
		srcRect.bottom += GetShape().height * 2.f;
	}

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

void WindowEnemy::Update(float elapsedSec, const World& world)
{
	m_Angled = false;
	if (GetShape().left > m_PlayerShape->left)
	{
		m_Direction = true;
		if (GetShape().left > m_PlayerShape->left + m_PlayerShape->width)
		{
			m_Angled = true;
		}
	}
	else
	{
		if (GetShape().left + GetShape().width < m_PlayerShape->left)
		{
			m_Angled = true;
		}
		m_Direction = false;
	}
	

	m_TimeSinceLastShot += elapsedSec;
	if (m_Reloading)
	{
		if (m_TimeSinceLastShot > m_ReloadTime)
		{
			m_Reloading = false;
			//m_TimeSinceLastShot = 0;
		}
	}
	else if (m_TimeSinceLastShot > m_TimeBetweenShots)
	{
		Point2f pos{ GetShape().left , GetShape().bottom };
		float angle{};
		if (m_Angled)
		{
			if (m_Direction)
			{
				angle = 215;
			}
			else 
			{
				angle = 315;
				pos.x += GetShape().width;
			}
		}
		else
		{
			angle = 270;
			pos.x += GetShape().width / 2.f;
		}
		m_Game->MakeBullet(angle, 160, pos, true);
		m_TimeSinceLastShot = 0;
		if (m_BulletsShot++ >= m_NumberBullets) //add after compare dince m_NumberBullets is 0-4 and not 1-5
		{
			m_Reloading = true;
			m_BulletsShot = 0;
		}
	}

	Move(elapsedSec, true);
}

bool WindowEnemy::UpdateVelocity(float elapsedSec, const World& world)
{
	//doesn't move
	return false;
}
