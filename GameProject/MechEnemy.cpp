#include "pch.h"
#include "MechEnemy.h"
#include "World.h"
#include "Texture.h"
#include "utils.h"
#include "Game.h"

MechEnemy::MechEnemy(const Texture* texture, Point2f pos, int health, Game* game)
	: Object(texture, health, Rectf(pos.x, pos.y, 260.f/2, 256.f/2), Vector2f(30.f, 0.f), false)
	, m_Game{game}
	, m_NrOfFrames{ 8 }
	, m_NrOfRows{1}
	, m_NrFramesPerSec{ 3 }
	, m_AnimTime{}
	, m_timeSinceFrame{}
	, m_AnimFrame{}
	, m_FramesMoved{}
	, m_MovementAmountArr{25.f,25.f,50.f,25.f,25.f,25.f,50.f,25.f}
	, m_NumberBullets{}
	, m_BulletsShot{}
	, m_TimeSinceLastShot{}
	, m_TimeBetweenShots{ 0.2f }
	, m_Reloading{ false }
{
	m_Acceleration.y = 0;
}

MechEnemy::~MechEnemy()
{
}

void MechEnemy::Draw() const
{
	Rectf srcRect{	(m_AnimFrame % (m_NrOfFrames / m_NrOfRows)) * (GetShape().width  * 2),
					/*(m_AnimFrame / (m_NrOfFrames / m_NrOfRows)) * */(GetShape().height * 2),
					GetShape().width  * 2,
					GetShape().height * 2 };
	Rectf destRect{ 0.f,
					0.f,
					GetShape().width,
					GetShape().height };

	if (!m_Direction)
	{
		srcRect.bottom *= 2;
	}

	glPushMatrix();
	{
		glTranslatef(GetShape().left, GetShape().bottom, 0.f);
		m_Texture->Draw(destRect, srcRect);
	}
	glPopMatrix();
	utils::FillRect(GetShape());
}

void MechEnemy::Update(float elapsedSec, const World& world)
{

	Move(elapsedSec, UpdateVelocity(elapsedSec, world));

}

bool MechEnemy::UpdateVelocity(float elapsedSec, const World& world)
{
	//Moving
	m_AnimTime += elapsedSec;
	m_timeSinceFrame += elapsedSec;
	if (m_timeSinceFrame >= 1.f / m_NrFramesPerSec)
	{
		if (m_Direction)
		{
			GetShape().left += m_MovementAmountArr[m_AnimFrame] / 2.f;
		}
		else
		{
			GetShape().left -= m_MovementAmountArr[(m_NrOfFrames - m_AnimFrame - 1)] / 2.f;
		}
		m_AnimFrame++;
		m_FramesMoved++;
		m_timeSinceFrame = 0;
	}
	m_AnimFrame %= m_NrOfFrames;
	if (m_FramesMoved >= 12)
	{
		m_Direction = !m_Direction;
		m_AnimTime = 0.f;
		m_FramesMoved = 0;
	}

	//Shooting
	if (((m_FramesMoved % 6) == 0) && m_Reloading)
	{
		m_Reloading = false;
		m_NumberBullets = rand() % 3 ;
	}

	m_TimeSinceLastShot += elapsedSec;
	if (m_Reloading)
	{
	}
	else if (m_TimeSinceLastShot > m_TimeBetweenShots)
	{
		Point2f pos{ GetShape().left , GetShape().bottom + 55};
		float angle{200};
		
		m_Game->MakeBullet(angle, 160, pos, true);
		m_TimeSinceLastShot = 0;
		if (m_BulletsShot++ >= m_NumberBullets) //add after compare dince m_NumberBullets is 0-4 and not 1-5
		{
			m_Reloading = true;
			m_BulletsShot = 0;
		}
	}
	return true;
}
