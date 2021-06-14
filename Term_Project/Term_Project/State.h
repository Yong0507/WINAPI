namespace PLAYER
{
	enum P_State {
		IDLE,
		MOVE,
		JUMP,
		FALL,
		ATTACK,
		DEAD
	};

	enum P_Locate {
		GROUND,
		OBSTACLE,
		FALLING
	};
}

namespace MONSTER
{
	enum M_State {
		IDLE,
		DEAD
	};
}

static PLAYER::P_State p_state;
static PLAYER::P_Locate p_locate;
static MONSTER::M_State m_state;
