#include "G_Screen.h"
#include "../Reflection/G_Reflection.h"

namespace Gem
{
    using namespace Reflection;

    Type * Screen::getType()
    {
        return Type::create("Screen",TypeID::SCREEN,sizeof(Screen),Object::getType());
    }

}