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

// ±Û·Î¹ú state

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

// ÇÜ¹ö°Å ¸¸µé±â

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

// ÇØÆÄ¸® Àâ±â

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


// È­Àå½Ç¿¡¼­ ¶Ë ½Î±â

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

// ¾ð´ö¿¡¼­ ¶Ë ½Î±â

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