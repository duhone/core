#pragma once
#include <mutex>
#include <functional>

namespace CR
{
	namespace Core
	{
		template<typename T>
		class Locked
		{
		public:			
			template<typename OperationType>
			auto operator()(OperationType a_operation) const -> typename std::result_of<OperationType(T&)>::type
			{
				std::lock_guard<std::mutex> lock(m_mutex);
				return a_operation(m_instance);
			}
			template<typename OperationType>
			auto operator()(OperationType a_operation) -> typename std::result_of<OperationType(T&)>::type
			{
				std::lock_guard<std::mutex> lock(m_mutex);
				return a_operation(m_instance);
			}
		private:
			T m_instance;
			mutable std::mutex m_mutex;
		};
	}
}