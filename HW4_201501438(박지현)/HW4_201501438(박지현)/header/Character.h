#pragma once
#include <d2d1.h>
#include "FiniteStateMachine.h"

// ����� ũ��. �¿�� [-PLANE_SIZE,PLANE_SIZE], �յڷε� [-PLANE_SIZE,PLANE_SIZE].
#define PLANE_SIZE_X 1400
#define PLANE_SIZE_Y 800


class Character
{
public:
	static bool isAttacked;
	static bool isAttacking;
	static time_t lastAttackedTime;

	enum StateID {
		STATE_UNDEFINED = 0,
		STATE_STAND,
		STATE_MOVE,
		STATE_ATTACK,
		STATE_RUNAWAY,
		STATE_FOLLOW,
	};

	enum EventID {
		EVENT_UNDEFINED = 0,
		EVENT_FINDTARGET,
		EVENT_LOSTTARGET,
		EVENT_BEATTACKED,
		EVENT_HEALTHDRAINED,
		EVENT_STOPWALK,
		EVENT_DUBIOUS,
		EVENT_WITHINATTACK,
		EVENT_OUTOFATTACK,
	};

	enum Type {
		TYPE_UNDEFINED = 0,
		TYPE_HUMAN,
		TYPE_AI,
	};
	enum Color { Black, Red, Blue, Green };
private:
	Type type;
	D2D1_POINT_2F position;
	D2D1_POINT_2F destPosition;
	float rangeView; //�� �� �ִ� ����.
	float rangeTouch; //���� �� �ִ� ����.
	float speed;
	float orientation;
	FiniteStateMachine* stateMachine;
	Color color;//MeshMD2* model;

public:
	Character(Type _type, DWORD stateTransitions[][3], int numTransitions);//, MeshMD2* _model);
	~Character();

	void update(Character* target, float timeDelta);

	void setPosition(D2D1_POINT_2F pos) { position = pos; }
	D2D1_POINT_2F getPosition() { return position; }
	float getRangeView() { return rangeView; }
	Color getColor() { return color; }//MeshMD2* getModel() { return model; }
	void setDestPosition(D2D1_POINT_2F dest);
	D2D1_POINT_2F getDestPositon() { return destPosition; }
	float getOrientation() { return orientation; }

private:
	void issueEvent(DWORD event);
	void moveTo(float timeDelta);
	bool isVisible(D2D1_POINT_2F pos);
	void updateHuman(float timeDelta);
	void updateAI(Character* target, float timeDelta);

	void actionStand(float timeDelta);
	void actionMove(float timeDelta);
	void actionFollow(float timeDelta);
	void actionAttack(float timeDelta);
	void actionRunaway(float timeDelta);
};


double GetLengthBetween(D2D1_POINT_2F p1, D2D1_POINT_2F p2);

struct D2DVECTOR
{
	float x;
	float y;
};
