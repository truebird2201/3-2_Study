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


class JingJing;
struct Telegram;

// 글로벌 state

class JingJingGlobalState : public State<JingJing>
{
private:

	JingJingGlobalState() {}

	//copy ctor and assignment should be private
	JingJingGlobalState(const JingJingGlobalState&);
	JingJingGlobalState& operator=(const JingJingGlobalState&);

public:

	//this is a singleton
	static JingJingGlobalState* Instance();

	virtual void Enter(JingJing* jing) {}

	virtual void Execute(JingJing* jing);

	virtual void Exit(JingJing* jing) {}

	virtual bool OnMessage(JingJing* jing, const Telegram& msg);
};

// 주문 받기

class TakeOrder : public State<JingJing>
{
private:

	TakeOrder() {}
	TakeOrder(const TakeOrder&);
	TakeOrder& operator=(const TakeOrder&);

public:

	static TakeOrder* Instance();

	virtual void Enter(JingJing* jing);

	virtual void Execute(JingJing* jing);

	virtual void Exit(JingJing* jing);

	virtual bool OnMessage(JingJing* jing, const Telegram& msg) { return false; };

};

// 클라리넷 불기

class Play : public State<JingJing>
{
private:

	Play() {}
	Play(const Play&);
	Play& operator=(const Play&);

public:

	static Play* Instance();

	virtual void Enter(JingJing* jing);

	virtual void Execute(JingJing* jing);

	virtual void Exit(JingJing* jing);

	virtual bool OnMessage(JingJing* jing, const Telegram& msg);
};

// 화내기

class Angry : public State<JingJing>
{
private:

	Angry() {}
	Angry(const Angry&);
	Angry& operator=(const Angry&);

public:

	static Angry* Instance();

	virtual void Enter(JingJing* jing);

	virtual void Execute(JingJing* jing);

	virtual void Exit(JingJing* jing);

	virtual bool OnMessage(JingJing* jing, const Telegram& msg);
};

// 스폰지밥 괴롭히기

class AngryToSponge : public State<JingJing>
{
private:

	AngryToSponge() {}
	AngryToSponge(const AngryToSponge&);
	AngryToSponge& operator=(const AngryToSponge&);

public:

	static AngryToSponge* Instance();

	virtual void Enter(JingJing* jing);

	virtual void Execute(JingJing* jing);

	virtual void Exit(JingJing* jing);

	virtual bool OnMessage(JingJing* jing, const Telegram& msg);
};

#endif