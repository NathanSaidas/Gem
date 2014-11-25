#ifndef G_FUNCTOR_H
#define G_FUNCTOR_H

#include "../Primitives/G_PrimitiveObjects.h"

namespace Gem
{
    ///Callback Definitions


    typedef void (*VoidCallback)();
    typedef bool (*BoolCallback)();
    typedef int (*IntCallback)();
    typedef Reflection::Primitive * (*PrimitiveCallback)();

    //template<typename T> T(*TemplateCallback)();


    

    typedef bool(*ConditionalCallback)(Reflection::Primitive * aArg1, Reflection::Primitive * aArg2);

    ///Function class defitions
    class Function
    {

    };

    class VoidFunction : Function
    {
    public:
        VoidFunction()
        {
            m_Callback = nullptr;
        }
        ~VoidFunction()
        {
        }
        inline void Invoke()
        {
            if (m_Callback != nullptr)
            {
                m_Callback();
            }
        }
        VoidCallback operator=(VoidCallback aCallback)
        {
            return m_Callback = aCallback;
        }
        bool operator==(VoidCallback aCallback)
        {
            return m_Callback == aCallback;
        }
        bool operator!=(VoidCallback aCallback)
        {
            return m_Callback != aCallback;
        }
        bool operator==(VoidFunction & aCallback)
        {
            return aCallback.m_Callback == m_Callback;
        }
        bool operator!=(VoidFunction & aCallback)
        {
            return aCallback.m_Callback != m_Callback;
        }
    private:
        VoidCallback m_Callback;
    };
    class BoolFunction : Function
    {
    public:
        BoolFunction()
        {
            m_Callback = nullptr;
        }
        ~BoolFunction()
        {
            
        }
        inline bool Invoke()
        {
            if (m_Callback != nullptr)
            {
                return m_Callback();
            }
            return false;
        }
        BoolCallback operator=(BoolCallback aCallback)
        {
            return m_Callback = aCallback;
        }
        bool operator==(BoolCallback aCallback)
        {
            return m_Callback == aCallback;
        }
        bool operator!=(BoolCallback aCallback)
        {
            return m_Callback != aCallback;
        }
        bool operator==(BoolFunction & aCallback)
        {
            return aCallback.m_Callback == m_Callback;
        }
        bool operator!=(BoolFunction & aCallback)
        {
            return aCallback.m_Callback != m_Callback;
        }
    private:
        BoolCallback m_Callback;
    };
    class IntFunction : Function
    {
    public:
        IntFunction()
        {
            m_Callback = nullptr;
        }
        ~IntFunction()
        {

        }

        inline int Invoke()
        {
            if (m_Callback != nullptr)
            {
                return m_Callback();
            }
            return -1;
        }

        IntCallback operator=(IntCallback aCallback)
        {
            return m_Callback = aCallback;
        }
        bool operator==(IntCallback aCallback)
        {
            return m_Callback == aCallback;
        }
        bool operator!=(IntCallback aCallback)
        {
            return m_Callback != aCallback;
        }
        bool operator==(IntFunction & aCallback)
        {
            return aCallback.m_Callback == m_Callback;
        }
        bool operator!=(IntFunction & aCallback)
        {
            return aCallback.m_Callback != m_Callback;
        }
    private:
        IntCallback m_Callback;
    };
    class PrimitiveFunction : Function
    {
    public:
        PrimitiveFunction()
        {
            m_Callback = nullptr;
        }
        ~PrimitiveFunction()
        {
            m_Callback = nullptr;
        }

        Reflection::Primitive * Invoke()
        {
            if (m_Callback != nullptr)
            {
                return m_Callback();
            }
            return nullptr;
        }

        bool operator==(PrimitiveCallback aCallback)
        {
            return m_Callback == aCallback;
        }
        bool operator!=(PrimitiveCallback aCallback)
        {
            return m_Callback != aCallback;
        }
        bool operator==(PrimitiveFunction & aCallback)
        {
            return aCallback.m_Callback == m_Callback;
        }
        bool operator!=(PrimitiveFunction & aCallback)
        {
            return aCallback.m_Callback != m_Callback;
        }
    private:
        PrimitiveCallback m_Callback;
    };

    class ConditionalFunction : Function
    {
    public:
        ConditionalFunction()
        {
            m_Callback = nullptr;
            m_Argument1 = nullptr;
            m_Argument2 = nullptr;
        }
        ~ConditionalFunction()
        {

        }
        inline bool Invoke(Reflection::Primitive * aArg1, Reflection::Primitive * aArg2)
        {
            m_Argument1 = aArg1;
            m_Argument2 = aArg2;
            if (m_Callback != nullptr)
            {
                return m_Callback(aArg1, aArg2);
            }
            return false;
        }
        ConditionalFunction operator=(ConditionalFunction aCallback)
        {
            m_Callback = aCallback.m_Callback;
            return (*this);
        }
        ConditionalCallback operator=(ConditionalCallback aCallback)
        {
            return m_Callback = aCallback;
        }
        bool operator==(ConditionalCallback aCallback)
        {
            return m_Callback == aCallback;
        }
        bool operator!=(ConditionalCallback aCallback)
        {
            return m_Callback != aCallback;
        }
        bool operator==(ConditionalFunction & aCallback)
        {
            return aCallback.m_Callback == m_Callback;
        }
        bool operator!=(ConditionalFunction & aCallback)
        {
            return aCallback.m_Callback != m_Callback;
        }


        Reflection::Primitive * argument1()
        {
            return m_Argument1;
        }
        Reflection::Primitive * argument1(Reflection::Primitive * aArgument)
        {
            return m_Argument1 = aArgument;
        }
        Reflection::Primitive * argument2()
        {
            return m_Argument2;
        }
        Reflection::Primitive * argument2(Reflection::Primitive * aArgument)
        {
            return m_Argument2 = aArgument;
        }
        ConditionalCallback callback()
        {
            return m_Callback;
        }
        ConditionalCallback callback(ConditionalCallback aCallback)
        {
            return m_Callback = aCallback;
        }
    private:
        ConditionalCallback m_Callback;
        Reflection::Primitive * m_Argument1;
        Reflection::Primitive * m_Argument2;
    };

    class Functor
    {
        virtual void Invoke() = 0;
        virtual void Bind() = 0;

    private:
        
    };
}

#endif
