#ifndef G_TYPE_LIST_H
#define G_TYPE_LIST_H

#include <vector>
#include <string>

namespace Gem
{
    namespace Reflection
    {
        class Primitive;

        typedef Primitive * (*CreatePrimitiveFunc)(Primitive*);
        typedef Primitive * (*DestroyPrimitiveFunc)(Primitive*);


        class TypeInfo
        {
        public:
            TypeInfo();
            ~TypeInfo();

            std::string className() const;
            unsigned int typeID();
            unsigned int size();
            unsigned int alignment();
            CreatePrimitiveFunc createFunc();
            DestroyPrimitiveFunc DestroyFunc();

            std::string baseClassName() const;

            static TypeInfo * create(std::string aClassName, unsigned int aID, unsigned int aSize, unsigned int aAlignment, CreatePrimitiveFunc aCreateFunc, DestroyPrimitiveFunc aDestroyFunc, std::string aBaseClass);
        private:
            std::string m_ClassName;
            unsigned int m_TypeID;
            unsigned int m_Size;
            unsigned int m_Alignment;
            CreatePrimitiveFunc m_CreateFunc;
            DestroyPrimitiveFunc m_DestroyFunc;

            std::string m_BaseClassName;
            friend class TypeList;
            
        };

        class TypeKey
        {
        public:
            bool compareName(const char * aName);
            bool compareID(int aID);

        private:

            std::string m_ClassName;
            unsigned int m_TypeID;

            TypeInfo * m_Info;

            friend class TypeList;
        };


        class TypeList
        {
        public:
            TypeInfo * GetType(std::string aName);
            TypeInfo * GetType(const char * aName);
            TypeInfo * GetType(int aId);

            int GetTypeCount();
            void addKey(TypeInfo * aType);
        private:

            

            std::vector<TypeKey> m_Keys;
        };
    }
}

#endif