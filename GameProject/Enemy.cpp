#include "pch.h"
#include "Enemy.h"
#include "World.h"
#include "Texture.h"

Enemy::Enemy(const Texture* texture, Point2f pos, const Rectf* playerShape)
	: Object(texture, 1, Rectf(pos.x, pos.y, 31.5f, 55.f), Vector2f(70.f, 400.f))
	, m_PlayerShape{ playerShape }
	, m_NrOfFrames{ 3 }
	, m_NrFramesPerSec{ 8 }
	, m_AnimTime{}
	, m_AnimFrame{}
{
}
Enemy::~Enemy()
{

}
void Enemy::Draw() const
{
	Rectf srcRect{ m_AnimFrame * GetShape().width*2,
					GetShape().height*2,
					GetShape().width*2,
					GetShape().height*2 };
	Rectf destRect{	0.f,
					0.f,
					GetShape().width,
					GetShape().height };

	glPushMatrix();
	{
		//glTranslatef(-GetShape().width / 2, 0.f, 0.f);
		glTranslatef(GetShape().left, GetShape().bottom, 0.f);
		if (!m_Direction)
		{
			glScalef(-1, 1, 1);
			glTranslatef(-GetShape().width, 0, 0);
		}
		m_Texture->Draw(destRect,srcRect);
	}
	glPopMatrix();
}
void Enemy::Update(float elapsedSec, const World& world)
{
	Move(elapsedSec, UpdateVelocity(elapsedSec, world));
	m_AnimTime += elapsedSec;
	m_AnimFrame = int(m_AnimTime * m_NrFramesPerSec);
	if (m_AnimFrame >= m_NrOfFrames)
	{
		m_AnimFrame = 0;
		m_AnimTime	= 0;
	}
	world.HandleCollision(GetShape(), m_Velocity);
}

bool Enemy::UpdateVelocity(float elapsedSec, const World& world)
{
	bool grounded = world.IsOnGround(GetShape());

	if (GetShape().left > m_PlayerShape->left)
	{
		m_Velocity.x = -m_Speed.x;
		m_Direction = true;
	}
	else
	{
		m_Velocity.x = m_Speed.x;
		m_Direction = false;

	}

	/*if (m_Velocity.x > -m_Speed.x and m_Velocity.x < m_Speed.x)
	{
		if (GetShape().left > m_PlayerShape->left)
		{
			m_Velocity.x -= elapsedSec * m_Acceleration.x;
		}
		else
		{
			m_Velocity.x += elapsedSec * m_Acceleration.x;
		}
	}*/
	return grounded;
}
