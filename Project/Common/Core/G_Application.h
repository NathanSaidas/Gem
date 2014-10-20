#ifndef G_APPLICATION_H
#define G_APPLICATION_H

#include "Primitives\G_PrimitiveObjects.h"

namespace Gem
{
    class Application : public Object
    {
    public:
        Application();
        ~Application();
        static Application * instance();
        static void destroy();


        //called once on main thread
        void execute();
		static void exit();

        virtual Pointer<Reflection::Type> getType() override;
    private:

        bool init();
        bool run();
        bool deinit();

        static Application * s_Instance;
        
		bool m_ShouldQuit;
        friend class PoolAllocator;
    };
}

GEM_CLASS(Application,Object)


#endif