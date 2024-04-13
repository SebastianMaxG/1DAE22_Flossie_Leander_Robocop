#include "pch.h"
#include "HUD.h"
#include "Texture.h"

HUD::HUD(const Texture* texture, const int* health, const int* powerBullets)
	: m_Texture{texture}
	, m_Health{health}
	, m_PowerBullets{powerBullets}
{
}

HUD::~HUD()
{
}

void HUD::Draw(const float screenWidth) const
{
	Rectf srcRect{  0,
					m_Texture->GetHeight() - float(*m_Health * 24),
					m_Texture->GetWidth(),
					24 };
	Rectf destRect{ screenWidth/2 - m_Texture->GetWidth(),10,
		m_Texture->GetWidth() * 2, srcRect.height*2 };
	m_Texture->Draw(destRect, srcRect);

	std::string fontPath{ "Robocop/PixelTandysoft.ttf" };

	Texture bulletNumber{ std::to_string(*m_PowerBullets),
		fontPath,
		40, Color4f(0.1f, 1.f, 0.3f, 1.f) };
	
	bulletNumber.Draw(Point2f(screenWidth - bulletNumber.GetWidth() - 10, 10));
}
