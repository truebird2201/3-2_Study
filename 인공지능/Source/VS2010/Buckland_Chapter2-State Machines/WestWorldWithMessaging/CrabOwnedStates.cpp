#include "CrabOwnedStates.h"
#include "SpongeBobOwnedStates.h"
#include "Crab.h"
#include "Locations.h"
#include "Time/CrudeTimer.h"
#include "MessageDispatcher.h"
#include "MessageTypes.h"
#include "EntityNames.h"

#include <iostream>
using std::cout;

#ifdef TEXTOUTPUT
#include <fstream>
extern std::ofstream os;
#define cout os
#endif

//-----------------------------------------------------------------------Global state

CrabGlobalState* CrabGlobalState::Instance()
{
  static CrabGlobalState instance;

  return &instance;
}


void CrabGlobalState::Execute(Crab* crab)
{

  if ( (RandFloat() < 0.2) && 
       !crab->GetFSM()->isInState(*GiveMoney::Instance()) )
  {
      crab->GetFSM()->ChangeState(GiveMoney::Instance());
  }
}

bool CrabGlobalState::OnMessage(Crab* crab, const Telegram& msg)
{
  SetTextColor(BACKGROUND_RED|FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_BLUE);

  return false;
}

//-------------------------------------------------------------------------�����ֱ�

GiveMoney* GiveMoney::Instance()
{
  static GiveMoney instance;

  return &instance;
}


void GiveMoney::Enter(Crab* crab)
{
  cout << "\n" << GetNameOfEntity(crab->ID()) << ": ���ʽ��� �ָ�!";
}


void GiveMoney::Execute(Crab* crab)
{
  switch(RandInt(0,1))
  {
  case 0:

      Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
          crab->ID(),
          ent_JingJing,
          Msg_Money,
          NO_ADDITIONAL_INFO);
    cout << "\n" << GetNameOfEntity(crab->ID()) << ": �����ִ� ¡¡��~!";

    break;

  case 1:

      Dispatch->DispatchMessage(SEND_MSG_IMMEDIATELY,
          crab->ID(),
          ent_SpongeBob,
          Msg_Money,
          NO_ADDITIONAL_INFO);
    cout << "\n" << GetNameOfEntity(crab->ID()) << ": �����ִ� ��������~!";

    break;
  }
  crab->GetFSM()->RevertToPreviousState();
}

void GiveMoney::Exit(Crab* crab)
{
}

bool GiveMoney::OnMessage(Crab* crab, const Telegram& msg)
{
  return false;
}

//------------------------------------------------------------------------TV ����

WatchTV* WatchTV::Instance()
{
  static WatchTV instance;

  return &instance;
}


void WatchTV::Enter(Crab* crab)
{  
  cout << "\n" << GetNameOfEntity(crab->ID()) << ": �Ͼ��ϰ� TV�� ������~";
}


void WatchTV::Execute(Crab* crab)
{
    switch (RandInt(0, 2))
    {
    case 0:

        cout << "\n" << GetNameOfEntity(crab->ID()) << ": ���ѵ��� ��մ�~!";

        break;

    case 1:

        cout << "\n" << GetNameOfEntity(crab->ID()) << ": ¯�� ��մ�~!";

        break;
    case 2:

        cout << "\n" << GetNameOfEntity(crab->ID()) << ": ���� ��մ�~!";

        break;
    }
}

void WatchTV::Exit(Crab* crab)
{

}


bool WatchTV::OnMessage(Crab* crab, const Telegram& msg)
{
  return false;
}
