#include "pch.h"
#include "TextureManager.h"
#include "Texture.h"

TextureManager::TextureManager()
	: m_pBackground		{ new Texture("Robocop/Background2.png"				) }
	, m_pPlayerSprites	{ new Texture("Robocop/Robocop_Sprites_Fixed.png"	) }
	, m_pBullet			{ new Texture("Robocop/bullet.png"					) }
	, m_pNormalBox		{ new Texture("Robocop/normalbox.png"				) }
	, m_pAmmoBox		{ new Texture("Robocop/itembox.png"					) }
	, m_pHUD			{ new Texture("Robocop/Power.png"					) }
	, m_pEnemySprites	{ new Texture("Robocop/Enemy_Sprites.png"			) }
	, m_pEnemyBike		{ new Texture("Robocop/Enemy_Bike.png"				) }
	, m_pEnemyWindow	{ new Texture("Robocop/Enemy_Building.png"			) }
	, m_pBossSprites	{ new Texture("Robocop/Enemy_Boss.png"				) }
{
	std::string fontPath{ "Robocop/PixelTandysoft.ttf" };
	//std::string fontPath{ "DIN-Light.otf" };
	m_PauseText.push_back(new Texture("INFO:", fontPath, 
						  30, Color4f(1.f, 1.f, 1.f, 1.f)));
	m_PauseText.push_back(new Texture("Game Controls:", 
						  fontPath, 
						  20, Color4f(1.f, 1.f, 1.f, 1.f)));
	m_PauseText.push_back(new Texture("I = Open/Close This Menu", 
						  fontPath, 
						  20, Color4f(1.f, 1.f, 1.f, 1.f)));
	m_PauseText.push_back(new Texture("W or UP = Look Up (With gun)", 
						  fontPath, 
						  20, Color4f(1.f, 1.f, 1.f, 1.f)));
	m_PauseText.push_back(new Texture("S or DOWN = Croush", 
						  fontPath, 
						  20, Color4f(1.f, 1.f, 1.f, 1.f)));
	m_PauseText.push_back(new Texture("A or LEFT = Walk Left", 
						  fontPath, 
						  20, Color4f(1.f, 1.f, 1.f, 1.f)));
	m_PauseText.push_back(new Texture("D or RIGHT = Walk Right", 
						  fontPath, 
						  20, Color4f(1.f, 1.f, 1.f, 1.f)));
	m_PauseText.push_back(new Texture("Space = Jump", 
						  fontPath, 
						  20, Color4f(1.f, 1.f, 1.f, 1.f)));
	m_PauseText.push_back(new Texture("Left click = Attack/Shoot", 
						  fontPath, 
						  20, Color4f(1.f, 1.f, 1.f, 1.f)));
	m_PauseText.push_back(new Texture("Walking + W or UP = Look diagonal (With gun)", 
						  fontPath, 
						  20, Color4f(1.f, 1.f, 1.f, 1.f)));
	m_PauseText.push_back(new Texture("You attack in the direction you look", 
						  fontPath, 
						  20, Color4f(1.f, 1.f, 1.f, 1.f)));


	m_CompletedText = (new Texture("Stage Cleared", 
						  fontPath, 
						  40, Color4f(1.f, 1.f, 1.f, 1.f)));
		
}

TextureManager::~TextureManager()
{
	delete m_pBackground;
	delete m_pPlayerSprites;

	delete m_pBullet;
	delete m_pNormalBox;

	delete m_pAmmoBox;
	delete m_pHUD;

	delete m_pEnemySprites;
	delete m_pEnemyBike;

	delete m_pEnemyWindow;
	delete m_pBossSprites;

	for (const Texture* texture : m_PauseText)
	{
		delete texture;
	}
	m_PauseText.clear();

	delete m_CompletedText;
}

const Texture* TextureManager::GetBackgroundTex() const
{
	return m_pBackground;
}

const Texture* TextureManager::GetPlayerSprites() const
{
	return m_pPlayerSprites;
}

const Texture* TextureManager::GetBulletTex() const
{
	return m_pBullet;
}

const Texture* TextureManager::GetBoxTex() const
{
	return m_pNormalBox;
}

const Texture* TextureManager::GetAmmoTex() const
{
	return m_pAmmoBox;
}

const Texture* TextureManager::GetHUD() const
{
	return m_pHUD;
}

const Texture* TextureManager::GetEnemySprites() const
{
	return m_pEnemySprites;
}

const Texture* TextureManager::GetEnemyBikeTex() const
{
	return m_pEnemyBike;
}

const Texture* TextureManager::GetEnemyWindowTex() const
{
	return m_pEnemyWindow;
}

const Texture* TextureManager::GetBossSprites() const
{
	return m_pBossSprites;
}

const std::vector<Texture*>& TextureManager::GetPauseText() const
{
	return m_PauseText;
}

const Texture* TextureManager::GetCompletedText() const
{
	return m_CompletedText;
}
