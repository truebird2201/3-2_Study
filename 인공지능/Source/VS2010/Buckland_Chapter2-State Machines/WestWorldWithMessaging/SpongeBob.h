#ifndef MINER_H
#define MINER_H

#include <string>

#include "fsm/State.h"
#include "BaseGameEntity.h"
#include "Locations.h"
#include "SpongeBobOwnedStates.h"
#include "misc/ConsoleUtils.h"
#include "fsm/StateMachine.h"
#include "misc/Utils.h"

template <class entity_type> class State; //pre-fixed with "template <class entity_type> " for vs8 compatibility

struct Telegram;

// ��ƾ��ϴ� ���ĸ� ��
const int MaxJellyFish = 5;

// ������ �ϴ� ���� ��
const int Burger_Goal = 5;

// �ִ� �ǰ���
const int MaxTired = 4;



class SpongeBob : public BaseGameEntity
{
private:

    //an instance of the state machine class
    StateMachine<SpongeBob>* m_pStateMachine;

    location_type         m_Location;

    // ���� ���� ��
    int                   m_iBurger;

    // ���� ���ĸ� ��
    int                   m_iJellyFish;

    // �ǰ��� ����
    int                   m_iTired;


public:

    SpongeBob(int id) :m_Location(KrabShop),
        m_iBurger(0),
        m_iJellyFish(0),
        m_iTired(0),
        BaseGameEntity(id)

    {
        //set up state machine
        m_pStateMachine = new StateMachine<SpongeBob>(this);

        m_pStateMachine->SetCurrentState(MakeBurger::Instance());

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

    int           MakedBurger()const { return m_iBurger; }
    void          AddBurger() { m_iBurger += 1; }
    bool          FinishWork()const { return m_iBurger >= Burger_Goal; }

    void          IncreaseTired() { m_iTired += 1; }
    bool          Tired() { m_iTired >= MaxTired; }

};



#endif
