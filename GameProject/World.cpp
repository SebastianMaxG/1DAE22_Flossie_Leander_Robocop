#include "pch.h"
#include "World.h"
#include "utils.h"
#include "Texture.h"
#include "SVGParser.h"

World::World(const Texture* background)
	: m_pBackgroundTexture{background}
	, m_LevelBoundries{nullptr}
{
	std::vector<std::vector<Point2f>> allVertices{};
	SVGParser::GetVerticesFromSvgFile("Robocop/Background.svg", allVertices);
	m_Vertices = allVertices[0];
	m_Windows = allVertices[1];
}

World::~World()
{
}

void World::DrawBackground() const
{
	m_pBackgroundTexture->Draw();
}

void World::DrawForeground() const
{
}

void World::HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const
{
	utils::HitInfo hitInfo{};
	Point2f topMid{ actorShape.left + actorShape.width / 2,actorShape.bottom + actorShape.height };
	Point2f bottomMid{ actorShape.left + actorShape.width / 2,actorShape.bottom };
	if (utils::Raycast(m_Vertices, topMid, bottomMid, hitInfo))
	{
		actorVelocity.y = 0;
		actorShape.bottom = hitInfo.intersectPoint.y;
	}
}

bool World::IsOnGround(const Rectf& actorShape) const
{
	utils::HitInfo hitInfo{};
	Point2f topMid{ actorShape.left + actorShape.width / 2,actorShape.bottom + actorShape.height };

	Point2f bottomMid{ actorShape.left + actorShape.width / 2,actorShape.bottom - 1 };

	return utils::Raycast(m_Vertices, topMid, bottomMid, hitInfo);
}

bool World::HasReachedEnd(const Rectf& actorShape)
{
	return false;
}

const Rectf World::GetBoundries() const
{
	return Rectf(0, 0, m_pBackgroundTexture->GetWidth(), m_pBackgroundTexture->GetHeight());
}

const Rectf World::GetLevelBoundries() const
{
	return *m_LevelBoundries;
}

void World::SetLevelBoundries(const Rectf* boundries)
{
	m_LevelBoundries = boundries;
}

const std::vector<Point2f>& World::GetWindowPositions() const
{
	return m_Windows;
}
