#include "pch.h"
#include "Objects.h"
#include "utils.h"
#include "Texture.h"
#include "World.h"


Object::Object(const Texture* texture, size_t health, Rectf shape, Vector2f speed, 
				bool direction, size_t damage)
	: m_Texture{texture}
	, m_Health{int(health)}
	, m_IsDead{false}
	, m_Shape{shape}
	, m_Velocity{ 0.0f, 0.0f }
	, m_Acceleration{ 50, -981.0f }
	, m_Speed{speed}
	, m_ActionState{ActionState::idle}
	, m_Direction{direction}
	, m_Damage{damage}
	, m_timeSinceDamage{}
	, m_SafeTime{0.2f}
{
}

Object::~Object()
{
}

void Object::Move(float elapsedSec, bool isOnGround) 
{
	m_timeSinceDamage += elapsedSec;
	m_Velocity.y += m_Acceleration.y * elapsedSec;
	m_Shape.left += elapsedSec * m_Velocity.x;
	m_Shape.bottom += elapsedSec * m_Velocity.y;

}

void Object::Damage(size_t amount)
{
	if (m_timeSinceDamage > m_SafeTime)
	{
		m_Health -= int(amount);
		if (m_Health <= 0)
		{
			m_IsDead = true;
		}
		m_timeSinceDamage = 0.f;
	}
}

void Object::Heal(size_t amount)
{
	m_Health += int(amount);
}

bool Object::IsDead()
{
	if (m_Health == 0)
	{
		m_ActionState = ActionState::dying;
		m_IsDead = true;
	}
	return m_IsDead;
}

const int* Object::GetHealth() const
{
	return &m_Health;
}



const Rectf& Object::GetShape() const
{
	
	return m_Shape;
}

Rectf& Object::GetShape()
{
	return m_Shape;
}

const size_t Object::GetDamage() const
{
	return m_Damage;
}

Vector2f& Object::GetVelocity()
{
	return m_Velocity;
}
