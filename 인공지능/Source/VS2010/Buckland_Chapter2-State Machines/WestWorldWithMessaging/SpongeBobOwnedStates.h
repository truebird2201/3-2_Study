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




// 햄버거 만들기

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

// 해파리 잡기

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


// 비눗방울 불기

class PlayBubble : public State<SpongeBob>
{
private:

	PlayBubble() {}
	PlayBubble(const PlayBubble&);
	PlayBubble& operator=(const PlayBubble&);

public:

	static PlayBubble* Instance();

	virtual void Enter(SpongeBob* sponge);

	virtual void Execute(SpongeBob* sponge);

	virtual void Exit(SpongeBob* sponge);

	virtual bool OnMessage(SpongeBob* agent, const Telegram& msg);
};

#endif