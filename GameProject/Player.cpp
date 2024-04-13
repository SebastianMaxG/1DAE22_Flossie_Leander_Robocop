#include "pch.h"
#include "Player.h"
#include "Game.h"
#include "utils.h"
#include "World.h"
#include "Texture.h"

Player::Player(const Texture* texture)
	: Object{texture,20,Rectf(50,30,30,60),Vector2f(60,400),true}
	, m_NrOfFrames{8}
	, m_NrFramesPerSec{8}
	, m_AnimTime{}
	, m_AnimFrame{}
	, m_Grounded{false}
	, m_HasGun{false}
	, m_Armed{false}
	, m_Crouched{false}
	, m_NrOfAttackFrames{3}
	, m_AttackTime{}
	, m_AttackFrame{}
	, m_Aim{AimDirection::forward}
	, m_PunchBox{}
	, m_gunEquip{"Robocop/robocop-gun-sling-2.mp3"}
	, m_TimeSinceLastShot{}
	, m_TimeBetweenShots{0.5f}
	, m_PowerBullets{}
{
	
}

Player::~Player()
{
}

void Player::Draw() const
{
	Rectf srcRect{ m_AnimFrame * (GetShape().width+10),
					GetShape().height,
					(GetShape().width + 10),
					GetShape().height };
	if (m_Grounded)
	{
		if (m_HasGun)
		{
			switch (m_Aim)
			{
			case Player::AimDirection::up:
				srcRect.bottom = 874;
				srcRect.left = 0;
				break;
			case Player::AimDirection::angled:
				srcRect.bottom = 942;
				break;
			case Player::AimDirection::forward:
				srcRect.bottom = 1008;
				break;
			}
		}
		if (m_Crouched)
		{
			srcRect.left = 0;
			srcRect.bottom = 600;
			//srcRect.height = 50;
			goto SpecialAttack;
		}
		switch (m_ActionState)
		{
		case Object::ActionState::gunDraw:
			srcRect.bottom = 809; // GunDraw Hight 15 frames
			break;
		case Object::ActionState::dying:
			break;
		case Object::ActionState::attacking:
			if (!m_HasGun)
			{ 
				srcRect.bottom += GetShape().height + GetShape().height * m_AnimFrame;
				srcRect.left = m_AttackFrame * (GetShape().width + 10);
			}
			break;
		default:
			break;
		}
	} 
	else 
	{
		srcRect.left = 0;
		srcRect.bottom = 687;    
		//srcRect.height = 65;
	SpecialAttack:
		if (m_HasGun)
		{
			switch (m_Aim)
			{
			case Player::AimDirection::up:
				srcRect.left = (GetShape().width + 10) * 3;
				break;
			case Player::AimDirection::angled:
				srcRect.left = (GetShape().width + 10) * 2;
				break;
			case Player::AimDirection::forward:
				srcRect.left = (GetShape().width + 10) * 1;
				break;
			}
		}
		else if (m_ActionState == ActionState::attacking) 
		{
			srcRect.left = m_AttackFrame * (GetShape().width + 10) + 4 * (GetShape().width + 10);
		}

	}

	
	glPushMatrix();
	{
		//glTranslatef(-GetShape().width / 2, 0.f, 0.f);
		glTranslatef(GetShape().left, GetShape().bottom, 0.f);
		if (!m_Direction)
		{
			glScalef(-1, 1, 1);
			glTranslatef(-GetShape().width, 0, 0);
		}
		//glScalef(1.2f, 1.2f, 1);
		m_Texture->Draw(Point2f(0, 0), srcRect);
	}
	glPopMatrix();
}

void Player::Update(float elapsedSec, const World& world)
{
	UpdateAnimation(elapsedSec);
	m_TimeSinceLastShot += elapsedSec;
	if (m_ActionState == ActionState::gunDraw) 
		m_NrOfFrames = 15;
	else 
	{
		m_NrOfFrames = 8;
		Move(elapsedSec, UpdateVelocity(elapsedSec, world));
		world.HandleCollision(GetShape(), m_Velocity);
	}
	if (m_ActionState == ActionState::attacking)
	{
		DoAttack(elapsedSec, world);
	}

}

bool Player::UpdateVelocity(float elapsedSec,const World& world)
{
	m_Grounded = world.IsOnGround(GetShape()); 
	bool attacking = m_ActionState == ActionState::attacking; // save atack state
	// Keyboard checks

	// left and right
	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	Uint32 pMouse = SDL_GetMouseState(0, 0);

	if	( (pStates[SDL_SCANCODE_LEFT]  || pStates[SDL_SCANCODE_A]) 
		^ (pStates[SDL_SCANCODE_RIGHT] || pStates[SDL_SCANCODE_D]))
	{
		if (pStates[SDL_SCANCODE_RIGHT] || pStates[SDL_SCANCODE_D])
		{
			m_Velocity.x = m_Speed.x;
			m_Direction = true;
		}
		if (pStates[SDL_SCANCODE_LEFT] || pStates[SDL_SCANCODE_A])
		{
			m_Velocity.x = -m_Speed.x;
			m_Direction = false;
		}
		m_ActionState = ActionState::moving;
	}
	else
	{
		if (m_Grounded)
		{
			m_ActionState = ActionState::idle;
		}
		m_Velocity.x = 0;
	}
	// Jump
	if (pStates[SDL_SCANCODE_SPACE] && m_Grounded)
	{
		m_Velocity.y = m_Speed.y;
		m_ActionState = ActionState::moving;
	}

	// Aiming

	if (pStates[SDL_SCANCODE_UP] || pStates[SDL_SCANCODE_W])
	{
		m_Aim = AimDirection::up;

		if (m_Velocity.x != 0 or m_Crouched)
		{
			 m_Aim = AimDirection::angled;
		}
	}
	else	 m_Aim = AimDirection::forward;

	
	// crouch
	if ((pStates[SDL_SCANCODE_DOWN] || pStates[SDL_SCANCODE_S]) && m_Grounded)
	{
		m_Crouched = true;
		m_ActionState = ActionState::idle;
		m_Velocity.x = 0;
	}
	else m_Crouched = false;

	if (world.GetLevelBoundries().left > GetShape().left)
	{
		GetShape().left = world.GetLevelBoundries().left;
	}
	else if (world.GetLevelBoundries().left + world.GetLevelBoundries().width < GetShape().left + GetShape().width)
	{
		GetShape().left = world.GetLevelBoundries().left + world.GetLevelBoundries().width - GetShape().width;
	}
	

	if (attacking) m_ActionState = ActionState::attacking; // restore attack state
	return m_Grounded;
}

void Player::UpdateAnimation(float elapsedSec)
{
	if ((m_ActionState != ActionState::idle 
		and m_Velocity.x != 0)
		or m_ActionState == ActionState::gunDraw)
	{
		m_AnimTime += elapsedSec;
		m_AnimFrame = int(m_AnimTime * m_NrFramesPerSec);
		//int TotalFramesElapsed{ int(m_AnimTime * m_NrFramesPerSec) };
		//m_AnimFrame = TotalFramesElapsed % m_NrOfFrames;
	}

	if (m_AnimFrame >= m_NrOfFrames)
	{
		m_AnimFrame = 0;
		m_AnimTime = 0;
		if (m_ActionState == ActionState::gunDraw)
		{
			m_ActionState = ActionState::idle;
		}
	}

	if (m_Grounded)
	{
		GetShape().height = 60;

		if (m_Crouched)
		{
			GetShape().height = 50;

		}
	}
	else
	{
		GetShape().height = 65;
	}
}

void Player::DoAttack(float elapsedSec, const World& world)
{
	if (m_ActionState != ActionState::idle)
	{
		m_AttackTime += elapsedSec;
		m_AttackFrame = int(m_AttackTime * m_NrFramesPerSec);
		m_PunchBox = Rectf	(
							GetShape().left + ((m_Direction) ? GetShape().width : -10.f),
							GetShape().bottom + GetShape().height - 20.f,
							10.f, 
							10.f
							);
		if (!m_Grounded)
		{
			m_PunchBox.bottom -= 5.f;
		}
	}

	if (m_AttackFrame >= m_NrOfAttackFrames)
	{
		m_ActionState = ActionState::idle;
		m_AttackTime  = 0;
		m_AttackFrame = 0;
		m_PunchBox = Rectf(0.f, 0.f, 0.f, 0.f);
	}
}

void Player::Attack( Game* game)
{
	if (m_HasGun)
	{
		if (m_TimeBetweenShots < m_TimeSinceLastShot)
		{
				float angle{};
			Point2f pos{ GetShape().left + GetShape().width / 2, GetShape().bottom };
			switch (m_Aim)
			{
			case Player::AimDirection::up:
				angle = 90.f;
				pos.y += GetShape().height;
				pos.x += ((m_Direction) ? 3 : -3);
				break;
			case Player::AimDirection::angled:
				angle = ((m_Direction) ? 45.f : 135.f);
				pos.y += GetShape().height - 5;
				pos.x += ((m_Direction) ? 1 : -1) * (GetShape().width / 2 + 5);
				break;
			case Player::AimDirection::forward:
				angle = ((m_Direction) ? 0.f : 180.f);
				pos.y += GetShape().height - 17;
				pos.x += ((m_Direction) ? 1 : -1) * GetShape().width / 2;
				break;
			default:
				break;
			}
			if (!m_Grounded)
			{
				pos.y -= 5.f;
			}
			game->MakeBullet(angle, 160.f, pos, false);
			if (m_PowerBullets > 0)
			{
				--m_PowerBullets;
				game->MakeBullet(angle + 30, 160.f, pos, false);
				game->MakeBullet(angle - 30, 160.f, pos, false);
			}
			m_TimeSinceLastShot = 0;
		}
	} 
	else
	{
		m_ActionState = ActionState::attacking;
	}

}

void Player::ToggleGun(bool unlock, bool closeEnemy)
{
	if (m_Armed && m_ActionState != ActionState::attacking) // switch gun/fist
	{
		m_HasGun = !closeEnemy;
	}
	else if (unlock) // play animation
	{
		m_gunEquip.Play(0);
		m_ActionState = ActionState::gunDraw;
		m_AnimFrame = 0;
		m_AnimTime = 0;
		m_Armed = true;
		m_HasGun = true;
	}
}

const Rectf Player::GetHitBox() const
{
	Rectf hitBox{ GetShape() };
	hitBox.height -= 5.f;
	return hitBox;
}

const Rectf Player::GetPunchbox() const
{
	return m_PunchBox;
}

void Player::ActivatePowerUp()
{
	m_PowerBullets += 30;
}

const int* Player::GetPowerBullets() const
{
	return &m_PowerBullets;
}
