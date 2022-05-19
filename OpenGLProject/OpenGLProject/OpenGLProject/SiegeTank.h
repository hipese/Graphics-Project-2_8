#pragma once
#include "Actor.h"
#include "Transform.h"

class SiegeTank : public Actor
{
private:
	int t_mode;
public:
	enum class EAnimationType
	{
		TANK_STAND,
		TANK_STAND_START,
		TANK_WALK,
		TANK_WALK_START,
		TANK_ATTACK
	};
	enum class ModeType
	{
		SIEGE,
		NOT_SIEGE
	};
public:
	SiegeTank();
	virtual ~SiegeTank();
	int getMode();
	void setMode(int mode);
	virtual void Update() override;
};

