#pragma once
class View final
{
public:
	View(float width, float height);
	~View();

	void SetLevelBoundaries(const Rectf& levelBoundaries);
	const Rectf* GetLevelBoundaries() const;
	void Draw() const;
	void Update(const Rectf& target);
	void BossMode();

private:
	Point2f Track(const Rectf& target);
	void Clamp(Point2f& bottomLeftPos);

	//members
	float m_Width;
	float m_Height;
	Rectf m_LevelBoundaries;
	Point2f m_CurrentPos;
	const float m_MoveTolerence;

};

