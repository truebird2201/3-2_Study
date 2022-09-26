#ifndef MINER_H
#define MINER_H

#include <string>
#include <cassert>
#include <iostream>

#include "BaseGameEntity.h"
#include "Locations.h"
#include "misc/ConsoleUtils.h"
#include "SpongeBobOwnedStates.h"
#include "fsm/StateMachine.h"

template <class entity_type> class State; //pre-fixed with "template <class entity_type> " for vs8 compatibility

struct Telegram;

// 잡아야하는 해파리 수
const int MaxJellyFish = 5;
// 최대 비눗방울 수
const int MaxBubble = 3;
// 만들어야 하는 버거 수
const int Bugger_Goal = 5;
// 최대 지루함
const int MaxBored = 4;



class SpongeBob : public BaseGameEntity
{
private:

    //an instance of the state machine class
    StateMachine<SpongeBob>* m_pStateMachine;

    location_type         m_Location;

    // 만든 버거 수
    int                   m_iBurger;

    // 잡은 해파리 수
    int                   m_iJellyFish;

    // 현재 지루함
    int                   m_iBored;

    // 사용한 비눗방울
    int                   m_iBubble;

public:

    SpongeBob(int id) :m_Location(KrabShop),
        m_iBurger(0),
        m_iJellyFish(0),
        m_iBored(0),
        m_iBubble(0),
        BaseGameEntity(id)

    {
        //set up state machine
        m_pStateMachine = new StateMachine<SpongeBob>(this);

        m_pStateMachine->SetCurrentState(GoHomeAndSleepTilRested::Instance());

        /* NOTE, A GLOBAL STATE HAS NOT BEEN IMPLEMENTED FOR THE MINER */
    }

    ~SpongeBob() { delete m_pStateMachine; }

    //this must be implemented
    void Update();

    //so must this
    virtual bool  HandleMessage(const Telegram& msg);


    StateMachine<SpongeBob>* GetFSM()const { return m_pStateMachine; }



    //-------------------------------------------------------------accessors
    location_type Location()const { return m_Location; }
    void          ChangeLocation(location_type loc) { m_Location = loc; }

    int           GoldCarried()const { return m_iGoldCarried; }
    void          SetGoldCarried(int val) { m_iGoldCarried = val; }
    void          AddToGoldCarried(int val);
    bool          PocketsFull()const { return m_iGoldCarried >= MaxNuggets; }

    bool          Fatigued()const;
    void          DecreaseFatigue() { m_iFatigue -= 1; }
    void          IncreaseFatigue() { m_iFatigue += 1; }

    int           Wealth()const { return m_iMoneyInBank; }
    void          SetWealth(int val) { m_iMoneyInBank = val; }
    void          AddToWealth(int val);

    bool          Thirsty()const;
    void          BuyAndDrinkAWhiskey() { m_iThirst = 0; m_iMoneyInBank -= 2; }

};



#endif
