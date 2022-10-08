#include "Crab.h"

bool Crab::HandleMessage(const Telegram& msg)
{
  return m_pStateMachine->HandleMessage(msg);
}


void Crab::Update()
{
  //set text color to green
  SetTextColor(FOREGROUND_RED | FOREGROUND_INTENSITY);
 
  m_pStateMachine->Update();
}