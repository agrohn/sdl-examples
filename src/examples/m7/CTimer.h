#ifndef __CTimer_h__
#define __CTimer_h__
#include <SDL.h>
////////////////////////////////////////////////////////////////////////////////

  class CTimer
  {
  private:
    Uint32 m_CurrentTime;
    Uint32 m_PreviousTime;
  public:
    CTimer();
    virtual ~CTimer() {}
    void Update();
    void Reset();
    float GetPassedTime();
    Uint32 GetPassedTimeMS();
  }; // CTimer

////////////////////////////////////////////////////////////////////////////////
#endif
