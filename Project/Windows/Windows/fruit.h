#ifndef COMPONENT_COMPONENT_H
#define COMPONENT_COMPONENT_H

// Standard libraries
#include <string>

// Custom libraries
#include "detail.h"


class Component
{
    // ...
};

class TypeInfo
{
public:
    char * m_Name;
    int m_Id;
    int m_Size;
    TypeInfo * m_BaseClass;
};

namespace component
{
    Component* create(const std::string& name);
    void destroy(const Component* comp);
}

#define COMPONENT_REGISTER(TYPE, NAME)                                        \
    namespace component {                                                     \
    namespace detail {                                                        \
    namespace                                                                 \
    {                                                                         \
        template<class T>                                                     \
        class ComponentRegistration;                                          \
                                                                              \
        template<>                                                            \
        class ComponentRegistration<TYPE>                                     \
        {                                                                     \
            static const RegistryEntry<TYPE>& reg;                            \
        };                                                                    \
                                                                              \
        const RegistryEntry<TYPE>& ComponentRegistration<TYPE>::reg =         \
                RegistryEntry<TYPE>::Instance(NAME);                          \
    }}}


#endif // COMPONENT_COMPONENT_H