#ifndef G_TIME_H
#define G_TIME_H
#include "../Primitives/G_PrimitiveObjects.h"

namespace Gem
{
    class Time sealed : public Object
    {
    public:
        static float GetTime();
        static float CurrentTime();
        static float DeltaTime();
        Pointer<Reflection::Type> GetType() override;
    private:
        static float s_CurrentTime;
        static float s_DeltaTime;

        friend class Application;
    };
}
GEM_CLASS(Time,Object)
#endif