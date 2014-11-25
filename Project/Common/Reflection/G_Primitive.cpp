#include "G_Primitive.h"
#include "G_Type.h"
#include "../Primitives/G_string.h"
#include "G_ReflectionOperators.h"

namespace Gem
{
    namespace Reflection
    {
        Primitive::Primitive()
        {
			m_Flags = 0;
        }
        Primitive::~Primitive()
        {

        }
        Pointer<Type> Primitive::GetType()
        {
            return typeOf("Primitive");
        }
		int Primitive::Flags()
		{
			return m_Flags;
		}
		void Primitive::SetHandle(int aHandle)
		{

		}
		int Primitive::GetHandle()
		{
			return 0;
		}
		bool Primitive::IsFlaggedForGC()
		{
			return (m_Flags & FLAG_GC_COLLECT) == FLAG_GC_COLLECT;
		}
    }
    
}