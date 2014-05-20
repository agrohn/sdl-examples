#include "CTimer.h"
////////////////////////////////////////////////////////////////////////////////
void
CTimer::Update()
{
  m_CurrentTime = SDL_GetTicks();
}
////////////////////////////////////////////////////////////////////////////////
CTimer::CTimer() 
{
  Reset();
}
////////////////////////////////////////////////////////////////////////////////
void 
CTimer::Reset()
{
  m_PreviousTime = m_CurrentTime = SDL_GetTicks();
}
////////////////////////////////////////////////////////////////////////////////
float 
CTimer::GetPassedTime()
{
  return (m_CurrentTime - m_PreviousTime) * 0.001f;
}
////////////////////////////////////////////////////////////////////////////////
Uint32 
CTimer::GetPassedTimeMS()
{
  return (m_CurrentTime - m_PreviousTime);
}
////////////////////////////////////////////////////////////////////////////////
