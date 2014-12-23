#ifndef G_TIME_H
#define G_TIME_H

#include "../G_Object.h"

namespace Gem
{
    class Time sealed : public object
    {
		G_CLASS_DEF(Time)
    public:
        static float GetTime();
        static float CurrentTime();
        static float DeltaTime();
    private:
        static float s_CurrentTime;
        static float s_DeltaTime;

        friend class Application;
    };
}
#endif