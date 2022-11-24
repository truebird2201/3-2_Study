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

  // ���� Ÿ��
  Raven_Bot*  m_pCurrentTarget;


public:

  Raven_TargetingSystem(Raven_Bot* owner);

  // ���� ����� ���� m_pCurrentTarget�� �Ǹ� �������� ���� ��� null�� �ȴ�.

  void       Update();

  // m_pCurrentTarget�� �ִ°�?

  bool       isTargetPresent()const{return m_pCurrentTarget != 0;}

  // ����� �þ߳��� �ִ°�?

  bool       isTargetWithinFOV()const;

  // ��밡 �����Ҽ� �ִ� ��ġ�� �ִ°�?

  bool       isTargetShootable()const;

  // ����� ���������� �߰ߵ� ��ġ�� ��ȯ. ���� �Ҵ�� ����� ���� ��� ���ܸ� �߻�

  Vector2D   GetLastRecordedPosition()const;

  // ����� �þ߿� �ִ� �ð�

  double      GetTimeTargetHasBeenVisible()const;

  // ����� �þ߿� ���� �ð�

  double      GetTimeTargetHasBeenOutOfView()const;
  
  // ����� ��ȯ

  Raven_Bot* GetTarget()const{return m_pCurrentTarget;}

  // ����� null�� �ٲ�

  void       ClearTarget(){m_pCurrentTarget=0;}
};




#endif
