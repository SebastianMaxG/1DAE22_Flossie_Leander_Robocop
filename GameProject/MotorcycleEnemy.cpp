#include "pch.h"
#include "MotorcycleEnemy.h"
#include "World.h"
#include "Texture.h"
#include "utils.h"

MotorcycleEnemy::MotorcycleEnemy(const Texture* texture, Point2f pos, const Rectf* playerShape)
	: Object(texture, 100, Rectf(pos.x, pos.y, 127.f/2, 93.f/2 - 10), Vector2f(200.f, 400.f), false, 1)
	, m_PlayerShape{ playerShape }
	, m_Grounded{}
	, m_Passes{}
	, m_MaxPasses{2}
{
	m_Acceleration.x = 150;
}

MotorcycleEnemy::~MotorcycleEnemy()
{
}

void MotorcycleEnemy::Draw() const
{

	Rectf srcRect{ 0,
				m_Texture->GetHeight(),
				m_Texture->GetWidth(),
				m_Texture->GetHeight() };
	Rectf destRect{ 0.f,
					0.f,
					GetShape().width,
					GetShape().height + 10 };
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
	//utils::FillRect(GetShape());
}

void MotorcycleEnemy::Update(float elapsedSec, const World& world)
{
	Move(elapsedSec, UpdateVelocity(elapsedSec, world));
	
	world.HandleCollision(GetShape(), m_Velocity);

	if (m_Passes >= m_MaxPasses && !IsDead()) {
		Damage(100);
	}
}

bool MotorcycleEnemy::UpdateVelocity(float elapsedSec, const World& world)
{
	const float jumpDistance{60};
	bool grounded = world.IsOnGround(GetShape());
	if (m_Passes < m_MaxPasses)
	{
		if (GetShape().left > (m_PlayerShape->left + jumpDistance) 
			&& (m_Velocity.x > -m_Speed.x))
		{
			m_Velocity.x -= elapsedSec * m_Acceleration.x;
		}
		if (GetShape().left < (m_PlayerShape->left - jumpDistance) 
			&& (m_Velocity.x < m_Speed.x))
		{
			m_Velocity.x += elapsedSec * m_Acceleration.x;
		}
	}
	if (m_Velocity.x > 0 and !m_Direction)
	{
		m_Direction = true;
		m_Passes++;
	}
	if (m_Velocity.x < 0 and m_Direction)
	{
		m_Direction = false;
		m_Passes++;
	}
	if (grounded && (m_Velocity.x < 0) && (GetShape().left < (m_PlayerShape->left + jumpDistance)) 
		&& (m_PlayerShape->left < GetShape().left))
	{
		m_Velocity.y = m_Speed.y;
	}
	m_Grounded = grounded;
	return grounded;
}
