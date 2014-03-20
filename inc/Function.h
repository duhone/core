/*
similar to std::function
both hold a list of functions. MultiFunction will call every assigned function when invoked. 
SelectableFunction will invoke a single one of its functions which can be independently selected.
 */
#pragma once

#include <vector>
#include <algorithm>
#include <functional>
#include "Algorithm.h"

namespace CR
{
	namespace Core
	{
		template<typename ReturnType, typename... ArgTypes>
		class MultiFunction
		{
		public:
			typedef std::function<ReturnType(ArgTypes...)> OperationType;

			std::size_t Size() const
			{
				return m_operations.size();
			}
			void operator=(std::nullptr_t)
			{
				m_operations.clear();
			}
			void operator=(OperationType& operation)
			{
				*this = nullptr;
				*this += operation;
			}
			void operator+=(OperationType& operation)
			{
				m_delegates.push_back(operation);
			}
			void operator(ArgTypes... params)()
			{
				ForEach(m_operations, [](OperationType& operation){ operation(params...); });
			}
			explicit operator bool() const
			{
				return AllOf(m_operations, [](OperationType& op) -> bool {return op});
			}
		private:
			std::vector<OperationType> m_operations;
		};

		template<typename ReturnType, typename... ArgTypes>
		class SelectableFunction
		{
		public:
			typedef std::function<ReturnType(ArgTypes...)> OperationType;

			std::size_t Size() const
			{
				return m_operations.size();
			}
			void operator=(std::nullptr_t)
			{
				m_operations.clear();
			}
			void operator+=(OperationType& operation)
			{
				m_delegates.push_back(operation);
			}
			void operator(ArgTypes... params)()
			{
				m_operations.at(m_currentOperation)(std::forward<params>...);
			}
			explicit operator bool() const
			{
				return m_operations.at(m_currentOperation);
			}
			void SetOperation(std::size_t op) { m_currentOperation = op; }

		private:
			std::vector<OperationType> m_operations;
			std::size_t m_currentOperation{ 0 };
		};
	}
}
