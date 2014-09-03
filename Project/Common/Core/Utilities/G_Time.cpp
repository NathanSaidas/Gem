#include "G_Time.h"
#include "../Reflection/G_Reflection.h"
#include <GLFW\glfw3.h>

namespace Gem
{
    float Time::s_CurrentTime = 0.0f;
    float Time::s_DeltaTime = 0.0f;
    float Time::getTime()
    {
        return glfwGetTime();
    }
    float Time::currentTime()
    {
        return s_CurrentTime;
    }
    float Time::deltaTime()
    {
        return s_DeltaTime; 
    }
    Pointer<Reflection::Type> Time::getType()
    {
        return typeOf("Time");
    }
}