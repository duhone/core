#pragma once
#include <mutex>
#include <functional>
#include <tuple>
#include "Concepts.h"
#include "Tuple.h"
#include "DefaultOperators.h"

namespace CR
{
	namespace Core
	{
		template<SemiRegular... T>
		class Locked : public DefaultOperatorsTotallyOrdered<Locked<T...>>
		{
		public:
			Locked() = default;
			~Locked() = default;
			Locked(const Locked& other) 
			{
				std::unique_lock<std::mutex> lockOther(other.m_mutex);
				m_instances(other.m_instances);
			}
			Locked& operator=(const Locked& other)
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				std::unique_lock<std::mutex> lockOther(other.m_mutex);
				m_instances = other.m_instances;
			}
			Locked(Locked&& other)
			{
				std::unique_lock<std::mutex> lockOther(other.m_mutex);
				m_instances = std::move(other.m_instances);
			}
			Locked& operator=(Locked&& other)
			{
				std::unique_lock<std::mutex> lock1(m_mutex, std::defer_lock);
				std::unique_lock<std::mutex> lock2(other.m_mutex, std::defer_lock);
				std::lock(lock1, lock2);
				m_instances = std::move(other.m_instances);
			}

			Locked(const T&... args)
			{
				m_instances = std::make_tuple(args...);
			}
			Locked(T&&... args)
			{
				m_instances = std::make_tuple(args...);
			}
			Locked& operator=(const std::tuple<T...>& arg)
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				m_instances = arg;
			}
			Locked& operator=(std::tuple<T...>&& arg)
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				m_instances = std::move(arg);
			}

			friend bool operator==(const Locked& a_arg1, const Locked& a_arg2)
			{
				std::unique_lock<std::mutex> lock1(a_arg1.m_mutex, std::defer_lock);
				std::unique_lock<std::mutex> lock2(a_arg2.m_mutex, std::defer_lock);
				std::lock(lock1, lock2);
				return a_arg1.m_instances == a_arg2.m_instances;
			}

			friend bool operator<(const Locked& a_arg1, const Locked& a_arg2)
			{
				std::unique_lock<std::mutex> lock1(a_arg1.m_mutex, std::defer_lock);
				std::unique_lock<std::mutex> lock2(a_arg2.m_mutex, std::defer_lock);
				std::lock(lock1, lock2);
				return a_arg1.m_instances < a_arg2.m_instances;
			}

			template<Callable OperationType>
			auto operator()(OperationType a_operation) const
			{
				std::unique_lock<std::mutex> lock(m_mutex);
				//std::shared_lock<std::shared_timed_mutex> lock(m_mutex); //c++14
				return std17::apply(a_operation, m_instances);
			}
			template<Callable OperationType>
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