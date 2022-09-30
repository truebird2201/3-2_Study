#include "SpongeBobOwnedStates.h"
#include "SpongeBob.h"
#include "Locations.h"
#include "Time/CrudeTimer.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "EntityNames.h"
#include "misc/ConsoleUtils.h"

#include <iostream>
using std::cout;


#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif

// �۷ι� state 

SpongeBobGlobalState* SpongeBobGlobalState::Instance()
{
    static SpongeBobGlobalState instance;

    return &instance;
}


void SpongeBobGlobalState::Execute(SpongeBob* sponge)
{
    // 
    if ((RandFloat() < 0.1) &&
        !sponge->GetFSM()->isInState(*Poop::Instance()))
    {
        sponge->GetFSM()->ChangeState(Poop::Instance());
    }
}

bool SpongeBobGlobalState::OnMessage(SpongeBob* sponge, const Telegram& msg)
{
    return false;
}

//------------------------------------------------------------------------methods for EnterMineAndDigForNugget
MakeBurger* MakeBurger::Instance()
{
    static MakeBurger instance;

    return &instance;
}


void MakeBurger::Enter(SpongeBob* sponge)
{
    if (sponge->Location() != KrabShop)
    {
        cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "���� �ϱ� ���Ͽ� ���Ը��Ʒ� ����մϴ�.";
        sponge->ChangeLocation(KrabShop);
    }
}


void MakeBurger::Execute(SpongeBob* sponge)
{
    // �ܹ��Ÿ� �����
    sponge->AddBurger();
    sponge->IncreaseTired();

    cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "�ܹ��Ÿ� �ϳ� ��������ϴ�. ( ���� ���� = " << sponge->MakedBurger() << " / " << sponge->MaxBurger();

    if (sponge->FinishWork())
    {
        sponge->GetFSM()->ChangeState(CatchJellyFish::Instance());
    }

    if (sponge->Tired())
    {
        sponge->GetFSM()->ChangeState(Rest::Instance());
    }
}


void MakeBurger::Exit(SpongeBob* sponge)
{
    if (sponge->Location() == KrabShop)
    cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "���Ը��ƿ��� ����մϴ�.";

}

//------------------------------------------------------------------------����
Rest* Rest::Instance()
{
    static Rest instance;

    return &instance;
}


void Rest::Enter(SpongeBob* sponge)
{
    Dispatch->DispatchMessage(1.5,                  //time delay
        sponge->ID(),           //sender ID
        sponge->ID(),           //receiver ID
        Msg_TakeRest,        //msg
        NO_ADDITIONAL_INFO);

        cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "����� �ھ�� ( ���� �ð� : " << Clock->GetCurrentTime() << " )";
}


void Rest::Execute(SpongeBob* sponge)
{
    cout << "\n" << GetNameOfEntity(sponge->ID()) << ": Zzzz .... ";
}


void Rest::Exit(SpongeBob* sponge)
{
    cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "���� �Ͼ����?";
    sponge->SetTiredZero();
}

bool Rest::OnMessage(SpongeBob* sponge, const Telegram& msg)
{
    SetTextColor(BACKGROUND_RED | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);

    switch (msg.Msg)
    {
    case Msg_TakeRest:
    {

        SetTextColor(FOREGROUND_GREEN | FOREGROUND_INTENSITY);

        cout << "\n" << GetNameOfEntity(sponge->ID()) << ": ����!" << " ( ���� �ð� : " << Clock->GetCurrentTime() << " )";

        sponge->GetFSM()->RevertToPreviousState();
    }

    return true;

    }//end switch

    return false;
}

//------------------------------------------------------------------------���ĸ� ���

CatchJellyFish* CatchJellyFish::Instance()
{
    static CatchJellyFish instance;

    return &instance;
}

void CatchJellyFish::Enter(SpongeBob* sponge)
{
    if (sponge->Location() != Hill)
    {
        cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "���ĸ� �������� ���ϴ�";

        sponge->ChangeLocation(Hill);
    }
}


void CatchJellyFish::Execute(SpongeBob* sponge)
{
    //deposit the gold
    sponge->AddJellyFish();
    sponge->IncreaseTired();

    cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "�ų���! ���ĸ��� ��Ҵ�! ( ���ĸ� =  " << sponge->CatchedJellyFish() << " / " << sponge->MaxJF();

    if (sponge->FinishCatch())
    {
        sponge->GetFSM()->ChangeState(GoHome::Instance());
    }

    if (sponge->Tired())
    {
        sponge->GetFSM()->ChangeState(Rest::Instance());
    }
}


void CatchJellyFish::Exit(SpongeBob* sponge)
{
    if (sponge->Location() == Hill)
    cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << "��~ ���ĸ� ������ �����ϴ�.";
}


bool CatchJellyFish::OnMessage(SpongeBob* sponge, const Telegram& msg)
{
    //send msg to global message handler
    return false;
}
//------------------------------------------------------------------------�˽α�

Poop* Poop::Instance()
{
    static Poop instance;

    return &instance;
}

void Poop::Enter(SpongeBob* sponge)
{
    if (sponge->Location() == Hill)
    {
        cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << " ���� ����� ��ó�� ȭ����� ������ ��� �������� ���ϴ�..";
    }
    else {
        cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << " ���� �������� ȭ��Ƿ� ���ϴ�! ";
    }
}

void Poop::Execute(SpongeBob* sponge)
{
    cout << "\n" << GetNameOfEntity(sponge->ID()) << ": " << " �� ";
}

void Poop::Exit(SpongeBob* sponge)
{
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "ZZZZ... ";
}


bool Poop::OnMessage(SpongeBob* sponge, const Telegram& msg)
{
    return false; //send message to global message handler
}

//------------------------------------------------------------------------QuenchThirst

QuenchThirst* QuenchThirst::Instance()
{
    static QuenchThirst instance;

    return &instance;
}

void QuenchThirst::Enter(Miner* pMiner)
{
    if (pMiner->Location() != saloon)
    {
        pMiner->ChangeLocation(saloon);

        cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Boy, ah sure is thusty! Walking to the saloon";
    }
}

void QuenchThirst::Execute(Miner* pMiner)
{
    pMiner->BuyAndDrinkAWhiskey();

    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "That's mighty fine sippin' liquer";

    pMiner->GetFSM()->ChangeState(EnterMineAndDigForNugget::Instance());
}


void QuenchThirst::Exit(Miner* pMiner)
{
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Leaving the saloon, feelin' good";
}


bool QuenchThirst::OnMessage(Miner* pMiner, const Telegram& msg)
{
    //send msg to global message handler
    return false;
}

//------------------------------------------------------------------------EatStew

EatStew* EatStew::Instance()
{
    static EatStew instance;

    return &instance;
}


void EatStew::Enter(Miner* pMiner)
{
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Smells Reaaal goood Elsa!";
}

void EatStew::Execute(Miner* pMiner)
{
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Tastes real good too!";

    pMiner->GetFSM()->RevertToPreviousState();
}

void EatStew::Exit(Miner* pMiner)
{
    cout << "\n" << GetNameOfEntity(pMiner->ID()) << ": " << "Thankya li'lle lady. Ah better get back to whatever ah wuz doin'";
}


bool EatStew::OnMessage(Miner* pMiner, const Telegram& msg)
{
    //send msg to global message handler
    return false;
}


