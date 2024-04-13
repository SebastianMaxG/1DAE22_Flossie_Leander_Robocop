#pragma once
#include <vector>
#include "Vector2f.h"
class Texture;

class World final
{
public:
	World(const Texture* background);
	~World();
	void DrawBackground() const;
	void DrawForeground() const;
	void HandleCollision(Rectf& actorShape, Vector2f& actorVelocity) const;
	bool IsOnGround(const Rectf& actorShape) const;
	bool HasReachedEnd(const Rectf& actorShape);
	const Rectf GetBoundries() const;
	const Rectf GetLevelBoundries() const;
	void SetLevelBoundries(const Rectf* boundries);
	//index 0-8 (9 is out of bounds)
	const std::vector< Point2f >& GetWindowPositions() const;
private:
	const Texture* m_pBackgroundTexture;
	std::vector< Point2f > m_Vertices;
	std::vector< Point2f > m_Windows;
	const Rectf* m_LevelBoundries;
	
};


