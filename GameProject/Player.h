#pragma once
#include "Objects.h"
#include "World.h"
#include "SoundEffect.h"

class Game;


class Player final: public Object 
{
public:
	Player(const Texture* texture);
	~Player();

	void Draw() const override;
	void Update(float elapsedSec, const World& world) override;
	void Attack( Game* game);
	void ToggleGun(bool unlock, bool closeEnemy = false);
	const Rectf GetHitBox() const;

	const Rectf GetPunchbox() const;
	void ActivatePowerUp();
	const int* GetPowerBullets() const;
	bool  m_Grounded;
private:
	enum class AimDirection
	{
		up = 3,
		angled = 2,
		forward = 1
	};
	int	  m_NrOfFrames;
	int   m_NrFramesPerSec;
	float m_AnimTime;
	int   m_AnimFrame;

	
	bool  m_HasGun;		// are you holding the gun?
	bool  m_Armed;		// has the gun been activated?
	bool  m_Crouched;		

	int   m_NrOfAttackFrames;
	float m_AttackTime;
	int	  m_AttackFrame;

	float m_TimeSinceLastShot;
	const float m_TimeBetweenShots;

	int m_PowerBullets;
	AimDirection m_Aim;
	Rectf m_PunchBox;


	SoundEffect m_gunEquip;

	//bool shot;

	bool UpdateVelocity(float elapsedSec, const World& world) override;
	void UpdateAnimation(float elapsedSec);
	void DoAttack(float elapsedSec, const World& world);

};


