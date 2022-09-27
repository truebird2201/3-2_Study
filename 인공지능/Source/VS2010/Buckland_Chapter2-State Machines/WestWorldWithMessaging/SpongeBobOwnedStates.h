#ifndef MINER_OWNED_STATES_H
#define MINER_OWNED_STATES_H
//------------------------------------------------------------------------
//
//  Name:   MinerOwnedStates.h
//
//  Desc:   All the states that can be assigned to the Miner class.
//          Note that a global state has not been implemented.
//
//  Author: Mat Buckland 2002 (fup@ai-junkie.com)
//
//------------------------------------------------------------------------
#include "fsm/State.h"


class SpongeBob;
struct Telegram;

// �۷ι� state

class SpongeBobGlobalState : public State<SpongeBob>
{
private:

	SpongeBobGlobalState() {}

	//copy ctor and assignment should be private
	SpongeBobGlobalState(const SpongeBobGlobalState&);
	SpongeBobGlobalState& operator=(const SpongeBobGlobalState&);

public:

	//this is a singleton
	static SpongeBobGlobalState* Instance();

	virtual void Enter(SpongeBob* sponge) {}

	virtual void Execute(SpongeBob* sponge);

	virtual void Exit(SpongeBob* sponge) {}

	virtual bool OnMessage(SpongeBob* sponge, const Telegram& msg);
};

// �ܹ��� �����

class MakeBurger : public State<SpongeBob>
{
private:

	MakeBurger() {}
	MakeBurger(const MakeBurger&);
	MakeBurger& operator=(const MakeBurger&);

public:

	static MakeBurger* Instance();

	virtual void Enter(SpongeBob* sponge);

	virtual void Execute(SpongeBob* sponge);

	virtual void Exit(SpongeBob* sponge);

	virtual bool OnMessage(SpongeBob* agent, const Telegram& msg);

};

// ���ĸ� ���

class CatchJellyFish : public State<SpongeBob>
{
private:

	CatchJellyFish() {}
	CatchJellyFish(const CatchJellyFish&);
	CatchJellyFish& operator=(const CatchJellyFish&);

public:

	static CatchJellyFish* Instance();

	virtual void Enter(SpongeBob* sponge);

	virtual void Execute(SpongeBob* sponge);

	virtual void Exit(SpongeBob* sponge);

	virtual bool OnMessage(SpongeBob* agent, const Telegram& msg);
};


// ȭ��ǿ��� �� �α�

class PoopToilet : public State<SpongeBob>
{
private:

	PoopToilet() {}
	PoopToilet(const PoopToilet&);
	PoopToilet& operator=(const PoopToilet&);

public:

	static PoopToilet* Instance();

	virtual void Enter(SpongeBob* sponge);

	virtual void Execute(SpongeBob* sponge);

	virtual void Exit(SpongeBob* sponge);

	virtual bool OnMessage(SpongeBob* agent, const Telegram& msg);
};

// ������� �� �α�

class PoopHill : public State<SpongeBob>
{
private:

	PoopHill() {}
	PoopHill(const PoopHill&);
	PoopHill& operator=(const PoopHill&);

public:

	static PoopHill* Instance();

	virtual void Enter(SpongeBob* sponge);

	virtual void Execute(SpongeBob* sponge);

	virtual void Exit(SpongeBob* sponge);

	virtual bool OnMessage(SpongeBob* agent, const Telegram& msg);
};

#endif