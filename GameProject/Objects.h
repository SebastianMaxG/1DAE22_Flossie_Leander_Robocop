#pragma once
class Texture;
class World;
#include "Vector2f.h"

class Object
{
public:
	Object( const Texture* texture, size_t health, Rectf shape, Vector2f speed,
			bool direction = true, size_t damage = 1);
	virtual ~Object();
	virtual void Draw() const = 0;
	virtual void Update(float elapsedSec, const World& world) = 0;
	
	bool IsDead();
	const int* GetHealth() const;
	const Rectf& GetShape() const;
	Rectf& GetShape();
	const size_t GetDamage() const;
	Vector2f& GetVelocity();

	void Damage(size_t amount);
	void Heal(size_t amount);

	enum class ActionState
	{
		moving,
		idle,
		dying,
		attacking,
		gunDraw
	};

protected:
	
	const Texture* m_Texture;
	//current speed
	Vector2f m_Velocity;
	//gavitational acceleration
	Vector2f m_Acceleration;
	//speed of jump and walking
	Vector2f m_Speed;
	//state of character
	ActionState m_ActionState;
	// true = right, false = left (player)
	// reverse for enemies;
	bool m_Direction;

	virtual bool UpdateVelocity(float elapsedSec, const World& world) = 0;
	void Move(float elapsedSec, bool isOnGround);
private:
	//health of the object
	int m_Health;
	//Do death animation and / or delete
	bool m_IsDead;
	//Shape of texture
	Rectf m_Shape;
	//Damage upon overlap
	const size_t m_Damage;
	//invulnerability timer
	float m_timeSinceDamage;
	const float m_SafeTime;
};



