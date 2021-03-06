﻿#pragma once

#include <3rdParty/fmt.h>
#include <3rdParty/spdlog.h>

#include <cassert>
#include <memory>
#include <thread>

namespace CR::Core::Log {
	template<typename... ArgTs>
	void Debug([[maybe_unused]] const char* a_fmt, [[maybe_unused]] ArgTs&&... a_args) {
		if constexpr(CR_DEBUG) { detail::GetLogger().Debug(a_fmt, std::forward<ArgTs>(a_args)...); }
	}

	template<typename... ArgTs>
	void Info([[maybe_unused]] const char* a_fmt, [[maybe_unused]] ArgTs&&... a_args) {
		if constexpr(CR_DEBUG || CR_RELEASE) { detail::GetLogger().Info(a_fmt, std::forward<ArgTs>(a_args)...); }
	}

	template<typename... ArgTs>
	void Warn(const char* a_fmt, ArgTs&&... a_args) {
		if constexpr(CR_DEBUG || CR_RELEASE) {
			detail::GetLogger().Error(a_fmt, std::forward<ArgTs>(a_args)...);
		} else {
			detail::GetLogger().Warn(a_fmt, std::forward<ArgTs>(a_args)...);
		}
	}

	template<typename... ArgTs>
	void Error(const char* a_fmt, ArgTs&&... a_args) {
		detail::GetLogger().Error(a_fmt, std::forward<ArgTs>(a_args)...);
	}

	template<typename... ArgTs>
	void Assert(bool condition, [[maybe_unused]] const char* a_fmt, [[maybe_unused]] ArgTs&&... a_args) {
		if constexpr(CR_DEBUG || CR_RELEASE) {
			if(!condition) { detail::GetLogger().Error(a_fmt, std::forward<ArgTs>(a_args)...); }
		} else {
			__assume(!condition);
		}
	}

	template<typename... ArgTs>
	void Require(bool condition, const char* a_fmt, ArgTs&&... a_args) {
		if(!condition) { detail::GetLogger().Error(a_fmt, std::forward<ArgTs>(a_args)...); }
	}

	namespace detail {
		class Logger final {
		  public:
			Logger();
			~Logger();
			Logger(const Logger&) = delete;
			Logger(Logger&&)      = delete;
			Logger& operator=(const Logger&) = delete;
			Logger& operator=(Logger&&) = delete;

			template<typename... ArgTs>
			void Debug(const char* a_fmt, ArgTs&&... a_args) {
				m_logger->debug(a_fmt, std::forward<ArgTs>(a_args)...);
			}

			template<typename... ArgTs>
			void Info(const char* a_fmt, ArgTs&&... a_args) {
				m_logger->info(a_fmt, std::forward<ArgTs>(a_args)...);
			}

			template<typename... ArgTs>
			void Warn(const char* a_fmt, ArgTs&&... a_args) {
				m_logger->warn(a_fmt, std::forward<ArgTs>(a_args)...);
			}

			template<typename... ArgTs>
			void Error(const char* a_fmt, ArgTs&&... a_args) {
				m_logger->error(a_fmt, std::forward<ArgTs>(a_args)...);
				m_logger->flush();
				throw std::exception(fmt::format(a_fmt, std::forward<ArgTs>(a_args)...).c_str());
			}

		  private:
			void Free();

			std::shared_ptr<spdlog::async_logger> m_logger;
		};

		Logger& GetLogger();
	}    // namespace detail
}    // namespace CR::Core::Log
