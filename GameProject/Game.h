#pragma once
#include "BaseGame.h"
#include "TextureManager.h"
#include "World.h"
#include "Objects.h"
#include "InteractObjects.h"
#include "Player.h"
#include "View.h"
#include "Projectile.h"
#include "HUD.h"
#include "SoundStream.h"
#include "SoundEffect.h"
#include <vector>
//class Enemy;
class Game : public BaseGame
{
public:
	explicit Game( const Window& window );
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game( Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update( float elapsedSec ) override;
	void Draw( ) const override;
	void MakeBullet(const float angle, const float speed, const Point2f pos, const bool enemy);

	// Event handling
	void ProcessKeyDownEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessKeyUpEvent( const SDL_KeyboardEvent& e ) override;
	void ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e ) override;
	void ProcessMouseDownEvent( const SDL_MouseButtonEvent& e ) override;
	void ProcessMouseUpEvent( const SDL_MouseButtonEvent& e ) override;

private:
	const TextureManager m_Textures;
	World m_World;
	Player m_Player;
	View m_View;
	//View m_ParalaxView;
	HUD m_HUD;

	std::vector <Projectile*> m_PlayerBullets;
	std::vector <Projectile*> m_EnemyBullets;
	std::vector <Object*> m_Enemies;
	std::vector <InteractObject*> m_Interactables;


	SoundStream m_Music;
	SoundEffect m_startSound;
	SoundEffect m_Gunshot;
	SoundEffect m_Punch;
	SoundEffect m_Damage;

	int m_FurthestPlayerDistance;
	bool m_WaveSpawned;

	bool m_IsPaused;
	bool m_Won;
	// FUNCTIONS
	void Initialize();
	void Cleanup( );
	void ClearBackground( ) const;
	void DrawForeground() const;
	void DrawHUD() const;
	void HandleSpawningEvents();
	void DoObjectCollisions();
	void CheckGunSwap();
	
	template <typename Trash>
	void CleanObjectDelete(const size_t index, std::vector<Trash*>& vector)
	{
		if (vector.at(index) != vector.back())
		{
			delete vector.at(index);
			vector.at(index) = vector.back();
			vector.pop_back();
		}
		else
		{
			delete vector.at(index);
			vector.pop_back();
		}
	}
};