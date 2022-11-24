#ifndef TARGETING_SYSTEM_H
#define TARGETING_SYSTEM_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:   Raven_TargetingSystem.h
//
//  Author: Mat Buckland (www.ai-junkie.com)
//
//  Desc:   class to select a target from the opponents currently in a bot's
//          perceptive memory.
//-----------------------------------------------------------------------------
#include "2d/Vector2D.h"
#include <list>


class Raven_Bot;




class Raven_TargetingSystem
{
private:

  //the owner of this system
  Raven_Bot*  m_pOwner;

  // 현재 타겟
  Raven_Bot*  m_pCurrentTarget;


public:

  Raven_TargetingSystem(Raven_Bot* owner);

  // 가장 가까운 적이 m_pCurrentTarget이 되며 존재하지 않을 경우 null이 된다.

  void       Update();

  // m_pCurrentTarget이 있는가?

  bool       isTargetPresent()const{return m_pCurrentTarget != 0;}

  // 대상이 시야내에 있는가?

  bool       isTargetWithinFOV()const;

  // 상대가 공격할수 있는 위치에 있는가?

  bool       isTargetShootable()const;

  // 대상이 마지막으로 발견된 위치를 반환. 현재 할당된 대상이 없는 경우 예외를 발생

  Vector2D   GetLastRecordedPosition()const;

  // 대상이 시야에 있던 시간

  double      GetTimeTargetHasBeenVisible()const;

  // 대상이 시야에 없던 시간

  double      GetTimeTargetHasBeenOutOfView()const;
  
  // 대상을 반환

  Raven_Bot* GetTarget()const{return m_pCurrentTarget;}

  // 대상을 null로 바꿈

  void       ClearTarget(){m_pCurrentTarget=0;}
};




#endif
