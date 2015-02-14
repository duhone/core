#pragma once
#include <mutex>
#include <functional>
#include <tuple>
#include "Tuple.h"

namespace CR
{
	namespace Core
	{
		template<typename... T>
		class Locked
		{
		public:
			template<typename OperationType>
			auto operator()(OperationType a_operation) const
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				//std::shared_lock<std::shared_timed_mutex> lock(m_mutex); //c++14
				return std17::apply(a_operation, m_instances);
			}
			template<typename OperationType>
			auto operator()(OperationType a_operation)
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				return std17::apply(a_operation, m_instances);
			}
		private:
			std::tuple<T...> m_instances;
			mutable std::mutex m_mutex;
			//mutable std::shared_timed_mutex m_mutex; //c++14
		};
	}
}