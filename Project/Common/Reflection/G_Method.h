#ifndef G_MEMBER_FUNCTIONS_H
#define G_MEMBER_FUNCTIONS_H

#pragma region CHANGE LOG
/* December, 20, 2014 - Nathan Hanlan - 
*/
#pragma endregion

namespace Gem
{
	namespace Reflection
	{
#pragma region METHODS
		template<class CLASS, class RETURN, class ARGUMENT1 = void, class ARGUMENT2 = void, class ARGUMENT3 = void, class ARGUMENT4 = void, class ARGUMENT5 = void, class ARGUMENT6 = void, class ARGUMENT7 = void, class ARGUMENT8 = void>
		class Method
		{
		public:
			typedef RETURN(CLASS::*Delegate)(ARGUMENT1, ARGUMENT2, ARGUMENT3, ARGUMENT4, ARGUMENT5, ARGUMENT6, ARGUMENT7, ARGUMENT8);
			Method()
			{
				m_Function = nullptr;
			}
			Method(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~Method()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			RETURN operator()(CLASS & aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5, ARGUMENT6 aArg6, ARGUMENT7 aArg7, ARGUMENT8 aArg8)
			{
				if (m_Function != nullptr)
				{
					return (aInstance.*m_Function)(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7, aArg8);
				}
				return RETURN();
			}
			RETURN operator()(CLASS * aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5, ARGUMENT6 aArg6, ARGUMENT7 aArg7, ARGUMENT8 aArg8)
			{
				if (m_Function != nullptr && aInstance != nullptr)
				{
					return (aInstance->*m_Function)(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7, aArg8);
				}
				return RETURN();
			}
			RETURN Invoke(CLASS & aInstance,ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5, ARGUMENT6 aArg6, ARGUMENT7 aArg7, ARGUMENT8 aArg8)
			{
				if (m_Function != nullptr )
				{
					return (aInstance.*m_Function)(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7, aArg8);
				}
				return RETURN();
			}
			RETURN Invoke(CLASS * aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5, ARGUMENT6 aArg6, ARGUMENT7 aArg7, ARGUMENT8 aArg8)
			{
				if (m_Function != nullptr)
				{
					return (aInstance->*m_Function)(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7, aArg8);
				}
				return RETURN();
			}
		private:
			Delegate m_Function;
		};

		template<class CLASS, class RETURN, class ARGUMENT1, class ARGUMENT2, class ARGUMENT3, class ARGUMENT4, class ARGUMENT5, class ARGUMENT6, class ARGUMENT7>
		class Method<CLASS, RETURN, ARGUMENT1, ARGUMENT2, ARGUMENT3, ARGUMENT4, ARGUMENT5, ARGUMENT6, ARGUMENT7>
		{
		public:
			typedef RETURN(CLASS::*Delegate)(ARGUMENT1, ARGUMENT2, ARGUMENT3, ARGUMENT4, ARGUMENT5, ARGUMENT6, ARGUMENT7);
			Method()
			{
				m_Function = nullptr;
			}
			Method(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~Method()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			RETURN operator()(CLASS & aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5, ARGUMENT6 aArg6, ARGUMENT7 aArg7)
			{
				if (m_Function != nullptr)
				{
					return (aInstance.*m_Function)(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7);
				}
				return RETURN();
			}
			RETURN operator()(CLASS * aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5, ARGUMENT6 aArg6, ARGUMENT7 aArg7)
			{
				if (m_Function != nullptr && aInstance != nullptr)
				{
					return (aInstance->*m_Function)(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7);
				}
				return RETURN();
			}
			RETURN Invoke(CLASS & aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5, ARGUMENT6 aArg6, ARGUMENT7 aArg7)
			{
				if (m_Function != nullptr)
				{
					return (aInstance.*m_Function)(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7);
				}
				return RETURN();
			}
			RETURN Invoke(CLASS * aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5, ARGUMENT6 aArg6, ARGUMENT7 aArg7)
			{
				if (m_Function != nullptr)
				{
					return (aInstance->*m_Function)(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7);
				}
				return RETURN();
			}
		private:
			Delegate m_Function;
		};

		template<class CLASS, class RETURN, class ARGUMENT1, class ARGUMENT2, class ARGUMENT3, class ARGUMENT4, class ARGUMENT5, class ARGUMENT6>
		class Method<CLASS, RETURN, ARGUMENT1, ARGUMENT2, ARGUMENT3, ARGUMENT4, ARGUMENT5, ARGUMENT6>
		{
		public:
			typedef RETURN(CLASS::*Delegate)(ARGUMENT1, ARGUMENT2, ARGUMENT3, ARGUMENT4, ARGUMENT5, ARGUMENT6);
			Method()
			{
				m_Function = nullptr;
			}
			Method(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~Method()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			RETURN operator()(CLASS & aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5, ARGUMENT6 aArg6)
			{
				if (m_Function != nullptr)
				{
					return (aInstance.*m_Function)(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6);
				}
				return RETURN();
			}
			RETURN operator()(CLASS * aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5, ARGUMENT6 aArg6)
			{
				if (m_Function != nullptr && aInstance != nullptr)
				{
					return (aInstance->*m_Function)(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6);
				}
				return RETURN();
			}
			RETURN Invoke(CLASS & aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5, ARGUMENT6 aArg6)
			{
				if (m_Function != nullptr)
				{
					return (aInstance.*m_Function)(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6);
				}
				return RETURN();
			}
			RETURN Invoke(CLASS * aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5, ARGUMENT6 aArg6)
			{
				if (m_Function != nullptr)
				{
					return (aInstance->*m_Function)(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6);
				}
				return RETURN();
			}
		private:
			Delegate m_Function;
		};

		template<class CLASS, class RETURN, class ARGUMENT1, class ARGUMENT2, class ARGUMENT3, class ARGUMENT4, class ARGUMENT5>
		class Method<CLASS, RETURN, ARGUMENT1, ARGUMENT2, ARGUMENT3, ARGUMENT4, ARGUMENT5>
		{
		public:
			typedef RETURN(CLASS::*Delegate)(ARGUMENT1, ARGUMENT2, ARGUMENT3, ARGUMENT4, ARGUMENT5);
			Method()
			{
				m_Function = nullptr;
			}
			Method(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~Method()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			RETURN operator()(CLASS & aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5)
			{
				if (m_Function != nullptr)
				{
					return (aInstance.*m_Function)(aArg1, aArg2, aArg3, aArg4, aArg5);
				}
				return RETURN();
			}
			RETURN operator()(CLASS * aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5)
			{
				if (m_Function != nullptr && aInstance != nullptr)
				{
					return (aInstance->*m_Function)(aArg1, aArg2, aArg3, aArg4, aArg5);
				}
				return RETURN();
			}
			RETURN Invoke(CLASS & aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5)
			{
				if (m_Function != nullptr)
				{
					return (aInstance.*m_Function)(aArg1, aArg2, aArg3, aArg4, aArg5);
				}
				return RETURN();
			}
			RETURN Invoke(CLASS * aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5)
			{
				if (m_Function != nullptr)
				{
					return (aInstance->*m_Function)(aArg1, aArg2, aArg3, aArg4, aArg5);
				}
				return RETURN();
			}
		private:
			Delegate m_Function;
		};

		template<class CLASS, class RETURN, class ARGUMENT1, class ARGUMENT2, class ARGUMENT3, class ARGUMENT4>
		class Method<CLASS, RETURN, ARGUMENT1, ARGUMENT2, ARGUMENT3, ARGUMENT4>
		{
		public:
			typedef RETURN(CLASS::*Delegate)(ARGUMENT1, ARGUMENT2, ARGUMENT3, ARGUMENT4);
			Method()
			{
				m_Function = nullptr;
			}
			Method(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~Method()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			RETURN operator()(CLASS & aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4)
			{
				if (m_Function != nullptr)
				{
					return (aInstance.*m_Function)(aArg1, aArg2, aArg3, aArg4);
				}
				return RETURN();
			}
			RETURN operator()(CLASS * aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4)
			{
				if (m_Function != nullptr && aInstance != nullptr)
				{
					return (aInstance->*m_Function)(aArg1, aArg2, aArg3, aArg4);
				}
				return RETURN();
			}
			RETURN Invoke(CLASS & aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4)
			{
				if (m_Function != nullptr)
				{
					return (aInstance.*m_Function)(aArg1, aArg2, aArg3, aArg4);
				}
				return RETURN();
			}
			RETURN Invoke(CLASS * aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4)
			{
				if (m_Function != nullptr)
				{
					return (aInstance->*m_Function)(aArg1, aArg2, aArg3, aArg4);
				}
				return RETURN();
			}
		private:
			Delegate m_Function;
		};


		template<class CLASS, class RETURN, class ARGUMENT1, class ARGUMENT2, class ARGUMENT3>
		class Method<CLASS, RETURN, ARGUMENT1, ARGUMENT2, ARGUMENT3>
		{
		public:
			typedef RETURN(CLASS::*Delegate)(ARGUMENT1, ARGUMENT2, ARGUMENT3);
			Method()
			{
				m_Function = nullptr;
			}
			Method(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~Method()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			RETURN operator()(CLASS & aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3)
			{
				if (m_Function != nullptr)
				{
					return (aInstance.*m_Function)(aArg1, aArg2, aArg3);
				}
				return RETURN();
			}
			RETURN operator()(CLASS * aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3)
			{
				if (m_Function != nullptr && aInstance != nullptr)
				{
					return (aInstance->*m_Function)(aArg1, aArg2, aArg3);
				}
				return RETURN();
			}
			RETURN Invoke(CLASS & aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3)
			{
				if (m_Function != nullptr)
				{
					return (aInstance.*m_Function)(aArg1, aArg2, aArg3);
				}
				return RETURN();
			}
			RETURN Invoke(CLASS * aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3)
			{
				if (m_Function != nullptr)
				{
					return (aInstance->*m_Function)(aArg1, aArg2, aArg3);
				}
				return RETURN();
			}
		private:
			Delegate m_Function;
		};


		template<class CLASS, class RETURN, class ARGUMENT1, class ARGUMENT2>
		class Method<CLASS, RETURN, ARGUMENT1, ARGUMENT2>
		{
		public:
			typedef RETURN(CLASS::*Delegate)(ARGUMENT1, ARGUMENT2);
			Method()
			{
				m_Function = nullptr;
			}
			Method(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~Method()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			RETURN operator()(CLASS & aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2)
			{
				if (m_Function != nullptr)
				{
					return (aInstance.*m_Function)(aArg1, aArg2);
				}
				return RETURN();
			}
			RETURN operator()(CLASS * aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2)
			{
				if (m_Function != nullptr && aInstance != nullptr)
				{
					return (aInstance->*m_Function)(aArg1, aArg2);
				}
				return RETURN();
			}
			RETURN Invoke(CLASS & aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2)
			{
				if (m_Function != nullptr)
				{
					return (aInstance.*m_Function)(aArg1, aArg2);
				}
				return RETURN();
			}
			RETURN Invoke(CLASS * aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2)
			{
				if (m_Function != nullptr)
				{
					return (aInstance->*m_Function)(aArg1, aArg2);
				}
				return RETURN();
			}
		private:
			Delegate m_Function;
		};

		template<class CLASS, class RETURN, class ARGUMENT1>
		class Method<CLASS, RETURN, ARGUMENT1>
		{
		public:
			typedef RETURN(CLASS::*Delegate)(ARGUMENT1);
			Method()
			{
				m_Function = nullptr;
			}
			Method(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~Method()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			RETURN operator()(CLASS & aInstance, ARGUMENT1 aArg1)
			{
				if (m_Function != nullptr)
				{
					return (aInstance.*m_Function)(aArg1);
				}
				return RETURN();
			}
			RETURN operator()(CLASS * aInstance, ARGUMENT1 aArg1)
			{
				if (m_Function != nullptr && aInstance != nullptr)
				{
					return (aInstance->*m_Function)(aArg1);
				}
				return RETURN();
			}
			RETURN Invoke(CLASS & aInstance, ARGUMENT1 aArg1)
			{
				if (m_Function != nullptr)
				{
					return (aInstance.*m_Function)(aArg1);
				}
				return RETURN();
			}
			RETURN Invoke(CLASS * aInstance, ARGUMENT1 aArg1)
			{
				if (m_Function != nullptr)
				{
					return (aInstance->*m_Function)(aArg1);
				}
				return RETURN();
			}
		private:
			Delegate m_Function;
		};

		template<class CLASS, class RETURN>
		class Method<CLASS, RETURN>
		{
		public:
			typedef RETURN(CLASS::*Delegate)(void);
			Method()
			{
				m_Function = nullptr;
			}
			Method(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~Method()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			RETURN operator()(CLASS & aInstance)
			{
				if (m_Function != nullptr)
				{
					return (aInstance.*m_Function)();
				}
				return RETURN();
			}
			RETURN operator()(CLASS * aInstance)
			{
				if (m_Function != nullptr && aInstance != nullptr)
				{
					return (aInstance->*m_Function)();
				}
				return RETURN();
			}
			RETURN Invoke(CLASS & aInstance)
			{
				if (m_Function != nullptr)
				{
					return (aInstance.*m_Function)();
				}
				return RETURN();
			}
			RETURN Invoke(CLASS * aInstance)
			{
				if (m_Function != nullptr)
				{
					return (aInstance->*m_Function)();
				}
				return RETURN();
			}
		private:
			Delegate m_Function;
		};

		template<class CLASS>
		class Method<CLASS, void>
		{
		public:
			typedef void(CLASS::*Delegate)(void);
			Method()
			{
				m_Function = nullptr;
			}
			Method(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~Method()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			void operator()(CLASS & aInstance)
			{
				if (m_Function != nullptr)
				{
					(aInstance.*m_Function)();
				}
			}
			void operator()(CLASS * aInstance)
			{
				if (m_Function != nullptr && aInstance != nullptr)
				{
					(aInstance->*m_Function)();
				}
			}
			void Invoke(CLASS & aInstance)
			{
				if (m_Function != nullptr)
				{
					(aInstance.*m_Function)();
				}
			}
			void Invoke(CLASS * aInstance)
			{
				if (m_Function != nullptr)
				{
					(aInstance->*m_Function)();
				}
			}
		private:
			Delegate m_Function;
		};
#pragma endregion

		template<class CLASS, class ARGUMENT1 = void, class ARGUMENT2 = void, class ARGUMENT3 = void,class ARGUMENT4 = void,class ARGUMENT5 = void>
		class MethodAction
		{
		public:
			typedef void(CLASS::*Delegate)(ARGUMENT1, ARGUMENT2, ARGUMENT3, ARGUMENT4, ARGUMENT5);
			MethodAction()
			{
				m_Function = nullptr;
			}
			MethodAction(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~MethodAction()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			void operator()(CLASS & aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5)
			{
				if (m_Function != nullptr)
				{
					(aInstance.*m_Function)(aArg1, aArg2, aArg3, aArg4,aArg5);
				}
			}
			void operator()(CLASS * aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5)
			{
				if (m_Function != nullptr && aInstance != nullptr)
				{
					(aInstance->*m_Function)(aArg1,aArg2,aArg3,aArg4,aArg5);
				}
			}
			void Invoke(CLASS & aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5)
			{
				if (m_Function != nullptr)
				{
					(aInstance.*m_Function)(aArg1, aArg2, aArg3, aArg4, aArg5);
				}
			}
			void Invoke(CLASS * aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5)
			{
				if (m_Function != nullptr)
				{
					(aInstance->*m_Function)(aArg1, aArg2, aArg3, aArg4, aArg5);
				}
			}
		private:
			Delegate m_Function;
		};

		template<class CLASS, class ARGUMENT1, class ARGUMENT2, class ARGUMENT3, class ARGUMENT4>
		class MethodAction <CLASS,ARGUMENT1,ARGUMENT2, ARGUMENT3,ARGUMENT4>
		{
		public:
			typedef void(CLASS::*Delegate)(ARGUMENT1, ARGUMENT2, ARGUMENT3, ARGUMENT4);
			MethodAction()
			{
				m_Function = nullptr;
			}
			MethodAction(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~MethodAction()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			void operator()(CLASS & aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4)
			{
				if (m_Function != nullptr)
				{
					(aInstance.*m_Function)(aArg1, aArg2, aArg3, aArg4);
				}
			}
			void operator()(CLASS * aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4)
			{
				if (m_Function != nullptr && aInstance != nullptr)
				{
					(aInstance->*m_Function)(aArg1, aArg2, aArg3, aArg4);
				}
			}
			void Invoke(CLASS & aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4)
			{
				if (m_Function != nullptr)
				{
					(aInstance.*m_Function)(aArg1, aArg2, aArg3, aArg4);
				}
			}
			void Invoke(CLASS * aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4)
			{
				if (m_Function != nullptr)
				{
					(aInstance->*m_Function)(aArg1, aArg2, aArg3, aArg4);
				}
			}
		private:
			Delegate m_Function;
		};

		template<class CLASS, class ARGUMENT1, class ARGUMENT2, class ARGUMENT3>
		class MethodAction<CLASS, ARGUMENT1, ARGUMENT2, ARGUMENT3>
		{
		public:
			typedef void(CLASS::*Delegate)(ARGUMENT1, ARGUMENT2, ARGUMENT3);
			MethodAction()
			{
				m_Function = nullptr;
			}
			MethodAction(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~MethodAction()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			void operator()(CLASS & aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3)
			{
				if (m_Function != nullptr)
				{
					(aInstance.*m_Function)(aArg1, aArg2, aArg3);
				}
			}
			void operator()(CLASS * aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3)
			{
				if (m_Function != nullptr && aInstance != nullptr)
				{
					(aInstance->*m_Function)(aArg1, aArg2, aArg3);
				}
			}
			void Invoke(CLASS & aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3)
			{
				if (m_Function != nullptr)
				{
					(aInstance.*m_Function)(aArg1, aArg2, aArg3);
				}
			}
			void Invoke(CLASS * aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3)
			{
				if (m_Function != nullptr)
				{
					(aInstance->*m_Function)(aArg1, aArg2, aArg3);
				}
			}
		private:
			Delegate m_Function;
		};


		template<class CLASS, class ARGUMENT1, class ARGUMENT2>
		class MethodAction<CLASS, ARGUMENT1, ARGUMENT2>
		{
		public:
			typedef void(CLASS::*Delegate)(ARGUMENT1, ARGUMENT2);
			MethodAction()
			{
				m_Function = nullptr;
			}
			MethodAction(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~MethodAction()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			void operator()(CLASS & aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2)
			{
				if (m_Function != nullptr)
				{
					(aInstance.*m_Function)(aArg1, aArg2);
				}
			}
			void operator()(CLASS * aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2)
			{
				if (m_Function != nullptr && aInstance != nullptr)
				{
					(aInstance->*m_Function)(aArg1, aArg2);
				}
			}
			void Invoke(CLASS & aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2)
			{
				if (m_Function != nullptr)
				{
					(aInstance.*m_Function)(aArg1, aArg2);
				}
			}
			void Invoke(CLASS * aInstance, ARGUMENT1 aArg1, ARGUMENT2 aArg2)
			{
				if (m_Function != nullptr)
				{
					(aInstance->*m_Function)(aArg1, aArg2);
				}
			}
		private:
			Delegate m_Function;
		};

		template<class CLASS, class ARGUMENT1>
		class MethodAction<CLASS, ARGUMENT1>
		{
		public:
			typedef void(CLASS::*Delegate)(ARGUMENT1);
			MethodAction()
			{
				m_Function = nullptr;
			}
			MethodAction(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~MethodAction()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			void operator()(CLASS & aInstance, ARGUMENT1 aArg1)
			{
				if (m_Function != nullptr)
				{
					(aInstance.*m_Function)(aArg1);
				}
			}
			void operator()(CLASS * aInstance, ARGUMENT1 aArg1)
			{
				if (m_Function != nullptr && aInstance != nullptr)
				{
					(aInstance->*m_Function)(aArg1);
				}
			}
			void Invoke(CLASS & aInstance, ARGUMENT1 aArg1 )
			{
				if (m_Function != nullptr)
				{
					(aInstance.*m_Function)(aArg1);
				}
			}
			void Invoke(CLASS * aInstance, ARGUMENT1 aArg1)
			{
				if (m_Function != nullptr)
				{
					(aInstance->*m_Function)(aArg1);
				}
			}
		private:
			Delegate m_Function;
		};

	}
}
#endif