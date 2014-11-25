#ifndef G_PRIMITIVE_H
#define G_PRIMITIVE_H


#include "../Memory/G_Pointer.h"
#include "G_ReflectionRegristration.h"

namespace Gem
{
    namespace Reflection
    {
		const int FLAG_INVALID = -1;
		const int FLAG_CLEAR = 0;
		const int FLAG_GC_COLLECT = 1;
		const int FLAG_SUB_OBJECT_HANDLE = 2;

        //Base class of every type in the OnLooker Reflection
        class Type;
        class Primitive
        {
        public:
            Primitive();
            virtual ~Primitive();
            virtual Pointer<Type> GetType();
			int Flags();

            static Primitive Empty()
            {
                return Primitive();
            }
        protected:
			virtual void SetHandle(int aHandle);
			virtual int GetHandle();
			friend class MemoryManager;
            friend class Allocator;
		private:
			int m_Flags;

			bool IsFlaggedForGC();
        };
    };
}
GEM_CLASS(Primitive, "")
#endif