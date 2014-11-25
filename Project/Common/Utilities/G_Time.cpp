#include "G_Time.h"
#include "../Reflection/G_Reflection.h"
#include <GLFW\glfw3.h>

namespace Gem
{
    float Time::s_CurrentTime = 0.0f;
    float Time::s_DeltaTime = 0.0f;
    float Time::GetTime()
    {
        return glfwGetTime();
    }
    float Time::CurrentTime()
    {
        return s_CurrentTime;
    }
    float Time::DeltaTime()
    {
        return s_DeltaTime; 
    }
    Pointer<Reflection::Type> Time::GetType()
    {
        return typeOf("Time");
    }
}