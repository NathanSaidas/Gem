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
            static Runtime * Instance();
            static void Destroy();

            
            Type * GetType(string & aTypename);
            Type * GetType(integer & aTypeID);

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