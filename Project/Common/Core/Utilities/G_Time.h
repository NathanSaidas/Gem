#ifndef G_TIME_H
#define G_TIME_H

namespace Gem
{
    class Time
    {
    public:
        static float currentTime();
        static float deltaTime();
    private:
        static float s_CurrentTime;
        static float s_DeltaTime;

        friend class Application;
    };
}

#endif