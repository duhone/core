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
				std::call_once(m_onceFlag, [](){
					//m_instance = std::make_unique<T>(); //would require making make_unique a friend of T too
					m_instance = std::unique_ptr<T>(new T());
				});
				return *m_instance;
			}					
		private:
			Singleton(void) = delete;
			~Singleton(void) = delete;
			Singleton<T>(const Singleton<T>&) = delete;
			Singleton<T>& operator=(const Singleton<T>&) = delete;

			static std::unique_ptr<T> m_instance;
			static std::once_flag m_onceFlag;
		};

		template<class T>
		std::unique_ptr<T> Singleton<T>::m_instance;

		template<class T>
		std::once_flag Singleton<T>::m_onceFlag;
	}
}