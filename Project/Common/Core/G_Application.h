#ifndef G_APPLICATION_H
#define G_APPLICATION_H

#include "Primitives\G_PrimitiveObjects.h"

namespace Gem
{
    class Application : public Object
    {
    public:
        static Application * instance();
        static void destroy();


        //called once on main thread
        void execute();


        virtual Pointer<Reflection::Type> getType() override;
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

GEM_CLASS(Application,Object)


#endif