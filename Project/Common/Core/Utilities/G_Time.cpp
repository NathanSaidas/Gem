#include "G_Time.h"

namespace Gem
{
    float Time::s_CurrentTime = 0.0f;
    float Time::s_DeltaTime = 0.0f;

    float Time::currentTime()
    {
        return s_CurrentTime;
    }
    float Time::deltaTime()
    {
        return s_DeltaTime; 
    }
}