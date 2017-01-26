#pragma once
#include <mutex>
#include <shared_mutex>
#include <functional>
#include <tuple>
#include <optional>
#include "core/Concepts.h"
#include "core/DefaultOperators.h"
#include "core/ScopeExit.h"

namespace CR::Core {
	template<SemiRegular... T>
	class MultiLock;

	template<SemiRegular... T>
	class Locked : public DefaultOperatorsTotallyOrdered<Locked<T...>> {
		template<SemiRegular... T>
		friend class MultiLock;
	public:
		Locked() = default;
		~Locked() = default;
		Locked(const Locked& other) {
			std::shared_lock<std::shared_timed_mutex> lockOther(other.m_mutex);
			m_instances(other.m_instances);
		}
		Locked& operator=(const Locked& other) {
			std::unique_lock<std::shared_timed_mutex> lock(m_mutex);
			std::shared_lock<std::shared_timed_mutex> lockOther(other.m_mutex);
			m_instances = other.m_instances;
		}
		Locked(Locked&& other) {
			std::unique_lock<std::shared_timed_mutex> lockOther(other.m_mutex);
			m_instances = std::move(other.m_instances);
		}
		Locked& operator=(Locked&& other) {
			std::unique_lock<std::shared_timed_mutex> lock1(m_mutex, std::defer_lock);
			std::unique_lock<std::shared_timed_mutex> lock2(other.m_mutex, std::defer_lock);
			std::lock(lock1, lock2);
			m_instances = std::move(other.m_instances);
		}

		Locked(const T&... args) {
			m_instances = std::make_tuple(args...);
		}
		Locked(T&&... args) {
			m_instances = std::make_tuple(args...);
		}
		Locked& operator=(const std::tuple<T...>& arg) {
			std::unique_lock<std::shared_timed_mutex> lock(m_mutex);
			m_instances = arg;
		}
		Locked& operator=(std::tuple<T...>&& arg) {
			std::unique_lock<std::shared_timed_mutex> lock(m_mutex);
			m_instances = std::move(arg);
		}

		friend bool operator==(const Locked& a_arg1, const Locked& a_arg2) {
			std::shared_lock<std::shared_timed_mutex> lock1(a_arg1.m_mutex, std::defer_lock);
			std::shared_lock<std::shared_timed_mutex> lock2(a_arg2.m_mutex, std::defer_lock);
			std::lock(lock1, lock2);
			return a_arg1.m_instances == a_arg2.m_instances;
		}

		friend bool operator<(const Locked& a_arg1, const Locked& a_arg2) {
			std::shared_lock<std::shared_timed_mutex> lock1(a_arg1.m_mutex, std::defer_lock);
			std::shared_lock<std::shared_timed_mutex> lock2(a_arg2.m_mutex, std::defer_lock);
			std::lock(lock1, lock2);
			return a_arg1.m_instances < a_arg2.m_instances;
		}

		template<Callable OperationType>
		auto operator()(OperationType a_operation) const {
			std::shared_lock<std::shared_timed_mutex> lock(m_mutex);
			return std::apply(a_operation, m_instances);
		}

		template<Callable OperationType>
		auto operator()(OperationType a_operation) {
			std::unique_lock<std::shared_timed_mutex> lock(m_mutex);
			return std::apply(a_operation, m_instances);
		}

		template<Callable OperationType>
		auto Try(OperationType a_operation) const -> std::enable_if_t<!std::is_void<std::result_of_t<OperationType(T&...)>>::value,
			std::optional<std::result_of_t<OperationType(T&...)>>> {
			if(!m_mutex.try_lock())
				return std::none;
			std::shared_lock<std::shared_timed_mutex> lock(m_mutex, std::adopt_lock);
			return std::apply(a_operation, m_instances);
		}

		template<Callable OperationType>
		auto Try(OperationType a_operation) -> std::enable_if_t<!std::is_void<std::result_of_t<OperationType(T&...)>>::value,
			std::optional<std::result_of_t<OperationType(T&...)>>> {
			if(!m_mutex.try_lock())
				return std::none;
			std::unique_lock<std::shared_timed_mutex> lock(m_mutex, std::adopt_lock);
			return std::apply(a_operation, m_instances);
		}

		template<Callable OperationType>
		auto Try(OperationType a_operation) const -> std::enable_if_t<std::is_void<std::result_of_t<OperationType(T&...)>>::value, bool> {
			if(!m_mutex.try_lock())
				return false;
			std::shared_lock<std::shared_timed_mutex> lock(m_mutex, std::adopt_lock);
			std::apply(a_operation, m_instances);
			return true
		}

		template<Callable OperationType>
		auto Try(OperationType a_operation) -> std::enable_if_t<std::is_void<std::result_of_t<OperationType(T&...)>>::value, bool> {
			if(!m_mutex.try_lock())
				return false;
			std::unique_lock<std::shared_timed_mutex> lock(m_mutex, std::adopt_lock);
			std::apply(a_operation, m_instances);
			return true;
		}
	private:
		std::tuple<T...> m_instances;
		mutable std::shared_timed_mutex m_mutex; //c++14
	};

	template<SemiRegular... T>
	class MultiLock {
	public:
		MultiLock() = delete; //doesn't make sense to create one of these with no child locks
		MultiLock(Locked<T>&... a_arg) : m_locks(a_arg...) {}
		~MultiLock() = default;
		MultiLock(MultiLock&) = delete;
		MultiLock& operator=(MultiLock&) = delete;
		MultiLock(MultiLock&&) = default;
		MultiLock& operator=(MultiLock&&) = default;

		template<Callable OperationType>
		auto operator()(OperationType a_operation) const {
			std17::apply(AcquireLock, m_locks);
			auto release = std17::make_scope_exit([this]() { std17::apply(ReleaseLocks, m_locks); });
			auto data = std17::apply(BuildTuple, m_locks);
			return std17::apply(a_operation, data);
		}

		template<Callable OperationType>
		auto operator()(OperationType a_operation) {
			std::apply(AcquireLock, m_locks);
			auto release = std17::make_scope_exit([this]() { std::apply(ReleaseLocks, m_locks); });
			auto data = std::apply(BuildTuple, m_locks);
			return std::apply(a_operation, data);
		}

		template<Callable OperationType>
		auto Try(OperationType a_operation) const -> std::enable_if_t<!std::is_void<std::result_of_t<OperationType(T&...)>>::value,
			std::optional<std::result_of_t<OperationType(T&...)>>> {
			if(!std::apply(TryAcquireLock, m_locks))
				return std::none;
			auto release = std17::make_scope_exit([this]() { std17::apply(ReleaseLocks, m_locks); });
			auto data = std::apply(BuildTuple, m_locks);
			return std::apply(a_operation, data);
		}

		template<Callable OperationType>
		auto Try(OperationType a_operation) -> std::enable_if_t<!std::is_void<std::result_of_t<OperationType(T&...)>>::value,
			std::optional<std::result_of_t<OperationType(T&...)>>> {
			if(!std::apply(TryAcquireLock, m_locks))
				return std::none;
			auto release = std17::make_scope_exit([this]() { std17::apply(ReleaseLocks, m_locks); });
			auto data = std::apply(BuildTuple, m_locks);
			return std::apply(a_operation, data);
		}

		template<Callable OperationType>
		auto Try(OperationType a_operation) const -> std::enable_if_t<std::is_void<std::result_of_t<OperationType(T&...)>>::value, bool> {
			if(!std17::apply(TryAcquireLock, m_locks))
				return false;
			auto release = std17::make_scope_exit([this]() { std17::apply(ReleaseLocks, m_locks); });
			auto data = std17::apply(BuildTuple, m_locks);
			std17::apply(a_operation, data);
			return true;
		}

		template<Callable OperationType>
		auto Try(OperationType a_operation) -> std::enable_if_t<std::is_void<std::result_of_t<OperationType(T&...)>>::value, bool> {
			if(!std17::apply(TryAcquireLock, m_locks))
				return false;
			auto release = std17::make_scope_exit([this]() { std17::apply(ReleaseLocks, m_locks); });
			auto data = std17::apply(BuildTuple, m_locks);
			std17::apply(a_operation, data);
			return true;
		}
	private:
		static void AcquireLock(Locked<T>&... a_locks) {
			std::lock(a_locks.m_mutex...);
		}

		static bool TryAcquireLock(Locked<T>&... a_locks) {
			return std::try_lock(a_locks.m_mutex...) != -1;
		}

		template<SemiRegular FirstT>
		static void ReleaseMutex(FirstT& a_first) {
			a_first.unlock();
		}

		template<SemiRegular FirstT, SemiRegular... RestT>
		static void ReleaseMutex(FirstT& a_first, RestT&... a_rest) {
			a_first.unlock();
			ReleaseMutex(a_rest...);
		}

		static void ReleaseLocks(Locked<T>&... a_locks) {
			ReleaseMutex(a_locks.m_mutex...);
		}

		template<SemiRegular FirstType>
		static void ReleaseLock(Locked<FirstType>& a_first) {
			a_first.m_mutex.unlock();
		}

		template<SemiRegular FirstType, SemiRegular... RestType>
		static void ReleaseLock(Locked<FirstType>& a_first, Locked<RestType>&... a_rest) {
			a_first.m_mutex.unlock();
			ReleaseLock(a_rest);
		}

		static auto BuildTuple(Locked<T>&... a_locks) {
			return std::tuple_cat(a_locks.m_instances...);
		}

		std::tuple<Locked<T>&...> m_locks;
	};

	template<SemiRegular... T>
	MultiLock<T...> MakeMultiLock(Locked<T>&... a_arg) {
		return MultiLock<T...>{ a_arg... };
	}
}