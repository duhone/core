#pragma once
#include <mutex>
//#include <shared_mutex>
#include <functional>
#include <tuple>
#include "Concepts.h"
#include "Tuple.h"
#include "DefaultOperators.h"
#include "OnScopeExit.h"

namespace CR
{
	namespace Core
	{
		template<SemiRegular... T>
		class MultiLock;

		template<SemiRegular... T>
		class Locked : public DefaultOperatorsTotallyOrdered<Locked<T...>>
		{
			template<SemiRegular... T>
			friend class MultiLock;
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
		
		template<SemiRegular... T>
		class MultiLock
		{
		public:
			MultiLock() = delete; //doesn't make sense to create one of these with no child locks
			MultiLock(Locked<T>&... a_arg) : m_locks(a_arg...) {}
			~MultiLock() = default;
			MultiLock(MultiLock&) = delete;
			MultiLock& operator=(MultiLock&) = delete;
			MultiLock(MultiLock&&) = default;
			MultiLock& operator=(MultiLock&&) = default;

			template<Callable OperationType>
			void operator()(OperationType a_operation) const
			{
				std17::apply(AcquireLock, m_locks);
				auto release = std17::make_scope_exit([this]() { std17::apply(ReleaseLocks, m_locks); });
				auto data = std17::apply(BuildTuple, m_locks);
				return std17::apply(a_operation, data);
			}

			template<Callable OperationType>
			void operator()(OperationType a_operation)
			{
				std17::apply(AcquireLock, m_locks);
				auto release = std17::make_scope_exit([this]() { std17::apply(ReleaseLocks, m_locks); });
				auto data = std17::apply(BuildTuple, m_locks);
				return std17::apply(a_operation, data);
			}
		private:
			static void AcquireLock(Locked<T>&... a_locks)
			{
				std::lock(a_locks.m_mutex...);
			}

			template<SemiRegular FirstT>
			static void ReleaseMutex(FirstT& a_first)
			{
				a_first.unlock();
			}

			template<SemiRegular FirstT, SemiRegular... RestT>
			static void ReleaseMutex(FirstT& a_first, RestT&... a_rest)
			{
				a_first.unlock();
				ReleaseMutex(a_rest...);
			}

			static void ReleaseLocks(Locked<T>&... a_locks)
			{
				ReleaseMutex(a_locks.m_mutex...);
			}

			template<SemiRegular FirstType>
			static void ReleaseLock(Locked<FirstType>& a_first)
			{
				a_first.m_mutex.unlock();
			}

			template<SemiRegular FirstType, SemiRegular... RestType>
			static void ReleaseLock(Locked<FirstType>& a_first, Locked<RestType>&... a_rest)
			{
				a_first.m_mutex.unlock();
				ReleaseLock(a_rest);
			}
			
			static auto BuildTuple(Locked<T>&... a_locks)
			{
				return std::tuple_cat(a_locks.m_instances...);
			}

			std::tuple<Locked<T>&...> m_locks;
		};

		template<SemiRegular... T>
		MultiLock<T...> MakeMultiLock(Locked<T>&... a_arg)
		{
			return MultiLock<T...>{ a_arg... };
		}
	}
}