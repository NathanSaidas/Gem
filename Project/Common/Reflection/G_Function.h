#ifndef G_FUNCTION
#define G_FUNCTION

#pragma region CHANGE LOG
/* December, 20, 2014 - Nathan Hanlan - Defining the Func and Action template functors 
*  
*/
#pragma endregion


namespace Gem
{
	namespace Reflection
	{
		///Non member functions
#pragma region FUNC
		///Base Func Definition
		template<class RETURN, class ARGUMENT1 = void, class ARGUMENT2 = void, class ARGUMENT3 = void, class ARGUMENT4 = void, class ARGUMENT5 = void, class ARGUMENT6 = void, class ARGUMENT7 = void, class ARGUMENT8 = void>
		class Func
		{
		public:
			typedef RETURN(*Delegate)(ARGUMENT1, ARGUMENT2, ARGUMENT3, ARGUMENT4, ARGUMENT5, ARGUMENT6, ARGUMENT7, ARGUMENT8);

			Func()
			{
				m_Function = nullptr;
			}
			Func(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~Func()
			{

			}

			
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			RETURN operator()(ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5, ARGUMENT6 aArg6, ARGUMENT7 aArg7, ARGUMENT8 aArg8)
			{
				if (m_Function != nullptr)
				{
					return m_Function(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7, aArg8);
				}
				return RETURN();
			}
			RETURN Invoke(ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5, ARGUMENT6 aArg6, ARGUMENT7 aArg7, ARGUMENT8 aArg8)
			{
				if (m_Function != nullptr)
				{
					return m_Function(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7, aArg8);
				}
				return RETURN();
			}
		private:
			Delegate m_Function;
		};

		template<class RETURN, class ARGUMENT1, class ARGUMENT2, class ARGUMENT3, class ARGUMENT4, class ARGUMENT5, class ARGUMENT6, class ARGUMENT7>
		class Func<RETURN, ARGUMENT1, ARGUMENT2, ARGUMENT3, ARGUMENT4, ARGUMENT5, ARGUMENT6, ARGUMENT7>
		{
		public:
			typedef RETURN(*Delegate)(ARGUMENT1, ARGUMENT2, ARGUMENT3, ARGUMENT4, ARGUMENT5, ARGUMENT6, ARGUMENT7);
			Func()
			{
				m_Function = nullptr;
			}
			Func(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~Func()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			RETURN operator()(ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5, ARGUMENT6 aArg6, ARGUMENT7 aArg7)
			{
				if (m_Function != nullptr)
				{
					return m_Function(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7);
				}
				return RETURN();
			}
			RETURN Invoke(ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5, ARGUMENT6 aArg6, ARGUMENT7 aArg7)
			{
				if (m_Function != nullptr)
				{
					return m_Function(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6, aArg7);
				}
				return RETURN();
			}
		private:
			Delegate m_Function;
		};

		template<class RETURN, class ARGUMENT1, class ARGUMENT2, class ARGUMENT3, class ARGUMENT4, class ARGUMENT5, class ARGUMENT6>
		class Func<RETURN, ARGUMENT1, ARGUMENT2, ARGUMENT3, ARGUMENT4, ARGUMENT5, ARGUMENT6>
		{
		public:
			typedef RETURN(*Delegate)(ARGUMENT1, ARGUMENT2, ARGUMENT3, ARGUMENT4, ARGUMENT5, ARGUMENT6);
			Func()
			{
				m_Function = nullptr;
			}
			Func(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~Func()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			RETURN operator()(ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5, ARGUMENT6 aArg6)
			{
				if (m_Function != nullptr)
				{
					return m_Function(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6);
				}
				return RETURN();
			}
			RETURN Invoke(ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5, ARGUMENT6 aArg6)
			{
				if (m_Function != nullptr)
				{
					return m_Function(aArg1, aArg2, aArg3, aArg4, aArg5, aArg6);
				}
				return RETURN();
			}
		private:
			Delegate m_Function;
		};

		template<class RETURN, class ARGUMENT1, class ARGUMENT2, class ARGUMENT3, class ARGUMENT4, class ARGUMENT5>
		class Func<RETURN, ARGUMENT1, ARGUMENT2, ARGUMENT3, ARGUMENT4, ARGUMENT5>
		{
		public:
			typedef RETURN(*Delegate)(ARGUMENT1, ARGUMENT2, ARGUMENT3, ARGUMENT4, ARGUMENT5);
			Func()
			{
				m_Function = nullptr;
			}
			Func(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~Func()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			RETURN operator()(ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5)
			{
				if (m_Function != nullptr)
				{
					return m_Function(aArg1, aArg2, aArg3, aArg4, aArg5);
				}
				return RETURN();
			}
			RETURN Invoke(ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5)
			{
				if (m_Function != nullptr)
				{
					return m_Function(aArg1, aArg2, aArg3, aArg4, aArg5);
				}
				return RETURN();
			}
		private:
			Delegate m_Function;
		};

		template<class RETURN, class ARGUMENT1, class ARGUMENT2, class ARGUMENT3, class ARGUMENT4>
		class Func<RETURN, ARGUMENT1, ARGUMENT2, ARGUMENT3, ARGUMENT4>
		{
		public:
			typedef RETURN(*Delegate)(ARGUMENT1, ARGUMENT2, ARGUMENT3, ARGUMENT4);
			Func()
			{
				m_Function = nullptr;
			}
			Func(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~Func()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			RETURN operator()(ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4)
			{
				if (m_Function != nullptr)
				{
					return m_Function(aArg1, aArg2, aArg3, aArg4);
				}
				return RETURN();
			}
			RETURN Invoke(ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4)
			{
				if (m_Function != nullptr)
				{
					return m_Function(aArg1, aArg2, aArg3, aArg4);
				}
				return RETURN();
			}
		private:
			Delegate m_Function;
		};

		template<class RETURN, class ARGUMENT1, class ARGUMENT2, class ARGUMENT3>
		class Func<RETURN, ARGUMENT1, ARGUMENT2, ARGUMENT3>
		{
		public:
			typedef RETURN(*Delegate)(ARGUMENT1, ARGUMENT2, ARGUMENT3);
			Func()
			{
				m_Function = nullptr;
			}
			Func(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~Func()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			RETURN operator()(ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3)
			{
				if (m_Function != nullptr)
				{
					return m_Function(aArg1, aArg2, aArg3);
				}
				return RETURN();
			}
			RETURN Invoke(ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3)
			{
				if (m_Function != nullptr)
				{
					return m_Function(aArg1, aArg2, aArg3);
				}
				return RETURN();
			}
		private:
			Delegate m_Function;
		};

		template<class RETURN, class ARGUMENT1, class ARGUMENT2>
		class Func<RETURN, ARGUMENT1, ARGUMENT2>
		{
		public:
			typedef RETURN(*Delegate)(ARGUMENT1, ARGUMENT2);
			Func()
			{
				m_Function = nullptr;
			}
			Func(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~Func()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			RETURN operator()(ARGUMENT1 aArg1, ARGUMENT2 aArg2)
			{
				if (m_Function != nullptr)
				{
					return m_Function(aArg1,aArg2);
				}
				return RETURN();
			}
			RETURN Invoke(ARGUMENT1 aArg1, ARGUMENT2 aArg2)
			{
				if (m_Function != nullptr)
				{
					return m_Function(aArg1, aArg2);
				}
				return RETURN();
			}
		private:
			Delegate m_Function;
		};

		template<class RETURN, class ARGUMENT1>
		class Func<RETURN, ARGUMENT1>
		{
		public:
			typedef RETURN(*Delegate)(ARGUMENT1);
			Func()
			{
				m_Function = nullptr;
			}
			Func(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~Func()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			RETURN operator()(ARGUMENT1 aArg)
			{
				if (m_Function != nullptr)
				{
					return m_Function(aArg);
				}
				return RETURN();
			}
			RETURN Invoke(ARGUMENT1 aArg)
			{
				if (m_Function != nullptr)
				{
					return m_Function(aArg);
				}
				return RETURN();
			}
		private:
			Delegate m_Function;
		};
		
		template<class RETURN>
		class Func<RETURN>
		{
		public:
			typedef RETURN(*Delegate)(void);
			Func()
			{
				m_Function = nullptr;
			}
			Func(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~Func()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			RETURN operator()(void)
			{
				if (m_Function != nullptr)
				{
					return m_Function();
				}
				return RETURN();
			}
			RETURN Invoke()
			{
				if (m_Function != nullptr)
				{
					return m_Function();
				}
				return RETURN();
			}
		private:
			Delegate m_Function;
		};

		template<>
		class Func<void>
		{
		public:
			typedef void(*Delegate)(void);
			Func()
			{
				m_Function = nullptr;
			}
			Func(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~Func()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			void operator()(void)
			{
				if (m_Function != nullptr)
				{
					m_Function();
				}
			}
			void Invoke()
			{
				if (m_Function != nullptr)
				{
					m_Function();
				}
			}
		private:
			Delegate m_Function;
		};
#pragma endregion
#pragma region ACTION
		///Base Action Def
		template<class ARGUMENT1 = void ,class ARGUMENT2 = void,class ARGUMENT3 = void, class ARGUMENT4 = void, class ARGUMENT5 = void>
		class Action
		{
		public:
			typedef void(*Delegate)(ARGUMENT1, ARGUMENT2, ARGUMENT3, ARGUMENT4, ARGUMENT5);
			Action()
			{
				m_Function = nullptr;
			}
			Action(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~Action()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			void operator()(ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5)
			{
				if (m_Function != nullptr)
				{
					m_Function(aArg1,aArg2,aArg3,aArg4,aArg5);
				}
			}
			void Invoke(ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4, ARGUMENT5 aArg5)
			{
				if (m_Function != nullptr)
				{
					m_Function(aArg1, aArg2, aArg3, aArg4, aArg5);
				}
			}
		private:
			Delegate m_Function;
		};

		template<class ARGUMENT1, class ARGUMENT2, class ARGUMENT3, class ARGUMENT4>
		class Action <ARGUMENT1, ARGUMENT2,ARGUMENT3,ARGUMENT4>
		{
		public:
			typedef void(*Delegate)(ARGUMENT1, ARGUMENT2, ARGUMENT3, ARGUMENT4);
			Action()
			{
				m_Function = nullptr;
			}
			Action(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~Action()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			void operator()(ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4)
			{
				if (m_Function != nullptr)
				{
					m_Function(aArg1, aArg2, aArg3, aArg4);
				}
			}
			void Invoke(ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3, ARGUMENT4 aArg4)
			{
				if (m_Function != nullptr)
				{
					m_Function(aArg1, aArg2, aArg3, aArg4);
				}
			}
		private:
			Delegate m_Function;
		};

		template<class ARGUMENT1, class ARGUMENT2, class ARGUMENT3>
		class Action <ARGUMENT1, ARGUMENT2, ARGUMENT3>
		{
		public:
			typedef void(*Delegate)(ARGUMENT1, ARGUMENT2, ARGUMENT3);
			Action()
			{
				m_Function = nullptr;
			}
			Action(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~Action()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			void operator()(ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3)
			{
				if (m_Function != nullptr)
				{
					m_Function(aArg1, aArg2, aArg3);
				}
			}
			void Invoke(ARGUMENT1 aArg1, ARGUMENT2 aArg2, ARGUMENT3 aArg3)
			{
				if (m_Function != nullptr)
				{
					m_Function(aArg1, aArg2, aArg3);
				}
			}
		private:
			Delegate m_Function;
		};

		template<class ARGUMENT1, class ARGUMENT2>
		class Action <ARGUMENT1, ARGUMENT2>
		{
		public:
			typedef void(*Delegate)(ARGUMENT1, ARGUMENT2);
			Action()
			{
				m_Function = nullptr;
			}
			Action(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~Action()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			void operator()(ARGUMENT1 aArg1, ARGUMENT2 aArg2)
			{
				if (m_Function != nullptr)
				{
					m_Function(aArg1, aArg2);
				}
			}
			void Invoke(ARGUMENT1 aArg1, ARGUMENT2 aArg2)
			{
				if (m_Function != nullptr)
				{
					m_Function(aArg1, aArg2);
				}
			}
		private:
			Delegate m_Function;
		};

		template<class ARGUMENT1>
		class Action <ARGUMENT1>
		{
		public:
			typedef void(*Delegate)(ARGUMENT1);
			Action()
			{
				m_Function = nullptr;
			}
			Action(Delegate aFunction)
			{
				m_Function = aFunction;
			}
			~Action()
			{

			}
			Delegate operator=(Delegate aFunction)
			{
				return m_Function = aFunction;
			}
			void operator()(ARGUMENT1 aArg1)
			{
				if (m_Function != nullptr)
				{
					m_Function(aArg1);
				}
			}
			void Invoke(ARGUMENT1 aArg1)
			{
				if (m_Function != nullptr)
				{
					m_Function(aArg1);
				}
			}
		private:
			Delegate m_Function;
		};

#pragma endregion
		///Empty Func Def
		typedef Func<void, void, void, void, void, void, void, void, void> EmptyFunc;

	}

}

#endif