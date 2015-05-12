#ifndef GAME_ENGINE_FUNCTION_ATTRIBUTE_H
#define GAME_ENGINE_FUNCTION_ATTRIBUTE_H

//============================================================
// Date:			May,	11,		2015
// Description:		Contains the FuncAttribute class.
//============================================================

#pragma region CHANGE LOG
/// -- January, 29, 2015 - Nathan Hanlan - Added FunctionAttribute class/file
/// -- April, 1, 2015 - Nathan Hanlan - Added Doxygen Style Comments.
/// -- April, 1, 2015 - Nathan Hanlan - Defined a type trait for FunctionAttribute.
#pragma endregion

//Using Attribute to inherit from.
#include "Attribute.h"
//Using Func to define Function objects.
#include "../Core/Func.h"

namespace Gem
{
    namespace Reflection
    {
		/**
		* Defines the attribute for Func values.
		* Func has a void * return type and void * argument
		*/
        class FunctionAttribute : public Attribute
        {
        public:
			/**
			* The constructor for FunctionAttribute, Requires the class name and attribute name of attribute as well as the value.
			* @param aClassName The name of the class to bind the attribute to.
			* @param aAttributeName The name of the attribute type. See MetaObjectLinker.h for reserved attribute types.
			* @param aValue The value of the attribute
			*/
            FunctionAttribute(const char * aClassName, const char * aAttributeName, Func<void*, void*> aValue)
                : Attribute(aClassName, aAttributeName)
            {
                m_Value = aValue;
            }
			/**
			* Gets the value of the attribute
			* @return Returns the value.
			*/
            inline Func<void*, void*> GetValue()
            {
                return m_Value;
            }
        private:
			/**
			* The value of the attribute
			*/
            Func<void*, void*> m_Value;
        };

		TYPE_DEFINE(FunctionAttribute);
    }
    
}

#endif