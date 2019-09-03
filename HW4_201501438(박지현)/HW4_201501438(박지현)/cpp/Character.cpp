#include "../header/Character.h"
#include "time.h"

bool Character::isAttacked = false;
bool Character::isAttacking = false;
time_t Character::lastAttackedTime = time(NULL);

Character::Character(Type _type, DWORD stateTransitions[][3], int numTransitions)//, MeshMD2* _model)
{
	//model = _model; //copy
	type = _type;

	// ���ѻ��±�踦 �����.
	stateMachine = new FiniteStateMachine();
	for (int i = 0; i < numTransitions; i++)
	{
		stateMachine->addStateTransition(stateTransitions[i][0], stateTransitions[i][1], stateTransitions[i][2]);
	}
	stateMachine->setCurrentStateID(STATE_STAND);

	// ĳ���� Ư���� �����Ѵ�.
	if (type == TYPE_AI)
	{
		rangeView = 100;//(float)(30 + rand() % 10);
		speed = (float)(80 + rand() % 3);
	}
	else
	{
		rangeView = 0;
		speed = 160;
	}
	rangeTouch = 3;

	// �ʱ�ȭ�Ѵ�.
	position = D2D1::Point2F(480, 300);
	orientation = 0.0f;
	setDestPosition(position);
}

Character::~Character()
{
	if (stateMachine != 0)
		delete stateMachine;
}

bool Character::isVisible(D2D1_POINT_2F pos)
{
	//pos�� �� �� �ִ����� �˻���.
	if (GetLengthBetween(position, pos) < rangeView)
		return true;
	else
		return false;
}

void Character::update(Character* target, float timeDelta)
{
	if (type == TYPE_HUMAN)
		updateHuman(timeDelta);
	else
		updateAI(target, timeDelta);
}

void Character::updateHuman(float timeDelta)
{
	position = destPosition;
	//destPosition = position;
	//switch (stateMachine->getCurrentStateID()) 
	//{
	//case STATE_STAND:
	//	if (GetLengthBetween(position,destPosition) >= 1.0f) 
	//	{ //��ǥ������ �������� ������.
	//		issueEvent(Character::EVENT_FINDTARGET);
	//		break;
	//	}

	//	actionStand(timeDelta);
	//	break;

	//case STATE_MOVE:
	//	if (GetLengthBetween(position,destPosition) < 1.0f) 
	//	{ //��ǥ������ �����Ͽ���.
	//		position = destPosition;
	//		issueEvent(Character::EVENT_STOPWALK);
	//		break;
	//	}

	//	actionMove(timeDelta);
	//	break;
	//}
}

void Character::updateAI(Character* target, float timeDelta)
{
	switch (stateMachine->getCurrentStateID())
	{
	case STATE_STAND:
		actionStand(timeDelta);

		if (isVisible(target->getPosition()))
		{ // ���ü�� ����.
			setDestPosition(target->getPosition()); // ���ü�� ��ǥ�������� ������.
			issueEvent(Character::EVENT_FINDTARGET);
			break;
		}

		// ������ ���¸� �ٲ�.
		if ((GetTickCount() % 10 == 0))
		{
			D2D_POINT_2F vt;
			vt.x = (float)(rand() % (2 * PLANE_SIZE_X) - PLANE_SIZE_X);
			vt.y = (float)(rand() % (2 * PLANE_SIZE_Y) - PLANE_SIZE_Y);
			if (vt.x > 0 && vt.x < PLANE_SIZE_X && vt.y >0 && vt.y < PLANE_SIZE_Y)
			{
				setDestPosition(vt);
				issueEvent(Character::EVENT_DUBIOUS);
			}
			break;
		}

		break;

	case STATE_MOVE:
		actionMove(timeDelta);

		if (isVisible(target->getPosition()))
		{ // ���ü�� ����.
			setDestPosition(target->getPosition()); // ���ü�� ��ǥ�������� ������.
			issueEvent(Character::EVENT_FINDTARGET);
			break;
		}

		// ������ ���¸� �ٲ�.
		if ((GetTickCount() % 100 == 0) && (rand() % 10 < 2))
		{
			issueEvent(Character::EVENT_STOPWALK);
			break;
		}

		break;

	case STATE_FOLLOW:
		setDestPosition(target->getPosition()); // ���ü�� ��ġ�� ������.
		actionFollow(timeDelta);

		if (!isVisible(target->getPosition()))
		{ // ���ü�� ������ ����. ��ǥ���� ������.
			issueEvent(Character::EVENT_LOSTTARGET);
			break;
		}

		if (GetLengthBetween(position, destPosition) < 30.0f)
		{ //�����ǳ��� ����.
			issueEvent(Character::EVENT_WITHINATTACK);
			break;
		}

		break;

	case STATE_ATTACK:
		setDestPosition(target->getPosition()); // ���ü�� ��ġ�� ������.
		actionAttack(timeDelta);

		if (!isVisible(target->getPosition())) { // ���ü�� ������ ����. ��ǥ���� ������.
			issueEvent(Character::EVENT_LOSTTARGET);
			isAttacking = false;
			break;
		}

		if (GetLengthBetween(position, destPosition) >= 30.0f) { //�������� �����.
			issueEvent(Character::EVENT_OUTOFATTACK);
			isAttacking = false;
			break;
		}

		break;

	case STATE_RUNAWAY:
		actionRunaway(timeDelta);
		break;
	}

}

void Character::actionStand(float timeDelta) { }
void Character::actionMove(float timeDelta) { moveTo(timeDelta); }
void Character::actionFollow(float timeDelta) { moveTo(timeDelta); }
void Character::actionAttack(float timeDelta) { 
	if (isAttacked == false)
		lastAttackedTime = time(NULL);
	isAttacked = true;
	isAttacking = true;
}
void Character::actionRunaway(float timeDelta) { }


void Character::issueEvent(DWORD event)
{
	stateMachine->issueEvent(event);

	// ���º��� ������ �ִϸ��̼��� �ٲ�.

	switch (stateMachine->getCurrentStateID())
	{
	case STATE_STAND:
		color = Black;
		break;
	case STATE_MOVE:
		color = Green;
		break;
	case STATE_ATTACK:
		color = Red;//Red
		break;
	case STATE_FOLLOW:
		color = Blue;//Blue
		break;
	case STATE_RUNAWAY:
		color = Black;
		break;
	}

}

void Character::moveTo(float timeDelta)
{
	D2DVECTOR toTarget;
	float SpeedX, SpeedY;

	toTarget.x = destPosition.x - position.x;
	toTarget.y = destPosition.y - position.y;
	SpeedX = speed * toTarget.x / (float)GetLengthBetween(destPosition, position);
	SpeedY = speed * toTarget.y / (float)GetLengthBetween(destPosition, position);

	//D3DXVec3Normalize(&toTarget, &toTarget);
	/*position.x += toTarget.x * timeDelta * speed;
	position.y += toTarget.y * timeDelta * speed;*/

	position.x += timeDelta * SpeedX;
	position.y += timeDelta * SpeedY;
	if (GetLengthBetween(destPosition, position) < 1)
	{
		issueEvent(Character::EVENT_STOPWALK);
	}
}

void Character::setDestPosition(D2D1_POINT_2F dest)
{
	destPosition = dest;
	//D2DVECTOR toTarget;
	//toTarget.x = destPosition.x - position.x;
	//toTarget.y = destPosition.y - position.y;
	//if (GetLengthBetween(destPosition,position) <30)
	//{
	//	return;
	//}
	//if (fabs(toTarget.x) < 0.01 && fabs(toTarget.y) < 0.01) 
	//{
	//	// ������ ������ �״�� ������Ű��.
	//} 
	//else 
	//{
	//	orientation = atan2(toTarget.x, toTarget.y);
	//}
}

double GetLengthBetween(D2D1_POINT_2F p1, D2D1_POINT_2F p2)
{
	return sqrt((p1.x - p2.x)*(p1.x - p2.x) + (p1.y - p2.y)*(p1.y - p2.y));
}
