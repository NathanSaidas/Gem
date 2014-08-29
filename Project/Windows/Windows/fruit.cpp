// Matching header
#include "fruit.h"

// Standard libraries
#include <string>

// Custom libraries
#include "detail.h"


Component* component::create(const std::string& name)
{
    detail::ComponentRegistry& reg = detail::getComponentRegistry();
    detail::ComponentRegistry::iterator it = reg.find(name);

    if (it == reg.end()) {
        // This happens when there is no component registered to this
        // name. Here I return a null pointer, but you can handle this
        // error differently if it suits you better.
        return nullptr;
    }

    detail::CreateComponentFunc func = it->second;
    return func();
}

void component::destroy(const Component* comp)
{
    delete comp;
}