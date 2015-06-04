#ifndef GAME_ENGINE_RUNTIME_H
#define GAME_ENGINE_RUNTIME_H

#pragma region CHANGE LOG
/// Nathan Hanlan - Added Runtime class/file
/// Nathan Hanlan - Added attribute binding support for MetaObjectLinker::ATTRIBUTE_TYPE_IS_ENUM
/// Nathan Hanlan - When linking inherited methods, it no longer discards overwritten methods.
#pragma endregion

#include <string>
#include <map>
#include "AttributeBinder.h"
#include "TypeTrait.h"
#include "../Core/GemAPI.h"

template class GEM_API std::map<std::string, Gem::Type>;

namespace Gem
{
    namespace Reflection
    {

        class GEM_API Runtime
        {
        public:
            static void Initialize();
            static void Terminate();

            static void Compile(AttributeBinder * aContext);
            static Type TypeOf(const std::string & aName);
            template<class T>
            static Type TypeOf()
            {
                return TypeOf(TypeTrait<T>::Name());
            }
			static Array<Type> GetAllTypes();
            static bool IsBaseOf(Type & aDerived, Type & aBaseClass);
            static bool IsBadType(const Type & aType);
            static bool IsCompiling();
			static bool IsCompiled();
        private:
            static Runtime * s_Instance;
            Runtime();
            ~Runtime();

            bool m_IsCompiling;
            bool m_IsCompiled;
            std::map<std::string, Type> m_CompiledTypes;
			int m_CompileErrorFlags;

            void BindIntegerAttributes(IntAttribute & aAttribute, Type & aType);
            void BindFloatAttributes(FloatAttribute & aAttribute, Type & aType);
            void BindBoolAttributes(BoolAttribute & aAttribute, Type & aType);
            void BindStringAttributes(StringAttribute & aAttribute, Type & aType);
            void BindFunctionAttributes(FunctionAttribute & aAttribute, Type & aType);
            void BindMemberInfoAttribute(MemberAttribute & aAttribute, Type & aType);

			void LinkBaseClassMembers(Type & aChild, bool * typeLinkComplete);
        };
    }
	
}

#endif