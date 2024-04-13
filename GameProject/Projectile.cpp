#include "pch.h"
#include "Projectile.h"
#include "Texture.h"
#include "utils.h"


Projectile::Projectile(float angle, float speed, Point2f pos, const Texture* bulletTexture)
	: Object(bulletTexture, 1, Rectf(pos.x, pos.y, bulletTexture->GetWidth(), bulletTexture->GetHeight())
	, Vector2f(cosDeg(angle)* speed, sinDeg(angle)* speed))
	, m_Angle(angle)
{
	
}

Projectile::~Projectile()
{
	
}

void Projectile::Draw() const
{
	glPushMatrix();
	{
		glTranslatef(GetShape().left, GetShape().bottom, 0.f);
		if (!m_Direction)
		{
			glScalef(-1, 1, 1);
			glTranslatef(-GetShape().width, 0, 0);
		}
		glRotatef(m_Angle, 0, 0, 1);
		m_Texture->Draw(Point2f(-GetShape().width, -GetShape().height / 2));
	}
	glPopMatrix();
	//utils::FillRect(GetShape());
}

void Projectile::Update(float elapsedSec, const World& world)
{
	GetShape().left		+= m_Speed.x * elapsedSec;
	GetShape().bottom	+= m_Speed.y * elapsedSec;
}

Point2f Projectile::GetPos()
{
	return Point2f(GetShape().left,GetShape().bottom);
}

bool Projectile::UpdateVelocity(float elapsedSec, const World& world)
{
	return false;
}

float Projectile::cosDeg(float angle)
{
	angle /= 180.f;
	angle *= float(M_PI);
	return cosf(angle);
}

float Projectile::sinDeg(float angle)
{
	angle /= 180.f;
	angle *= float(M_PI);
	return sinf(angle);
}
