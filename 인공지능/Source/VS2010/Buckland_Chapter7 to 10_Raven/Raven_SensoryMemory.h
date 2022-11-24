#ifndef RAVEN_SENSORY_SYSTEM_H
#define RAVEN_SENSORY_SYSTEM_H
#pragma warning (disable:4786)
//-----------------------------------------------------------------------------
//
//  Name:
//
//  Author: Mat Buckland (ai-junkie.com)
//
//  Desc:
//
//-----------------------------------------------------------------------------
#include <map>
#include <list>
#include "2d/vector2d.h"

class Raven_Bot;


class MemoryRecord
{
public:
  
  // 상대를 기억하는 최대시간
  // (CurrentTime() - m_dTimeLastSensed가 봇의 메모리 범위보다 크면 이 레코드의 데이터를 클라이언트에서 사용할 수 없게 됩니다.)

  double       fTimeLastSensed;

  //상대가 처음 표시된 시간. 상대가 얼마나 오랫동안 시야에 들어왔는지 -> (CurrentTime - fTimeBecameVisible).

  double       fTimeBecameVisible;

  // 상대가 마지막으로 보인 시간

  double       fTimeLastVisible;

  // 상대가 마지막으로 감지된 위치를 표시하는 벡터로 상대가 시야 밖으로 나갈 경우 상대를 추적하는 데 도움

  Vector2D    vLastSensedPosition;

  // 상대가 시야내에 있는지

  bool        bWithinFOV;

  // 상대가 쏠수 있는 위치에 있는지

  bool        bShootable;
  

  MemoryRecord():fTimeLastSensed(-999),
            fTimeBecameVisible(-999),
            fTimeLastVisible(0),
            bWithinFOV(false),
            bShootable(false)
  {}
};



class Raven_SensoryMemory
{
private:

  typedef std::map<Raven_Bot*, MemoryRecord> MemoryMap;

private:
  
  Raven_Bot* m_pOwner;

  // 최근에 감지된 상대 목록

  MemoryMap  m_MemoryMap;

  // 봇이 최근에 감지된 상대 목록 ( m_MemoryMap )을 요청할때 봇이 상대를 기억하기 위한 값

  double      m_dMemorySpan;

  // 기존 레코드가 있는지 확인하고 없다면 메모리 맵에 추가됩니다
  // (UpdateWithSoundSource& UpdateVision에서 호출)

  void       MakeNewRecordIfNotAlreadyPresent(Raven_Bot* pBot);

public:

  Raven_SensoryMemory(Raven_Bot* owner, double MemorySpan);

  // 상대가 소리를 낼 때마다 메모리 맵을 업데이트

  void     UpdateWithSoundSource(Raven_Bot* pNoiseMaker);

  // 메모리에서 봇의 기록을 제거

  void     RemoveBotFromMemory(Raven_Bot* pBot);

  // 모든 객체를 통해 반복되며 나의 시야에 있는 상대의 기록을 업데이트

  void     UpdateVision();

  bool     isOpponentShootable(Raven_Bot* pOpponent)const;
  bool     isOpponentWithinFOV(Raven_Bot* pOpponent)const;
  Vector2D GetLastRecordedPositionOfOpponent(Raven_Bot* pOpponent)const;
  double    GetTimeOpponentHasBeenVisible(Raven_Bot* pOpponent)const;
  double    GetTimeSinceLastSensed(Raven_Bot* pOpponent)const;
  double    GetTimeOpponentHasBeenOutOfView(Raven_Bot* pOpponent)const;

  // 마지막 m_dMemorySpan 내에 레코드를 업데이트한 모든 상대의 목록을 반환합니다.

  std::list<Raven_Bot*> GetListOfRecentlySensedOpponents()const;

  void     RenderBoxesAroundRecentlySensed()const;

};


#endif