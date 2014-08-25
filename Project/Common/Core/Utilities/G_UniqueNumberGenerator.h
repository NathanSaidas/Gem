#ifndef G_UNIQUE_NUMBER_GENERATOR_H
#define G_UNIQUE_NUMBER_GENERATOR_H


#include "../Base Objects/G_Object.h"
#include "G_Array.h"


namespace Gem
{
    #define UNG UniqueNumberGenerator

    class UniqueNumberGenerator : public Object
    {
    public:
        UniqueNumberGenerator();
        ~UniqueNumberGenerator();


        int getUniqueNumber();
        void returnUniqueNumber(int aNumber);

        virtual Reflection::Type * getType();

    private:
        int m_NextNumber;
        Array<int,int> m_FreeNumberList;
        
    };
}

#endif