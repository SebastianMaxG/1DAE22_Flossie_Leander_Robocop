#include "pch.h"
#include "View.h"
#include "utils.h"

View::View(float width, float height)
	:m_Width{ width }
	, m_Height{ height }
	, m_LevelBoundaries{ 0,0,width,height }
	, m_CurrentPos{}
	, m_MoveTolerence{10.f}
{
}

View::~View()
{
}

void View::SetLevelBoundaries(const Rectf& levelBoundaries)
{
	this->m_LevelBoundaries = levelBoundaries;
}

const Rectf* View::GetLevelBoundaries() const
{
	return &m_LevelBoundaries;
}

void View::Draw() const
{
	
	//utils::SetColor(Color4f(0, 0, 1, 1));
	//utils::DrawRect(bottomLeftPos, m_Width, m_Height);
	glTranslatef(-m_CurrentPos.x, -m_CurrentPos.y, 0);

}

void View::Update(const Rectf& target)
{
	Track(target);
	Clamp(m_CurrentPos);
}

void View::BossMode()
{
	m_LevelBoundaries.left = m_LevelBoundaries.width - m_Width;
	m_LevelBoundaries.width = m_Width;
}


Point2f View::Track(const Rectf& target) 
{
	Point2f bottomLeftPos
	{
		target.left ,
		target.bottom
	};
	float moveDis{};
	bottomLeftPos.x -= m_Width / 2;
	bottomLeftPos.y -= m_Height / 2;
	if (m_CurrentPos.x < bottomLeftPos.x - m_MoveTolerence)
	{
		moveDis = m_CurrentPos.x - (bottomLeftPos.x - m_MoveTolerence);
	}
	if (m_CurrentPos.x > bottomLeftPos.x + m_MoveTolerence)
	{
		moveDis = m_CurrentPos.x - (bottomLeftPos.x + m_MoveTolerence);
	}
	m_CurrentPos.x -= moveDis;

	m_CurrentPos.y = bottomLeftPos.y;
	//m_CurrentPos.x = bottomLeftPos.x;

	return m_CurrentPos;
}

void View::Clamp(Point2f& bottomLeftPos) 
{
	if (!(bottomLeftPos.x >= m_LevelBoundaries.left))
	{
		bottomLeftPos.x = m_LevelBoundaries.left;
	}
	else if (!(bottomLeftPos.x + m_Width <= m_LevelBoundaries.left + m_LevelBoundaries.width))
	{
		bottomLeftPos.x = m_LevelBoundaries.left + m_LevelBoundaries.width - m_Width;
	}

	if (!(bottomLeftPos.y >= m_LevelBoundaries.bottom))
	{
		bottomLeftPos.y = m_LevelBoundaries.bottom;
	}
	else if (!(bottomLeftPos.y + m_Height <= m_LevelBoundaries.bottom + m_LevelBoundaries.height))
	{
		bottomLeftPos.y = m_LevelBoundaries.bottom + m_LevelBoundaries.height - m_Height;
	}
}
