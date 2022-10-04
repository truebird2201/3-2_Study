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

	virtual bool OnMessage(SpongeBob* agent, const Telegram& msg) { return false; };

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

class Poop : public State<SpongeBob>
{
private:

	Poop() {}
	Poop(const Poop&);
	Poop& operator=(const Poop&);

public:

	static Poop* Instance();

	virtual void Enter(SpongeBob* sponge);

	virtual void Execute(SpongeBob* sponge);

	virtual void Exit(SpongeBob* sponge);

	virtual bool OnMessage(SpongeBob* agent, const Telegram& msg);
};

// ����

class Rest : public State<SpongeBob>
{
private:

	Rest() {}
	Rest(const Rest&);
	Rest& operator=(const Rest&);

public:

	static Rest* Instance();

	virtual void Enter(SpongeBob* sponge);

	virtual void Execute(SpongeBob* sponge);

	virtual void Exit(SpongeBob* sponge);

	virtual bool OnMessage(SpongeBob* agent, const Telegram& msg);
};

// ���� ����

class GoHome : public State<SpongeBob>
{
private:

	GoHome() {}
	GoHome(const GoHome&);
	GoHome& operator=(const GoHome&);

public:

	static  GoHome* Instance();

	virtual void Enter(SpongeBob* sponge);

	virtual void Execute(SpongeBob* sponge);

	virtual void Exit(SpongeBob* sponge);

	virtual bool OnMessage(SpongeBob* agent, const Telegram& msg);
};


#endif