#pragma once
class Texture;
class HUD
{
public:
	HUD(const Texture* texture, const int* health, const int* powerBullets);
	~HUD();

	void Draw(const float screenWidth) const;

private:
	const int* m_Health;
	const Texture* m_Texture;
	const int* m_PowerBullets;
};


