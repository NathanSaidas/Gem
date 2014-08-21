#ifndef G_CLASS_H
#define G_CLASS_H


#include "G_Type.h"

namespace Gem
{
    namespace Reflection
    {
        class Class //: public Type 
        {
        public:
            Class(){}
            ~Class(){}

            //3 Basic functions to establish a type
            virtual Type getType();
            virtual Type baseType();
            virtual Type * instanceOf(int & aCount);
        protected:
            
            
            //Field * fields; 
            //Function * functions;
        };
    }

}

#endif