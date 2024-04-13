#include "pch.h"
#include "Game.h"
#include "utils.h"
#include "Texture.h"
#include "Enemy.h"
#include "MotorcycleEnemy.h"
#include "WindowEnemy.h"
#include "MechEnemy.h"
#include "Crate.h"
#include "AmmoCrate.h"
#include <iostream>

Game::Game( const Window& window ) 
	:BaseGame{ window }
	, m_Textures{}
	, m_World{ m_Textures.GetBackgroundTex() }
	, m_Player{ m_Textures.GetPlayerSprites() }
	, m_View{300,230}
	, m_Music{"Robocop/RobocopArcadeSoundtrack1988.mp3"}
	, m_startSound{ "Robocop/roboc-dead.wav" }
	, m_Gunshot{"Robocop/GunShot.mp3"}
	, m_Punch{"Robocop/Punch73.oga"}
	, m_Damage{ "Robocop/damage.wav" }
	, m_HUD{ m_Textures.GetHUD(), m_Player.GetHealth() , m_Player.GetPowerBullets()}
	, m_FurthestPlayerDistance{}
	, m_WaveSpawned{true}
	, m_IsPaused{false}
	, m_Won{}
	//, m_ParalaxView{300/1.2f, 230 / 1.2f}
{
	Initialize();
}

Game::~Game( )
{
	Cleanup( );
}

void Game::Initialize( )
{
	m_View.SetLevelBoundaries(m_World.GetBoundries());
	m_World.SetLevelBoundries(m_View.GetLevelBoundaries());
	//std::cout << cosf(M_PI) << "\n";
	m_Textures.GetEnemySprites();
	if (m_Music.IsLoaded())
	{
		m_Music.SetVolume(32);
		m_Music.Play(true);
		m_startSound.Play(0);
		//m_Music.SetVolume(10);

		//std::cout << m_Music.GetVolume();
	}
}

void Game::Cleanup( )
{
	for (const Projectile* bullet : m_PlayerBullets)
	{
		delete bullet;
	}
	m_PlayerBullets.clear();
	for (const Projectile* bullet : m_EnemyBullets)
	{
		delete bullet;
	}
	m_EnemyBullets.clear();
	for (const Object* enemy : m_Enemies)
	{
		delete enemy;
	}
	m_Enemies.clear();
	for (const InteractObject* object : m_Interactables)
	{
		delete object;
	}
	m_Interactables.clear();
}

void Game::Update( float elapsedSec )
{
	if (m_Player.IsDead())
	{
		m_IsPaused = true;
	}
	if (!m_IsPaused)
	{
		m_View.Update(m_Player.GetShape());
		m_Player.Update(elapsedSec, m_World);
		for (size_t index{ 0 }; index < m_PlayerBullets.size(); index++)
		{
			m_PlayerBullets.at(index)->Update(elapsedSec, m_World);
		}
		for (size_t index{ 0 }; index < m_EnemyBullets.size(); index++)
		{
			m_EnemyBullets.at(index)->Update(elapsedSec, m_World);
		}
		for (size_t index{ 0 }; index < m_Enemies.size(); index++)
		{
			m_Enemies.at(index)->Update(elapsedSec, m_World);
		}
		for (size_t index{ 0 }; index < m_Interactables.size(); index++)
		{
			m_Interactables.at(index)->Update(elapsedSec, m_World);
		}

		HandleSpawningEvents();
		DoObjectCollisions();
		CheckGunSwap();
	}
	// Check keyboard state
	//const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	//if ( pStates[SDL_SCANCODE_RIGHT] )
	//{
	//	std::cout << "Right arrow key is down\n";
	//}
	//if ( pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
	//{
	//	std::cout << "Left and up arrow keys are down\n";
	//}
}

void Game::Draw( ) const
{
	ClearBackground();
	glPushMatrix();
	{
		glScalef(4, 4, 1);
		m_View.Draw();
		m_World.DrawBackground();
		for (const Projectile* bullet : m_PlayerBullets)
		{
			bullet->Draw();
		}
		for (const Projectile* bullet : m_EnemyBullets)
		{
			bullet->Draw();
		}
		for (const Object* enemy : m_Enemies)
		{
			enemy->Draw();
		}	
		for (const InteractObject* object : m_Interactables)
		{
			object->Draw();
		}
		m_Player.Draw();
		utils::SetColor(Color4f(1, 0, 1, 0.2f));
		//utils::FillRect(m_Player.GetHitBox());
		//utils::FillRect(m_Player.GetPunchbox());
		glScalef(1.1f, 1.1f, 1);

	}
	glPopMatrix();
	glPushMatrix();
	{
		glScalef(8, 8, 1);
		m_View.Draw();
		glScalef(0.5f, 0.5f, 1);
		DrawForeground();

	}
	glPopMatrix();
	DrawHUD();
	
}

void Game::ProcessKeyDownEvent( const SDL_KeyboardEvent & e )
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent( const SDL_KeyboardEvent& e )
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	switch ( e.keysym.sym )
	{
	case SDLK_i:
		m_IsPaused = !m_IsPaused;
		break;
	}
}

void Game::ProcessMouseMotionEvent( const SDL_MouseMotionEvent& e )
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
	switch ( e.button )
	{
	case SDL_BUTTON_LEFT:
		//std::cout << " left button " << std::endl;
		m_Player.Attack(this);
		break;
	case SDL_BUTTON_RIGHT:
		//std::cout << " right button " << std::endl;
		break;
	case SDL_BUTTON_MIDDLE:
		//std::cout << " middle button " << std::endl;
		break;
	}
	
}

void Game::ProcessMouseUpEvent( const SDL_MouseButtonEvent& e )
{
	//std::cout << "MOUSEBUTTONUP event: ";
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}
}

void Game::ClearBackground( ) const
{
	glClearColor( 0.0f, 0.0f, 0.3f, 1.0f );
	glClear( GL_COLOR_BUFFER_BIT );
}

void Game::DrawForeground() const
{
	for (int index = 0; index < 5; index++)
	{
		m_Textures.GetBoxTex()->Draw(Point2f(100 + 700 * float(index), -30));
		m_Textures.GetBoxTex()->Draw(Point2f(500 + 700 * float(index), -30));
		m_Textures.GetBoxTex()->Draw(Point2f(600 + 700 * float(index), -20));
	}
	
}

void Game::DrawHUD() const
{
	m_HUD.Draw(GetViewPort().width);
	if (m_IsPaused)
	{
		utils::SetColor(Color4f(0.f,0.f,0.f,0.5f));
		utils::FillRect(GetViewPort());
		// #TODO Print Pause screen text
		if (!m_Won)
		{
			Point2f textLocation{ 400,GetViewPort().height - 100.f };
			for (const Texture* text : m_Textures.GetPauseText())
			{
				text->Draw(textLocation);
				textLocation.y -= text->GetHeight();
			}
		}
		else
		{
			Point2f textLocation{};
			textLocation.x = (GetViewPort().width - m_Textures.GetCompletedText()->GetWidth())/2;
			textLocation.y = (GetViewPort().height / 2 - m_Textures.GetCompletedText()->GetHeight());
			m_Textures.GetCompletedText()->Draw(textLocation);
		}
	}
}

void Game::HandleSpawningEvents()
{
	const int moveAmountPerWave{100};
	if (int(m_Player.GetShape().left) > m_FurthestPlayerDistance + moveAmountPerWave)
	{
		m_FurthestPlayerDistance += moveAmountPerWave;
		m_WaveSpawned = false;
	}
	if (!m_WaveSpawned)
	{
		switch (m_FurthestPlayerDistance)
		{
		case 100:
			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(300, 30), &m_Player.GetShape()));
			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(320, 30), &m_Player.GetShape()));
			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(340, 30), &m_Player.GetShape()));
			break;

		case 400:

			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(200, 30), &m_Player.GetShape()));
			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(220, 30), &m_Player.GetShape()));

			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(600, 30), &m_Player.GetShape()));
			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(620, 30), &m_Player.GetShape()));
			break;

		case 300:
			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(50, 30), &m_Player.GetShape()));
			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(70, 30), &m_Player.GetShape()));

			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(500, 30), &m_Player.GetShape()));
			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(520, 30), &m_Player.GetShape()));
			break;

		case 500:
			m_Enemies.push_back(new MotorcycleEnemy(m_Textures.GetEnemyBikeTex(), Point2f(800, 30), &m_Player.GetShape()));

			m_Enemies.push_back(new WindowEnemy(m_Textures.GetEnemyWindowTex(), m_World.GetWindowPositions().at(0),
				&m_Player.GetShape(), this));
			break;
			
		case 600:
			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(200, 30), &m_Player.GetShape()));
			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(220, 30), &m_Player.GetShape()));
		
			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(400, 30), &m_Player.GetShape()));
			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(420, 30), &m_Player.GetShape()));

			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(900, 30), &m_Player.GetShape()));
			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(920, 30), &m_Player.GetShape()));


			m_Enemies.push_back(new WindowEnemy(m_Textures.GetEnemyWindowTex(), m_World.GetWindowPositions().at(1),
				&m_Player.GetShape(), this));
			break;

		case 700:
			m_Player.ToggleGun(true);

			
			m_Interactables.push_back(new Crate(m_Textures.GetBoxTex(), Point2f(1100, 100), &m_Player));
			m_Interactables.push_back(new AmmoCrate(m_Textures.GetAmmoTex(), Point2f(1100, 32), &m_Player));

			m_Enemies.push_back(new WindowEnemy(m_Textures.GetEnemyWindowTex(), m_World.GetWindowPositions().at(2),
				&m_Player.GetShape(), this));
			break;
		case 800:
			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(500, 30), &m_Player.GetShape()));
			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(520, 30), &m_Player.GetShape()));
			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(540, 30), &m_Player.GetShape()));
			m_Enemies.push_back(new WindowEnemy(m_Textures.GetEnemyWindowTex(), m_World.GetWindowPositions().at(3),
				&m_Player.GetShape(), this));
			break;
		case 900:
			m_Enemies.push_back(new WindowEnemy(m_Textures.GetEnemyWindowTex(), m_World.GetWindowPositions().at(4),
				&m_Player.GetShape(), this));
		case 1000:
			m_Interactables.push_back(new Crate(m_Textures.GetBoxTex(), Point2f(1320, 100), &m_Player));
			m_Interactables.push_back(new Crate(m_Textures.GetBoxTex(), Point2f(1320, 30), &m_Player));
			m_Enemies.push_back(new WindowEnemy(m_Textures.GetEnemyWindowTex(), m_World.GetWindowPositions().at(5),
				&m_Player.GetShape(), this));
			break;
		case 1200:

			m_Enemies.push_back(new WindowEnemy(m_Textures.GetEnemyWindowTex(), m_World.GetWindowPositions().at(6),
				&m_Player.GetShape(), this));
			break;
		case 1300:

			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(1050, 30), &m_Player.GetShape()));
			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(1070, 30), &m_Player.GetShape()));

			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(1500, 30), &m_Player.GetShape()));
		
			m_Enemies.push_back(new WindowEnemy(m_Textures.GetEnemyWindowTex(), m_World.GetWindowPositions().at(7),
				&m_Player.GetShape(), this));

			m_Interactables.push_back(new Crate(m_Textures.GetBoxTex(), Point2f(1520, 30), &m_Player));

			break;
		case 1400:

			m_Enemies.push_back(new WindowEnemy(m_Textures.GetEnemyWindowTex(), m_World.GetWindowPositions().at(5),
				&m_Player.GetShape(), this));

			m_Enemies.push_back(new WindowEnemy(m_Textures.GetEnemyWindowTex(), m_World.GetWindowPositions().at(8),
				&m_Player.GetShape(), this));

			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(1180, 30), &m_Player.GetShape()));

			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(1600, 30), &m_Player.GetShape()));
			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(1620, 30), &m_Player.GetShape()));
			m_Enemies.push_back(new Enemy(m_Textures.GetEnemySprites(), Point2f(1640, 30), &m_Player.GetShape()));
			break;

		case 1600:
			for (const Object* enemy : m_Enemies)
			{
				delete enemy;
			}
			m_Enemies.clear();
			m_Enemies.push_back(new MechEnemy(m_Textures.GetBossSprites(), Point2f(1750, 20), 15, this));
			m_View.BossMode();
			//Spawn Boss
			break;
		default:
			break;
		}
		// #TODO add waves till 1400
		std::cout << m_FurthestPlayerDistance << "\n";

		m_WaveSpawned = true;

	}
	if (m_FurthestPlayerDistance > 1600 && m_Enemies.empty())
	{
		m_Won = true;
		m_IsPaused = true;
	}
}

void Game::DoObjectCollisions()
{
	const float maxBulletDistance{ 400.f };

	const size_t bulletDamage	{ 1 };
	const size_t fistDamage		{ 1 };
	const size_t CollisionDamage{ 1 };

	// all colisions with enemies
	for (size_t index{}; index < m_Enemies.size(); index++)
	{
		if (utils::IsOverlapping(m_Player.GetHitBox(), m_Enemies.at(index)->GetShape()))
		{
			m_Enemies.at(index)->Damage(CollisionDamage);
			m_Player.Damage(m_Enemies.at(index)->GetDamage());
			m_Damage.Play(0);
		}
		if (utils::IsOverlapping(m_Player.GetPunchbox(), m_Enemies.at(index)->GetShape()))
		{
			m_Enemies.at(index)->Damage(fistDamage);
			m_Punch.Play(0);
		}
		for (size_t bulletIndex{}; bulletIndex < m_PlayerBullets.size(); bulletIndex++)
		{
			bool shouldDelete{false};
			if (utils::IsPointInRect(m_PlayerBullets.at(bulletIndex)->GetPos(), m_Enemies.at(index)->GetShape()))
			{
				m_Enemies.at(index)->Damage(bulletDamage);
				shouldDelete = true;
				//CleanObjectDelete(bulletIndex, m_PlayerBullets);
				//can skip bullet colision for a frame due to moving the last one to the current pos
			}
			if (utils::GetDistance(m_PlayerBullets.at(bulletIndex)->GetPos(),
				Point2f(m_Player.GetShape().left, m_Player.GetShape().bottom)) > maxBulletDistance)
			{
				shouldDelete = true;
				//CleanObjectDelete(bulletIndex, m_PlayerBullets);
			}
			if (shouldDelete) CleanObjectDelete(bulletIndex, m_PlayerBullets);
		}
		if (m_Enemies.at(index)->IsDead())
		{
			CleanObjectDelete(index, m_Enemies);
			//#todo Spawn dead Body
		}
	}

	//Bullets against player
	for (size_t bulletIndex{}; bulletIndex < m_EnemyBullets.size(); bulletIndex++)
	{
		bool shouldDelete{ false };
		if (utils::IsPointInRect(m_EnemyBullets.at(bulletIndex)->GetPos(), m_Player.GetShape()))
		{
			m_Player.Damage(m_EnemyBullets.at(bulletIndex)->GetDamage());
			m_Damage.Play(0);
			shouldDelete = true;

			//CleanObjectDelete(bulletIndex, m_EnemyBullets);
			//can skip bullet colision for a frame due to moving the last one to the current pos
		}
		if (utils::GetDistance(m_EnemyBullets.at(bulletIndex)->GetPos(),
			Point2f(m_Player.GetShape().left, m_Player.GetShape().bottom)) > maxBulletDistance)
		{
			shouldDelete = true;
			//CleanObjectDelete(bulletIndex, m_EnemyBullets);
		}
		if (shouldDelete) CleanObjectDelete(bulletIndex, m_EnemyBullets);
	}
	
	//interactables;
	for (size_t index{ 0 }; index < m_Interactables.size(); index++)
	{
		
		if (utils::IsOverlapping(m_Player.GetHitBox(), m_Interactables.at(index)->GetShape()))
		{
			m_Interactables.at(index)->Interact(&m_Player);
		}
		if (utils::IsOverlapping(m_Player.GetPunchbox(), m_Interactables.at(index)->GetShape()))
		{
			m_Interactables.at(index)->Damage(fistDamage);
			m_Punch.Play(0);
		}
		for (size_t bulletIndex{}; bulletIndex < m_PlayerBullets.size(); bulletIndex++)
		{
			if (utils::IsPointInRect(m_PlayerBullets.at(bulletIndex)->GetPos(), m_Interactables.at(index)->GetShape()))
			{
				m_Interactables.at(index)->Damage(bulletDamage);
				CleanObjectDelete(bulletIndex, m_PlayerBullets);
				m_Interactables.at(index)->Damage(bulletDamage);
			}
		}
		for (size_t otherIndex{ 0 }; otherIndex < m_Interactables.size(); otherIndex++)
		{
			if (otherIndex != index)
			if (utils::IsOverlapping(m_Interactables.at(index)->GetShape(), m_Interactables.at(otherIndex)->GetShape()))
			{
				m_Interactables.at(index)->Interact(m_Interactables.at(otherIndex));
			}
		}
		if (m_Interactables.at(index)->IsDead())
		{
			CleanObjectDelete(index, m_Interactables);
		}
	}

}

void Game::CheckGunSwap()
{
	const float punchDistance{ 50.f };
	bool closeEnemy{ false };
	for (Object* enemy : m_Enemies)
	{
		if (utils::GetDistance(enemy->GetShape().left, enemy->GetShape().bottom, 
				m_Player.GetShape().left, m_Player.GetShape().bottom) < punchDistance)
		{
			closeEnemy = true;
		}
	}
	for (InteractObject* object : m_Interactables)
	{
		if (utils::GetDistance(object->GetShape().left, object->GetShape().bottom, 
				m_Player.GetShape().left, m_Player.GetShape().bottom) < punchDistance)
		{
			closeEnemy = true;
		}
	}
	m_Player.ToggleGun(false, closeEnemy);
}

void Game::MakeBullet(const float angle, const float speed, const Point2f pos, const bool enemy)
{
	m_Gunshot.Play(0);
	if (enemy)
	{
		m_EnemyBullets.push_back(new Projectile(angle, speed, pos, m_Textures.GetBulletTex()));
	}
	else
	{
		m_PlayerBullets.push_back(new Projectile(angle, speed, pos, m_Textures.GetBulletTex()));
	}
}

