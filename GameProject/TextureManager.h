#pragma once
#include <vector>
class Texture;
class TextureManager final
{
public:
	TextureManager();
	~TextureManager();
	
	const Texture* GetBackgroundTex() const;
	const Texture* GetPlayerSprites() const;

	const Texture* GetBulletTex() const;
	const Texture* GetBoxTex() const;

	const Texture* GetAmmoTex() const;
	const Texture* GetHUD() const;

	const Texture* GetEnemySprites() const;
	const Texture* GetEnemyBikeTex() const;

	const Texture* GetEnemyWindowTex() const;
	const Texture* GetBossSprites() const;

	const std::vector<Texture*>& GetPauseText() const;

	const Texture* GetCompletedText() const;

private:
	const Texture* m_pBackground;
	const Texture* m_pPlayerSprites;
	//60x40

	const Texture* m_pBullet;
	const Texture* m_pNormalBox;

	const Texture* m_pAmmoBox;
	const Texture* m_pHUD;

	const Texture* m_pEnemySprites;
	const Texture* m_pEnemyBike;

	const Texture* m_pEnemyWindow;
	const Texture* m_pBossSprites;

	std::vector<Texture*> m_PauseText;

	Texture* m_CompletedText;
};

