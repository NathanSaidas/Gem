#ifndef GAME_ENGINE_RUNTIME_H
#define GAME_ENGINE_RUNTIME_H

#pragma region CHANGE LOG
/// -- January 30, 2015 - Nathan Hanlan - Added Runtime class/file
/// -- April    3, 2015 - Nathan Hanlan - Added attribute binding support for MetaObjectLinker::ATTRIBUTE_TYPE_IS_ENUM
#pragma endregion

#include <string>
#include <map>
#include "AttributeBinder.h"
#include "TypeTrait.h"

namespace Gem
{
    namespace Reflection
    {

        class Runtime
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
            static bool IsBaseOf(Type & aDerived, Type & aBaseClass);
            static bool IsBadType(const Type & aType);
            static bool IsCompiling();
        private:
            static Runtime * s_Instance;
            Runtime();
            ~Runtime();

            bool m_IsCompiling;
            bool m_IsCompiled;
            std::map<std::string, Type> m_CompiledTypes;


            void BindIntegerAttributes(IntAttribute & aAttribute, Type & aType);
            void BindFloatAttributes(FloatAttribute & aAttribute, Type & aType);
            void BindBoolAttributes(BoolAttribute & aAttribute, Type & aType);
            void BindStringAttributes(StringAttribute & aAttribute, Type & aType);
            void BindFunctionAttributes(FunctionAttribute & aAttribute, Type & aType);
            void BindMemberInfoAttribute(MemberAttribute & aAttribute, Type & aType);
        };
    }
	
}

#endif