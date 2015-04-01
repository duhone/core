#pragma once
#include<vector>
#include<functional>

namespace CR
{
	namespace Core
	{
		//!RAII class to automaticly execute code on scope exit
		/*! Example usage:

		*/
		class OnScopeExit
		{
		public:
			typedef std::function<void()> Function_t;
			OnScopeExit() {}
			OnScopeExit(Function_t& a_function) { m_onExit.push_back(a_function); }
			~OnScopeExit()
			{
				for (auto& function : m_onExit)
				{
					function();
				}
			}
			void Cancel() { m_onExit.clear(); }

		private:
			std::vector<Function_t> m_onExit;
		};
	}
}