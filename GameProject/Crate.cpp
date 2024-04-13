#include "pch.h"
#include "Crate.h"
#include "Texture.h"
#include "utils.h"
#include <vector>

Crate::Crate(const Texture* texture, Point2f position, Player* player)
	:InteractObject(texture, 3, Rectf(position.x, position.y, 32,32), player)
{
}

Crate::~Crate()
{
}

void Crate::Draw() const
{
	glPushMatrix();
	{
		glTranslatef(GetShape().left, GetShape().bottom, 0.f);
		Rectf srcRect{ 0, GetShape().height * 2, GetShape().width * 2, GetShape().height * 2 };
		Rectf destRect{ 0, 0, GetShape().width, GetShape().height };
		m_Texture->Draw(destRect, srcRect);
	}
	glPopMatrix();
}

void Crate::Interact(Object* object)
{
	if ((object->GetShape().bottom < GetShape().bottom + GetShape().height - 10.f) 
		and object->GetShape().bottom + object->GetShape().height - 10.f > GetShape().bottom)
	{
		if (object->GetShape().left < GetShape().left)
		{
			object->GetShape().left = GetShape().left - object->GetShape().width;
		}
		else
		{
			object->GetShape().left = GetShape().left + GetShape().width;
		}
	}
	else
	{
		if (object->GetShape().bottom >= GetShape().bottom + GetShape().height - 5)
		{
			object->GetShape().bottom = GetShape().bottom + GetShape().height;
			object->GetVelocity().y = 0;
			if (object == m_Player)
			{
				m_Player->m_Grounded = true;
			}
		}


		/*std::vector<Point2f> vertice{	Point2f(GetShape().left,					GetShape().bottom + GetShape().height), 
										Point2f(GetShape().left + GetShape().width,	GetShape().bottom + GetShape().height)};
		utils::HitInfo hitInfo{};
		Point2f topMid{ object->GetShape().left + object->GetShape().width / 2,object->GetShape().bottom + object->GetShape().height };
		Point2f bottomMid{ object->GetShape().left + object->GetShape().width / 2,object->GetShape().bottom };

		if (utils::Raycast(vertice, topMid, bottomMid, hitInfo))
		{
			object->GetVelocity().y = 0;
			object->GetShape().bottom = hitInfo.intersectPoint.y;
			if (object == m_Player)
			{
				m_Player->m_Grounded = true;
			}
		}*/
	}
}

