#ifndef G_REFLECTION_RUNTIME_H
#define G_REFLECTION_RUNTIME_H

namespace Gem
{
    class string;
    class integer;

    namespace Reflection
    {
        class Type;

        class Runtime
        {
        public:
            static Runtime * instance();
            static void destroy();

            
            Type * getType(string & aTypename);
            Type * getType(integer & aTypeID);

        private:
            static Runtime * s_Instance;
            Runtime();
            ~Runtime();

            int m_TypeCount;
            //The list of types
            Type ** m_Types;

            friend class Hidden;

        };
    }
}

#endif