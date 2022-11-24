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
  
  //records the time the opponent was last sensed (seen or heard). This
  //is used to determine if a bot can 'remember' this record or not. 
  //(if CurrentTime() - m_dTimeLastSensed is greater than the bot's
  //memory span, the data in this record is made unavailable to clients)
  double       fTimeLastSensed;

  //it can be useful to know how long an opponent has been visible. This 
  //variable is tagged with the current time whenever an opponent first becomes
  //visible. It's then a simple matter to calculate how long the opponent has
  //been in view (CurrentTime - fTimeBecameVisible)
  double       fTimeBecameVisible;

  //it can also be useful to know the last time an opponent was seen
  double       fTimeLastVisible;

  //a vector marking the position where the opponent was last sensed. This can
  // be used to help hunt down an opponent if it goes out of view
  Vector2D    vLastSensedPosition;

  //set to true if opponent is within the field of view of the owner
  bool        bWithinFOV;

  //set to true if there is no obstruction between the opponent and the owner, 
  //permitting a shot.
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
  
  // 센서메모리의 주인 봇
  Raven_Bot* m_pOwner;

  // 최근에 감지된 상대 목록
  MemoryMap  m_MemoryMap;

  // 봇이 최근에 감지된 상대 목록 ( m_MemoryMap )을 요청할때 봇이 상대를 기억하기 위한 값
  double      m_dMemorySpan;

  //this methods checks to see if there is an existing record for pBot. If
  //not a new MemoryRecord record is made and added to the memory map.(called
  //by UpdateWithSoundSource & UpdateVision)
  void       MakeNewRecordIfNotAlreadyPresent(Raven_Bot* pBot);

public:

  Raven_SensoryMemory(Raven_Bot* owner, double MemorySpan);

  //this method is used to update the memory map whenever an opponent makes
  //a noise
  void     UpdateWithSoundSource(Raven_Bot* pNoiseMaker);

  //this removes a bot's record from memory
  void     RemoveBotFromMemory(Raven_Bot* pBot);

  //this method iterates through all the opponents in the game world and 
  //updates the records of those that are in the owner's FOV
  void     UpdateVision();

  bool     isOpponentShootable(Raven_Bot* pOpponent)const;
  bool     isOpponentWithinFOV(Raven_Bot* pOpponent)const;
  Vector2D GetLastRecordedPositionOfOpponent(Raven_Bot* pOpponent)const;
  double    GetTimeOpponentHasBeenVisible(Raven_Bot* pOpponent)const;
  double    GetTimeSinceLastSensed(Raven_Bot* pOpponent)const;
  double    GetTimeOpponentHasBeenOutOfView(Raven_Bot* pOpponent)const;

  //this method returns a list of all the opponents that have had their
  //records updated within the last m_dMemorySpan seconds.
  std::list<Raven_Bot*> GetListOfRecentlySensedOpponents()const;

  void     RenderBoxesAroundRecentlySensed()const;

};


#endif