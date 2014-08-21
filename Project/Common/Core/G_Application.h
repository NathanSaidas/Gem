#ifndef G_APPLICATION_H
#define G_APPLICATION_H

#include "Base Objects/G_Object.h"

namespace Gem
{
    class Application : public Object
    {
    public:
        static Application * instance();
        static void destroy();


        //called once on main thread
        void execute();


        virtual Reflection::Type getType();
        virtual Reflection::Type baseType();
        virtual Reflection::Type * instanceOf(int & aCount);
    private:

        bool init();
        bool run();
        bool deinit();

        static Application * s_Instance;
        Application();
        ~Application();

        friend class PoolAllocator;
    };
}

#endif