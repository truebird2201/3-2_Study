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
  
  // ��븦 ����ϴ� �ִ�ð�
  // (CurrentTime() - m_dTimeLastSensed�� ���� �޸� �������� ũ�� �� ���ڵ��� �����͸� Ŭ���̾�Ʈ���� ����� �� ���� �˴ϴ�.)

  double       fTimeLastSensed;

  //��밡 ó�� ǥ�õ� �ð�. ��밡 �󸶳� �������� �þ߿� ���Դ��� -> (CurrentTime - fTimeBecameVisible).

  double       fTimeBecameVisible;

  // ��밡 ���������� ���� �ð�

  double       fTimeLastVisible;

  // ��밡 ���������� ������ ��ġ�� ǥ���ϴ� ���ͷ� ��밡 �þ� ������ ���� ��� ��븦 �����ϴ� �� ����

  Vector2D    vLastSensedPosition;

  // ��밡 �þ߳��� �ִ���

  bool        bWithinFOV;

  // ��밡 ��� �ִ� ��ġ�� �ִ���

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

  // �ֱٿ� ������ ��� ���

  MemoryMap  m_MemoryMap;

  // ���� �ֱٿ� ������ ��� ��� ( m_MemoryMap )�� ��û�Ҷ� ���� ��븦 ����ϱ� ���� ��

  double      m_dMemorySpan;

  // ���� ���ڵ尡 �ִ��� Ȯ���ϰ� ���ٸ� �޸� �ʿ� �߰��˴ϴ�
  // (UpdateWithSoundSource& UpdateVision���� ȣ��)

  void       MakeNewRecordIfNotAlreadyPresent(Raven_Bot* pBot);

public:

  Raven_SensoryMemory(Raven_Bot* owner, double MemorySpan);

  // ��밡 �Ҹ��� �� ������ �޸� ���� ������Ʈ

  void     UpdateWithSoundSource(Raven_Bot* pNoiseMaker);

  // �޸𸮿��� ���� ����� ����

  void     RemoveBotFromMemory(Raven_Bot* pBot);

  // ��� ��ü�� ���� �ݺ��Ǹ� ���� �þ߿� �ִ� ����� ����� ������Ʈ

  void     UpdateVision();

  bool     isOpponentShootable(Raven_Bot* pOpponent)const;
  bool     isOpponentWithinFOV(Raven_Bot* pOpponent)const;
  Vector2D GetLastRecordedPositionOfOpponent(Raven_Bot* pOpponent)const;
  double    GetTimeOpponentHasBeenVisible(Raven_Bot* pOpponent)const;
  double    GetTimeSinceLastSensed(Raven_Bot* pOpponent)const;
  double    GetTimeOpponentHasBeenOutOfView(Raven_Bot* pOpponent)const;

  // ������ m_dMemorySpan ���� ���ڵ带 ������Ʈ�� ��� ����� ����� ��ȯ�մϴ�.

  std::list<Raven_Bot*> GetListOfRecentlySensedOpponents()const;

  void     RenderBoxesAroundRecentlySensed()const;

};


#endif