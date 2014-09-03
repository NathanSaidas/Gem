#ifndef G_UNIQUE_NUMBER_GENERATOR_H
#define G_UNIQUE_NUMBER_GENERATOR_H


#include "../Primitives/G_PrimitiveObjects.h"
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

        virtual Pointer<Reflection::Type> getType() override;

    private:
        int m_NextNumber;
        Array<int,int> m_FreeNumberList;
        
    };
    
}
GEM_CLASS(UniqueNumberGenerator, Object)
#endif