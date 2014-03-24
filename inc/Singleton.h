#pragma once
#include <mutex>
#include <atomic>
#include <memory>

namespace CR
{
	namespace Core
	{		
		//! Singleton base template class.
		/*!
			Derive from this class to create a Singleton class. Your derived class must be a friend of 
			this template. Make sure the constructor is private in your dervived
			class to insure no one can create an instance of it, and other auto generated functions too.
			You must have a no argument	constructor. No other constructors will be called.
			This template also works using CRTP style singletons too, i.e. class OnlyOne : public Singleton<OnlyOne>

			This template is thread safe.

			Example:
			\verbatim
			class OnlyOne
			{
			public:
				friend Singleton<OnlyOne>;

				void IncCount() { ++m_count; }
				int GetCount() const { return m_count; }
			private:
				OnlyOne() {}
				OnlyOne(const OnlyOne&) = delete;
				OnlyOne(OnlyOne&&) = delete;
				OnlyOne& operator=(const OnlyOne&) = delete;

				int m_count{ 0 };
			};

			typedef Singleton<OnlyOne> MySingleton;

			MySingleton::Instance().Test();
			\endverbatim
		*/
		template<class T>
		class Singleton
		{			
		public:
			//! Get the one and only instance
			/*!
				Returns the only instance of this Singleton class. Function is thread safe.
				Always returns the same pointer.
			*/
			static T& Instance()
			{
				//not thread safe in VS 2013, but should be. is fine in gcc and clang. need
				//	to use meyers singleton to work with auto registration, so hopefully
				//	microsoft fixes this eventually. call_once, and double check lock wont
				//	work with auto registration because of unknown init time of member statics
				static T instance;
				return instance;
			}	
		private:
			~Singleton(void) = default;
			Singleton(void) = default;
			Singleton<T>(const Singleton<T>&) = delete;
			Singleton<T>& operator=(const Singleton<T>&) = delete;
		};
	}
}