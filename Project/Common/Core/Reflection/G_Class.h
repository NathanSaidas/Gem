#ifndef G_CLASS_H
#define G_CLASS_H


#include "G_Primitive.h"

namespace Gem
{
    namespace Reflection
    {
        class Class : public Primitive
        {
        public:
            Class(){}
            ~Class(){}
    
            //3 Basic functions to establish a type
            virtual Pointer<Reflection::Type> getType() override;
        protected:
            
            
            //Field * fields; 
            //Function * functions;
        };
    }

}
GEM_CLASS(Class, Primitive)
#endif