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
			auto operator()(OperationType a_operation) const
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				//std::shared_lock<std::shared_timed_mutex> lock(m_mutex); //c++14
				return a_operation(m_instance);
			}
			template<typename OperationType>
			auto operator()(OperationType a_operation)
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				return a_operation(m_instance);
			}
		private:
			T m_instance;
			mutable std::mutex m_mutex;
			//mutable std::shared_timed_mutex m_mutex; //c++14
		};
	}
}