#include "G_UniqueNumberGenerator.h"
#include "../Reflection/G_Reflection.h"


namespace Gem
{
    using namespace Reflection;

    UNG::UniqueNumberGenerator()
    {
        m_NextNumber =0;
    }
    UNG::~UniqueNumberGenerator()
    {

    }
    int UNG::getUniqueNumber()
    {
        int uniqueNumber = 0;
        //Get a Number off the list
        if(m_FreeNumberList.count() > 0)
        {
            uniqueNumber= m_FreeNumberList[0];
            m_FreeNumberList.removeAt(0);
            return uniqueNumber;
        }
        uniqueNumber = m_NextNumber;
        m_NextNumber++;
        return uniqueNumber;
    }
    void UNG::returnUniqueNumber(int aNumber)
    {
        if(aNumber > m_NextNumber)
        {
            return;
        }
        for(int i = 0; i < m_FreeNumberList.count(); i++)
        {
            if(m_FreeNumberList[i] == aNumber)
            {
                return;
            }
        }
        m_FreeNumberList.addElement(aNumber);
    }
    Type UNG::getType()
    {
        return TypeFactory::create("UniqueNumberGenerator",TypeID::UNIQUE_NUMBER_GENERATOR,sizeof(UniqueNumberGenerator));
    }
    Type UNG::baseType()
    {
        return Object::getType();
    }
    Type * UNG::instanceOf(int & aCount)
    {
        int aPrevCount;
        Type * aPrevTypes = Object::instanceOf(aCount);
        Type base = baseType();
        Type * types = TypeFactory::create(base,aPrevCount+1,aPrevTypes,aPrevCount);
        return types;
    }
}