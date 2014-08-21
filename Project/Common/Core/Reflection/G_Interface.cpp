#include "G_Interface.h"

#include "G_TypeFactory.h"
#include "G_Types.h"
namespace Gem
{
    namespace Reflection
    {
        Type Interface::getType()
        {
            return TypeFactory::create("Interface",TypeID::INTERFACE,sizeof(Interface));
        }
    }
}