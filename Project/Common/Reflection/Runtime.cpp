#include "Runtime.h"

#include "MetaObjectLinker.h"

namespace Gem
{
    bool Type::s_IsCompiling = false;

    namespace Reflection
    {
        
        Runtime * Runtime::s_Instance = nullptr;
        Runtime::Runtime()
        {

        }
        Runtime::~Runtime()
        {
            //for (std::map<std::string, Type>::iterator typeIt = m_CompiledTypes.begin(); typeIt != m_CompiledTypes.end(); ++typeIt)
            //{
            //    std::vector<ClassMember*> members = typeIt->second.m_Members;
            //    for (std::vector<ClassMember*>::iterator it = members.begin(); it != members.end(); ++it)
            //    {
            //        if ((*it) != nullptr)
            //        {
            //            delete (*it);
            //        }
            //    }
            //}
        }

        void Runtime::Initialize()
        {
            if (s_Instance == nullptr)
            {
                s_Instance = new Runtime();
            }
        }
        void Runtime::Terminate()
        {
            if (s_Instance != nullptr)
            {
                delete s_Instance;
                s_Instance = nullptr;
            }
        }

        void Runtime::Compile(AttributeBinder * aContext)
        {
            Initialize();
            if (s_Instance->m_IsCompiled)
            {
                return;
            }
            s_Instance->m_IsCompiling = true;
            Type::s_IsCompiling = true;

            std::vector<char *> & types = MetaObjectLinker::GetTypes();
            IntAttribMap & intAttributes = MetaObjectLinker::GetIntAttributes();
            FloatAttribMap & floatAttributes = MetaObjectLinker::GetFloatAttributes();
            BoolAttribMap & boolAttributes = MetaObjectLinker::GetBooleanAttributes();
            StringAttribMap & stringAttributes = MetaObjectLinker::GetStringAttributes();
            FuncAttribMap & functionAttributes = MetaObjectLinker::GetFuncAttributes();
            MemberAttribMap & members = MetaObjectLinker::GetMemberAttributes();

            for (std::vector<char*>::iterator typeIter = types.begin(); typeIter != types.end(); ++typeIter)
            {
                IntAttribIterator intKeyIter = intAttributes.equal_range((*typeIter));
                FloatAttribIterator floatKeyIter = floatAttributes.equal_range((*typeIter));
                BoolAttribIterator boolKeyIter = boolAttributes.equal_range((*typeIter));
                StringAttribIterator stringKeyIter = stringAttributes.equal_range((*typeIter));
                FuncAttribIterator functionKeyIter = functionAttributes.equal_range((*typeIter));
                MemberAttribIterator memberKeyIter = members.equal_range((*typeIter));

                Type type;

                for (IntAttribMap::iterator it = intKeyIter.first; it != intKeyIter.second; ++it)
                {
                    if (aContext != nullptr)
                    {
                        aContext->BindInteger((*it).second, type);
                    }
                    s_Instance->BindIntegerAttributes((*it).second, type);
                }
                for (FloatAttribMap::iterator it = floatKeyIter.first; it != floatKeyIter.second; ++it)
                {
                    if (aContext != nullptr)
                    {
                        aContext->BindFloat((*it).second, type);
                    }
                    s_Instance->BindFloatAttributes((*it).second, type);
                }
                for (BoolAttribMap::iterator it = boolKeyIter.first; it != boolKeyIter.second; ++it)
                {
                    if (aContext != nullptr)
                    {
                        aContext->BindBool((*it).second, type);
                    }
                    s_Instance->BindBoolAttributes((*it).second, type);
                }
                for (StringAttribMap::iterator it = stringKeyIter.first; it != stringKeyIter.second; ++it)
                {
                    if (aContext != nullptr)
                    {
                        aContext->BindString((*it).second, type);
                    }
                    s_Instance->BindStringAttributes((*it).second, type);
                }
                for (FuncAttribMap::iterator it = functionKeyIter.first; it != functionKeyIter.second; ++it)
                {
                    if (aContext != nullptr)
                    {
                        aContext->BindFunction((*it).second, type);
                    }
                    s_Instance->BindFunctionAttributes((*it).second, type);
                }

                for (MemberAttribMap::iterator it = memberKeyIter.first; it != memberKeyIter.second; ++it)
                {
                    if (aContext != nullptr)
                    {

                    }
                    s_Instance->BindMemberInfoAttribute((*it).second, type);
                }

                printf("Compiled Type: %s\n", type.GetName().c_str());
                s_Instance->m_CompiledTypes.insert(std::pair<std::string, Type>(type.GetName(), type));
            }

            types.clear();
            intAttributes.clear();
            floatAttributes.clear();
            boolAttributes.clear();
            stringAttributes.clear();
            functionAttributes.clear();

            if (aContext != nullptr)
            {
                delete aContext;
                aContext = nullptr;
            }

            s_Instance->m_IsCompiled = true;
            s_Instance->m_IsCompiling = false;
            Type::s_IsCompiling = false;
        }
        Type Runtime::TypeOf(const std::string & aName)
        {
            if (s_Instance == nullptr)
            {
                return Type();
            }
            std::map<std::string, Type>::iterator type = s_Instance->m_CompiledTypes.find(aName);
            if (type == s_Instance->m_CompiledTypes.end())
            {
                return Type();
            }
            return (*type).second;
        }
		Array<Type> Runtime::GetAllTypes()
		{
			if (s_Instance == nullptr)
			{
				return Array<Type>();
			}
			Array<Type> types = s_Instance->m_CompiledTypes.size();
			int index = 0;
			for (std::map<std::string, Type>::iterator it = s_Instance->m_CompiledTypes.begin();
				it != s_Instance->m_CompiledTypes.end();
				it++)
			{
				types[index] = (*it).second;
				index++;
			}
			return types;
		}
        bool Runtime::IsBaseOf(Type & aDerived, Type & aBaseClass)
        {
            Type type = aDerived;

            while (!IsBadType(type))
            {
                type = TypeOf(type.GetBaseClass());
                if (aBaseClass == type)
                {
                    return true;
                }
            }

            return false;
        }
        bool Runtime::IsBadType(const Type & aType)
        {
            return aType.GetName() == "";
        }
        bool Runtime::IsCompiling()
        {
            return s_Instance->m_IsCompiling;
        }
        void Runtime::BindIntegerAttributes(IntAttribute & aAttribute, Type & aType)
        {
            if (aAttribute.Is(Attribute("",MetaObjectLinker::ATTRIBUTE_TYPE_ALIGNMENT)))
            {
                aType.m_Alignment = aAttribute.GetValue();
            }
			else if (aAttribute.Is(Attribute("",MetaObjectLinker::ATTRIBUTE_TYPE_SIZE)))
            {
                aType.m_Size = aAttribute.GetValue();
            }
			else if (aAttribute.Is(Attribute("",MetaObjectLinker::ATTRIBUTE_TYPE_TYPE_ID)))
            {
                aType.m_TypeID = aAttribute.GetValue();
            }
        }
        void Runtime::BindFloatAttributes(FloatAttribute & aAttribute, Type & aType)
        {

        }
        void Runtime::BindBoolAttributes(BoolAttribute & aAttribute, Type & aType)
        {
			if (aAttribute.Is(Attribute("",MetaObjectLinker::ATTRIBUTE_TYPE_IS_ABSTRACT)))
            {
                aType.m_IsAbstract = aAttribute.GetValue();
            }
			else if (aAttribute.Is(Attribute("",MetaObjectLinker::ATTRIBUTE_TYPE_IS_CLASS)))
            {
                aType.m_IsClass = aAttribute.GetValue();
            }
			else if (aAttribute.Is(Attribute("",MetaObjectLinker::ATTRIBUTE_TYPE_IS_INTERFACE)))
            {
                aType.m_IsInterface = aAttribute.GetValue();
            }
			else if (aAttribute.Is(Attribute("", MetaObjectLinker::ATTRIBUTE_TYPE_IS_ENUM)))
			{
				aType.m_IsEnum = aAttribute.GetValue();
			}
        }
        void Runtime::BindStringAttributes(StringAttribute & aAttribute, Type & aType)
        {
			if (aAttribute.Is(Attribute("",MetaObjectLinker::ATTRIBUTE_TYPE_BASE_CLASS_NAME)))
            {
                aType.m_BaseClass = aAttribute.GetValue();
            }
			else if (aAttribute.Is(Attribute("",MetaObjectLinker::ATTRIBUTE_TYPE_NAME)))
            {
                aType.m_Name = aAttribute.GetValue();
            }
			else if (aAttribute.Is(Attribute("",MetaObjectLinker::ATTRIBUTE_TYPE_INTERFACE)))
            {
                aType.m_Interfaces.push_back(aAttribute.GetValue());
            }

        }
        void Runtime::BindFunctionAttributes(FunctionAttribute & aAttribute, Type & aType)
        {
			if (aAttribute.Is(Attribute("",MetaObjectLinker::ATTRIBUTE_TYPE_CREATE_FUNC)))
            {
                aType.m_Constructor = aAttribute.GetValue();
            }
			else if (aAttribute.Is(Attribute("",MetaObjectLinker::ATTRIBUTE_TYPE_DESTROY_FUNC)))
            {
                aType.m_Destructor = aAttribute.GetValue();
            }
        }
        void Runtime::BindMemberInfoAttribute(MemberAttribute & aAttribute, Type & aType)
        {
            if (aAttribute.Is(Attribute("", MetaObjectLinker::ATTRIBUTE_TYPE_MEMBER_INFO)))
            {
                aType.m_Members.push_back(aAttribute.GetValue());
            }
        }


    }

}
